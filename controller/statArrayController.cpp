// Custom includes
#include "statArrayController.h"
#include "tools.h"

namespace controller
{
    //--------------------------------------------------------------------------------
    StatArrayController::StatArrayController(const EFilter p_filter1,
                                             const EFilter p_filter2,
                                             const EFilter p_filter3,
                                             const EFilter p_filter4)
        : m_fullDatas {}
        , m_currentFilterType { p_filter1 }
        , m_filter1 { p_filter1 }
        , m_filter2 { p_filter2 }
        , m_currentFilter2Type { p_filter3 }
        , m_filter3 { p_filter3 }
        , m_filter4 { p_filter4 }
        , m_statArray { {p_filter1,
                        p_filter2,
                        p_filter3,
                        p_filter4} }
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
        m_currentFilter2Type = m_filter3;
        m_statArray.resetText();
        filteredDatas(p_filter, m_currentFilterType);
        StatisticsData statistics;
        switch(m_currentFilterType) {
            case FilterDiscipline:
            case FilterLeaver:
            case FilterDistance:
            case FilterYellow:
            case FilterLPJ:
            case FilterLP:
            case FilterLPD:
                if(EFilter::FilterNone != m_currentFilter2Type) {
                    statistics = applySecondFilter(
                        p_filter,
                        m_currentFilter2Type,
                        m_currentFilteredDatas);
                } else {
                    statistics = generateStatistics(m_currentFilteredDatas);
                }
                break;
            case FilterColorFull:
                if(EFilter::FilterNone != m_currentFilter2Type) {
                    statistics = applySecondFilter(
                        p_filter,
                        m_currentFilter2Type,
                        m_currentFilteredDatasByColorsByColumn);
                } else {
                    statistics = generateStatistics(m_currentFilteredDatasByColorsByColumn);
                }
                break;
            case FilterColor:
                if(EFilter::FilterNone != m_currentFilter2Type) {
                    statistics = applySecondFilter(
                        p_filter,
                        m_currentFilter2Type,
                        m_currentFilteredDatasByColors);
                } else {
                    statistics = generateStatistics(m_currentFilteredDatasByColors);
                }
                break;
            default:
                break;
        }
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
            case FilterColorFull:
                m_currentFilteredDatasByColorsByColumn = startColorFullFilter(p_filter);
                break;
            case FilterColor:
                m_currentFilteredDatasByColors = startColorFilter(p_filter);
                break;
            default:
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
            }
        }

        return result;
    }

    //--------------------------------------------------------------------------------
    std::array<std::vector<RaceData>, 24> StatArrayController::startColorFullFilter(const Filter& p_filter)
    //--------------------------------------------------------------------------------
    {
        std::array<std::vector<RaceData>, 24> result;
        if(m_fullDatas.empty()) {
            qWarning() << "Set the full datas before using startColorFullFilter";
            return result;
        }

        for(const RaceData& data: m_fullDatas) {
            const std::map<int, ColorsValue> raceDataNumberOfColor = data.numberOfColor();
            for(std::map<int, ColorsValue>::const_iterator it = raceDataNumberOfColor.begin();
                it != raceDataNumberOfColor.end();
                ++it) {
                if(p_filter.colorFilter.end() != p_filter.colorFilter.find(it->first)) {
                    std::map<int, ColorsValue>::const_iterator value = p_filter.colorFilter.find(it->first);
                    if((it->second.greenNumber == value->second.greenNumber)
                        && (it->second.blueNumber == value->second.blueNumber)
                        && (it->second.yellowNumber == value->second.yellowNumber)
                        && (it->second.orangeNumber == value->second.orangeNumber)
                        && (it->second.blankNumber == value->second.blankNumber)) {
                        result[it->first].push_back(data);
                    }
                }
            }
        }

        return result;
    }

    //--------------------------------------------------------------------------------
    std::map<QString, std::array<std::vector<RaceData>, 24>> StatArrayController::startColorFilter(const Filter& p_filter)
    //--------------------------------------------------------------------------------
    {
        // Todo to change to have RaceData by color and by column
        std::map<QString, std::array<std::vector<RaceData>, 24>> result;
        if(m_fullDatas.empty()) {
            qWarning() << "Set the full datas before using startColorFilter";
            return result;
        }

        for(const RaceData& datas: m_fullDatas) {
            const std::map<int, ColorsValue> raceDataNumberOfColor = datas.numberOfColor();
            for(std::map<int, ColorsValue>::const_iterator it = raceDataNumberOfColor.begin();
                it != raceDataNumberOfColor.end();
                ++it) {
                if(p_filter.colorFilter.end() != p_filter.colorFilter.find(it->first)) {
                    std::map<int, ColorsValue>::const_iterator value = p_filter.colorFilter.find(it->first);
                    if(it->second.greenNumber == value->second.greenNumber) {
                        result["Green"][it->first].push_back(datas);
                    }
                    if(it->second.blueNumber == value->second.blueNumber) {
                        result["Blue"][it->first].push_back(datas);
                    }
                    if(it->second.yellowNumber == value->second.yellowNumber) {
                        result["Yellow"][it->first].push_back(datas);
                    }
                    if(it->second.orangeNumber == value->second.orangeNumber) {
                        result["Orange"][it->first].push_back(datas);
                    }
                    if(it->second.blankNumber == value->second.blankNumber) {
                        result["Blank"][it->first].push_back(datas);
                    }
                }
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
        for(const RaceData& data: p_filteredDatas) {
            for(int indexRow = 0; indexRow < static_cast<int>(UnknownRowArray); ++indexRow) {
                for(int indexCol = 0; indexCol < 24; ++indexCol) {
                    switch(indexRow) {
                        case CircleArray:
                            if(data.isPassFilter(indexRow, indexCol)) {
                                result.arrayCircle[indexCol]++;
                            }
                            break;
                        case GreenArray:
                            if(data.colorInColumn("Green", indexCol)) {
                                result.totalGreen[indexCol]++;
                                if(data.isPassFilter(indexRow, indexCol)) {
                                    result.arrayGreen[indexCol]++;
                                }
                            }
                            break;
                        case BlueArray:
                            if(data.colorInColumn("Blue", indexCol)) {
                                result.totalBlue[indexCol]++;
                                if(data.isPassFilter(indexRow, indexCol)) {
                                    result.arrayBlue[indexCol]++;
                                }
                            }
                            break;
                        case YellowArray:
                            if(data.colorInColumn("Yellow", indexCol)) {
                                result.totalYellow[indexCol]++;
                                if(data.isPassFilter(indexRow, indexCol)) {
                                    result.arrayYellow[indexCol]++;
                                }
                            }
                            break;
                        case OrangeArray:
                            if(data.colorInColumn("Orange", indexCol)) {
                                result.totalOrange[indexCol]++;
                                if(data.isPassFilter(indexRow, indexCol)) {
                                    result.arrayOrange[indexCol]++;
                                }
                            }
                            break;
                        case BlankArray:
                            if(data.colorInColumn("Blank", indexCol) || data.colorInColumn("None", indexCol)) {
                                result.totalBlank[indexCol]++;
                                if(data.isPassFilter(indexRow, indexCol)) {
                                    result.arrayBlank[indexCol]++;
                                }
                            }
                            break;
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
    const StatisticsData StatArrayController::generateStatistics(const std::array<std::vector<RaceData>, 24>& p_filteredDatas)
    //--------------------------------------------------------------------------------
    {
        StatisticsData result;

        for(int colIndex = 0; colIndex < p_filteredDatas.size(); ++colIndex) {
            std::vector<RaceData> currentRace = p_filteredDatas[colIndex];
            result.totalByColumn[colIndex] = currentRace.size();
            for(const RaceData& data: currentRace) {
                for(int indexRow = 0; indexRow < static_cast<int>(UnknownRowArray); ++indexRow) {
                    switch(indexRow) {
                        case CircleArray:
                            if(data.isPassFilter(indexRow, colIndex)) {
                                result.arrayCircle[colIndex]++;
                            }
                            break;
                        case GreenArray:
                            if(data.colorInColumn("Green", colIndex)) {
                                result.totalGreen[colIndex]++;
                                if(data.isPassFilter(indexRow, colIndex)) {
                                    result.arrayGreen[colIndex]++;
                                }
                            }
                            break;
                        case BlueArray:
                            if(data.colorInColumn("Blue", colIndex)) {
                                result.totalBlue[colIndex]++;
                                if(data.isPassFilter(indexRow, colIndex)) {
                                    result.arrayBlue[colIndex]++;
                                }
                            }
                            break;
                        case YellowArray:
                            if(data.colorInColumn("Yellow", colIndex)) {
                                result.totalYellow[colIndex]++;
                                if(data.isPassFilter(indexRow, colIndex)) {
                                    result.arrayYellow[colIndex]++;
                                }
                            }
                            break;
                        case OrangeArray:
                            if(data.colorInColumn("Orange", colIndex)) {
                                result.totalOrange[colIndex]++;
                                if(data.isPassFilter(indexRow, colIndex)) {
                                    result.arrayOrange[colIndex]++;
                                }
                            }
                            break;
                        case BlankArray:
                            if(data.colorInColumn("Blank", colIndex)) {
                                result.totalBlank[colIndex]++;
                                if(data.isPassFilter(indexRow, colIndex)) {
                                    result.arrayBlank[colIndex]++;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
                computeGreenWinner(data, result);
            }
        }

        return result;
    }

    //--------------------------------------------------------------------------------
    const StatisticsData StatArrayController::generateStatistics(
        const std::map<QString, std::array<std::vector<RaceData>, 24>>& p_filteredDatas)
    //--------------------------------------------------------------------------------
    {
        StatisticsData result;

        std::array<std::vector<RaceData>, 24> greenDatas = {};
        if(p_filteredDatas.end() != p_filteredDatas.find("Green")) {
            greenDatas = p_filteredDatas.find("Green")->second;
        }

        std::array<std::vector<RaceData>, 24> blueDatas = {};
        if(p_filteredDatas.end() != p_filteredDatas.find("Blue")) {
            blueDatas = p_filteredDatas.find("Blue")->second;
        }

        std::array<std::vector<RaceData>, 24> yellowDatas = {};
        if(p_filteredDatas.end() != p_filteredDatas.find("Yellow")) {
            yellowDatas = p_filteredDatas.find("Yellow")->second;
        }

        std::array<std::vector<RaceData>, 24> orangeDatas = {};
        if(p_filteredDatas.end() != p_filteredDatas.find("Orange")) {
            orangeDatas = p_filteredDatas.find("Orange")->second;
        }

        std::array<std::vector<RaceData>, 24> blankDatas = {};
        if(p_filteredDatas.end() != p_filteredDatas.find("Blank")) {
            blankDatas = p_filteredDatas.find("Blank")->second;
        }

        for(int colIndex = 0; colIndex < 24; ++colIndex) {
            std::vector<RaceData> greenRaces = greenDatas[colIndex];
            result.totalGreen[colIndex] = greenRaces.size();
            std::vector<RaceData> blueRaces = blueDatas[colIndex];
            result.totalBlue[colIndex] = blueRaces.size();
            std::vector<RaceData> yellowRaces = yellowDatas[colIndex];
            result.totalYellow[colIndex] = yellowRaces.size();
            std::vector<RaceData> orangeRaces = orangeDatas[colIndex];
            result.totalOrange[colIndex] = orangeRaces.size();
            std::vector<RaceData> blankRaces = blankDatas[colIndex];
            result.totalBlank[colIndex] = blankRaces.size();

            result.arrayCircle[colIndex] = 0;

            for(const RaceData& greenRace: greenRaces) {
                if(greenRace.isPassFilter(GreenArray, colIndex)) {
                    result.arrayGreen[colIndex]++;
                }
            }
            for(const RaceData& blueRace: blueRaces) {
                if(blueRace.isPassFilter(BlueArray, colIndex)) {
                    result.arrayBlue[colIndex]++;
                }
            }
            for(const RaceData& yellowRace: yellowRaces) {
                if(yellowRace.isPassFilter(YellowArray, colIndex)) {
                    result.arrayYellow[colIndex]++;
                }
            }
            for(const RaceData& orangeRace: orangeRaces) {
                if(orangeRace.isPassFilter(OrangeArray, colIndex)) {
                    result.arrayOrange[colIndex]++;
                }
            }
            for(const RaceData& blankRace: blankRaces) {
                if(blankRace.isPassFilter(BlankArray, colIndex)) {
                    result.arrayBlank[colIndex]++;
                }
            }
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
        if(m_currentFilteredDatas.empty() &&
            m_currentFilteredDatasByColorsByColumn.empty() &&
            m_currentFilteredDatasByColors.empty())
        {
            qWarning() << "No current filtered datas, start generic statistics before use onStartSubFilter";
            return;
        }

        StatisticsData statistics;
        switch(m_currentFilterType) {
            case FilterDiscipline:
            case FilterLeaver:
            case FilterDistance:
            case FilterYellow:
            case FilterLPJ:
            case FilterLP:
            case FilterLPD:
            {
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

                statistics = generateStatistics(datas);
                break;
            }
            case FilterColorFull:
            {
                std::array<std::vector<RaceData>, 24> datas = m_currentFilteredDatasByColorsByColumn;
                std::vector<RaceData> tmpData = {};
                for(const QModelIndex& modelIndex: p_filterList) {
                    const int rowIndex = modelIndex.row();
                    const int colIndex = modelIndex.column();
                    std::vector<RaceData> currentColDatas = datas[colIndex];
                    for(const RaceData& data: currentColDatas) {
                        if(!data.isPassFilter(rowIndex, colIndex)) {
                            tmpData.push_back(data);
                        }
                    }
                    datas[colIndex].clear();
                    datas[colIndex] = tmpData;
                    tmpData.clear();
                }
                statistics = generateStatistics(datas);
                break;
            }
            case FilterColor:
            {
                std::map<QString, std::array<std::vector<RaceData>, 24>> datas = m_currentFilteredDatasByColors;
                std::vector<RaceData> tmpData = {};
                for(const QModelIndex& modelIndex: p_filterList) {
                    const int rowIndex = modelIndex.row();
                    const int colIndex = modelIndex.column();
                    std::vector<RaceData> currentColDatas = {};
                    QString currentColor = "";
                    if((2 == rowIndex) && (datas.end() != datas.find("Green"))) {
                        currentColDatas = datas.find("Green")->second[colIndex];
                        currentColor = "Green";
                    } else if((4 == rowIndex) && (datas.end() != datas.find("Blue"))) {
                        currentColDatas = datas.find("Blue")->second[colIndex];
                        currentColor = "Blue";
                    } else if((5 == rowIndex) && (datas.end() != datas.find("Yellow"))) {
                        currentColDatas = datas.find("Yellow")->second[colIndex];
                        currentColor = "Yellow";
                    } else if((6 == rowIndex) && (datas.end() != datas.find("Orange"))) {
                        currentColDatas = datas.find("Orange")->second[colIndex];
                        currentColor = "Orange";
                    } else if((7 == rowIndex) && (datas.end() != datas.find("Blank"))) {
                        currentColDatas = datas.find("Blank")->second[colIndex];
                        currentColor = "Blank";
                    }
                    for(const RaceData& data: currentColDatas) {
                        if(!data.isPassFilter(rowIndex, colIndex)) {
                            tmpData.push_back(data);
                        }
                    }
                    datas.find(currentColor)->second[colIndex].clear();
                    datas.find(currentColor)->second[colIndex] = tmpData;
                    tmpData.clear();
                }
                statistics = generateStatistics(datas);
                break;
            }
            default:
                break;
        }
        
        m_statArray.displayDatas(statistics);
    }

    //--------------------------------------------------------------------------------
    void StatArrayController::onResetSubFilter()
    //--------------------------------------------------------------------------------
    {
        StatisticsData statistics;
        switch(m_currentFilterType) {
            case FilterDiscipline:
            case FilterLeaver:
            case FilterDistance:
            case FilterYellow:
            case FilterLPJ:
            case FilterLP:
            case FilterLPD:
                statistics = generateStatistics(m_currentFilteredDatas);
                break;
            case FilterColorFull:
                statistics = generateStatistics(m_currentFilteredDatasByColorsByColumn);
                break;
            case FilterColor:
                statistics = generateStatistics(m_currentFilteredDatasByColors);
                break;
            default:
                break;
        }
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
        StatisticsData statistics;
        switch(m_currentFilterType) {
            case FilterDiscipline:
            case FilterLeaver:
            case FilterDistance:
            case FilterYellow:
            case FilterLPJ:
            case FilterLP:
            case FilterLPD:
                if(EFilter::FilterNone != m_currentFilter2Type) {
                    statistics = applySecondFilter(
                        p_filter,
                        m_currentFilter2Type,
                        m_currentFilteredDatas);
                } else {
                    statistics = generateStatistics(m_currentFilteredDatas);
                }
                break;
            case FilterColorFull:
                if(EFilter::FilterNone != m_currentFilter2Type) {
                    statistics = applySecondFilter(
                        p_filter,
                        m_currentFilter2Type,
                        m_currentFilteredDatasByColorsByColumn);
                } else {
                    statistics = generateStatistics(m_currentFilteredDatasByColorsByColumn);
                }
                break;
            case FilterColor:
                if(EFilter::FilterNone != m_currentFilter2Type) {
                    statistics = applySecondFilter(
                        p_filter,
                        m_currentFilter2Type,
                        m_currentFilteredDatasByColors);
                } else {
                    statistics = generateStatistics(m_currentFilteredDatasByColors);
                }
                break;
            default:
                break;
        }
        m_statArray.displayDatas(statistics);
    }

    //--------------------------------------------------------------------------------
    void StatArrayController::changeStatFilter2(const Filter& p_filter)
    //--------------------------------------------------------------------------------
    {
        if(m_currentFilter2Type == m_filter3)
        {
            m_currentFilter2Type = m_filter4;
        }
        else
        {
            m_currentFilter2Type = m_filter3;
        }
        m_statArray.changeFilter2Text();
        filteredDatas(p_filter, m_currentFilterType);
        StatisticsData statistics;
        switch(m_currentFilterType) {
            case FilterDiscipline:
            case FilterLeaver:
            case FilterDistance:
            case FilterYellow:
            case FilterLPJ:
            case FilterLP:
            case FilterLPD:
                if(EFilter::FilterNone != m_currentFilter2Type) {
                    statistics = applySecondFilter(
                        p_filter,
                        m_currentFilter2Type,
                        m_currentFilteredDatas);
                } else {
                    statistics = generateStatistics(m_currentFilteredDatas);
                }
                break;
            case FilterColorFull:
                if(EFilter::FilterNone != m_currentFilter2Type) {
                    statistics = applySecondFilter(
                        p_filter,
                        m_currentFilter2Type,
                        m_currentFilteredDatasByColorsByColumn);
                } else {
                    statistics = generateStatistics(m_currentFilteredDatasByColorsByColumn);
                }
                break;
            case FilterColor:
                if(EFilter::FilterNone != m_currentFilter2Type) {
                    statistics = applySecondFilter(
                        p_filter,
                        m_currentFilter2Type,
                        m_currentFilteredDatasByColors);
                } else {
                    statistics = generateStatistics(m_currentFilteredDatasByColors);
                }
                break;
            default:
                break;
        }
        m_statArray.displayDatas(statistics);
    }

    //--------------------------------------------------------------------------------
    const StatisticsData StatArrayController::applySecondFilter(
        const Filter& p_currentFilter,
        const EFilter& p_filter,
        const std::vector<RaceData>& p_filteredDatas)
    //--------------------------------------------------------------------------------
    {
        std::vector<RaceData> tmpDatas = {};
        for(const RaceData& data: p_filteredDatas) {
            switch(p_filter) {
                case FilterLPJ:
                {
                    const int yellowCounter = computeYellow(m_currentRaceDatas, data.totalDatas());
                    if(isFilterLPJ(p_currentFilter,
                                   data.discipline,
                                   data.nbrLeaver,
                                   yellowCounter))
                    {
                        tmpDatas.push_back(data);
                    }
                    break;
                }
                case FilterLP:
                {
                    if(isFilterLP(p_currentFilter,
                                   data.discipline,
                                   data.nbrLeaver))
                    {
                        tmpDatas.push_back(data);
                    }
                    break;
                }
                default:
                    qWarning() << "Filter not yet implement" << p_filter;
                    break;
            }
        }

        m_currentFilteredDatas.clear();
        m_currentFilteredDatas = tmpDatas;

        return generateStatistics(tmpDatas);
    }

    //--------------------------------------------------------------------------------
    const StatisticsData StatArrayController::applySecondFilter(
        const Filter& p_currentFilter,
        const EFilter& p_filter,
        const std::array<std::vector<RaceData>, 24>& p_filteredDatas)
    //--------------------------------------------------------------------------------
    {
        std::array<std::vector<RaceData>, 24> tmpDatas = {};
        for(size_t index = 0; index < p_filteredDatas.size(); ++index) {
            const std::vector<RaceData>& dataByColumn = p_filteredDatas[index];
            std::vector<RaceData> vectorDatas = {};
            for(const RaceData& data: dataByColumn) {
                switch(p_filter) {
                    case FilterLPJ:
                    {
                        const int yellowCounter = computeYellow(m_currentRaceDatas, data.totalDatas());
                        if(isFilterLPJ(p_currentFilter,
                                    data.discipline,
                                    data.nbrLeaver,
                                    yellowCounter))
                        {
                            vectorDatas.push_back(data);
                        }
                        break;
                    }
                    case FilterLP:
                    {
                        if(isFilterLP(p_currentFilter,
                                    data.discipline,
                                    data.nbrLeaver))
                        {
                            vectorDatas.push_back(data);
                        }
                        break;
                    }
                    default:
                        qWarning() << "Filter not yet implement" << p_filter;
                        break;
                }
            }
            tmpDatas[index] = vectorDatas;
        }

        m_currentFilteredDatasByColorsByColumn = tmpDatas;

        return generateStatistics(m_currentFilteredDatasByColorsByColumn);
    }

    //--------------------------------------------------------------------------------
    const StatisticsData StatArrayController::applySecondFilter(
        const Filter& p_currentFilter,
        const EFilter& p_filter,
        const std::map<QString, std::array<std::vector<RaceData>, 24>>& p_filteredDatas)
    //--------------------------------------------------------------------------------
    {
        std::map<QString, std::array<std::vector<RaceData>, 24>> tmpDatas = {};
        for(std::map<QString, std::array<std::vector<RaceData>, 24>>::const_iterator it = p_filteredDatas.begin();
            it != p_filteredDatas.end();
            ++it) {
            std::array<std::vector<RaceData>, 24> arrayDatas = {};
            for(size_t index = 0; index < it->second.size(); ++index) {
                const std::vector<RaceData>& dataByColumn = it->second[index];
                std::vector<RaceData> vectorDatas = {};
                for(const RaceData& data: dataByColumn) {
                    switch(p_filter) {
                        case FilterLPJ:
                        {
                            const int yellowCounter = computeYellow(m_currentRaceDatas, data.totalDatas());
                            if(isFilterLPJ(p_currentFilter,
                                        data.discipline,
                                        data.nbrLeaver,
                                        yellowCounter))
                            {
                                vectorDatas.push_back(data);
                            }
                            break;
                        }
                        case FilterLP:
                        {
                            if(isFilterLP(p_currentFilter,
                                        data.discipline,
                                        data.nbrLeaver))
                            {
                                vectorDatas.push_back(data);
                            }
                            break;
                        }
                        default:
                            qWarning() << "Filter not yet implement" << p_filter;
                            break;
                    }
                }
                arrayDatas[index] = vectorDatas;
            }
            tmpDatas[it->first] = arrayDatas;
        }

        m_currentFilteredDatasByColors = tmpDatas;
        return generateStatistics(m_currentFilteredDatasByColors);
    }
}