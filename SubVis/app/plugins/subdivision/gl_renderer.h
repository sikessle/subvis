#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H

#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

class GLRenderer {
 public:
  virtual ~GLRenderer();

  /**
   * @brief Renders the given mesh.
   * It basically just before_render(), render() and after_render() (in this order).
   * @param mesh
   */
  virtual void render_mesh_opengl(const surface_mesh::Surface_mesh& mesh);

 protected:
  /**
  * @brief Can be used to initialize OpenGL etc.
  */
  virtual void before_render();
  /**
  * @brief Main render method which does the main job of rendering.
  * @param mesh
  */
  virtual void render(const surface_mesh::Surface_mesh& mesh) = 0;
  /**
  * @brief Can be used to deinitialize OpenGL etc.
  */
  virtual void after_render();
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
