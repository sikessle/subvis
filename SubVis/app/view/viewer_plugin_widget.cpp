#include "view/viewer_plugin_widget.h"

namespace SubVis {

ViewerPluginWidget::ViewerPluginWidget(QWidget *parent, DrawController &draw_ctrl)
    : ViewerWidget{parent, draw_ctrl}
{
}

void ViewerPluginWidget::draw_mesh(Surface_mesh &mesh)
{

}

} // namespace SubVis
