#include <GL/glut.h>
#include <QtDebug>
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
  mouse_handler_.set_enabled(edit);
  qDebug() << "Edit set to" << edit;
}

void ViewerMeshWidget::set_editable_capatability(bool editable) {
  must_react_to_edit_events_ = editable;
  qDebug() << "Viewer must react to edit events?" << editable;
}

void ViewerMeshWidget::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  // placeholder for custom data structure
  mesh_ = &mesh;
  // TODO extract vertices in a custom data structure (member variable) to speed up drawing!

  // force redraw
  updateGL();

  // Notify the mouse grabber
  if (must_react_to_edit_events_) {
    qDebug() << "Delegating mesh update event to mouse handler.";
    mouse_handler_.mesh_updated(mesh);
  }
}

void ViewerMeshWidget::init_gl() {
  qDebug() << "Initializing open gl.";
  // TODO compute the correct scene radius and configure camera.
  setSceneRadius(10.0);
  camera()->setZNearCoefficient(0.0001);
  camera()->setZClippingCoefficient(10.0);
  //glEnable(GL_DEPTH_TEST);
  qDebug() << "Finished initializing open gl.";
}

void ViewerMeshWidget::draw_gl() {
  if (edit_) {
    qDebug() << "Delegating draw_gl to mouse handler.";
    mouse_handler_.draw_gl();
  }
  qDebug() << "Drawing mesh.";

  // black background
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  // no lighting TODO: shadows etc. would be good..
  glDisable(GL_LIGHTING);
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

} // namespace subvis

