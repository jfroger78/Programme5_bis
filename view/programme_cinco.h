#ifndef PROGRAMME_CINCO_H
#define PROGRAMME_CINCO_H

// Qt includes
#include <QMainWindow>

// Standard c++ includes
#include <map>
#include <array>

// Custom includes
#include "controller/fillBDD.h"
#include "display_BDD.h"

class Course; // Forward declaration of Course class

namespace Ui {
class Programme_cinco;
}

class Programme_cinco : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Programme_cinco Constructor
     * @param parent Parent widget
     */
    explicit Programme_cinco(QWidget *p_parent = nullptr);

    /**
     * @brief Destructor
     */
    ~Programme_cinco();

    /**
     * @brief Connects signals / slots.
     */
    void connection();

    /**
     * @brief Returns all the courses.
     * @return An array which contains all courses.
     */
    std::array<view::Course*, 20> courses();

    /**
     * @brief Returns the displayBDD HMI.
     * @return A ref to the displayBDD HMI.
     */
    DisplayBDD& displayBDD()
    {
        return m_displayBDD;
    }

public:
    /**
     * @brief Emits when big start is clicked.
     */
    Q_SIGNAL void bigStartClicked(bool p_isFillBDD);

    /**
     * @brief Emits when big clear is clicked.
     */
    Q_SIGNAL void bigClearClicked();

private:
    /**
     * @brief setupUi Initializes the user interface
     */
    void setupUi();

    /**
     * @brief createTab Creates a tab with the specified course number.
     * @param courseNumber The course number to create the tab for.
     * @param courseTab The course tab in which to add courses IHM.
     */
    void createTab(const int8_t &p_courseNumber, QWidget* p_courseTab);

    /**
     * @brief Triggers when user click on fill BDD button.
     */
    void onFillBDD();

private:
    Ui::Programme_cinco *m_ui; //!< Pointer to the user interface.
    std::array<view::Course*, 20> m_courses; //!< Array to store courses.
    FillBDD m_fillBDD; //!< Instance of FillBDD controller.
    DisplayBDD m_displayBDD; //!< Instance of the displayBDD HMI.
};

#endif // PROGRAMME_CINCO_H
