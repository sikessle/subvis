#ifndef SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H
#define SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H

#include <QObject>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <string>

#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/algorithm.h"

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
    std::unique_ptr<Algorithm> catmull{nullptr};
    std::unique_ptr<Algorithm> doosabin{nullptr};

    QPushButton* subdivide{nullptr};
    QSpinBox* steps{nullptr};
    QComboBox* dropdown{nullptr};
};

} // namespace SubdivisionPlugin
#endif // SUBDIVISIONPLUGIN_SUBDIVISON_ALGORITHMS_H
