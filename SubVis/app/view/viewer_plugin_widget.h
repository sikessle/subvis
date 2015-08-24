#ifndef SUBVIS_VIEWER_PLUGIN_WIDGET_H
#define SUBVIS_VIEWER_PLUGIN_WIDGET_H

#include "view/viewer_widget.h"

namespace SubVis {

class ViewerPluginWidget : public ViewerWidget
{
public:
    ViewerPluginWidget(QWidget *parent);

protected:
    void draw();
};

} // namespace SubVis
#endif // SUBVIS_VIEWER_PLUGIN_WIDGET_H
