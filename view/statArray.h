#ifndef STAT_ARRAY_H
#define STAT_ARRAY_H

// Qt includes
#include <QString>
#include <QWidget>
#include <QModelIndexList>

// Standard C++ includes
#include <array>

// Custom includes
#include "../model/statistic.h"

class QStandardItemModel;

namespace Ui{
    class StatArray;
}

namespace view
{
    class StatArray: public QWidget
    {
        Q_OBJECT
        public:
            /**
             * @brief Constructor.
             * @param p_filter1: The name of the first filter.
             * @param p_filter2: The name of the second filter.
             * @param p_parent: The parent widget.
             */
            StatArray(const QString p_filter1,
                      const QString p_filter2,
                      QWidget* p_parent = nullptr);

            /**
             * @brief Destructor.
             */
            ~StatArray();

            /**
             * @brief Connects all signals slots.
             */
            void connection();

            /**
             * @brief Displays statistics datas.
             * @param p_datasToDisplay The data to display.
             */
            void displayDatas(const StatisticsData& p_datasToDisplay);

            /**
             * @brief Changes the filter associated text to the other filter.
             */
            void changeFilterText();

            /**
             * @brief Resets text to their default values.
             */
            void resetText();

        public:
            /**
             * @brief Starts the sub filters.
             * @param p_filterList: The list of cells selected by the user.
             */
            Q_SIGNAL void startSubFilter(const QModelIndexList& p_filterList);

            /**
             * @brief Reset all sub filters.
             */
            Q_SIGNAL void resetSubFilter();

            /**
             * @brief Changes the filter for the statistics.
             */
            Q_SIGNAL void changeStatFilter();

        private:
            /**
             * @brief Sets the circle percent into the standard item model.
             * @param p_rowIndex: The row index.
             * @param p_percents: The percents to display.
             * @param p_total: The total with which divided current percent.
             * @param p_model: The model in which display the percent.
             */
            void setPercent(const int p_rowIndex,
                            const std::array<int, 24>& p_percents,
                            const int p_total,
                            QStandardItemModel* p_model);

            
            /**
             * @brief Sets the circle percent into the standard item model.
             * @param p_rowIndex: The row index.
             * @param p_percents: The percents to display.
             * @param p_totals: Totals with which divided current percent.
             * @param p_model: The model in which display the percent.
             */
            void setPercent(const int p_rowIndex,
                            const std::array<int, 24>& p_percents,
                            const std::array<int, 24>& p_totals,
                            QStandardItemModel* p_model);
        private:
            Ui::StatArray *m_ui; //!< Pointer to the HMI.

            QString m_filter1; //!< The name of the first filter.
            QString m_filter2; //!< The name of the second filter.
    };
}

#endif // STAT_ARRAY_H
