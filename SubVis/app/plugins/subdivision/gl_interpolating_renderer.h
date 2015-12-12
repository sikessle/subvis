#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_INTERPOLATING_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_INTERPOLATING_RENDERER_H

#include <vector>

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

/// Interpolate the control points of a triangle mesh (Butterfly/modified Butterfly).
class GLInterpolatingRenderer : public GLRenderer {
 public:
  void mesh_updated(const Surface_mesh& mesh) override;

 protected:
  void render() override;

 private:
  std::vector<unsigned int> triangles_;
  Surface_mesh::Vertex_property<Point>  points_;
  Surface_mesh::Vertex_property<Point>  vnormals_;
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_INTERPOLATING_RENDERER_H

