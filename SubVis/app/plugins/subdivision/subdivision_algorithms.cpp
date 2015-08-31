#include <exception>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "surface_mesh/Surface_mesh.h"

#include "plugins/subdivision/sd_catmull.h"
#include "plugins/subdivision/sd_doosabin.h"
#include "plugins/subdivision/sd_loop.h"
#include "plugins/subdivision/sd_butterfly.h"

#include "plugins/subdivision/subdivision_algorithms.h"

namespace subdivision {

SubdivisionAlgorithms::SubdivisionAlgorithms()
{
    std::vector<Algorithm*> instances;
    // Add here all the algorithms
    instances.push_back(new SubdivCatmull);
    instances.push_back(new SubdivDooSabin);
    instances.push_back(new SubdivLoop);
    instances.push_back(new SubdivButterfly);

    for (const auto instance : instances) {
        algorithms_[instance->id()] = std::unique_ptr<Algorithm>{instance};
    }
}

const QString SubdivisionAlgorithms::id()
{
    return "de.htwg.ios.subvis.plugin.subdivision";
}

const QString SubdivisionAlgorithms::name()
{
    return "Subdivision";
}

void SubdivisionAlgorithms::set_draw_controller(subvis::DrawController* draw_controller)
{
    draw_controller_ = draw_controller;
}

void SubdivisionAlgorithms::draw_opengl()
{
    auto& algorithm = active_algorithm();
    algorithm->draw_limit_surface(draw_controller_->mesh_data().mesh());
}

std::unique_ptr<Algorithm>& SubdivisionAlgorithms::active_algorithm()
{
    if (dropdown_->count() == 0) {
        throw new std::logic_error("no algorithms loaded. ensure that at least one is loaded in the constructor.");
    }
    const QString id = dropdown_->currentData().toString();

    return algorithms_.at(id);
}

void SubdivisionAlgorithms::create_gui(QWidget* parent)
{
    // Gui creation
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setAlignment(Qt::AlignTop);

    layout->addWidget(new QLabel("Algorithm:"));

    dropdown_ = new QComboBox(parent);
    for (const auto& it : algorithms_) {
        dropdown_->addItem(it.second->name(), QVariant(it.first));
    }
    layout->addWidget(dropdown_);

    QHBoxLayout* layout_steps = new QHBoxLayout;
    layout_steps->setAlignment(Qt::AlignTop);

    layout_steps->addWidget(new QLabel("Steps:"));
    steps_ = new QSpinBox(parent);
    steps_->setRange(1, 100);
    layout_steps->addWidget(steps_);

    layout->addLayout(layout_steps);

    subdivide_ = new QPushButton("subdivide", parent);
    QObject::connect(subdivide_, SIGNAL(clicked(bool)),
                     this, SLOT(subdivide_clicked(bool)));
    layout->addWidget(subdivide_);
}

void SubdivisionAlgorithms::subdivide_clicked(bool)
{
    int steps = steps_->value();
    auto& mesh_data = draw_controller_->mesh_data();
    auto& algorithm = active_algorithm();

    auto result = algorithm->subdivide(mesh_data.mesh(), steps);

    mesh_data.load(std::move(result));
}

} // namespace SubdivisionPlugin
