// Custom includes
#include "statArrayController.h"
#include "tools.h"

namespace controller
{
    //--------------------------------------------------------------------------------
    StatArrayController::StatArrayController(const EFilter p_filter1,
                                             const EFilter p_filter2)
        : m_fullDatas {}
        , m_currentFilterType { p_filter1 }
        , m_filter1 { p_filter1 }
        , m_filter2 { p_filter2 }
        , m_statArray { Tools::convertFilterToString(p_filter1),
                        Tools::convertFilterToString(p_filter2) }
        , m_currentRaceDatas { -1 }
        , m_isChangeFilterConnected { false }
    //--------------------------------------------------------------------------------
    {
        connect(&m_statArray, &view::StatArray::startSubFilter, this, &StatArrayController::onStartSubFilter);
        connect(&m_statArray, &view::StatArray::resetSubFilter, this, &StatArrayController::onResetSubFilter);
    }

    //--------------------------------------------------------------------------------
    StatArrayController::~StatArrayController()
    //--------------------------------------------------------------------------------
    {

    }

    //--------------------------------------------------------------------------------
    void StatArrayController::setFullDatas(const std::vector<RaceData> p_fullDatas)
    //--------------------------------------------------------------------------------
    {
        m_fullDatas = p_fullDatas;
    }

    //--------------------------------------------------------------------------------
    void StatArrayController::setCurrentRaceDatas(const std::array<int, 24> p_currentRaceDatas)
    //--------------------------------------------------------------------------------
    {
        m_currentRaceDatas = p_currentRaceDatas;
    }

    //--------------------------------------------------------------------------------
    view::StatArray& StatArrayController::statArrayHMI()
    //--------------------------------------------------------------------------------
    {
        return m_statArray;
    }

    //--------------------------------------------------------------------------------
    void StatArrayController::onStartFilter(const Filter& p_filter)
    //--------------------------------------------------------------------------------
    {
        m_currentFilterType = m_filter1;
        m_statArray.resetText();
        filteredDatas(p_filter, m_currentFilterType);
        const StatisticsData statistics = generateStatistics(m_currentFilteredDatas);
        m_statArray.displayDatas(statistics);
    }

    //--------------------------------------------------------------------------------
    void StatArrayController::filteredDatas(const Filter& p_filter,
                                            const EFilter& p_filterType)
    //--------------------------------------------------------------------------------
    {
        switch(p_filterType)
        {
            case FilterLPJ:
                m_currentFilteredDatas = startLPJFilter(p_filter);
                break;
            case FilterLP:
                m_currentFilteredDatas = startLPFilter(p_filter);
                break;
            case FilterLPD:
                m_currentFilteredDatas = startLPDFilter(p_filter);
                break;
        }
    }

    //--------------------------------------------------------------------------------
    const bool StatArrayController::isFilterLP(const Filter& p_filter,
                                               const QString& p_letterToFilter,
                                               const int p_nbrLeaverToFilter)
    //--------------------------------------------------------------------------------
    {
        if(p_filter.isLetterFilter(p_letterToFilter))
        {
            if(p_filter.isNbrLeaverFilter(p_nbrLeaverToFilter))
            {
                return true;
            }
        }
        return false;
    }

    //--------------------------------------------------------------------------------
    const bool StatArrayController::isFilterLPJ(const Filter& p_filter,
                                                const QString& p_letterToFilter,
                                                const int p_nbrLeaverToFilter,
                                                const int p_yellowToFilter)
    //--------------------------------------------------------------------------------
    {
        if(isFilterLP(p_filter, p_letterToFilter, p_nbrLeaverToFilter))
        {
            if(p_filter.isYellowFilter(p_yellowToFilter))
            {
                return true;
            }
        }
        return false;
    }

    //--------------------------------------------------------------------------------
    const bool StatArrayController::isFilterLPD(const Filter& p_filter,
                                                const QString& p_letterToFilter,
                                                const int p_nbrLeaverToFilter,
                                                const int p_distanceToFilter)
    //--------------------------------------------------------------------------------
    {
        if(isFilterLP(p_filter, p_letterToFilter, p_nbrLeaverToFilter))
        {
            if(p_filter.isDistanceFilter(p_distanceToFilter))
            {
                return true;
            }
        }
        return false;
    }

