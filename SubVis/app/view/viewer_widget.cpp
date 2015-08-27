#include "view/viewer_widget.h"

namespace SubVis {

ViewerWidget::ViewerWidget(QWidget* parent) : QGLViewer{parent}
{
}

void ViewerWidget::set_draw_controller(DrawController* draw_controller)
{
    draw_controller_ = draw_controller;
}

void ViewerWidget::enforce_redraw()
{
    // do not call draw() directly.
    updateGL();
}

} // namespace SubVis
