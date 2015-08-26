#ifndef SUBDIVISON_ALGORITHMS_H
#define SUBDIVISON_ALGORITHMS_H

#include <QObject>
#include <string>
#include "subvis_plugin.h"

namespace SubdivisonPlugin {

using SubVis::SubVisPlugin;
using SubVis::DrawController;

class SubdivisonAlgorithms : public QObject, SubVisPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SUBVIS_PLUGIN_IID FILE "subdivision.json")
    // fully scoped name required in macro
    Q_INTERFACES(SubVis::SubVisPlugin)

public:
     void set_draw_controller(DrawController *draw_ctrl) override;
     void draw_opengl() override;
     void create_gui(QWidget *parent) override;

private:
     DrawController *draw_controller{nullptr};
};

} // namespace SubdivisonPlugin
#endif // SUBDIVISON_ALGORITHMS_H
