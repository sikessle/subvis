#ifndef SUBVIS_VIEW_PLUGIN_RENDER_WIDGET_H
#define SUBVIS_VIEW_PLUGIN_RENDER_WIDGET_H

#include "QWidget"
#include "QGLViewer/qglviewer.h"

namespace SubVis {
namespace View {

class PluginRenderWidget : public QGLViewer
{
public:
    PluginRenderWidget();

protected:
    virtual void draw();
};

} // namespace View
} // namespace SubVis

#endif // SUBVIS_VIEW_PLUGIN_RENDER_WIDGET_H
