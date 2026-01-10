// Custom includes
#include "constants.h"
#include "controller/tools.h"

constexpr int TOTAL_ROW_INDEX = 8;

//----------------------------------------------------
const bool Filter::isLetterFilter(const QString& p_letterToFilter) const
//----------------------------------------------------
{
    if(letterStrict.isEmpty() && letterMin.isEmpty())
    {
        return true;
    }

    if((!letterStrict.isEmpty()) && (0 == letterStrict.compare(p_letterToFilter, Qt::CaseInsensitive)))
    {
        return true;
    }
    else if((!letterMin.isEmpty()) && (0 == letterMin.compare(p_letterToFilter, Qt::CaseInsensitive)))
    {
        return true;
    }

    return false;
}

//----------------------------------------------------
const bool Filter::isNbrLeaverFilter(const int p_leaverToFilter) const
//----------------------------------------------------
{
    if(((-1 == nbrLeaverStrict) || (0 == nbrLeaverStrict))
        && ((-1 == nbrLeaverMin) || (0 == nbrLeaverMin)))
    {
        return true;
    }

    if((0 < nbrLeaverStrict) && (p_leaverToFilter == nbrLeaverStrict))
    {
        return true;
    }
    else if((0 < nbrLeaverMin) && (p_leaverToFilter >= nbrLeaverMin))
    {
        return true;
    }

    return false;
}

//----------------------------------------------------
const bool Filter::isYellowFilter(const int p_yellowToFilter) const
//----------------------------------------------------
{
    if(((-1 == yellowStrict) || (0 == yellowStrict))
        && ((-1 == yellowMin) ||(0 == yellowMin)))
    {
        return true;
    }

    if((-1 != yellowStrict) && (0 != yellowStrict) && (p_yellowToFilter == yellowStrict))
    {
        return true;
    }
    else if((-1 != yellowMin) && (0 != yellowMin) && (p_yellowToFilter >= yellowMin))
    {
        return true;
    }

    return false;
}

//----------------------------------------------------
const bool Filter::isDistanceFilter(const int p_distanceToFilter) const
//----------------------------------------------------
{
    if((-1 == distance) || (0 == distance))
    {
        return true;
    }

    if((-1 != distance) && (p_distanceToFilter == distance))
    {
        return true;
    }

    return false;
}

//----------------------------------------------------
const bool Filter::isHorsesFilter(const std::array<int, 8>& p_horsesToFilter) const
//----------------------------------------------------
{
    for(int horse: horses)
    {
        if((-1 == horse) || (0 == horse))
        {
            qDebug() << "No horses filter found or incomplet filter, pass the filter";
            return true;
        }
    }

    for(const int horseToFilter: p_horsesToFilter)
    {
        bool horseFound = false;
        for(const int horse: horses)
        {
            if(horseToFilter == horse)
            {
                horseFound = true;
                break;
            }
        }
        if(!horseFound)
        {
            qDebug() << "At least one horse is not found, not pass the filter";
            return false;
        }
    }

    return true;
}

//----------------------------------------------------
const int RaceData::winnerRow() const
//----------------------------------------------------
{
    for(size_t row = 0; row < numbers.size(); ++row)
    {
        if(winner == numbers[row])
        {
            return row;
        }
    }

    return -1;
}

//----------------------------------------------------
const std::array<int, 24> RaceData::winnerDatas() const
//----------------------------------------------------
{
    std::array<int, 24> result = {-1, -1, -1, -1, -1, -1, -1, -1,
                                  -1, -1, -1, -1, -1, -1, -1, -1,
                                  -1, -1, -1, -1, -1, -1, -1, -1};
    for(size_t index = 0; index < colIndexes.size(); ++index)
    {
        const int colIndex = colIndexes[index];
        const int rowIndex = winnerRow();
        if(-1 == rowIndex)
        {
            return result;
        }
        result[index] = en2[rowIndex][colIndex].value;
        result[index + 8] = en3[rowIndex][colIndex].value;
        result[index + 16] = en2En3[rowIndex][colIndex].value;
    }

    return result;
}

