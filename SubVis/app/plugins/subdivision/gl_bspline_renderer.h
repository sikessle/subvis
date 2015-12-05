#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_BSPLINE_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_BSPLINE_RENDERER_H

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

/// render Catmull-Clark and Doo Sabin (convert to bezier surface)
class GLBSplineRenderer : public GLRenderer {
 protected:
  void render() override;
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_BSPLINE_RENDERER_H
