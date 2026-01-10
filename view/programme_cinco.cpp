// Custom includes
#include "programme_cinco.h"
#include "ui_programme_cinco.h"
#include "course.h"

// Qt includes
#include <QGridLayout>
#include <QDebug>
#include <QMessageBox>
#include <QScrollArea>

//-----------------------------------------------
Programme_cinco::Programme_cinco(QWidget *p_parent)
    : QMainWindow(p_parent)
    , m_ui(new Ui::Programme_cinco)
//-----------------------------------------------
{
    m_ui->setupUi(this);
    setupUi();
    connection();
}

//-----------------------------------------------
Programme_cinco::~Programme_cinco()
//-----------------------------------------------
{
    delete m_ui;
}

//-----------------------------------------------
void Programme_cinco::connection()
//-----------------------------------------------
{
    connect(m_ui->m_bigStart, &QPushButton::clicked, this, &Programme_cinco::bigStartClicked);
    connect(m_ui->m_bigClear, &QPushButton::clicked, this, &Programme_cinco::bigClearClicked);
    connect(m_ui->m_fillBDD, &QPushButton::clicked, this, &Programme_cinco::onFillBDD);
    connect(&m_fillBDD, &FillBDD::bigStartClicked, this, &Programme_cinco::bigStartClicked);
    connect(&m_fillBDD, &FillBDD::bigClearClicked, this, &Programme_cinco::bigClearClicked);
}

//-----------------------------------------------
std::array<view::Course*, 20> Programme_cinco::courses()
//-----------------------------------------------
{
    return m_courses;
}

//-----------------------------------------------
void Programme_cinco::setupUi()
//-----------------------------------------------
{
    createTab(1, m_ui->scrollAreaWidgetContents);
    createTab(2, m_ui->scrollAreaWidgetContents_2);
    createTab(3, m_ui->scrollAreaWidgetContents_3);
    createTab(4, m_ui->scrollAreaWidgetContents_4);
    createTab(5, m_ui->scrollAreaWidgetContents_5);
    createTab(6, m_ui->scrollAreaWidgetContents_6);
    createTab(7, m_ui->scrollAreaWidgetContents_7);
    createTab(8, m_ui->scrollAreaWidgetContents_8);
    createTab(9, m_ui->scrollAreaWidgetContents_9);
    createTab(10, m_ui->scrollAreaWidgetContents_10);

    m_fillBDD.setCourses(m_courses);

    QGridLayout* grid = qobject_cast<QGridLayout*>(m_ui->m_BDD->layout());
    if(nullptr != grid)
    {
        grid->addWidget(&m_displayBDD);
    }
}

//-----------------------------------------------
void Programme_cinco::createTab(const int8_t &p_courseNumber, QWidget* p_courseTab)
//-----------------------------------------------
{
    std::string courseName = "Course " + std::to_string(p_courseNumber);
    m_courses[p_courseNumber - 1] = new view::Course(courseName + "_1");
    m_courses[p_courseNumber + 9] = new view::Course(courseName + "_2");
    
    QGridLayout* grid = qobject_cast<QGridLayout*>(p_courseTab->layout());
    if(nullptr != grid)
    {
        grid->addWidget(m_courses[p_courseNumber - 1], 0, 0);
        grid->addWidget(m_courses[p_courseNumber + 9], 0, 1);
    }
}

//-----------------------------------------------
void Programme_cinco::onFillBDD()
//-----------------------------------------------
{
    m_fillBDD.onStart();

    QMessageBox msgBox;
    msgBox.setText("Remplissage de la base de donnée finit.");
    msgBox.exec();
}