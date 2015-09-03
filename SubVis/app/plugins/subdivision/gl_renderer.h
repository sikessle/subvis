#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H

#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

class GLRenderer {
 public:
  virtual ~GLRenderer();

  /**
   * @brief Renders the given mesh.
   * @param mesh
   */
  virtual void render_mesh_opengl(const surface_mesh::Surface_mesh& mesh);
  /**
  * @brief Is called once before render_mesh_opengl()
  * Override for specific settings.
  */
  virtual void init_opengl();

 protected:
  /**
  * @brief Main render method which does the main job of rendering.
  * @param mesh
  */
  virtual void render(const surface_mesh::Surface_mesh& mesh) = 0;
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
