#ifndef COURSE_H
#define COURSE_H

// Qt includes
#include <QWidget>
#include <QStandardItem>

// Custom includes
#include "model/constants.h"
#include "model/horse.h"
#include "model/statistic.h"

// Standard C++ includes
#include <optional>

namespace Ui {
    class Course;
}

namespace view {
    struct Combination
    {
        std::array<char, 3> en2;
        std::array<char, 3> en3;
        std::array<char, 3> en2En3;
    };

    struct Totals
    {
        std::array<int, 16> en2;
        std::array<int, 16> en3;
        std::array<int, 16> en2En3;
    };

    class Course : public QWidget
    {
        Q_OBJECT

    public:
        /**
         * @brief Course Constructor
         * @param p_courseName Name of the course
         * @param parent Parent widget
         */
        explicit Course(const std::string &p_courseName,
                        QWidget *parent = nullptr);

        /**
         * @brief Destructor
         */
        ~Course();

        /**
         * @brief Emits when the start button is clicked.
         */
        Q_SIGNAL void startClicked(const bool p_isFullStart = true);

        /**
         * @brief Emits when the clear button is clicked.
         */
        Q_SIGNAL void clearClicked();

        /**
         * @brief Returns all user numbers.
         * @return An array with all numbers fill by the user.
         */
        const std::array<int, 8> numbers() const;

        /**
         * @brief Returns the course name.
         * @return The string corresponding to the course name.
         */
        std::string name();

        /**
         * @brief Returns the number of the winner.
         * @return The number of the winner.
         */
        const int winner() const;

        /**
         * @brief Returns the number of the place1.
         * @return The number of the place1.
         */
        const int place1() const;

        /**
         * @brief Returns the number of the place2.
         * @return The number of the place2.
         */
        const int place2() const;

        /**
         * @brief Returns the value of the discipline.
         * @return The value of the discipline.
         */
        const QString discipline() const;

        /**
         * @brief Returns the number of leaver.
         * @return The number of leaver.
         */
        const int numberLeavers() const;

        /**
         * @brief Returns the distance of the race.
         * @return The distance of the race.
         */
        const int distance() const;

        /**
         * @brief Fills the en2 and en3 values from the user input.
         * @param p_horses: Horses data to display.
         * @param p_totalsEn2: Totals for en2.
         * @param p_totalsEn3: Totals for en3.
         * @param p_totalsEn2En3: Totals for en2En3.
         */
        void fillEn2En3(const std::array<Horse*, 8>& p_horses,
                        const std::array<int, 16>& p_totalsEn2,
                        const std::array<int, 16>& p_totalsEn3,
                        const std::array<int, 16>& p_totalsEn2En3);

        /**
         * @brief Display the combintion values.
         * @param p_combination: The BDD combination for en2, en3, en2En3.
         * @param p_totals: The total of each column for the 3 array.
         */
        void fillCombination(const Combination& p_combination,
                            const Totals& p_totals);

        /**
         * @brief Clear the IHM.
         */
        void clear();

        /**
         * @brief Checks if the course is valid.
         * @return True if the course is valid, false otherwise.
         */
        bool isValid();

        /**
         * @brief Returns the model associated to the en2 array.
         * @return The model associated to the en2 array.
         */
        QAbstractItemModel* modelEn2() const;

        /**
         * @brief Returns the model associated to the en3 array.
         * @return The model associated to the en3 array.
         */
        QAbstractItemModel* modelEn3() const;

        /**
         * @brief Returns the model associated to the en2En3 array.
         * @return The model associated to the en2En3 array.
         */
        QAbstractItemModel* modelEn2En3() const;

        /**
         * @brief Fills the courses with the given data.
         * @param p_numbers The numbers of the horses.
         * @param p_winner The number of the winner.
         * @param p_place1 The number of the first place.
         * @param p_place2 The number of the second place.
         * @param p_discipline The discipline of the course.
         * @param p_numberLeavers The number of leavers.
         * @param p_distance The distance
         */
        void fillCourses(const std::array<int, 8>& p_numbers,
                        const int& p_winner,
                        const int& p_place1,
                        const int& p_place2,
                        const QString& p_discipline,
                        const int& p_numberLeavers,
                        const int& p_distance);

