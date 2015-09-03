#include "view/viewer_widget.h"

namespace subvis {

ViewerWidget::ViewerWidget(QWidget* parent) : QGLViewer{parent} {
}

void ViewerWidget::set_draw_controller(DrawController* draw_controller) {
  assert(draw_controller);
  draw_controller_ = draw_controller;
}

void ViewerWidget::enforce_redraw() {
  // do not call draw() or init() directly.
  updateGL();
}

} // namespace subvis
