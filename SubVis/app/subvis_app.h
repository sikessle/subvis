#ifndef SUBVISAPP_SUBVIS_H
#define SUBVISAPP_H

#include <memory>
#include <QSplashScreen>
#include <QApplication>

namespace SubVis {

using std::unique_ptr;

class SubVisApp : protected QApplication
{
    Q_OBJECT

public:
    SubVisApp(int argc, char* argv[]);
    int run();

private:
    unique_ptr<QSplashScreen> create_show_splash();
};

} // namespace SubVis

#endif // SUBVIS_SUBVISAPP_H
