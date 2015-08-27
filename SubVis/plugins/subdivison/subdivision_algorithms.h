#ifndef SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H
#define SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H

#include <QObject>
#include <string>
#include <memory>

#include "subvis_plugin.h"
//#include "catmull.h"

namespace SubdivisionPlugin {

using SubVis::SubVisPlugin;
using SubVis::DrawController;
using std::unique_ptr;

class SubdivisionAlgorithms : public QObject, public SubVisPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SUBVIS_PLUGIN_IID FILE "subdivision.json")
    // fully scoped name required in macro
    Q_INTERFACES(SubVis::SubVisPlugin)

public:
    void set_draw_controller(DrawController* draw_controller) override;
    void draw_opengl() override;
    void create_gui(QWidget* parent) override;

private:
     DrawController* draw_controller_{nullptr};
     //unique_ptr<Algorithm> catmull_{new Catmull};
};

} // namespace SubdivisionPlugin
#endif // SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H
