#ifndef SUBVIS_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEWER_MESH_WIDGET_H

#include "view/viewer_widget.h"

namespace subvis {

class ViewerMeshWidget : public ViewerWidget {
 public:
  ViewerMeshWidget(QWidget* parent = 0);

 protected:
  void draw() override;
};


} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
