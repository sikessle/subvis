#include <GL/glu.h>
#include "view/viewer_mesh_widget.h"

namespace subvis {

ViewerMeshWidget::ViewerMeshWidget(QWidget* parent) : ViewerWidget{parent} {
}

void ViewerMeshWidget::init() {
  // black background
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void ViewerMeshWidget::draw() {
  // TODO: do our own mesh based rendering here.
  // get mesh: draw_controller->mesh_data().mesh();




  glBegin(GL_POLYGON);

  glDisable(GL_LIGHTING);
  glColor3f(1.0, 1.0, 1.0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glEnableClientState(GL_VERTEX_ARRAY);
  //GL::glVertexPointer(mesh_.points());

  //glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, &indices_[0]);

  glDisableClientState(GL_VERTEX_ARRAY);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnd();
}

} // namespace subvis

