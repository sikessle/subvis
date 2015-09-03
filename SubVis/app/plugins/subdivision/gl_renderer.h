#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H

#include <QObject>
#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

class GLRenderer : public QObject {
  Q_OBJECT

 public:
  virtual ~GLRenderer();

  void render_mesh_opengl();
  /**
  * @brief Is called once before render_mesh_opengl()
  * Override for specific settings.
  */
  virtual void init_opengl();

 protected:
  // TODO placeholder, here we should use a custom data structure to hold vertices
  const surface_mesh::Surface_mesh* mesh_ {nullptr};

  /**
   * @brief Guaranteed to be called only if the mesh is available.
   */
  virtual void render() = 0;

 public slots:
  /**
   * @brief Extracts the data required to render to a custom data structure.
   * @param mesh
   */
  virtual void mesh_updated(const surface_mesh::Surface_mesh& mesh);
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
