#include <QApplication>
#include "PresentationWidget.h"
#include "Settings.h"

int main(int argc, char *argv[])
{
    QApplication::setApplicationName("Paraoke");
    QApplication::setOrganizationName("Troodon Software, LLC");
    QApplication::setOrganizationDomain("troodon-software.com");
    QApplication::setApplicationVersion("0.1");

    QApplication a(argc, argv);

    Settings settings;
    PresentationWidget pw(&settings);
    pw.show();

    return a.exec();
}
