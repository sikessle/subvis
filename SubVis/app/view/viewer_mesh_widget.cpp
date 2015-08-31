#include "view/viewer_mesh_widget.h"

namespace subvis {

ViewerMeshWidget::ViewerMeshWidget(QWidget* parent) : ViewerWidget{parent}
{
}

void ViewerMeshWidget::draw()
{
    // TODO: do our own mesh based rendering here.
    // get mesh: draw_controller->mesh_data().mesh();

    /* Something like this..
	glDisable(GL_LIGHTING);
    glColor3f(1.0, 1.0, 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnableClientState(GL_VERTEX_ARRAY);
    //GL::glVertexPointer(mesh_.points());

    //glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);

    glDisableClientState(GL_VERTEX_ARRAY);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    */

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

} // namespace SubVis

