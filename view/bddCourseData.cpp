// Custom includes
#include "bddCourseData.h"
#include "ui_bddCourseData.h"
#include "controller/tools.h"

// Qt includes
#include <QStandardItemModel>

//---------------------------------------------
BDDCourseData::BDDCourseData(QWidget* p_parent)
    : QWidget { p_parent}
    , m_ui {new Ui::BDDCourseData}
//---------------------------------------------
{
    m_ui->setupUi(this);
}

//---------------------------------------------
BDDCourseData::~BDDCourseData()
//---------------------------------------------
{

}

//---------------------------------------------
void BDDCourseData::fillBDDCourseData(const RaceData& p_raceData)
//---------------------------------------------
{
    m_ui->m_nbr1->setText(QString::number(p_raceData.numbers[0]));
    m_ui->m_nbr2->setText(QString::number(p_raceData.numbers[1]));
    m_ui->m_nbr3->setText(QString::number(p_raceData.numbers[2]));
    m_ui->m_nbr4->setText(QString::number(p_raceData.numbers[3]));
    m_ui->m_nbr5->setText(QString::number(p_raceData.numbers[4]));
    m_ui->m_nbr6->setText(QString::number(p_raceData.numbers[5]));
    m_ui->m_nbr7->setText(QString::number(p_raceData.numbers[6]));
    m_ui->m_nbr8->setText(QString::number(p_raceData.numbers[7]));

    m_ui->m_nbrLeaver->setText(QString::number(p_raceData.nbrLeaver));
    m_ui->m_discipline->setText(p_raceData.discipline);
    m_ui->m_distance->setText(QString::number(p_raceData.distance));
    m_ui->m_winner->setText(QString::number(p_raceData.winner));
    m_ui->m_place1->setText(QString::number(p_raceData.place1));
    m_ui->m_place2->setText(QString::number(p_raceData.place2));

    fillen2En3(p_raceData);
}

//---------------------------------------------
void BDDCourseData::fillen2En3(const RaceData& p_raceData)
//---------------------------------------------
{
    QStringList horizontalHeader {"P", "3", " ", "T", "T", "P", "3", " ", "4", " ", "N", "T", "T", "T", "T", "T"};
    QStringList verticalHeader;
    for(int number:p_raceData.numbers)
    {
        verticalHeader << QString::number(number);
    }
    verticalHeader << "T";

    QStandardItemModel* modelEn2 = new QStandardItemModel(m_ui->m_en2);
    modelEn2->setVerticalHeaderLabels(verticalHeader);
    modelEn2->setHorizontalHeaderLabels(horizontalHeader);
    fillData(modelEn2, p_raceData.en2);
    m_ui->m_en2->setModel(modelEn2);
    m_ui->m_en2->resizeColumnsToContents();

    QStandardItemModel* modelEn3 = new QStandardItemModel(m_ui->m_en3);
    modelEn3->setVerticalHeaderLabels(verticalHeader);
    modelEn3->setHorizontalHeaderLabels(horizontalHeader);
    fillData(modelEn3, p_raceData.en3);
    m_ui->m_en3->setModel(modelEn3);
    m_ui->m_en3->resizeColumnsToContents();

    QStandardItemModel* modelEn2En3 = new QStandardItemModel(m_ui->m_en2En3);
    modelEn2En3->setVerticalHeaderLabels(verticalHeader);
    modelEn2En3->setHorizontalHeaderLabels(horizontalHeader);
    fillData(modelEn2En3, p_raceData.en2En3);
    m_ui->m_en2En3->setModel(modelEn2En3);
    m_ui->m_en2En3->resizeColumnsToContents();
}

//---------------------------------------------
void BDDCourseData::fillData(QStandardItemModel* p_model, const std::array<std::array<en2En3Struct, 16>, 9>& p_datas)
//---------------------------------------------
{
    int row = 0;
    for(std::array<en2En3Struct, 16> rowData: p_datas)
    {
        int column = 0;
        for(en2En3Struct en2En3: rowData)
        {
            QStandardItem* item = new QStandardItem(QString::number(en2En3.value));
            item->setBackground(convertColorName(en2En3.color));
            p_model->setItem(row, column, item);
            column++;
        }
        row++;
    }
}

//---------------------------------------------
QBrush BDDCourseData::convertColorName(const QString& p_colorName)
//---------------------------------------------
{
    if(0 == p_colorName.compare("green", Qt::CaseInsensitive))
    {
        return QBrush(QColor(QString::fromStdString(GREEN_COLOR)));
    }
    else if(0 == p_colorName.compare("blue", Qt::CaseInsensitive))
    {
        return QBrush(QColor(QString::fromStdString(BLUE_COLOR)));
    }
    else if(0 == p_colorName.compare("yellow", Qt::CaseInsensitive))
    {
        return QBrush(QColor(QString::fromStdString(YELLOW_COLOR)));
    }
    else if(0 == p_colorName.compare("orange", Qt::CaseInsensitive))
    {
        return QBrush(QColor(QString::fromStdString(ORANGE_COLOR)));
    }

    return QBrush(QColor(QString::fromStdString(BLANK_COLOR)));
}