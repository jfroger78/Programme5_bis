#ifndef CONTROLLER_H
#define CONTROLLER_H

// Custom includes
#include "view/programme_cinco.h"
#include "raceController.h"

// Qt includes
#include <QObject>

namespace controller {
    class Controller: public QObject
    {
        Q_OBJECT

        public:

            /**
             * @brief Destructor
             */
            ~Controller();

            /**
             * @brief Get the singleton instance of the Controller.
             * @return Pointer to the Controller instance.
             * This method implements the Singleton design pattern to ensure that only one instance of Controller exists.
             * It is thread-safe and lazy-initialized, meaning the instance is created only when it is first requested.
             * @note This method is static and can be called without an instance of Controller.
             */
            static Controller* instance();

        private:
            /**
             * @brief Constructor
             */
            Controller(QObject* p_parent = nullptr);

            /**
             * @brief Copy constructor (delete).
             */
            Controller(const Controller&) = delete;
            
            /**
             * @brief Operator= (delete).
             */
            Controller& operator=(const Controller&) = delete;

            /**
             * @brief Connects signals / slots.
             */
            void connection();

        private:
            /**
             * @brief Triggers when big start is clicked.
             * @param p_isFillBDD True if the slot is trigger by using fill BDD button.
             */
            void onBigStart(bool p_isFillBDD);

            /**
             * @brief Triggers when big clear is clicked.
             */
            void onBigClear();

        private:
            static Controller* m_instance; //!< Static instance.
            Programme_cinco m_programme; //!< Main IHM acces.
            std::vector<RaceController> m_raceControllers; //!< Array of raceController.
    };
};

#endif // CONTROLLER_H