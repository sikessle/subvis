#ifndef SUBVISAPP_SUBVIS_H
#define SUBVISAPP_H

#include <memory>
#include <unordered_map>
#include <QSplashScreen>
#include <QApplication>
#include "subvis_plugin.h"

namespace SubVis {

using std::unique_ptr;
using std::unordered_map;
using std::string;

class SubVisApp : protected QApplication
{
    Q_OBJECT

public:
    SubVisApp(int argc, char *argv[]);
    int run();

private:
    unique_ptr<QSplashScreen> create_show_splash();
    unordered_map<string, unique_ptr<SubVisPlugin>> plugins;

    bool load_plugins();
};

} // namespace SubVis

#endif // SUBVIS_SUBVISAPP_H