    //--------------------------------------------------------------------------------
    const std::vector<RaceData> StatArrayController::startLPJFilter(const Filter& p_filter)
    //--------------------------------------------------------------------------------
    {
        std::vector<RaceData> result;
        if(m_fullDatas.empty())
        {
            qWarning() << "Set the full datas before using startLPJFilter";
            return result;
        }

        for(const RaceData& data: m_fullDatas)
        {
            const int yellowCounter = computeYellow(m_currentRaceDatas, data.totalDatas());
            if(isFilterLPJ(p_filter,
                           data.discipline,
                           data.nbrLeaver,
                           yellowCounter))
            {
                result.push_back(data);
            }
        }

        return result;
    }

    //--------------------------------------------------------------------------------
    const std::vector<RaceData> StatArrayController::startLPFilter(const Filter& p_filter)
    //--------------------------------------------------------------------------------
    {
        std::vector<RaceData> result;
        if(m_fullDatas.empty())
        {
            qWarning() << "Set the full datas before using startLPJFilter";
            return result;
        }

        for(const RaceData& data: m_fullDatas)
        {
            if(isFilterLP(p_filter,
                          data.discipline,
                          data.nbrLeaver))
            {
                result.push_back(data);
            }
        }

        return result;
    }

    //--------------------------------------------------------------------------------
    const std::vector<RaceData> StatArrayController::startLPDFilter(const Filter& p_filter)
    //--------------------------------------------------------------------------------
    {
        std::vector<RaceData> result;
        if(m_fullDatas.empty())
        {
            qWarning() << "Set the full datas before using startLPJFilter";
            return result;
        }

        for(const RaceData& data: m_fullDatas)
        {
            if(isFilterLPD(p_filter,
                           data.discipline,
                           data.nbrLeaver,
                           data.distance))
            {
                result.push_back(data);
                const std::array<CompareValue, 24> current = data.convertValue();
            }
        }

        return result;
    }

    //--------------------------------------------------------------------------------
    const int StatArrayController::computeYellow(const std::array<int, 24>& p_currentRace,
                                                 const std::array<int, 24> p_dataFromDB)
    //--------------------------------------------------------------------------------
    {
        int yellowCounter = 0;
        if(-1 == p_dataFromDB[0])
        {
            return yellowCounter;
        }
        for(size_t index = 0; index < p_currentRace.size(); ++index)
        {
            if(p_currentRace[index] == p_dataFromDB[index])
            {
                yellowCounter++;
            }
        }

        return yellowCounter;
    }

