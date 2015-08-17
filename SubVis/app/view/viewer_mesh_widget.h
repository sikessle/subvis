#ifndef SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEW_VIEWER_MESH_WIDGET_H

#include "view/viewer_widget.h"

namespace SubVis {
namespace View {

class ViewerMeshWidget : public ViewerWidget
{
public:
    ViewerMeshWidget(QWidget *parent);

protected:
    virtual void draw();
};

} // namespace View
} // namespace SubVis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
