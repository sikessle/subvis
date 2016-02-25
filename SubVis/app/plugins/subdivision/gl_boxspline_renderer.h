#ifndef SUBVIS_PLUGINS_SUBDIVISION_GL_BOXSPLINE_RENDERER_H
#define SUBVIS_PLUGINS_SUBDIVISION_GL_BOXSPLINE_RENDERER_H

#include <QtOpenGL>

#include "plugins/subdivision/gl_renderer.h"

namespace subdivision {

/// Render Loop subdivision (convert to bezier triangle) TODO!
class GLBoxSplineRenderer : public GLRenderer {
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
  void create_color_values(void);
  void draw_mesh(void);
};

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_GL_BOXSPLINE_RENDERER_H

