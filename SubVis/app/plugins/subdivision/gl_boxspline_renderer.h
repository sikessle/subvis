#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_BOXSPLINE_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_BOXSPLINE_RENDERER_H

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

/// render Loop subdivision (convert to bezier triangle)
class GLBoxSplineRenderer : public GLRenderer {
 protected:
  void render() override {}
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_BOXSPLINE_RENDERER_H

