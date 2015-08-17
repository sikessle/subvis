#include <thread>
#include "QApplication"
#include "QPixmap"
#include "QSplashScreen"
#include "view/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPixmap splash_image(":/media/splash.png");

    if (splash_image.isNull()) {
        std::cerr << "Failed to load splash screen image" << std::endl;
        exit(1);
    }

    QSplashScreen splash(splash_image);

    splash.show();
    app.processEvents();

    SubVis::View::MainWindow main_window;

    main_window.show();
    splash.finish(&main_window);

    return app.exec();
}
