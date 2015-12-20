#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_BSPLINE_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_BSPLINE_RENDERER_H

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

/// Render Catmull-Clark and Doo Sabin (convert to bezier surface) TODO!
class GLBSplineRenderer : public GLRenderer {
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
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_BSPLINE_RENDERER_H
