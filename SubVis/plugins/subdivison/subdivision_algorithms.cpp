#include <GL/gl.h>
#include <QLabel>

#include "subdivision_algorithms.h"

namespace SubdivisionPlugin {

void SubdivisionAlgorithms::set_draw_controller(DrawController* draw_controller)
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
    // create own gui components
    QLabel* label = new QLabel{parent};
    label->setText("My Plugin Interface");
    // SIGNALS etc.
    //catmull_->subdivide(draw_controller_->mesh_data());
}

} // namespace SubdivisionPlugin
