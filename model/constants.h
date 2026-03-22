#ifndef CONSTANTS_H
#define CONSTANTS_H

// Standard c++ includes
#include <cstdint>
#include <string>

// Qt includes
#include <QDebug>

static uint const MIN_HORSE_NUMBER = 1; //!< Minimum horse number.
static uint const MAX_HORSE_NUMBER = 18; //!< Maximum horse number.

static QString const GREEN_COLOR = "#81d41a"; //!< Green color in hex format.
static QString const BLUE_COLOR = "#3365a4"; //!< Blue color in hex format.
static QString const YELLOW_COLOR = "#ffff00"; //!< Yellow color in hex format.
static QString const ORANGE_COLOR = "#ff8d39"; //!< Orange color in hex format.
static QString const BLANK_COLOR = "#ffffff"; //!< Blank color in hex format.

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
    FilterLPD, // Discipline + Leaver + Distance
    FilterColor,
    FilterColorFull
};

struct NumberValue {
    std::map<int, int> numbersValue;
    
    void numberData(const int p_value) {
        std::map<int, int>::iterator it = numbersValue.find(p_value);
        if(it != numbersValue.end()) {
            it->second++;
        } else {
            numbersValue.insert(std::pair<int, int>(p_value, 1));
        }
    }
};

struct Filter
{
    QString letterStrict = QString(); //!< Strict letter filter.
    QString letterMin = QString(); //!< Minimum letter filter.

    std::map<int, NumberValue> numberFilter;

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
    public:
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
    
    public:
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
         * @param p_isConverted: True of the conversion is well done, false otherwise.
         * @return The winner row converted in O and X.
         */
        const std::array<CompareValue, 24> convertValue(bool& p_isConverted) const;

        /**
         * @brief Convert a value in O and X.
         * @param p_value: The value to convert.
         * @param p_datas: The rest of the datas contain in the column.
         * @return The converted value.
         */
        QString convertValueToOOrX(const int p_value, const std::array<int, 8>& p_datas) const;

        /**
         * @brief Checks if the winner datas pass the sub filter.
         * @param p_value: The value to check if winner pass data.
         * @param p_column: The selected column for the filter.
         * @return True if it passes the filter, false otherwise.
         */
        const bool isWinnerPassFilter(const int p_value, const int p_column) const;

        /**
         * @brief Checks if the value corresponding to the filter.
         * @param p_filter: The filter to pass.
         * @param p_value: The value to test.
         * @return True if the value passes the filter, false otherwise.
         */
        const bool filtered(const ERowArrayValue& p_filter, const CompareValue& p_value) const;

        /**
         * @brief Check if the input color exist in the input column.
         * @param p_color: The color to search.
         * @param p_column: The column index in which search.
         * @return True if the color is find, false otherwise.
         */
        const bool colorInColumn(const QString p_color, const int p_column) const;

        /**
         * @brief Compute the number of each number for each column.
         * @return A map by number of it's number by column.
         */
        const std::map<int, NumberValue> numberOfNumbers() const;

    private:
        /**
         * @brief Converts a color into it's hexa value.
         * @param p_color: The color name to convert.
         * @return The hexa value of the color.
         */
        const QString convertColor(const QString& p_color) const;

};

struct en2En3Array
{
    std::array<en2En3Struct, 16> en2;
    std::array<en2En3Struct, 16> en3;
    std::array<en2En3Struct, 16> en2En3;
};

#endif // CONSTANTS_H