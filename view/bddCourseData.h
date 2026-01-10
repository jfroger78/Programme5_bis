#ifndef BDD_COURSE_DATA_H
#define BDD_COURSE_DATA_H

// Qt includes
#include <QWidget>

// Custom includes
#include "model/constants.h"

class QStandardItemModel;

namespace Ui
{
    class BDDCourseData;
}

class BDDCourseData: public QWidget
{
    public:
        /**
         * @brief Constructor.
         * @param p_parent: Pointer to the parent widget.
         */
        BDDCourseData(QWidget* p_parent = nullptr);

        /**
         * @brief Destructor.
         */
        ~BDDCourseData();

        /**
         * @brief Fills the IHM.
         * @param p_raceDatas: Current race data linked to the selected combination.
         */
        void fillBDDCourseData(const RaceData& p_raceDatas);

    private:
        /**
         * @brief Fills the en2 en3 array.
         * @param p_raceDatas: The data to set into the en2 en3 array.
         */
        void fillen2En3(const RaceData& p_raceDatas);

        /**
         * @brief Fills data into the model.
         * @param p_model: The model to fill.
         * @param p_datas: The datas to set.
         */
        void fillData(QStandardItemModel* p_model, const std::array<std::array<en2En3Struct, 16>, 9>& p_datas);

        /**
         * @brief Convert a color name to it's value.
         * @param p_colorName The color name.
         * @return The hexa value of the color.
         */
        QBrush convertColorName(const QString& p_colorName);

    private:
        Ui::BDDCourseData* m_ui; //!< Pointer to the user interface.
};

#endif //BDD_COURSE_DATA_H