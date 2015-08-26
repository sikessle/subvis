#include <exception>
#include "view/viewer_widget.h"

namespace SubVis {

using std::logic_error;

ViewerWidget::ViewerWidget(QWidget *parent) : QGLViewer{parent}
{
}

void ViewerWidget::set_draw_controller(DrawController *draw_ctrl)
{
    draw_controller = draw_ctrl;
}

void ViewerWidget::enforce_redraw()
{
    draw();
}

} // namespace SubVis