//----------------------------------------------------
const std::array<int, 24> RaceData::totalDatas() const
//----------------------------------------------------
{
    std::array<int, 24> result = {-1, -1, -1, -1, -1, -1, -1, -1,
                                  -1, -1, -1, -1, -1, -1, -1, -1,
                                  -1, -1, -1, -1, -1, -1, -1, -1};
    
    for(size_t index = 0; index < colIndexes.size(); ++index)
    {
        const int colIndex = colIndexes[index];
        result[index] = en2[TOTAL_ROW_INDEX][colIndex].value;
        result[index + 8] = en3[TOTAL_ROW_INDEX][colIndex].value;
        result[index + 16] = en2En3[TOTAL_ROW_INDEX][colIndex].value;
    }
    return result;
}

//----------------------------------------------------
const std::array<CompareValue, 24> RaceData::convertValue() const
//----------------------------------------------------
{
    const int rowIndex = winnerRow();
    if(-1 == rowIndex)
    {
        return {};
    }

    std::array<CompareValue, 24> result = {};
    int index = 0;
    const int offset = 8;
    for(int colIndex: colIndexes)
    {
        en2En3Struct currentDataEn2 = en2[rowIndex][colIndex];
        result[index].color = Tools::convertStringToColor(currentDataEn2.color);
        en2En3Struct currentDataEn3 = en3[rowIndex][colIndex];
        result[index + offset].color = Tools::convertStringToColor(currentDataEn3.color);
        en2En3Struct currentDataEn2En3 = en2En3[rowIndex][colIndex];
        result[index + (2 * offset)].color = Tools::convertStringToColor(currentDataEn2En3.color);

        std::array<int, 8> allColDatasEn2 = {0};
        std::array<int, 8> allColDatasEn3 = {0};
        std::array<int, 8> allColDatasEn2En3 = {0};
        for(int i = 0; i < en2.size() - 1; ++i) // remove 1 to do not parse total row
        {
            allColDatasEn2[i] = en2[i][colIndex].value;
            allColDatasEn3[i] = en3[i][colIndex].value;
            allColDatasEn2En3[i] = en2En3[i][colIndex].value;
        }

        result[index].value = convertValueToOOrX(currentDataEn2.value, allColDatasEn2);
        result[index + offset].value = convertValueToOOrX(currentDataEn3.value, allColDatasEn3);
        result[index + (2 * offset)].value = convertValueToOOrX(currentDataEn2En3.value, allColDatasEn2En3);
        ++index;
    }

    return result;
}

//----------------------------------------------------
QString RaceData::convertValueToOOrX(const int p_value, const std::array<int, 8>& p_datas) const
//----------------------------------------------------
{
    std::array<int, 8> sorted = p_datas;
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    int count = 0;
    int lastValue = -1;
    int lastValueIndex = -1;
    for(const int value: sorted)
    {
        if(value == p_value)
        {
            if(4 > count)
            {
                return "O";
            }
            else if((lastValue == p_value) && (4 > lastValueIndex))
            {
                return "O";
            }
            return "X";
        }
        if(4 < count)
        {
            return "X";
        }
        if(lastValue != value)
        {
            lastValueIndex = count;
            lastValue = value;
        }
        count++;
    }

    return "X";
}

//----------------------------------------------------
const bool RaceData::isPassFilter(const int p_row, const int p_column) const
//----------------------------------------------------
{
    ERowArrayValue filter = static_cast<ERowArrayValue>(p_row);
    const std::array<CompareValue, 24> datas = convertValue();
    return filtered(filter, datas[p_column]);
}

//----------------------------------------------------
const bool RaceData::filtered(const ERowArrayValue& p_filter, const CompareValue& p_value) const
//----------------------------------------------------
{
    switch(p_filter)
    {
        case CircleArray :
            return (0 == p_value.value.compare("O", Qt::CaseInsensitive));
        case GreenArray:
            return (GreenColor == p_value.color);
        case BlueArray:
            return (BlueColor == p_value.color);
        case YellowArray:
            return (YellowColor == p_value.color);
        case OrangeArray:
            return (OrangeColor == p_value.color);
        case BlankArray:
            return (BlankColor == p_value.color);
        case UnknownRowArray:
            qWarning() << "Unknown filter: " << p_filter;
            return false;
    }
    return false;
}