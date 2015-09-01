#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H

#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

class GLRenderer {
 public:
  virtual ~GLRenderer();

  /**
   * @brief Renders the given mesh. Calls start_rendering() and end_rendering()
   * before respective after calling render().
   * @param mesh
   */
  virtual void render_mesh_opengl(const surface_mesh::Surface_mesh& mesh);

 protected:
  /**
  * @brief Called before render()
  */
  virtual void start_rendering();
  /**
  * @brief Main render method
  * @param mesh
  */
  virtual void render(const surface_mesh::Surface_mesh& mesh) = 0;
  /**
  * @brief Called after render()
  */
  virtual void end_rendering();
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_RENDERER_H
