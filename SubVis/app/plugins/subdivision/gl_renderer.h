#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H

#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

class GLRenderer {
 public:
  virtual ~GLRenderer();

  /**
   * @brief Renders the given mesh
   * @param mesh
   */
  virtual void render_mesh_opengl(const surface_mesh::Surface_mesh& mesh) = 0;

 protected:
  // add common methods for rendering here (like init OpenGL, ..)
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
