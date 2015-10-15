#ifndef SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
#define SUBVIS_VIEW_VIEWER_MESH_WIDGET_H

#include "surface_mesh/Surface_mesh.h"

#include "view/viewer_widget.h"

namespace subvis {

class ViewerMeshWidget : public ViewerWidget {
  Q_OBJECT

 public:
  ViewerMeshWidget(QWidget* parent = 0);

 protected:
  void draw() override;
  void init() override;
  void mousePressEvent(QMouseEvent* e) override;

 private:
  // TODO placeholder, here we should use a custom data structure to hold vertices
  const surface_mesh::Surface_mesh* mesh_ {nullptr};

  void draw_mesh();
  /// RGB color values will be stored in rgb array
  void index_to_rgb(const int index, int rgb[3]);
  int rgb_to_index(const int rgb[3]);

 public slots:
  void mesh_updated(const surface_mesh::Surface_mesh& mesh) override;
};


} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_MESH_WIDGET_H
