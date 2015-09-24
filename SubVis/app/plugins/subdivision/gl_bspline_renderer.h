#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_BSPLINE_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_BSPLINE_RENDERER_H

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

class GLBSplineRenderer : public GLRenderer {
 protected:
  void render(const surface_mesh::Surface_mesh& mesh) override;
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_BSPLINE_RENDERER_H
