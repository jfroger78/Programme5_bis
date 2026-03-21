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
#include "../model/constants.h"

class QStandardItemModel;

namespace Ui{
    class StatArray;
}

namespace view
{
    struct SFilters {
        EFilter filter1;
        EFilter filter2;
        EFilter filter3;
        EFilter filter4;

        QString filter1Str() {
            return convertFilterToStr(filter1);
        }

        QString filter2Str() {
            return convertFilterToStr(filter2);
        }

        QString filter3Str() {
            return convertFilterToStr(filter3);
        }

        QString filter4Str() {
            return convertFilterToStr(filter4);
        }

        private:
        QString convertFilterToStr(const EFilter& p_filter) {
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
                case FilterColorFull:
                    return "Color full";
                case FilterColor:
                    return "Color";
                case FilterNone:
                    return "";
                default:
                    qWarning() << "Unknown filter";
                    return "";
            }
        }
    };

    class StatArray: public QWidget
    {
        Q_OBJECT
        public:
            /**
             * @brief Constructor.
             * @param p_filters: The filters used in this stat array.
             * @param p_parent: The parent widget.
             */
            StatArray(const SFilters p_filters,
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

            /**
             * @brief Changes the filter associated text to the second filter.
             */
            void changeFilter2Text();

            /**
             * @brief Resets text to their defailt value for the second filter.
             */
            void reset2Text();

            /**
             * @brief Return the model of the array.
             * @param The abstractItemModel of the array.
             */
            QAbstractItemModel* model() const;

            /**
             * @brief Returns true if the second filter is activated, false otherwise.
             * @return True if the second filter is activated, false otherwise.
             */
            const bool isSecondFilterActivated() const;

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

            /**
             * @brief Changes the second filter for the statistics.
             */
            Q_SIGNAL void changeStatFilter2();

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
             * @param p_colIndex: The column index.
             * @param p_percents: The percents to display.
             * @param p_totals: Totals with which divided current percent.
             * @param p_model: The model in which display the percent.
             */
            void setPercent(const int p_colIndex,
                            const std::map<int, int>& p_percents,
                            const std::array<int, 24>& p_totals,
                            QStandardItemModel* p_model);

            /**
             * @brief Gets the little percent in each row of the model
             * @param p_model: The model to parse.
             */
            void getLittleOne(QStandardItemModel& p_model);
        private:
            Ui::StatArray *m_ui; //!< Pointer to the HMI.

            SFilters m_filters; //!< The filters used for this stat array.
    };
}

#endif // STAT_ARRAY_H
