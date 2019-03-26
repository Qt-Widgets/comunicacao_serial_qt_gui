#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{   
    QApplication::setDesktopSettingsAware(false);
    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}


