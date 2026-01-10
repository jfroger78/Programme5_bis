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
    private:
        Ui::Display_BDD *m_ui; //!< Pointer to the user interface
};

#endif // DISPLAY_BDD_H