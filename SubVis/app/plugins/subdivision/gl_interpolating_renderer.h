#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_INTERPOLATING_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_INTERPOLATING_RENDERER_H

#include <vector>
#include <QtOpenGL>

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

/// Interpolate the control points of a triangle mesh (Butterfly/modified Butterfly) TODO!
class GLInterpolatingRenderer : public GLRenderer {
 public:
  void mesh_updated(const Surface_mesh& mesh) override;

 protected:
  void render() override;

 private:
  std::vector<unsigned int> triangles_;
  std::vector<float> color_values_;
  Surface_mesh::Vertex_property<Point>  points_;
  Surface_mesh::Vertex_property<Point>  vnormals_;

  GLenum drawing_type_;
  GLenum shading_type_;
  bool coloring_active_;
  bool lighting_active_;

  void init_gl(void);
  void create_color_values();
  void draw_mesh(void);
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_INTERPOLATING_RENDERER_H

