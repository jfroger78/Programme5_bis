// Custom includes
#include "controller.h"
#include "view/course.h"
#include "en2en3.h"
#include "model/horse.h"
#include "view/display_BDD.h"

// Qt includes
#include <QApplication>
#include <QFile>
#include <QDebug>
#include <QDateTime>

// Standard c++ includes
#include <iostream>
#include <optional>


static std::string sFilename;
static bool logFileCreated = false;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    if(!logFileCreated)
    {
        sFilename = QString(QApplication::applicationDirPath() +"/log_%1.txt").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy_HH:mm:ss")).toStdString();
        logFileCreated = true;
    }
    // Create log file path and name (log_dd-mm-yyy_HH:mm:ss)
    QFile logFile {QString::fromStdString(sFilename)};
    if(logFile.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream outfile{&logFile};
        const char* file = context.function ? context.function : "";
        QByteArray localMsg = msg.toLocal8Bit();
        switch (type) {
        case QtDebugMsg:
            outfile << QString("Debug: %1 (%2: %3)\n").arg(localMsg.constData(), file, QString::number(context.line));
            break;
        case QtInfoMsg:
            outfile << QString("Info: %1 (%2: %3)\n").arg(localMsg.constData(), file, QString::number(context.line));
            break;
        case QtWarningMsg:
            outfile << QString("Warning: %1 (%2: %3)\n").arg(localMsg.constData(), file, QString::number(context.line));
            break;
        case QtCriticalMsg:
            outfile << QString("Critical: %1 (%2: %3)\n").arg(localMsg.constData(), file, QString::number(context.line));
            break;
        case QtFatalMsg:
            outfile << QString("Fatal: %1 (%2: %3)\n").arg(localMsg.constData(), file, QString::number(context.line));
            abort();
        }
        logFile.close();
    }
}

namespace controller {

Controller* Controller::m_instance = nullptr;

    //--------------------------------
    Controller::Controller(QObject* p_parent)
        :QObject( p_parent )
        , m_raceControllers{ 20 }
    //--------------------------------
    {
        qInstallMessageHandler(myMessageOutput);
        connection();
        m_programme.show();

        DisplayBDD displayBDD;
    }

    //--------------------------------
    Controller::~Controller()
    //--------------------------------
    {
    }

    //--------------------------------
    Controller* Controller::instance()
    //--------------------------------
    {
        if(nullptr == m_instance)
        {
            m_instance = new Controller();
        }

        return m_instance;
    }

    //--------------------------------
    void Controller::connection()
    //--------------------------------
    {
        connect(&m_programme, &Programme_cinco::bigStartClicked, this, &Controller::onBigStart);
        connect(&m_programme, &Programme_cinco::bigClearClicked, this, &Controller::onBigClear);

        for(int index = 0; index < m_programme.courses().size(); ++index)
        {
            view::Course* course = m_programme.courses()[index];
            if(nullptr != course)
            {
                m_raceControllers[index].setCourse(course);
                m_raceControllers[index].setLPJStatArrayController(new StatArrayController(EFilter::FilterLPJ, EFilter::FilterLP));
                m_raceControllers[index].course()->addResultArray(&m_raceControllers[index].statLPJArrayController()->statArrayHMI(),
                                                                  3, 0, 1, 8);
                m_raceControllers[index].setLPDStatArrayController(new StatArrayController(EFilter::FilterLPD));
                m_raceControllers[index].course()->addResultArray(&m_raceControllers[index].statLPDArrayController()->statArrayHMI(),
                                                                  4, 0, 1, 8);
            }
        }
    }

    //--------------------------------
    void Controller::onBigStart(bool p_isFillBDD)
    //--------------------------------
    {
        for(RaceController& raceController: m_raceControllers)
        {
            emit raceController.onStart(p_isFillBDD);
        }
    }

    //--------------------------------
    void Controller::onBigClear()
    //--------------------------------
    {
        for(RaceController& raceController: m_raceControllers)
        {
            emit raceController.onClear();
        }
    }
}