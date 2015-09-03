#include <GL/glut.h>
#include <QDebug>

#include "view/viewer_mesh_widget.h"

namespace subvis {

ViewerMeshWidget::ViewerMeshWidget(QWidget* parent) : ViewerWidget{parent} {
}

void ViewerMeshWidget::init() {
  // black background
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  // blue color
  glColor3f(0.0, 0.5, 1.0);
  // no lighting
  glDisable(GL_LIGHTING);
  // wireframe
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // TODO compute the correct scene radius and configure camera.
  setSceneRadius(10.0);
  camera()->setZNearCoefficient(0.0001);
  camera()->setZClippingCoefficient(10.0);
}

// TODO: We should rather pass a VBO to the graphics card to speed things up.
// this is just a temporary prototype.
// This implementation is EXTREMLY SLOW!
void ViewerMeshWidget::draw() {
  using Surface_mesh = surface_mesh::Surface_mesh;
  using Point = surface_mesh::Point;

  const Surface_mesh& mesh =
    draw_controller_->get_mesh_data().get_mesh();

  for (const auto& face : mesh.faces()) {
    glBegin(GL_POLYGON);
    for (const auto& vertex : mesh.vertices(face)) {
      const Point& p = mesh.get_vertex_property<Point>("v:point")[vertex];
      glVertex3d(p[0], p[1], p[2]);
    }
    glEnd();
  }
}

} // namespace subvis

