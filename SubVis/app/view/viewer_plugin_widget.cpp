#include "view/viewer_plugin_widget.h"

namespace subvis {

ViewerPluginWidget::ViewerPluginWidget(QWidget* parent) : ViewerWidget{parent}
{
}

void ViewerPluginWidget::draw()
{
    if (drawing_plugin_) {
        drawing_plugin_->draw_opengl();
    }
}

void ViewerPluginWidget::set_drawing_plugin(SubVisPlugin* plugin)
{
    drawing_plugin_ = plugin;
}

} // namespace subvis