        /**
         * @brief Returns the letter strict.
         * @return The letter strict.
         */
        const QString letterStrict() const;

        /**
         * @brief Returns the letter min.
         * @return The letter min.
         */
        const QString letterMin() const;

        /**
         * @brief Returns the number of leaver strict.
         * @return The number of leaver strict.
         */
        const int nbrLeaverStrict() const;

        /**
         * @brief Returns the number of leaver min.
         * @return The number of leaver min.
         */
        const int nbrLeaverMin() const;

        /**
         * @brief Returns the yellow strict.
         * @return The yellow strict.
         */
        const int yellowStrict() const;

        /**
         * @brief Returns the yellow min.
         * @return The yellow min.
         */
        const int yellowMin() const;

        /**
         * @brief Returns the distance filter.
         * @return The distance filter.
         */
        const int distanceFilter() const;

        /**
         * @brief Returns the horses filter.
         * @return The horses filter.
         */
        const std::array<int, 8> horsesFilter() const;

        /**
         * @brief Returns the combination in string format.
         * @return The combination in string format.
         */
        const std::array<std::string, 9> combinationString() const;

        /**
         * @brief Returns true if the BDD results should be filtered by combination.
         * @return True if the BDD results should be filtered by combination, false otherwise.
         */
        const bool isFilterByCombination() const;

        /**
         * @brief Add a result array into the HMI.
         * @param p_resultArray: The HMI which contains the result array.
         * @param p_row: The row index of the array in the HMI.
         * @param p_column: The column index of the array in the HMI.
         * @param p_rowSpan: The row span of the array in the HMI.
         * @param p_colSpan: The col span of the array in the HMI.
         */
        void addResultArray(QWidget* p_resultArray,
                            const int p_row,
                            const int p_column,
                            const int p_rowSpan,
                            const int p_colSpan);

        /**
         * @brief Returns the totality of the total row
         * @return An array which contain the totality of the total row.
         */
        std::array<int, 48> totalRow()
        {
            return m_totalRow;
        }

    private:
        /**
         * @brief Connects the user interface elements to their respective slots
         */
        void connectIHM();
        
        /**
         * @brief Initializes the user interface
         */
        void setupUi();

        /**
         * @brief Validates the input value
         * @param p_value The value to check
         */
        void onCheckValue(const QString &p_value);

        /**
         * @brief Writes column total.
         * @param p_totals: The total to write.
         * @param p_row: The row where to write the total.
         * @param p_model: The model where to write the total.
         */
        void writeColumnTotal(const std::array<int, 16>& p_totals,
                            const int p_row,
                            QStandardItemModel* p_model);

        /**
         * @brief Creates the type header for the model.
         * @param p_model: The model where to create the type header.
         */
        void createTypeHeader(QStandardItemModel* p_model);

        /**
         * @brief Colorizes the array based on the values.
         * @param p_model: The model containing the data to colorize.
         */
        void colorizeArray(QStandardItemModel* p_model);

        /**
         * @brief Displays the values used to create the combination.
         * @param p_totals The values used to create the combination.
         * @param p_model The model use in the view to display datas.
         */
        void displayCombinationBasedValues(const Totals& p_totals,
                                        QStandardItemModel* p_model);

        /**
         * @brief Displays the value of one type in the model.
         * @param p_columnIndex The index of the column to display.
         * @param p_valueIndex The index of the value to display.
         * @param p_totals The totals used to display the value.
         * @param p_model The model where to display the value.
         */
        void displayOneTypeValue(const int p_columnIndex,
                                const int p_valueIndex,
                                const Totals& p_totals,
                                QStandardItemModel* p_model);

        /**
         * @brief Displays the combination in the model.
         * @param p_combination The combination to display.
         * @param p_model The model where to display the combination.
         */
        void displayCombination(const Combination& p_combination,
                                QStandardItemModel* p_model);

    private:
        Ui::Course *m_ui;//!< Pointer to the user interface
        std::string m_courseName; //!< Name of the course
        Combination m_combination; //!< The race combination.
        std::array<int, 48> m_totalRow; //!< The totality of the total row.
    };
};

#endif // COURSE_H
