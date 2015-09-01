#ifndef SUBVIS_PLUGINS_SUBDIVISION_QL_BSPLINE_QLRENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_QL_BSPLINE_QLRENDERER_H

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

class BSplineGLRenderer : public GLRenderer {
 protected:
  void render(const surface_mesh::Surface_mesh& mesh) override;
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_QL_BSPLINE_QLRENDERER_H