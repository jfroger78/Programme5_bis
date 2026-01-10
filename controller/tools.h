#ifndef TOOLS_H
#define TOOLS_H

// Standard C++ includes
#include <string>
#include <vector>

// Custom includes
#include "model/constants.h"

class Tools
{
public:
    /**
     * @brief Convert hexa string color into string color.
     * @param p_color: The color in hexa version.
     * @return The color in string format.
     */
    static std::string convertColor(const std::string p_color);

    /**
     * @brief Returns the types of a number based on its divisibility by 3 and 4.
     * @param p_number: The number to test.
     * @return A vector of EType representing the types of the number.
     * If the number is divisible by both 3 and 4, it returns both is3Double and is4Double.
     * If the number is a prime number and divisible by 3, it returns is3Double.
     * If the number is a prime number, it returns isP.
     * If the number is divisible by 3, it returns is3Double.
     * If the number is divisible by 4, it returns is4Double.
     * If the number is neither divisible by 3, 4 nor prime number, it returns isN.
     */
    static std::vector<EType> type(const int p_number);

    /**
     * @brief Converts the EType enum value to a string.
     * @param p_type: The type to convert.
     * @return The type in string format.
     */
    static const std::string convertTypeToString(const EType p_type);

    /**
     * @brief Returns true if p_number is a prime number.
     * @param p_number: The number to test.
     * @return True if it's a prime number, false otherwise.
     */
    static bool isP(const uint16_t p_number);

    /**
     * @brief Returns true if p_number is divided by 3.
     * @param p_number: The number to test.
     * @return True if p_number is divided by 3, false otherwise.
     */
    static bool is3(const uint16_t p_number);

    /**
     * @brief Returns true if p_number is divided by 4.
     * @param p_number: The number to test.
     * @return True if p_number is divided by 4, false otherwise.
     */
    static bool is4(const uint16_t p_number);

    /**
     * @brief Remove duplicates from an array and sort it in descending order.
     * @param p_input: The input array of 8 uint16_t values.
     * @return A new array with duplicates removed and sorted in descending order.
     */
    static std::array<uint16_t, 8> removeDuplicatesAndSortDesc(const std::array<uint16_t, 8>& p_input);

    /**
     * @brief Converts the color enum value into a string.
     * @param p_color The color to convert.
     * @return The string value.
     */
    static std::string convertColorToString(const EColor& p_color);

    /**
     * @brief Converts the string to the color enum.
     * @param p_color The color string to convert.
     * @return The enum value.
     */
    static const EColor convertStringToColor(const QString& p_color);

    /**
     * @brief Return true if the input color correspond to the green color.
     * @param p_background The background to test.
     * @return True if the color is green, false otherwise.
     */
    static bool isGreen(const QVariant& p_background);

    /**
     * @brief Converts filter enum value into string.
     * @param p_filter The filter enum value to convert.
     * @return A string corresponding to the filter enum valu.
     */
    static const QString convertFilterToString(const EFilter& p_filter);
};

#endif // TOOLS_H