#include "view/viewer_widget.h"

namespace subvis {

ViewerWidget::ViewerWidget(QWidget* parent) : QGLViewer{parent} {
}

void ViewerWidget::set_draw_controller(DrawController& draw_controller) {
  draw_controller_ = &draw_controller;
}

} // namespace subvis
