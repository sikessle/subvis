#include <GL/gl.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSpinBox>

#include "plugins/subdivision/subdivision_algorithms.h"

namespace SubdivisionPlugin {

QString SubdivisionAlgorithms::id()
{
    return "de.htwg.ios.subvis.plugin.subdivision";
}

QString SubdivisionAlgorithms::name()
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
    QVBoxLayout* layout = new QVBoxLayout(parent);
    layout->setAlignment(Qt::AlignTop);

    layout->addWidget(new QLabel("Algorithm:"));

    QComboBox* dropdown = new QComboBox(parent);
    dropdown->addItem("Catmull-Clark");
    dropdown->addItem("Doo-Sabin");
    dropdown->addItem("Butterfly");
    dropdown->addItem("Loop");
    layout->addWidget(dropdown);

    QHBoxLayout* layout_steps = new QHBoxLayout(parent);
    layout_steps->setAlignment(Qt::AlignTop);

    layout_steps->addWidget(new QLabel("Steps:"));
    QSpinBox* steps = new QSpinBox(parent);
    steps->setRange(1, 100);
    layout_steps->addWidget(steps);

    layout->addLayout(layout_steps);

    QPushButton* subdivide = new QPushButton("subdivide", parent);
    layout->addWidget(subdivide);



    // SIGNALS etc.
    //catmull_->subdivide(draw_controller_->mesh_data());
}

} // namespace SubdivisionPlugin
