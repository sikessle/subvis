#ifndef SUBVIS_VIEW_VIEWER_WIDGET_H
#define SUBVIS_VIEW_VIEWER_WIDGET_H

#include "QGLViewer/qglviewer.h"

#include "model/mesh_data.h"

namespace subvis {

class ViewerWidget : public QGLViewer {
    Q_OBJECT

 public:
  ViewerWidget(QWidget* parent = 0, int mesh_id = 0);

  void set_model(MeshData& mesh_data);
  void set_mesh_id(int mesh_id);

 protected:
  MeshData* mesh_data_{nullptr};
  int mesh_id_{0};

  void draw() override = 0;
  void init() override = 0;
  virtual void mesh_updated(const surface_mesh::Surface_mesh& mesh) = 0;

 public slots:
  void mesh_updated(
    std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>
    meshes);
};

} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_WIDGET_H
