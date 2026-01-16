// Custom includes
#include "raceController.h"
#include "BDD.h"

namespace controller {
    //----------------------------------------------
    RaceController::RaceController()
        : QObject(nullptr)
        , m_course{ nullptr }
    //----------------------------------------------
    {
    }

    //----------------------------------------------
    RaceController::RaceController(view::Course* p_course)
        : QObject(nullptr)
        , m_course { p_course }
    //----------------------------------------------
    {
        connection();
    }

    //----------------------------------------------
    RaceController::~RaceController()
    //----------------------------------------------
    {
        // Cleanup if necessary
    }

    //----------------------------------------------
    void RaceController::connection()
    //----------------------------------------------
    {
        if(nullptr != m_course)
        {
            connect(m_course, &view::Course::startClicked, this, &RaceController::onStart);
            connect(m_course, &view::Course::clearClicked, this, &RaceController::onClear);
        }
    }

    //----------------------------------------------
    void RaceController::onStart(const bool p_isFullStart)
    //----------------------------------------------
    {
        if(nullptr == m_course)
        {
            qWarning() << "Course is not set for RaceController";
            return;
        }

        if(false == m_course->isValid())
        {
            return;
        }

        En2En3 en2En3(m_course->numbers());
        m_course->fillEn2En3(en2En3.horses(),
                        en2En3.columnsTotalEn2(),
                        en2En3.columnsTotalEn3(),
                        en2En3.columnsTotalEn2En3());

        fillCourseInBDD(en2En3);
        
        if(!p_isFullStart)
        {
            // Create current race data for yellow filter
            std::array<int, 24> totalUsed;
            for(size_t index = 0; index < colIndexes.size(); ++index)
            {
                totalUsed[index] = m_course->totalRow()[colIndexes[index]];
                totalUsed[index + 8] = m_course->totalRow()[colIndexes[index] + 16];
                totalUsed[index + 16] = m_course->totalRow()[colIndexes[index] + 32];
            }

            model::Filter filter = courseFilter();

            BDD bdd;
            bdd.setIsFilterByCombination(m_course->isFilterByCombination());
            const std::vector<RaceData> fullCombinationBDDDatas = bdd.raceDataByCombination(m_course->combinationString());

            m_statLPJArrayController->setFullDatas(fullCombinationBDDDatas);
            m_statLPJArrayController->setCurrentRaceDatas(totalUsed);
            m_statLPJArrayController->onStartFilter(convertFilter(filter));
            if(!m_statLPJArrayController->isChangeFilterConnected())
            {
                const bool value = connect(&m_statLPJArrayController->statArrayHMI(),
                                           &view::StatArray::changeStatFilter,
                                           this,
                                           [this](){
                                                onChangeStatFilter(m_statLPJArrayController);
                                            });
                m_statLPJArrayController->setChangeFilterConnected(value);
            }

            m_statLPDArrayController->setFullDatas(fullCombinationBDDDatas);
            m_statLPDArrayController->setCurrentRaceDatas(totalUsed);
            m_statLPDArrayController->onStartFilter(convertFilter(filter));
            if(!m_statLPDArrayController->isChangeFilterConnected())
            {
                const bool value = connect(&m_statLPDArrayController->statArrayHMI(),
                                           &view::StatArray::changeStatFilter,
                                           this,
                                           [this](){
                                                onChangeStatFilter(m_statLPDArrayController);
                                            });
                m_statLPDArrayController->setChangeFilterConnected(value);
            }
        }
    }

    //----------------------------------------------
    void RaceController::onClear()
    //----------------------------------------------
    {
        if(nullptr != m_course)
        {
            m_course->clear();
        }
    }

