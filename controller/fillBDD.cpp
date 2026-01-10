// Custom includes
#include "fillBDD.h"
#include "tools.h"
#include "view/course.h"

// Qt includes
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>


//---------------------------------
FillBDD::FillBDD(QObject* p_parent)
    : QObject(p_parent)
    , m_courses(std::array<view::Course*, 20>{})
//---------------------------------
{

}

//---------------------------------
FillBDD::~FillBDD()
//---------------------------------
{

}

//---------------------------------
void FillBDD::onStart()
//---------------------------------
{
    QFileDialog dialog;
    dialog.setWindowTitle("Ouvrir un fichier contenant les données");
    dialog.setModal(true);

    QString filename = dialog.getOpenFileName(nullptr, "Ouvrir un fichier", QString(), "Files (*.txt)");
    if (filename.isEmpty()) {
        return; // No file selected
    }

    fillRace(filename);
}

//---------------------------------
void FillBDD::fillRace(const QString& p_fileName)
//---------------------------------
{
    QFile file(p_fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning("Could not open file for reading");
        return;
    }

    QTextStream in(&file);
    bool leftCourse = true;
    int courseIndex = 0;
    int offsetIndex = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(";");
        for(QString data: parts)
        {
            if(data.isEmpty())
            {
                qDebug() << "A data is empty, skip race";
                continue; // Skip the race if one data is empty.
            }
        }

        std::array<int, 8> numbers;
        for(int index = 0; index < 8; ++index)
        {
            numbers[index] = parts.at(index).toInt();
        }
        if(leftCourse)
        {
            offsetIndex = 0;
            leftCourse = false;
        }
        else
        {
            offsetIndex = 10;
            leftCourse = true;
        }

        if(10 == courseIndex)
        {
            emit bigStartClicked(true);
            emit bigClearClicked();
            courseIndex = 0;
        }
        
        QString discipline = parts.at(11);
        
        m_courses[courseIndex + offsetIndex]->fillCourses(numbers,
                                                          parts.at(8).toInt(),
                                                          parts.at(9).toInt(),
                                                          parts.at(10).toInt(),
                                                          discipline,
                                                          parts.at(12).left(parts.at(12).length() - 1).toInt(),
                                                          parts.at(13).toInt());
        if(leftCourse)
        {
            ++courseIndex;
        }
    }
    if(10 != courseIndex)
    {
        emit bigStartClicked(true);
        emit bigClearClicked();
    }
}