#ifndef FILL_BDD_H
#define FILL_BDD_H

// Qt includes
#include <QObject>

// Standard c++ includes
#include <array>

namespace view {
    class Course;
}

class FillBDD: public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Default constructor.
     * @param p_parent Parent object for QObject hierarchy.
     */
    FillBDD(QObject* p_parent = nullptr);

    /**
     * @brief Default destructor.
     */
    ~FillBDD();

    /**
     * @brief Triggers when user click on fill bdd button.
     */
    void onStart();

    /**
     * @brief Sets the courses to be filled.
     * @param p_courses Array of Course pointers to be filled.
     */
    void setCourses(std::array<view::Course*, 20>& p_courses)
    {
        m_courses = p_courses;
    }

    /**
     * @brief Start race algorithm.
     * @param True if this signal is send by using fill BDD
     */
    Q_SIGNAL void bigStartClicked(bool p_isFillBDD = false);

    /**
     * @brief Clear IHM
     */
    Q_SIGNAL void bigClearClicked();

private:
    /**
     * @brief Fills race data from a file.
     * @param p_fileName Name of the file to read.
     */
    void fillRace(const QString& p_fileName);

private:
    std::array<view::Course*, 20> m_courses; ///< Array of courses to be filled.
};

#endif // FILL_BDD_H