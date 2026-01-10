// Custom includes
#include "display_BDD.h"
#include "ui_display_BDD.h"
#include "controller/BDD.h"
#include "bddCourseData.h"

// Qt includes
#include <QComboBox>

//-----------------------------------------
DisplayBDD::DisplayBDD(QWidget* p_parent)
    : QWidget(p_parent)
    , m_ui { new Ui::Display_BDD}
//-----------------------------------------
{
    m_ui->setupUi(this);
    setupUi();
}

//-----------------------------------------
DisplayBDD::~DisplayBDD()
//-----------------------------------------
{
    delete m_ui;
}

//-----------------------------------------
void DisplayBDD::setupUi()
//-----------------------------------------
{
    BDD bdd;
    bdd.connectToDatabase();
    fillCombinations(bdd.combinations());
    bdd.closeConnection();
}

//-----------------------------------------
void DisplayBDD::fillCombinations(const std::vector<CombinationToDisplay>& p_combinations)
//-----------------------------------------
{
    if(0 == p_combinations.size())
    {
        qWarning() << "No combinations found";
        return;
    }

    m_ui->m_combinationList->addItem("");
    for(CombinationToDisplay comb: p_combinations)
    {
        m_ui->m_combinationList->addItem(QString::fromStdString(comb.combination));
    }
    connect(m_ui->m_combinationList, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DisplayBDD::onCombSelectionChanged);
}

//-----------------------------------------
void DisplayBDD::onCombSelectionChanged(int p_index)
//-----------------------------------------
{
    BDD bdd;
    bdd.connectToDatabase();
    std::vector<RaceData> raceDatas = bdd.raceDataByCombinationId(p_index);
    bdd.closeConnection();
    
    if(nullptr == m_ui)
    {
        qDebug() << "m_ui nullptr";
        return;
    }
    if(nullptr == m_ui->m_courseContainer)
    {
        qDebug() << "m_courseContainer nullptr";
        return;
    }

    clearCourseContainer();
    
    for(RaceData data: raceDatas)
    {
        BDDCourseData* bddCourseData = new BDDCourseData(this);
        
        bddCourseData->fillBDDCourseData(data);
        m_ui->m_courseContainer->layout()->addWidget(bddCourseData);
    }
}

//-----------------------------------------
void DisplayBDD::clearCourseContainer()
//-----------------------------------------
{
    if(nullptr == m_ui || nullptr == m_ui->m_courseContainer)
    {
        qDebug() << "m_ui or m_courseContainer is nullptr";
        return;
    }
    QLayoutItem* item = nullptr;
    while(nullptr != (item = m_ui->m_courseContainer->layout()->takeAt(0)))
    {
        QWidget* widget = item->widget();
        if(widget)
        {
            widget->deleteLater();
        }
        delete item;
    }
}