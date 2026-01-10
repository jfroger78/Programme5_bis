// Custom includes
#include "controller/controller.h"

// Qt includes
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    controller::Controller::instance();
    return a.exec();
}