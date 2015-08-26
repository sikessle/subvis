#ifndef SUBVIS_VIEWER_PLUGIN_WIDGET_H
#define SUBVIS_VIEWER_PLUGIN_WIDGET_H

#include "view/viewer_widget.h"

namespace SubVis {

using surface_mesh::Surface_mesh;

class ViewerPluginWidget : public ViewerWidget
{
public:
    ViewerPluginWidget(QWidget *parent = 0);

protected:
    void draw_mesh(Surface_mesh &mesh);
};

} // namespace SubVis
#endif // SUBVIS_VIEWER_PLUGIN_WIDGET_H
