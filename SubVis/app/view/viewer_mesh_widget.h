#ifndef SUBVIS_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEWER_MESH_WIDGET_H

#include "view/viewer_widget.h"

namespace SubVis {

class ViewerMeshWidget : public ViewerWidget
{
public:
    ViewerMeshWidget(QWidget *parent);

protected:
    void draw();
};


} // namespace SubVis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
