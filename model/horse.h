#ifndef HORSE_H
#define HORSE_H 

// Standard C++ includes
#include <array>
#include <map>

// Custom includes
#include "constants.h"

class Horse
{
    public:
        /**
         * @brief Horse Default Constructor.
         * Initializes the horse with a default number of 0.
         */
        Horse();

        /**
         * @brief Horse Constructor.
         * @param p_number The number of the horse.
         */
        Horse(const int p_number);

        /**
         * @brief Horse Copy Constructor.
         * @param p_horse The horse to copy.
         */
        Horse(const Horse& p_horse);

        /**
         * @brief Horse Destructor.
         */
        ~Horse() = default;

        /**
         * @brief Initializes the horse data.
         */
        inline void init();

        /**
         * @brief Returns the horse number.
         * @return The number of the horse.
         */
        inline int number() const
        {
            return m_number;
        }

        /**
         * @brief Returns the value of the horse for en2.
         * @return A map containing the value of the horse for en2.
         */
        inline const std::map<EType, int>& typesValueEn2() const
        {
            return m_typesValueEn2;
        }

        /**
         * @brief Returns the value of the horse for en3.
         * @return A map containing the value of the horse for en3.
         */
        inline const std::map<EType, int>& typesValueEn3() const
        {
            return m_typesValueEn3;
        }

        /**
         * @brief Returns the value of the horse for en2 and en3.
         * @return A map containing the value of the horse for en2 and en3.
         */
        inline const std::map<EType, int>& typesValueEn2En3() const
        {
            return m_typesValueEn2En3;
        }

        /**
         * @brief Returns the row totals for en2.
         * @return An array containing the row totals for en2.
         */
        inline const std::array<int, 7>& rowTotalsEn2() const
        {
            return m_rowTotalsEn2;
        }

        /**
         * @brief Returns the row totals for en3.
         * @return An array containing the row totals for en3.
         */
        inline const std::array<int, 7>& rowTotalsEn3() const
        {
            return m_rowTotalsEn3;
        }

        /**
         * @brief Returns the row totals for en2 and en3.
         * @return An array containing the row totals for en2 and en3.
         */
        inline const std::array<int, 7>& rowTotalsEn2En3() const
        {
            return m_rowTotalsEn2En3;
        }

        /**
         * @brief Increments the value of the horse for a specific type and value type.
         * @param p_type The type to increment.
         * @param p_valueType The value type (en2 or en3).
         */
        void incrementTypesValue(const EType& p_type, const EValueType& p_value);

    private:
        /**
         * @brief Adds the type into the total row.
         * @param p_type The type to add.
         * @param p_totalRow The total row to update.
         */
        void addIntoTotal(const EType& p_type, std::array<int, 7>& p_totalRow);

    private:
        int m_number; //!< The number of the horse.
        std::map<EType, int> m_typesValueEn2; //!< The value of the horse for en2.
        std::map<EType, int> m_typesValueEn3; //!< The value of the horse for en3.
        std::map<EType, int> m_typesValueEn2En3; //!< The value of the horse for en2 and en3.
        std::array<int, 7> m_rowTotalsEn2; //!< The row totals for en2.
        std::array<int, 7> m_rowTotalsEn3; //!< The row totals for en3.
        std::array<int, 7> m_rowTotalsEn2En3; //!< The row totals for en2 and en3.
};
#endif // HORSE_H
