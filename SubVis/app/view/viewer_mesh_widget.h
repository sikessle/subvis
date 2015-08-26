#ifndef SUBVIS_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEWER_MESH_WIDGET_H

#include "view/viewer_widget.h"

namespace SubVis {

using surface_mesh::Surface_mesh;

class ViewerMeshWidget : public ViewerWidget
{
public:
    ViewerMeshWidget(QWidget *parent = 0);

protected:
    void draw_mesh(Surface_mesh &mesh);
};


} // namespace SubVis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
