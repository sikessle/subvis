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
public:
    virtual ~SubVisPlugin() {}

    virtual const QString id() = 0;
    virtual const QString name() = 0;
    virtual void set_draw_controller(DrawController* draw_controller) = 0;
    virtual void draw_opengl() = 0;
    virtual void create_gui(QWidget* parent) = 0;
};

} // namespace SubVis

#endif // SUBVIS_SUBVIS_PLUGIN_H

