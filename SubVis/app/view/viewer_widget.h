#ifndef SUBVIS_VIEW_VIEWER_WIDGET_H
#define SUBVIS_VIEW_VIEWER_WIDGET_H

#include "QGLViewer/qglviewer.h"

#include "model/mesh_data.h"

namespace subvis {

///
/// \brief A abstract base class which takes care of handling the mesh updates etc.
///
class ViewerWidget : public QGLViewer {
  Q_OBJECT

 public:
  ///
  /// \brief Constructor.
  /// \param parent
  /// \param mesh_id The mesh idx which this viewer should use from the mesh data object.
  ///
  ViewerWidget(QWidget* parent = 0, int mesh_id = 0);
  ///
  /// \brief Sets the model
  /// \param mesh_data
  ///
  void set_model(MeshData& mesh_data);
  ///
  /// \brief Callback if the mesh data was updated. Will call updateGL() after
  /// informing subclasses about the update.
  /// \param meshes The updates meshes.
  ///
  void mesh_updated_self(
    std::pair<const surface_mesh::Surface_mesh&, const surface_mesh::Surface_mesh&>
    meshes);

 protected:
  MeshData* mesh_data_ {nullptr};
  int mesh_id_{0};

  void draw() override;
  void init() override;
  ///
  /// \brief Subclasses should do here their draw calls.
  ///
  virtual void draw_gl() = 0;
  ///
  /// \brief Subclasses can initialize here the opengl context.
  ///
  virtual void init_gl() = 0;
  ///
  /// \brief Will be called if the mesh was updated. No need to call updateGL()
  /// afterwards as this is handled by this class.
  /// \param mesh The mesh which is associated with the mesh_id of this viewer.
  ///
  virtual void mesh_updated(const surface_mesh::Surface_mesh& mesh) = 0;
};

} // namespace subvis
#endif // SUBVIS_VIEW_VIEWER_WIDGET_H
