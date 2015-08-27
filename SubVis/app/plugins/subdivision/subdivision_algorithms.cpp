#include <GL/gl.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "plugins/subdivision/subdivision_algorithms.h"

namespace SubdivisionPlugin {

SubdivisionAlgorithms::SubdivisionAlgorithms()
{
    std::vector<Algorithm*> instances;
    //instances.push_back(new Catmull);
    // Add here all the algorithms

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

void SubdivisionAlgorithms::set_draw_controller(SubVis::DrawController* draw_controller)
{
    draw_controller_ = draw_controller;
}

void SubdivisionAlgorithms::draw_opengl()
{
    // TODO draw calls go here
    // render limit surface
    // maybe move that to a new class

    // placeholder demo code:
    const float nbSteps = 200.0;

    glBegin(GL_QUAD_STRIP);
    for (int i=0; i<nbSteps; ++i) {
        const float ratio = i/nbSteps;
        const float angle = 21.0*ratio;
        const float c = cos(angle);
        const float s = sin(angle);
        const float r1 = 1.0 - 0.8f*ratio;
        const float r2 = 0.8f - 0.8f*ratio;
        const float alt = ratio - 0.5f;
        const float nor = 0.5f;
        const float up = sqrt(1.0-nor*nor);
        glColor3f(1.0-ratio, 0.2f , ratio);
        glNormal3f(nor*c, up, nor*s);
        glVertex3f(r1*c, alt, r1*s);
        glVertex3f(r2*c, alt+0.05f, r2*s);
    }
    glEnd();
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
    if (dropdown_->count() == 0) {
        return;
    }
    int steps = steps_->value();
    const QString id = dropdown_->currentData().toString();

    algorithms_.at(id)->subdivide(draw_controller_->mesh_data(), steps);
}

} // namespace SubdivisionPlugin
