#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H

#include <QObject>
#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

class GLRenderer {
 public:
  virtual ~GLRenderer();

  virtual void render_mesh_opengl();
  /// Is called once before render_mesh_opengl().
  /// Override for specific settings (background color etc.)
  virtual void init_opengl();
  /// Extracts the data required to render to a custom data structure.
  virtual void mesh_updated(const surface_mesh::Surface_mesh& mesh);

 protected:
  // TODO placeholder, here we should use a custom data structure to hold vertices
  const surface_mesh::Surface_mesh* mesh_ {nullptr};

  /**
   * @brief Called with the custom data structure TODO!!
   */
  virtual void render(const surface_mesh::Surface_mesh& mesh) = 0;
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
