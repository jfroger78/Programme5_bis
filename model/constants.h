#ifndef CONSTANTS_H
#define CONSTANTS_H

// Standard c++ includes
#include <cstdint>
#include <string>

// Qt includes
#include <QDebug>

static uint8_t const MIN_HORSE_NUMBER = 1; //!< Minimum horse number.
static uint8_t const MAX_HORSE_NUMBER = 18; //!< Maximum horse number.

static std::string const GREEN_COLOR = "#81d41a"; //!< Green color in hex format.
static std::string const BLUE_COLOR = "#3365a4"; //!< Blue color in hex format.
static std::string const YELLOW_COLOR = "#ffff00"; //!< Yellow color in hex format.
static std::string const ORANGE_COLOR = "#ff8d39"; //!< Orange color in hex format.
static std::string const BLANK_COLOR = "#ffffff"; //!< Blank color in hex format.

const std::array<int, 8> colIndexes = {5, 6, 7, 8, 9, 10, 4, 14};

enum EType
{
    TypeNone = 0,
    isP,            // 1
    is3Double,      // 2
    is3,            // 3
    is4Double,      // 4
    is4,            // 5
    isN,            // 6
    TypeCount
};

enum EValueType
{
    ValueTypeNone = 0,
    En2,
    En3
};

enum EColor
{
    None = 0,
    GreenColor,
    BlueColor,
    YellowColor,
    OrangeColor,
    BlankColor
};

enum EFilter
{
    FilterNone = 0,
    FilterDiscipline, // L
    FilterLeaver, // P
    FilterDistance, // D
    FilterYellow, // J
    FilterLPJ, // Discipine + Leaver + Yellow
    FilterLP, // Discipline + Leaver
    FilterLPD // Discipline + Leaver + Distance
};

struct Filter
{
    QString letterStrict = QString(); //!< Strict letter filter.
    QString letterMin = QString(); //!< Minimum letter filter.

    /**
     * @brief Returns true if the letter passes the filter, false otherwise.
     * @param p_letterToFilter: The letter to test if it passes the filter.
     * @return True if it passes the filter, false otherwise.
     */
    const bool isLetterFilter(const QString& p_letterToFilter) const;

    int nbrLeaverStrict = -1; //!< Strict number of leaver filter.
    int nbrLeaverMin = -1; //!< Minimum number of leaver filter.

    /**
     * @brief Returns true if the number of leaver passes the filter, false otherwise.
     * @param p_nbrLeaverToFilter: The number of leaver to test if it passes the filter.
     * @return True if it passes the filter, false otherwise.
     */
    const bool isNbrLeaverFilter(const int p_nbrLeaverToFilter) const;

    int yellowStrict = -1; //!< Strict yellow filter.
    int yellowMin = -1; //!< Minimum yellow filter.

    /**
     * @brief Returns true if the yellow number passes the filter, false otherwise.
     * @param p_yellowToFilter: The yellow number to test if it passes the filter.
     * @return True if it passes the filter, false otherwise.
     */
    const bool isYellowFilter(const int p_yellowToFilter) const;

    int distance = -1; //!< Strict distance filter.

    /**
     * @brief Returns true if the distance passes the filter, false otherwise.
     * @param p_distanceToFilter: The distance value to test if it passes the filter.
     * @return True if it passes the filter, false otherwise.
     */
    const bool isDistanceFilter(const int p_distanceToFilter) const;

    std::array<int, 8> horses = {-1, -1, -1, -1, -1, -1, -1, -1}; //!< Strict horses filter

    /**
     * @brief Returns true if the horses pass the filter, false otherwiwe.
     * @param p_horsesToFilter: Horses to test if they pass the filter.
     * @return True if theyr pass the filter, false otherwise.
     */
    const bool isHorsesFilter(const std::array<int, 8>& p_horsesToFilter) const;

};

struct CombinationToDisplay
{
    int id;
    std::string combination;
};

struct en2En3Struct
{
    QString color;
    int value;
};

struct CompareValue
{
    QString value = "";
    EColor color = EColor::BlankColor;
};

enum ERowArrayValue
{
    CircleArray = 0,
    GreenCircleArray,
    GreenArray,
    GreenWinner,
    BlueArray,
    YellowArray,
    OrangeArray,
    BlankArray,
    UnknownRowArray
};

struct RaceData
{
    std::array<int, 8> numbers;
    int winner;
    int place1;
    int place2;
    int nbrLeaver;
    int distance;
    QString discipline;
    int id = 0;

    std::array<std::array<en2En3Struct, 16>, 9> en2;
    std::array<std::array<en2En3Struct, 16>, 9> en3;
    std::array<std::array<en2En3Struct, 16>, 9> en2En3;

    /**
     * @brief Returns the index corresponding to the winner row.
     * @return The winner row.
     */
    const int winnerRow() const;

    /**
     * @brief Returns the datas which are contain in the winner data.
     * @return The datas which are contain in the winner data, -1 if the winner is not found.
     */
    const std::array<int, 24> winnerDatas() const;

    /**
     * @brief Returns the datas which correspond to the total row.
     * @return An array which contain the total row datas.
     */
    const std::array<int, 24> totalDatas() const;

    /**
     * @brief Convert winner datas to O and X symbol.
     * @return The winner row converted in O and X.
     */
    const std::array<CompareValue, 24> convertValue() const;

    /**
     * @brief Convert a value in O and X.
     * @param p_value: The value to convert.
     * @param p_datas: The rest of the datas contain in the column.
     * @return The converted value.
     */
    QString convertValueToOOrX(const int p_value, const std::array<int, 8>& p_datas) const;

    /**
     * @brief Checks if the winner datas pass the sub filter.
     * @param p_row: The selected row for the filter.
     * @param p_column: The selected column for the filter.
     * @return True if it passes the filter, false otherwise.
     */
    const bool isPassFilter(const int p_row, const int p_column) const;

    /**
     * @brief Checks if the value corresponding to the filter.
     * @param p_filter: The filter to pass.
     * @param p_value: The value to test.
     * @return True if the value passes the filter, false otherwise.
     */
    const bool filtered(const ERowArrayValue& p_filter, const CompareValue& p_value) const;
};

struct en2En3Array
{
    std::array<en2En3Struct, 16> en2;
    std::array<en2En3Struct, 16> en3;
    std::array<en2En3Struct, 16> en2En3;
};

#endif // CONSTANTS_H