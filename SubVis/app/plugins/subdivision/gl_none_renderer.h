#ifndef GLNONERENDERER_H
#define GLNONERENDERER_H

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

class GLNoneRenderer : public GLRenderer {
 public:
  void render_mesh_opengl() override {}
  void init_opengl() override {}
  void mesh_updated(const surface_mesh::Surface_mesh& /*mesh*/) override {}

 protected:
  void render(const surface_mesh::Surface_mesh& /*mesh*/) override {}
};

} // namespace subdivision
#endif // GLNONERENDERER_H
