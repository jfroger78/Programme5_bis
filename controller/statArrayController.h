#ifndef STAT_ARRAY_CONTROLLER_H
#define STAT_ARRAY_CONTROLLER_H

// Qt includes
#include <QString>
#include <QObject>
#include <QModelIndexList>

// C++ Standard includes
#include <vector>

// Custom includes
#include "../model/constants.h"
#include "../view/statArray.h"

namespace controller
{
    class StatArrayController: public QObject
    {
        Q_OBJECT
        public:
            /**
             * @brief Constructor.
             * @param p_filter1: The filter associated to this array.
             * @param p_filter2: The second filter associated to this array (optional).
             */
            StatArrayController(const EFilter p_filter1,
                                const EFilter p_filter2 = EFilter::FilterNone);

            /**
             * @brief Destructor.
             */
            ~StatArrayController();

            /**
             * @brief Sets the full datas from DB.
             * @param p_fullDatas: The full datas from DB.
             */
            void setFullDatas(const std::vector<RaceData> p_fullDatas);

            /**
             * @brief Sets the current race datas.
             * @param p_currentRaceDatas: The new current race datas.
             */
            void setCurrentRaceDatas(const std::array<int, 24> p_currentRaceDatas);

            /**
             * @brief Return a reference to the HMI.
             * @return A reference to the HMI.
             */
            view::StatArray& statArrayHMI();

            /**
             * @brief Changes the filter for the statistics.
             * @param p_filter The race filter.
             */
            void changeStatFilter(const Filter& p_filter);

            /**
             * @brief Sets the change filter connected value.
             * @param p_isChangeFilterConnected: The new change filter connected value.
             */
            void setChangeFilterConnected(const bool p_isChangeFilterConnected)
            {
                m_isChangeFilterConnected = p_isChangeFilterConnected;
            }

            /**
             * @brief Returns the change filter connected value.
             * @return The m_isChangeFilterConnected value.
             */
            const bool isChangeFilterConnected() const
            {
                return m_isChangeFilterConnected;
            }

        public:
            //-----------------------------
            //          SLOTS
            //-----------------------------
            /**
             * @brief Slots triggers when user do a start.
             * @note For click on button to change filter use the onChangeFilter function.
             * @param p_filter: The filter fills by the user.
             */
            void onStartFilter(const Filter& p_filter);
        
        private:
            /**
             * @brief Filters full data from filter1.
             * @param p_filter: The applied filter.
             */
            void filteredDatas(const Filter& p_filter,
                               const EFilter& p_filterType);
            
            /**
             * @brief Returns true if the race passes the filter.
             * @param p_filter: The filter fills by the user.
             * @param p_letterToFilter: The letter data from the current race.
             * @param p_nbrLeaverToFilter: The number of leaver from the current race.
             * @return True if the race passes the filter, false otherwise.
             */
            const bool isFilterLP(const Filter& p_filter,
                                  const QString& p_letterToFilter,
                                  const int p_nbrLeaverToFilter);
            
            /**
             * @brief Returns true if the race passes the filter.
             * @param p_filter: The filter fills by the user.
             * @param p_letterToFilter: The letter data from the current race.
             * @param p_nbrLeaverToFilter: The number of leaver from the current race.
             * @param p_yellowToFilter: The number of yellow from the current race.
             * @return True if the race passes the filter, false otherwise.
             */
            const bool isFilterLPJ(const Filter& p_filter,
                                   const QString& p_letterToFilter,
                                   const int p_nbrLeaverToFilter,
                                   const int p_yellowToFilter);
            
            /**
             * @brief Returns true if the race passes the filter.
             * @param p_filter: The filter fills by the user.
             * @param p_letterToFilter: The letter data from the current race.
             * @param p_nbrLeaverToFilter: The number of leaver from the current race.
             * @param p_distanceToFilter: The distance value from the current race.
             * @return True if the race passes the filter, false otherwise.
             */
            const bool isFilterLPD(const Filter& p_filter,
                                   const QString& p_letterToFilter,
                                   const int p_nbrLeaverToFilter,
                                   const int p_distanceToFilter);

            /**
             * @brief Launches the LPJ filter.
             * @param p_filter: The filter fills by the user.
             * @param p_currentRace: The data from the current race.
             * @return A filtered list of RaceData.
             */
            const std::vector<RaceData> startLPJFilter(const Filter& p_filter);

            /**
             * @brief Launches the LP filter.
             * @param p_filter: The filter fills by the user.
             * @return A filtered list of RaceData.
             */
            const std::vector<RaceData> startLPFilter(const Filter& p_filter);

            /**
             * @brief Launches the LPD filter.
             * @param p_filter: The filter fills by the user.
             * @return A filtered list of RaceData.
             */
            const std::vector<RaceData> startLPDFilter(const Filter& p_filter);

            /**
             * @brief Computes the number of yellow from the current DB race.
             * @param p_currentRace: The race fills by the user.
             * @param p_dataFromDB: The race extract from the DB.
             * @return The number of yellow.
             */
            const int computeYellow(const std::array<int, 24>& p_currentRace,
                                    const std::array<int, 24> p_dataFromDB);

            /**
             * @brief Generates the statistics data.
             * @param p_filteredDatas: The datas which pass filters.
             * @return The statistics.
             */
            const StatisticsData generateStatistics(const std::vector<RaceData>& p_filteredDatas);

            /**
             * @brief Computes the number winner green compare to the total of green in the column.
             * @param p_data: The data of the race.
             * @param p_statistics: The statistics to fill.
             */
            void computeGreenWinner(const RaceData& p_data,
                                    StatisticsData& p_statistics);
        private:
            //-----------------------------
            //          SLOTS
            //-----------------------------
            /**
             * @brief Triggers when user click on start button.
             * @param p_filterlist: The list of selected filters.
             */
            void onStartSubFilter(const QModelIndexList& p_filterList);

            /**
             * @brief Triggers when user click on reset button.
             */
            void onResetSubFilter();
        private:
            std::vector<RaceData> m_fullDatas; //!< The full datas extract from DB.
            std::vector<RaceData> m_currentFilteredDatas; //!< The datas filter by the current used filter.

            EFilter m_currentFilterType; //!< The current used filter to this array.
            EFilter m_filter1; //!< The first filter associated to this array.
            EFilter m_filter2; //!< The second filter associated to this array.

            view::StatArray m_statArray; //!< The HMI corresponding to the data.

            std::array<int, 24> m_currentRaceDatas; //!< The current race datas.

            bool m_isChangeFilterConnected; //!< True if the change filter is already connected, false otherwise.
    };
}

#endif // STAT_ARRAY_CONTROLLER_H