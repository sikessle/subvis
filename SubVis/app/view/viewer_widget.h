#ifndef SUBVIS_VIEW_VIEWER_WIDGET_H
#define SUBVIS_VIEW_VIEWER_WIDGET_H

#include "QGLViewer/qglviewer.h"

#include "model/mesh_data.h"

namespace subvis {

class ViewerWidget : public QGLViewer {
 public:
  ViewerWidget(QWidget* parent = 0);

  void set_model(MeshData& mesh_data);

 protected:
  MeshData* mesh_data_{nullptr};

  void draw() override = 0;
  void init() override = 0;

 public slots:
  virtual void mesh_updated(const surface_mesh::Surface_mesh& mesh);
};

} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_WIDGET_H