    //--------------------------------------------------------------------------------
    const StatisticsData StatArrayController::generateStatistics(const std::vector<RaceData>& p_filteredDatas)
    //--------------------------------------------------------------------------------
    {
        StatisticsData result;
        result.total = p_filteredDatas.size();
        for(const RaceData& data: p_filteredDatas)
        {
            for(int indexRow = 0; indexRow < static_cast<int>(UnknownRowArray); ++indexRow)
            {
                for(int indexCol = 0; indexCol < 24; ++indexCol)
                {
                    switch(indexRow)
                    {
                        case CircleArray:
                            if(data.isPassFilter(indexRow, indexCol))
                            {
                                result.arrayCircle[indexCol]++;
                            }
                            break;
                        case GreenArray:
                        {
                            if(data.colorInColumn("Green", indexCol))
                            {
                                result.totalGreen[indexCol]++;
                                if(data.isPassFilter(indexRow, indexCol))
                                {
                                    result.arrayGreen[indexCol]++;
                                }
                          }
                          break;
                        }
                        case BlueArray:
                        {
                            if(data.colorInColumn("Blue", indexCol))
                            {
                                result.totalBlue[indexCol]++;
                                if(data.isPassFilter(indexRow, indexCol))
                                {
                                    result.arrayBlue[indexCol]++;
                                }
                            }
                            break;
                        }
                        case YellowArray:
                        {
                            if(data.colorInColumn("Yellow", indexCol))
                            {
                                result.totalYellow[indexCol]++;
                                if(data.isPassFilter(indexRow, indexCol))
                                {
                                    result.arrayYellow[indexCol]++;
                                }
                            }
                            break;
                        }
                        case OrangeArray:
                        {
                            if(data.colorInColumn("Orange", indexCol))
                            {
                                result.totalOrange[indexCol]++;
                                if(data.isPassFilter(indexRow, indexCol))
                                {
                                    result.arrayOrange[indexCol]++;
                                }
                            }
                            break;
                        }
                        case BlankArray:
                        {
                            if(data.colorInColumn("Blank", indexCol))
                            {
                                result.totalBlank[indexCol]++;
                                if(data.isPassFilter(indexRow, indexCol))
                                {
                                    result.arrayBlank[indexCol]++;
                                }
                            }
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
            computeGreenWinner(data, result);
        }
        return result;
    }

    //--------------------------------------------------------------------------------
    void StatArrayController::computeGreenWinner(const RaceData& p_data,
                                                 StatisticsData& p_statistics)
    //--------------------------------------------------------------------------------
    {
        const int winnerRow = p_data.winnerRow();
        if(-1 == winnerRow)
        {
            return;
        }

        for(size_t indexRow = 0; indexRow < p_data.numbers.size(); ++indexRow)
        {
            for(size_t indexCol = 0; indexCol < colIndexes.size(); ++indexCol)
            {
                const int colIndex = colIndexes[indexCol];
                if(0 == p_data.en2[indexRow][colIndex].color.compare("Green", Qt::CaseInsensitive))
                {
                    p_statistics.arrayGreenWinnerTotal[indexCol]++;
                    if(indexRow == winnerRow)
                    {
                        p_statistics.arrayGreenWinner[indexCol]++;
                    }
                }
                if(0 == p_data.en3[indexRow][colIndex].color.compare("Green", Qt::CaseInsensitive))
                {
                    p_statistics.arrayGreenWinnerTotal[indexCol + 8]++;
                    if(indexRow == winnerRow)
                    {
                        p_statistics.arrayGreenWinner[indexCol + 8]++;
                    }
                }
                if(0 == p_data.en2En3[indexRow][colIndex].color.compare("Green", Qt::CaseInsensitive))
                {
                    p_statistics.arrayGreenWinnerTotal[indexCol + 16]++;
                    if(indexRow == winnerRow)
                    {
                        p_statistics.arrayGreenWinner[indexCol + 16]++;
                    }
                }
            }
        }
    }

    //--------------------------------------------------------------------------------
    void StatArrayController::onStartSubFilter(const QModelIndexList& p_filterList)
    //--------------------------------------------------------------------------------
    {
        if(m_currentFilteredDatas.empty())
        {
            qWarning() << "No current filtered datas, start generic statistics before use onStartSubFilter";
            return;
        }

        std::vector<RaceData> datas = m_currentFilteredDatas;
        std::vector<RaceData> tmpData;
        for(const QModelIndex& modelIndex: p_filterList)
        {
            const int rowIndex = modelIndex.row();
            const int colIndex = modelIndex.column();
            for(const RaceData& data: datas)
            {
                // We want all data which do not corresponding to the filter
                if(!data.isPassFilter(rowIndex, colIndex))
                {
                    tmpData.push_back(data);
                }
            }
            datas.clear();
            datas = tmpData;
            tmpData.clear();
        }

        const StatisticsData statistics = generateStatistics(datas);
        m_statArray.displayDatas(statistics);
    }

    //--------------------------------------------------------------------------------
    void StatArrayController::onResetSubFilter()
    //--------------------------------------------------------------------------------
    {
        const StatisticsData statistics = generateStatistics(m_currentFilteredDatas);
        m_statArray.displayDatas(statistics);
    }

    //--------------------------------------------------------------------------------
    void StatArrayController::changeStatFilter(const Filter& p_filter)
    //--------------------------------------------------------------------------------
    {
        if(m_currentFilterType == m_filter1)
        {
            m_currentFilterType = m_filter2;
        }
        else
        {
            m_currentFilterType = m_filter1;
        }
        m_statArray.changeFilterText();
        filteredDatas(p_filter, m_currentFilterType);
        const StatisticsData statistics = generateStatistics(m_currentFilteredDatas);
        m_statArray.displayDatas(statistics);
    }
}