#ifndef SUBVISAPP_SUBVIS_H
#define SUBVISAPP_H

#include <memory>
#include <QSplashScreen>
#include <QApplication>

using namespace std;

namespace SubVis {

class SubVisApp
{
public:
    SubVisApp();
    int run(int argc, char *argv[]);

private:
    unique_ptr<QSplashScreen> create_show_splash(const QApplication &app);
};

} // namespace SubVis

#endif // SUBVIS_SUBVISAPP_H
