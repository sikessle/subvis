#ifndef SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H
#define SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H

#include <QObject>
#include <string>
#include <memory>

#include "plugins/subvis_plugin.h"

namespace SubdivisionPlugin {

using SubVis::SubVisPlugin;
using SubVis::DrawController;
using std::unique_ptr;

class SubdivisionAlgorithms : public SubVisPlugin
{
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
