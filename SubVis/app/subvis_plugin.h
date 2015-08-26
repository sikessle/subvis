#ifndef SUBVIS_SUBVIS_PLUGIN_H
#define SUBVIS_SUBVIS_PLUGIN_H

#include <string>
#include <QWidget>
#include "controller/draw_controller.h"

namespace SubVis {

using surface_mesh::Surface_mesh;

/**
 * @brief Interface for plugins
 */
class SubVisPlugin
{
#define SUBVIS_PLUGIN_IID "de.htwg.ios.subvis.SubVisPlugin"

public:
    virtual ~SubVisPlugin() {}
    virtual void set_draw_controller(const DrawController *draw_ctrl) = 0;
    virtual void draw_opengl() = 0;
    virtual void create_gui(QWidget *parent) = 0;
};

} // namespace SubVis

// must be outside namespace
Q_DECLARE_INTERFACE(SubVis::SubVisPlugin, SUBVIS_PLUGIN_IID)

#endif // SUBVIS_SUBVIS_PLUGIN_H

