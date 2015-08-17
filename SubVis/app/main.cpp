#include "QApplication"
#include "QStyleFactory"
#include "view/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SubVis::View::MainWindow main_window;

    main_window.show();

    return app.exec();
}
