// Custom includes
#include "tools.h"

// Qt includes
#include <QDebug>
#include <QColor>
#include <QBrush>

//------------------------------------------------------------------------------
std::vector<EType> Tools::type(const int p_number)
//------------------------------------------------------------------------------
{
    std::vector<EType> types;
    if((is3(p_number)) && (is4(p_number)))
    {
        types.push_back(EType::is3);
        types.push_back(EType::is3Double);
        types.push_back(EType::is4Double);
    }
    else if((isP(p_number)) && (is3(p_number)))
    {
        types.push_back(EType::is3Double);
        types.push_back(EType::isP);
    }
    else if(is3(p_number))
    {
        types.push_back(EType::is3);
        types.push_back(EType::is3Double);
    }
    else if(is4(p_number))
    {
        types.push_back(EType::is4);
        types.push_back(EType::is4Double);
    }
    else if(isP(p_number))
    {
        types.push_back(EType::isP);
    }
    else
    {
        types.push_back(EType::isN);
    }

    return types;
}

//------------------------------------------------------------------------------
const std::string Tools::convertTypeToString(const EType p_type)
//------------------------------------------------------------------------------
{
    switch(p_type)
    {
        case EType::isP:
            return "P";
        case EType::is3:
            return " ";
        case is3Double:
            return "3";
        case EType::is4:
            return " ";
        case EType::is4Double:
            return "4";
        case EType::isN:
            return "N";
        default:
            qWarning() << "EType not treat.";
            break;
    }
    return "";
}

//------------------------------------------------------------------------------
bool Tools::isP(const uint16_t p_number)
//------------------------------------------------------------------------------
{
    if(0 == (p_number % 2)) return false;
    for(uint16_t i = 3; i* i <= p_number; i+= 2)
    {
        if(0 == (p_number % i)) return false;
    }
    return true;
}

//------------------------------------------------------------------------------
bool Tools::is3(const uint16_t p_number)
//------------------------------------------------------------------------------
{
    return (0 == (p_number % 3));
}

//------------------------------------------------------------------------------
bool Tools::is4(const uint16_t p_number)
//------------------------------------------------------------------------------
{
    return (0 == (p_number % 4));
}

//------------------------------------------------------------------------------
std::array<uint16_t, 8> Tools::removeDuplicatesAndSortDesc(const std::array<uint16_t, 8>& p_input)
//------------------------------------------------------------------------------
{
    std::array<uint16_t, 8> result = p_input;
    // Sort in descending order
    std::sort(result.begin(), result.end(), std::greater<uint16_t>());
    // Remove duplicate values
    auto last = std::unique(result.begin(), result.end());
    // Fill rest with 0 (if needed)
    std::fill(last, result.end(), 0);
    return result;
}

//------------------------------------------------------------------------------
std::string Tools::convertColorToString(const EColor& p_color)
//------------------------------------------------------------------------------
{
    switch(p_color)
    {
        case EColor::GreenColor:
            return "Green";
        case EColor::BlueColor:
            return "Blue";
        case EColor::YellowColor:
            return "Yellow";
        case EColor::OrangeColor:
            return "Orange";
        default:
            return "Blank";
    }
}

//------------------------------------------------------------------------------
const EColor Tools::convertStringToColor(const QString& p_color)
//------------------------------------------------------------------------------
{
    if(0 == p_color.compare("Green", Qt::CaseInsensitive))
    {
        return GreenColor;
    }
    else if(0 == p_color.compare("Blue", Qt::CaseInsensitive))
    {
        return BlueColor;
    }
    else if(0 == p_color.compare("Yellow", Qt::CaseInsensitive))
    {
        return YellowColor;
    }
    else if(0 == p_color.compare("Orange", Qt::CaseInsensitive))
    {
        return OrangeColor;
    }
    else
    {
        return BlankColor;
    }
}

//------------------------------------------------------------------------------
std::string Tools::convertColor(const std::string p_color)
//------------------------------------------------------------------------------
{
    if(GREEN_COLOR == p_color)
    {
        return "Green";
    }
    else if(BLUE_COLOR == p_color)
    {
        return "Blue";
    }
    else if(YELLOW_COLOR == p_color)
    {
        return "Yellow";
    }
    else if(ORANGE_COLOR == p_color)
    {
        return "Orange";
    }

    return "Blank";
}

//------------------------------------------------------------------------------
bool Tools::isGreen(const QVariant& p_background)
//------------------------------------------------------------------------------
{
    if(!p_background.canConvert<QBrush>())
    {
        return false;
    }
    QColor color = p_background.value<QBrush>().color();
    if(GREEN_COLOR == color.name().toStdString())
    {
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
const QString Tools::convertFilterToString(const EFilter& p_filter)
//------------------------------------------------------------------------------
{
    switch(p_filter)
    {
        case FilterDiscipline:
            return "L";
        case FilterLeaver:
            return "P";
        case FilterDistance:
            return "D";
        case FilterYellow:
            return "Y";
        case FilterLPJ:
            return "L+P+J";
        case FilterLP:
            return "L+P";
        case FilterLPD:
            return "L+P+D";
        case FilterNone:
            return "";
        default:
            qWarning() << "Unknown filter";
            return "";
    }
}