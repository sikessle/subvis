#ifndef SUBVIS_VIEW_MESH_RENDER_WIDGET_H
#define SUBVIS_VIEW_MESH_RENDER_WIDGET_H

#include "QWidget"
#include "QGLViewer/qglviewer.h"

namespace SubVis {
namespace View {

class MeshRenderWidget : public QGLViewer
{
public:
    MeshRenderWidget();

protected:
    virtual void draw();
};

} // namespace View
} // namespace SubVis
#endif // SUBVIS_VIEW_MESH_RENDER_WIDGET_H
