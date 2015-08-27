#ifndef SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H
#define SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H

#include <QObject>
#include <string>

#include "plugins/subvis_plugin.h"

namespace SubdivisionPlugin {

class SubdivisionAlgorithms : public SubVis::SubVisPlugin
{
public:
    virtual QString id() override;
    virtual QString name() override;
    virtual void set_draw_controller(SubVis::DrawController* draw_controller) override;
    virtual void draw_opengl() override;
    virtual void create_gui(QWidget* parent) override;

private:
    SubVis::DrawController* draw_controller_{nullptr};
};

} // namespace SubdivisionPlugin
#endif // SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H
