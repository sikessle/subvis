#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_INTERPOLATING_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_INTERPOLATING_RENDERER_H

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

/// render Butterfly - simply interpolate the points
class GLInterpolatingRenderer : public GLRenderer {
 protected:
  void render(const surface_mesh::Surface_mesh& /*mesh*/) override {}
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_INTERPOLATING_RENDERER_H

