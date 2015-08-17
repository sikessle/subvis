#ifndef SUBVIS_VIEW_MESH_RENDER_WIDGET_H
#define SUBVIS_VIEW_MESH_RENDER_WIDGET_H

#include "view/render_widget.h"

namespace SubVis {
namespace View {

class MeshRenderWidget : public RenderWidget
{
public:
    MeshRenderWidget(QWidget *parent);

protected:
    virtual void draw();
};

} // namespace View
} // namespace SubVis
#endif // SUBVIS_VIEW_MESH_RENDER_WIDGET_H