    //----------------------------------------------
    model::Filter RaceController::courseFilter()
    //----------------------------------------------
    {
        model::Filter filter;

        if(nullptr == m_course)
        {
            return filter;
        }

        filter.horses = m_course->horsesFilter();
        filter.letterStrict = m_course->letterStrict();
        filter.letterMin = m_course->letterMin();
        filter.nbrLeaverStrict = m_course->nbrLeaverStrict();
        filter.nbrLeaverMin = m_course->nbrLeaverMin();
        filter.yellowStrict = m_course->yellowStrict();
        filter.yellowMin = m_course->yellowMin();
        filter.distance = m_course->distanceFilter();

        return filter;
    }

    //----------------------------------------------
    void RaceController::fillCourseInBDD(En2En3& p_en2eEn3)
    //----------------------------------------------
    {
        if(nullptr == m_course)
        {
            qWarning() << "Course is not set for RaceController";
            return;
        }

        BDD bdd;
        std::array<char, 3> en2Combination = bdd.computeCombinationProg5(p_en2eEn3.columnsTotalEn2());
        std::array<char, 3> en3Combination = bdd.computeCombinationProg5(p_en2eEn3.columnsTotalEn3());
        std::array<char, 3> en2En3Combination = bdd.computeCombinationProg5(p_en2eEn3.columnsTotalEn2En3());
        view::Combination combination{en2Combination, en3Combination, en2En3Combination};
        view::Totals totals{p_en2eEn3.columnsTotalEn2(),
                    p_en2eEn3.columnsTotalEn3(),
                    p_en2eEn3.columnsTotalEn2En3()};
        m_course->fillCombination(combination, totals);

        bdd.connectToDatabase();
        
        int raceId = 0;
        QString discipline = m_course->discipline();

        if(false == bdd.raceDatas(m_course->numbers(),
                                m_course->winner(),
                                m_course->place1(),
                                m_course->place2(),
                                discipline,
                                m_course->numberLeavers(),
                                m_course->distance(),
                                raceId))
        {
            qWarning() << "Error during set race datas into database";
            bdd.insertValidation(false);
            return;
        }

        // En2 en3 informations are not set if raceDatas are not set
        std::array<int, 3> en2En3Ids = {0, 0, 0};
        if(false == bdd.formatEn2En3Datas(m_course->modelEn2(),
                                        m_course->modelEn3(),
                                        m_course->modelEn2En3(),
                                        en2En3Ids))
        {
            qWarning() << "Error during set en2 en3 array into database";
            bdd.insertValidation(false);
            return;
        }

        int combinationId = 0;
        if(false == bdd.writeCombination(en2Combination,
                                        en3Combination,
                                        en2En3Combination,
                                        combinationId))
        {
            qWarning() << "Error during set combination into database";
            bdd.insertValidation(false);
            return;
        }

        std::array<int, 3> dataId = {0, 0, 0};
        if(false == bdd.joinDatas(raceId, en2En3Ids, dataId))
        {
            qWarning() << "Error during join data into database";
            bdd.insertValidation(false);
            return;
        }

        if(false == bdd.joinCombination(combinationId, dataId))
        {
            qWarning() << "Error during join combination and data into database";
            bdd.insertValidation(false);
            return;
        }

        bdd.insertValidation(true);
    }

    //----------------------------------------------
    const Filter RaceController::convertFilter(const model::Filter& p_filterToConvert) const
    //----------------------------------------------
    {
        Filter result;
        result.letterMin = p_filterToConvert.letterMin;
        result.letterStrict = p_filterToConvert.letterStrict;
        result.nbrLeaverMin = p_filterToConvert.nbrLeaverMin;
        result.nbrLeaverStrict = p_filterToConvert.nbrLeaverStrict;
        result.yellowMin = p_filterToConvert.yellowMin;
        result.yellowStrict = p_filterToConvert.yellowStrict;
        result.distance = p_filterToConvert.distance;
        result.horses = p_filterToConvert.horses;

        return result;
    }

    //----------------------------------------------
    void RaceController::onChangeStatFilter(StatArrayController* p_controller)
    //----------------------------------------------
    {
        model::Filter filter = courseFilter();
        p_controller->changeStatFilter(convertFilter(filter));
    }
}