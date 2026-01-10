#ifndef RACE_CONTROLLER_H
#define RACE_CONTROLLER_H

// Qt includes
#include <QObject>

// Custom includes
#include "view/course.h"
#include "model/course.h"
#include "en2en3.h"
#include "statArrayController.h"

namespace controller {
    class RaceController: public QObject
    {
        Q_OBJECT
    public:
        /**
         * @brief Constructor
         */
        RaceController();

        /**
         * @brief Constructor
         */
        RaceController(view::Course* p_course);

        /**
         * @brief Destructor
         */
        ~RaceController();

        /**
         * @brief Connects signals and slots.
         */
        void connection();

        /**
         * @brief Triggers when a course start is clicked.
         * @param p_isFullStart: True if user click on start button, false otherwise.
         */
        void onStart(const bool p_isFullStart);

        /**
         * @brief Triggers when a course clear is clicked.
         */
        void onClear();

        /**
         * @brief Sets the course for this controller.
         * @param p_course Pointer to the course to set.
         */
        void setCourse(view::Course* p_course)
        {
            m_course = p_course;
            connection();
        }

        /**
         * @brief Returns a pointer to the race.
         * @return A pointer to the race.
         */
        view::Course* course()
        {
            return m_course;
        }

        /**
         * @brief Sets the stat array controller for this controller.
         * @param p_statArrayController: Pointer to the stat array controller to set.
         */
        void setLPDStatArrayController(StatArrayController* p_statArrayController)
        {
            m_statLPDArrayController = p_statArrayController;
        }

        /**
         * @brief Returns a pointer to the stat array controller.
         * @return A pointer to the stat array controller.
         */
        StatArrayController* statLPDArrayController()
        {
            return m_statLPDArrayController;
        }

        /**
         * @brief Sets the LPJ stat array controller for this controller.
         * @param p_statArrayController: Pointer to the LPJ stat array controller to set.
         */
        void setLPJStatArrayController(StatArrayController* p_statArrayController)
        {
            m_statLPJArrayController = p_statArrayController;
        }

        /**
         * @brief Returns a pointer to the LPJ stat array controller.
         * @return A pointer to the LPJ stat array controller.
         */
        StatArrayController* statLPJArrayController()
        {
            return m_statLPJArrayController;
        }

    private:
        /**
         * @brief Fills the course filter.
         */
        model::Filter courseFilter();

        /**
         * @brief Fills the course data into the database.
         * @param p_en2eEn3 The En2En3 object containing the horses
         */
        void fillCourseInBDD(En2En3& p_en2eEn3);

        /**
         * @brief [Temporary] Converts course filter to constant filter.
         * @param p_filterToConvert: The course filter.
         * @return The constant filter.
         */
        const Filter convertFilter(const model::Filter& p_filterToConvert) const;

        /**
         * @brief Changes the filter set to define percent.
         * @param The controller affected by the filter change.
         */
        void onChangeStatFilter(StatArrayController* p_controller);

    private:
        view::Course* m_course; //!< Pointer to the course being controlled.
        controller::StatArrayController* m_statLPDArrayController; //!< Pointer to the LPD stat array controller.
        controller::StatArrayController* m_statLPJArrayController; //!< Pointer to the LPJ stat array controller.
    };
};

#endif // RACE_CONTROLLER_H