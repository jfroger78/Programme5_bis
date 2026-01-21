#ifndef DISPLAY_BDD_H
#define DISPLAY_BDD_H

// Qt includes
#include <QWidget>

// Custom includes
#include "model/constants.h"

namespace Ui
{
    class Display_BDD;
}

namespace view
{
    struct DisplayBDDFilter{
        QString disciplineFilter = "";
        bool isDisciplineStrict = false;
        int nbrLeaverFilter = -1;
        bool isNbrLeaverStrict = false;
        int distanceFilter = -1;
        bool isDistanceStrict = false;

        /**
         * @brief Define the discipline filter and if it's strict or not.
         * @param p_discplineStrict: The value of the strict discipline filter.
         * @param p_discipineMin: The minimal value of the discipline filter.
         */
        void filterDiscipline(const QString& p_disciplineStrict,
                              const QString& p_discplineMin);

        /**
         * @brief Defines the number of leaver filter and if it's strict or not.
         * @param p_nbrLeaverStrict: The value of the strict number of leaver filter.
         * @param p_nbrLeaverMin: The minimal value of the number of leaver filter
         */
        void filterNbrLeaver(const int p_nbrLeaverStrict,
                             const int p_nbrLeaverMin);

        /**
         * @brief Defines the distance filter and if it's strict or not.
         * @param p_distanceStrict: The value of the strict distance filter.
         * @param p_distanceMin: The minimal value of the distance filter.
         */
        void filterDistance(const int p_distanceStrict,
                            const int p_distanceMin);
    };

    class DisplayBDD: public QWidget
    {
        Q_OBJECT;
        public:
            /**
             * @brief Constructor.
             */
            DisplayBDD(QWidget* p_parent = nullptr);

            /**
             * @brief Destructor.
             */
            ~DisplayBDD();

            /**
             * @brief Fills combinations.
             * @param p_combinations: The combinations list to add into the HMI.
             */
            void fillCombinations(const std::vector<CombinationToDisplay>& p_combinations);
        private:
            /**
             * @brief Initializes the UI.
             */
            void setupUi();

            /**
             * @brief Triggers when the user change the current selected combination.
             * @param p_index: The index of the selected item.
             */
            void onCombSelectionChanged(int p_index);

            /**
             * @brief Clears the course container.
             */
            void clearCourseContainer();

            /**
             * @brief Retrieves the filter associated to the bdd.
             */
            void retrieveFilter();

            /**
             * @brief Check if the race data pass the filter.
             * @param p_data: The data to test.
             * @return Trueif the data pass the filter, false otherwise.
             */
            const bool isFiltered(const RaceData& p_data) const;
        private:
            Ui::Display_BDD *m_ui; //!< Pointer to the user interface.
            DisplayBDDFilter m_bddFilter; //!< The filter associated datas.
    };
}

#endif // DISPLAY_BDD_H