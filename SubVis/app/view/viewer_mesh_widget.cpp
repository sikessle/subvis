#include <GL/glut.h>
#include <QDebug>
#include <QMouseEvent>

#include "view/viewer_mesh_widget.h"

namespace subvis {

ViewerMeshWidget::ViewerMeshWidget(QWidget* parent,
                                   int mesh_id) :
  ViewerWidget{parent, mesh_id} {
}

void ViewerMeshWidget::set_edit(bool edit) {
  edit_ = edit;
  setMouseTracking(edit);
  setMouseGrabberIsEnabled(&mouse_grabber_, edit);
  mouse_grabber_.set_enabled(edit);
}


void ViewerMeshWidget::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  // placeholder for custom data structure
  mesh_ = &mesh;
  // TODO extract vertices in a custom data structure (member variable) to speed up drawing!

  // force redraw
  updateGL();
}

void ViewerMeshWidget::init_gl() {
  // black background
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  // no lighting TODO: shadows etc. would be good..
  glDisable(GL_LIGHTING);
  // TODO compute the correct scene radius and configure camera.
  setSceneRadius(10.0);
  camera()->setZNearCoefficient(0.0001);
  camera()->setZClippingCoefficient(10.0);
  //glEnable(GL_DEPTH_TEST);
}

void ViewerMeshWidget::draw_gl() {
  // WIREFRAME
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // dark blue color
  glColor3f(0, 0.294, 0.419);
  glEnable(GL_POLYGON_OFFSET_LINE);
  glPolygonOffset(-1.0, -1.0);
  draw_mesh();

  // SOLID
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // blue (SubVis) color
  glColor3f(0.003, 0.615, 0.878);
  draw_mesh();
}

// TODO: We should rather pass a VBO to the graphics card to speed things up.
// this is just a temporary prototype.
// This implementation is EXTREMLY SLOW!
void ViewerMeshWidget::draw_mesh() {
  using Point = surface_mesh::Point;

  // mesh not yet loaded. TODO this can be removed if we use an appropriate container for our custom
  // data structure
  if (!mesh_) {
    return;
  }

  for (const auto& face : mesh_->faces()) {
    glBegin(GL_POLYGON);
    for (const auto& vertex : mesh_->vertices(face)) {
      const Point& p = mesh_->get_vertex_property<Point>("v:point")[vertex];
      glVertex3d(p[0], p[1], p[2]);
    }
    glEnd();
  }
}

void ViewerMeshWidget::index_to_rgba(const int index, int rgba[4]) const {
  rgba[0] = (index & 0x000000FF) >>  0; // r: least significant 2 bytes of index
  rgba[1] = (index & 0x0000FF00) >>  8; // b: next two bytes
  rgba[2] = (index & 0x00FF0000) >> 16; // g: next two bytes
  rgba[3] = (index & 0xFF000000) >> 24; // a: next two bytes
}

int ViewerMeshWidget::rgba_to_index(const int rgba[4]) const {
  // rebuild index from r, g and b values.
  // inverse function of index_to_rgb.
  return
    rgba[0] +
    rgba[1] * 256 +
    rgba[2] * 256 * 256 +
    rgba[2] * 256 * 256 * 256;
}

} // namespace subvis

