// Custom includes
#include "display_BDD.h"
#include "ui_display_BDD.h"
#include "controller/BDD.h"
#include "bddCourseData.h"

// Qt includes
#include <QComboBox>

namespace view
{
    //-----------------------------------------
    void DisplayBDDFilter::filterDiscipline(const QString& p_disciplineStrict,
                                            const QString& p_disciplineMin)
    //-----------------------------------------
    {
        isDisciplineStrict = false;
        disciplineFilter = p_disciplineMin;
        if(!p_disciplineStrict.isEmpty())
        {
            isDisciplineStrict = true;
            disciplineFilter = p_disciplineStrict;
        }
    }

    //-----------------------------------------
    void DisplayBDDFilter::filterNbrLeaver(const int p_nbrLeaverStrict,
                                           const int p_nbrLeaverMin)
    //-----------------------------------------
    {
        isNbrLeaverStrict = false;
        nbrLeaverFilter = p_nbrLeaverMin;
        if(0 < p_nbrLeaverStrict)
        {
            isNbrLeaverStrict = true;
            nbrLeaverFilter = p_nbrLeaverStrict;
        }
    }

    //-----------------------------------------
    void DisplayBDDFilter::filterDistance(const int p_distanceStrict,
                                          const int p_distanceMin)
    //-----------------------------------------
    {
        isDistanceStrict = false;
        distanceFilter = p_distanceMin;
        if(0 < p_distanceStrict)
        {
            isDistanceStrict = true;
            distanceFilter = p_distanceStrict;
        }
    }

    //-----------------------------------------
    DisplayBDD::DisplayBDD(QWidget* p_parent)
        : QWidget(p_parent)
        , m_ui { new Ui::Display_BDD}
    //-----------------------------------------
    {
        m_ui->setupUi(this);
        setupUi();
        connect(m_ui->m_start, &QPushButton::clicked, this, [this]{
            const int index = m_ui->m_combinationList->currentIndex();
            onCombSelectionChanged(index);
        });
    }

    //-----------------------------------------
    DisplayBDD::~DisplayBDD()
    //-----------------------------------------
    {
        delete m_ui;
    }

    //-----------------------------------------
    void DisplayBDD::setupUi()
    //-----------------------------------------
    {
        BDD bdd;
        bdd.connectToDatabase();
        fillCombinations(bdd.combinations());
        bdd.closeConnection();
    }

    //-----------------------------------------
    void DisplayBDD::fillCombinations(const std::vector<CombinationToDisplay>& p_combinations)
    //-----------------------------------------
    {
        if(0 == p_combinations.size())
        {
            qWarning() << "No combinations found";
            return;
        }

        m_ui->m_combinationList->addItem("");
        for(CombinationToDisplay comb: p_combinations)
        {
            m_ui->m_combinationList->addItem(QString::fromStdString(comb.combination));
        }
        connect(m_ui->m_combinationList, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DisplayBDD::onCombSelectionChanged);
    }

    //-----------------------------------------
    void DisplayBDD::onCombSelectionChanged(int p_index)
    //-----------------------------------------
    {
        retrieveFilter();
        BDD bdd;
        bdd.connectToDatabase();
        std::vector<RaceData> raceDatas = bdd.raceDataByCombinationId(p_index);
        bdd.closeConnection();
        
        if(nullptr == m_ui)
        {
            qDebug() << "m_ui nullptr";
            return;
        }
        if(nullptr == m_ui->m_courseContainer)
        {
            qDebug() << "m_courseContainer nullptr";
            return;
        }

        clearCourseContainer();
        
        for(RaceData data: raceDatas)
        {
            if(!isFiltered(data))
            {
                continue; // Do not display the race it does not pass filter.
            }

            BDDCourseData* bddCourseData = new BDDCourseData(this);
            
            bddCourseData->fillBDDCourseData(data);
            m_ui->m_courseContainer->layout()->addWidget(bddCourseData);
        }
    }

    //-----------------------------------------
    void DisplayBDD::clearCourseContainer()
    //-----------------------------------------
    {
        if(nullptr == m_ui || nullptr == m_ui->m_courseContainer)
        {
            qDebug() << "m_ui or m_courseContainer is nullptr";
            return;
        }
        QLayoutItem* item = nullptr;
        while(nullptr != (item = m_ui->m_courseContainer->layout()->takeAt(0)))
        {
            QWidget* widget = item->widget();
            if(widget)
            {
                widget->deleteLater();
            }
            delete item;
        }
    }

    //-----------------------------------------
    void DisplayBDD::retrieveFilter()
    //-----------------------------------------
    {
        m_bddFilter.filterDiscipline(m_ui->m_disciplineStrict->text(),
                                     m_ui->m_disciplineMin->text());
        m_bddFilter.filterNbrLeaver(m_ui->m_nbrLeaverStrict->text().toInt(),
                                    m_ui->m_nbrLeaverMin->text().toInt());
        m_bddFilter.filterDistance(m_ui->m_distanceStrict->text().toInt(),
                                   m_ui->m_disciplineMin->text().toInt());
    }

    //-----------------------------------------
    const bool DisplayBDD::isFiltered(const RaceData& p_data) const
    //-----------------------------------------
    {
        // Discipline filter
        const QString filterDiscipline = m_bddFilter.disciplineFilter;
        if(!filterDiscipline.isEmpty())
        {
            if(m_bddFilter.isDisciplineStrict && (0 != p_data.discipline.compare(filterDiscipline, Qt::CaseInsensitive)))
            {
                return false;
            }
            else if(!m_bddFilter.isDisciplineStrict && !p_data.discipline.contains(filterDiscipline, Qt::CaseInsensitive))
            {
                return false;
            }
        }

        // Number of leaver
        const int filterNbrLeaver = m_bddFilter.nbrLeaverFilter;
        if(0 < filterNbrLeaver)
        {
            if(m_bddFilter.isNbrLeaverStrict && (filterNbrLeaver != p_data.nbrLeaver))
            {
                return false;
            }
            else if(!m_bddFilter.isNbrLeaverStrict && (filterNbrLeaver > p_data.nbrLeaver))
            {
                return false;
            }
        }

        // Distance
        const int filterDistance = m_bddFilter.distanceFilter;
        if(0 < filterDistance)
        {
            if(m_bddFilter.isDistanceStrict && (filterDistance != p_data.distance))
            {
                return false;
            }
            else if(!m_bddFilter.isDistanceStrict && (filterDistance > p_data.nbrLeaver))
            {
                return false;
            }
        }

        return true;
    }
}