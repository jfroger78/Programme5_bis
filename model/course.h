#ifndef COURSE_MODEL_H
#define COURSE_MODEL_H

// C++ standard includes
#include <array>

// Custom includes
#include "constants.h"

namespace model {

    struct Filter
    {
        QString letterStrict = QString(); //!< Strict letter filter.
        QString letterMin = QString(); //!< Minimum letter filter.
        int nbrLeaverStrict = 0; //!< Strict number of leavers.
        int nbrLeaverMin = 0; //!< Minimum number of leavers.
        int yellowStrict = 0; //!< Strict yellow value.
        int yellowMin = 0; //!< Minimum yellow value.
        int distance = 0; //!< Distance filter.
        std::array<int, 8> horses; //!< Horses filter.

        /**
         * @brief Returns the letter filter and true if the strict letter is used.
         * @param p_letter The letter filter.
         * @return True if the strict letter is used, false otherwise.
         */
        const bool letterFilter(QString& p_letter) const
        {
            if(!letterStrict.isEmpty())
            {
                p_letter = letterStrict;
                return true;
            }
            else
            {
                p_letter = letterMin;
                return false;
            }
        }

        /**
         * @brief Returns the number of leavers filter and true if the strict number of leavers is used.
         * @param p_isStrict True if the strict number of leavers is used, false otherwise.
         * @return The number of leavers filter.
         */
        const int nbrLeaverFilter(bool& p_isStrict) const
        {
            if(nbrLeaverStrict > 0)
            {
                p_isStrict = true;
                return nbrLeaverStrict;
            }
            else
            {
                p_isStrict = false;
                return nbrLeaverMin;
            }
        }

        /**
         * @brief Returns the yellow filter and true if the strict yellow is used.
         * @param p_isStrict True if the strict yellow is used, false otherwise.
         * @return The yellow filter.
         */
        const int yellowFilter(bool& p_isStrict) const
        {
            if(yellowStrict > 0)
            {
                p_isStrict = true;
                return yellowStrict;
            }
            else
            {
                p_isStrict = false;
                return yellowMin;
            }
        }
    };

    class Course {
        public:
            /**
             * @brief Course Constructor
             * @param p_horses: Horses numbers.
             * @param p_winner: The winner number.
             * @param p_place1: The first place number.
             * @param p_place2: The second place number.
             * @param p_nbrLeaver: The number of leavers.
             * @param p_distance: The distance of the course.
             * @param p_discipline: The discipline of the course.
             */
            Course(const std::array<int, 8>& p_horses,
                   const int p_winner,
                   const int p_place1,
                   const int p_place2,
                   const int p_nbrLeaver,
                   const int p_distance,
                   const QString p_discipline);

            /** 
             * @brief Course Destructor
            */
            ~Course();

            /**
             * @brief Returns the horses numbers.
             * @return An array with all horses numbers.
             */
            const std::array<int, 8>& horses() const { return m_horses; }

            /**
             * @brief Returns the winner number.
             * @return The winner number.
             */
            const int winner() const { return m_winner; }

            /**
             * @brief Returns the first place number.
             * @return The first place number.
             */
            const int place1() const { return m_place1; }

            /**
             * @brief Returns the second place number.
             * @return The second place number.
             */
            const int place2() const { return m_place2; }

            /**
             * @brief Returns the number of leavers.
             * @return The number of leavers.
             */
            const int numberLeavers() const { return m_nbrLeaver; }

            /**
             * @brief Returns the distance of the course.
             * @return The distance of the course.
             */
            const int distance() const { return m_distance; }

            /**
             * @brief Returns the discipline of the course.
             * @return The discipline of the course.
             */
            const QString discipline() const { return m_discipline; }

            /**
             * @brief Checks if the course passes a specific filter.
             * @param p_filterValue: The filter to check against.
             * @param p_filterDatas: The filter data to apply.
             * @return True if the course passes the filter, false otherwise.
             */
            const bool passFilter(const EFilter p_filterValue, const Filter& p_filterDatas) const;;


        private:
            std::array<int, 8> m_horses; //!< Horses numbers
            int m_winner; //!< The winner number
            int m_place1; //!< The first place number
            int m_place2; //!< The second place number
            int m_nbrLeaver; //!< The number of leavers
            int m_distance; //!< The distance of the course
            QString m_discipline; //!< The discipline of the course
    };
}

#endif // COURSE_MODEL_H