#include "view/viewer_widget.h"

namespace SubVis {

ViewerWidget::ViewerWidget(QWidget *parent, DrawController &draw_ctrl)
    : QGLViewer{parent}, draw_controller(draw_ctrl)
{
    QObject::connect(&draw_controller, SIGNAL(redraw_required()),
                     this, SLOT(redraw_required()));
}

void ViewerWidget::draw()
{
    draw_mesh(draw_controller.mesh());
}

void ViewerWidget::redraw_required()
{
    draw();
}

} // namespace SubVis
