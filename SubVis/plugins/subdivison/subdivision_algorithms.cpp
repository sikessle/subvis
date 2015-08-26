#include <GL/gl.h>
#include <QLabel>
#include "subdivision_algorithms.h"

namespace SubdivisonPlugin {

void SubdivisonAlgorithms::set_draw_controller(const DrawController *draw_ctrl)
{
    draw_controller = draw_ctrl;
}

void SubdivisonAlgorithms::draw_opengl()
{
    // draw calls go here

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

void SubdivisonAlgorithms::create_gui(QWidget *parent)
{
    // create own gui components
    QLabel *label = new QLabel{parent};
    label->setText("My Plugin Interface");
}

} // namespace SubdivisonPlugin
