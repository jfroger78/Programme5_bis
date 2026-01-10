#ifndef EN2_EN3_H
#define EN2_EN3_H

// Standard c++ includes
#include <array>
#include <cstdint>

// Custom includes
#include "model/constants.h"
#include "model/horse.h"

// Qt includes
#include <QObject>

class En2En3: public QObject
{
    Q_OBJECT
    public:
        /**
         * @brief Constructor.
         * @param p_horses: Horses numbers.
         */
        En2En3(const std::array<int, 8> p_horsesNumber);

        /**
         * @brief Destructor.
         */
        ~En2En3();

        /**
         * @brief Returns the horses datas.
         * @return The horses datas.
         */
        inline const std::array<Horse*, 8> horses()
        {
            return m_horses;
        }

        /**
         * @brief Returns the totals of the column for en2.
         * @return The totals of the column.
         */
        inline std::array<int, 16> columnsTotalEn2()
        {
            return m_columnsTotalEn2;
        }

        /**
         * @brief Returns the totals of the column for en3.
         * @return The totals of the column for en3.
         */
        inline std::array<int, 16> columnsTotalEn3()
        {
            return m_columnsTotalEn3;
        }

        /**
         * @brief Returns the totals of the column for en2En3.
         * @return The totals of the column for en2En3.
         */
        inline std::array<int, 16> columnsTotalEn2En3()
        {
            return m_columnsTotalEn2En3;
        }
    private:
        /**
         * @brief Initializes the horses numbers.
         * @param p_horsesNumbers An array containing the horses numbers.
         */
        void initHorses(const std::array<int, 8>& p_horsesNumbers);

        /**
         * @brief Launches the start of the data computation.
         */
        void en2En3Computation();

        /**
         * @brief Computes the value for the horses number.
         * @param p_horses A part of horses informations
         */
        void computeValue(std::array<Horse*, 4>& p_horses);
        
        /**
         * @brief COmputes the value for en2.
         * @param p_horse1 The first horse structure.
         * @param p_horse2 The second horse structure.
         */
        void computeValueEn2(Horse* p_horse1,
                             Horse* p_horse2);

        /**
         * @brief Computes the value for en3.
         * @param p_horse1 The first horse structure.
         * @param p_horse2 The second horse structure.
         * @param p_horse3 The third horse structure.
         * @param p_horse4 The fourth horse structure.
         */
        void computeValueEn3(Horse* p_horse1,
                             Horse* p_horse2,
                             Horse* p_horse3,
                             Horse* p_horse4);

        /**
         * @brief Computes the total of each columns in the order.
         */
        void computeColumnsTotal();

    private:
        std::array<Horse*, 8> m_horses; //!< Horses data.
        std::array<int, 16> m_columnsTotalEn2; //!< Column total for en2.
        std::array<int, 16> m_columnsTotalEn3; //!< Column total for en3.
        std::array<int, 16> m_columnsTotalEn2En3; //!< Column total for en2En3.
};

#endif // EN2_EN3_H