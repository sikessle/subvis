#include "view/viewer_plugin_widget.h"

namespace subvis {

ViewerPluginWidget::ViewerPluginWidget(QWidget* parent) : ViewerWidget{parent} {
}

void ViewerPluginWidget::mesh_updated(const surface_mesh::Surface_mesh& mesh) {
  if (drawing_plugin_) {
    drawing_plugin_->mesh_updated(mesh);
  }
  updateGL();
}

void ViewerPluginWidget::init() {
  if (drawing_plugin_) {
    drawing_plugin_->init_opengl();
  }
}

void ViewerPluginWidget::draw() {
  if (drawing_plugin_) {
    drawing_plugin_->draw_opengl();
  }
}

void ViewerPluginWidget::set_drawing_plugin(SubVisPlugin* plugin) {
  drawing_plugin_ = plugin;
}

} // namespace subvis
