#include "GL/gl.h"

#include "plugins/subdivision/gl_bspline_renderer.h"

namespace subdivision {

void BSplineGLRenderer::render(const surface_mesh::Surface_mesh* /*mesh*/) {
  // placeholder demo code:
  const float nbSteps = 200.0;

  for (int i = 0; i < nbSteps; ++i) {
    const float ratio = i / nbSteps;
    const float angle = 21.0 * ratio;
    const float c = cos(angle);
    const float s = sin(angle);
    const float r1 = 1.0 - 0.8f * ratio;
    const float r2 = 0.8f - 0.8f * ratio;
    const float alt = ratio - 0.5f;
    const float nor = 0.5f;
    const float up = sqrt(1.0 - nor * nor);
    glColor3f(1.0 - ratio, 0.2f , ratio);
    glNormal3f(nor * c, up, nor * s);
    glVertex3f(r1 * c, alt, r1 * s);
    glVertex3f(r2 * c, alt + 0.05f, r2 * s);
  }
}

} // namespace subdivision
