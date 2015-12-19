#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_NONE_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_NONE_RENDERER_H

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

  ///
  /// \brief A dummy renderer which does nothing.
  ///
class GLNoneRenderer : public GLRenderer {
 public:
  void render_mesh_opengl() override {}
  void init_opengl() override {}
  void mesh_updated(const Surface_mesh& /*mesh*/) override {}

 protected:
  void render() override {}
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_NONE_RENDERER_H
