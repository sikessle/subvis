#include "view/viewer_plugin_widget.h"

namespace SubVis {

ViewerPluginWidget::ViewerPluginWidget(QWidget *parent) : ViewerWidget{parent}
{
}

void ViewerPluginWidget::draw()
{
    if (drawing_plugin) {
        drawing_plugin->draw_opengl();
    }
}

void ViewerPluginWidget::set_drawing_plugin(SubVisPlugin *plugin)
{
    drawing_plugin = plugin;
}

} // namespace SubVis
