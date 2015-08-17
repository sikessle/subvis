#ifndef SUBVIS_VIEW_PLUGIN_RENDER_WIDGET_H
#define SUBVIS_VIEW_PLUGIN_RENDER_WIDGET_H

#include "view/render_widget.h"

namespace SubVis {
namespace View {

class PluginRenderWidget : public RenderWidget
{
public:
    PluginRenderWidget(QWidget *parent);

protected:
    virtual void draw();
};

} // namespace View
} // namespace SubVis
#endif // SUBVIS_VIEW_PLUGIN_RENDER_WIDGET_H
