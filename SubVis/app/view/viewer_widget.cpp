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

/**
 * @brief Called by QGLViewer
 */
void ViewerWidget::draw()
{
    if (!draw_controller) {
        throw logic_error("draw_controller is null. call set_draw_controller() before.");
    }
    draw_mesh(draw_controller->mesh());
}

void ViewerWidget::enforce_redraw()
{
    draw();
}

} // namespace SubVis
