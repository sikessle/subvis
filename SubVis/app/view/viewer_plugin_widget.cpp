#include <QtDebug>

#include "view/viewer_plugin_widget.h"

namespace subvis {

ViewerPluginWidget::ViewerPluginWidget(QWidget* parent, int mesh_id) :
  ViewerWidget{parent, mesh_id} {
}

void ViewerPluginWidget::mesh_updated(
  const surface_mesh::Surface_mesh& mesh) {
  if (drawing_plugin_) {
    qDebug() << "Delegating mesh_updated to plugin.";
    drawing_plugin_->mesh_updated(mesh, mesh_id_);
  }
}

void ViewerPluginWidget::init_gl() {
  if (drawing_plugin_) {
    qDebug() << "Delegating init_gl to plugin.";
    drawing_plugin_->init_opengl(mesh_id_);
  }
}

void ViewerPluginWidget::draw_gl() {
  if (drawing_plugin_) {
    qDebug() << "Delegating draw_opengl to plugin.";
    drawing_plugin_->draw_opengl(mesh_id_);
  }
}

void ViewerPluginWidget::set_drawing_plugin(SubVisPlugin* plugin) {
  drawing_plugin_ = plugin;
  qDebug() << "Drawing plugin set.";

  // Listen to redraw requests of the plugin.
  // Must use old style string connection here.
  connect(drawing_plugin_, SIGNAL(needs_redraw()), this, SLOT(updateGL()),
          Qt::UniqueConnection);
}

} // namespace subvis
