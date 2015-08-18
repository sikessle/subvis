#ifndef SUBVIS_VIEW_VIEWER_PLUGIN_WIDGET_H
#define SUBVIS_VIEW_VIEWER_PLUGIN_WIDGET_H

#include "view/viewer_widget.h"

namespace SubVis {
namespace View {

class ViewerPluginWidget : public ViewerWidget
{
public:
    ViewerPluginWidget(QWidget *parent);

protected:
    void draw();
};

} // namespace View
} // namespace SubVis
#endif // SUBVIS_VIEW_VIEWER_PLUGIN_WIDGET_H
