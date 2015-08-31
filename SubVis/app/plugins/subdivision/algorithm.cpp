#include "plugins/subdivision/algorithm.h"

namespace subdivision {

using surface_mesh::Surface_mesh;
using surface_mesh::Point;

std::unique_ptr<Surface_mesh> Algorithm::subdivide(Surface_mesh& mesh, int steps)
{
    result_mesh_.reset(new Surface_mesh);
    input_mesh_.reset(new Surface_mesh{mesh});

    for (int i = 0; i < steps; i++) {
        result_mesh_->clear();
        DEBUG_MESH(*input_mesh_.get(), "input mesh")
        subdivide_specific_algorithm();
        DEBUG_MESH(*result_mesh_.get(), "result mesh")
        // input mesh is now the previous result mesh
        input_mesh_.reset(new Surface_mesh{*(result_mesh_.get())});
    }

    // free memory
    input_mesh_.reset(nullptr);

    return std::move(result_mesh_);
}

void Algorithm::draw_limit_surface(const Surface_mesh& mesh)
{
    // TODO draw limit surface.
    // Put common drawing code in this function but put
    // algorithm specific drawing in each subdiv class and override this
    // function.

    // just a placeholder for the compiler
    if (mesh.empty()) {

    }

    // placeholder demo code:
    const float nbSteps = 200.0;

    glBegin(GL_QUAD_STRIP);
    for (int i=0; i<nbSteps; ++i) {
        const float ratio = i/nbSteps;
        const float angle = 21.0*ratio;
        const float c = cos(angle);
        const float s = sin(angle);
        const float r1 = 1.0 - 0.8f*ratio;
        const float r2 = 0.8f - 0.8f*ratio;
        const float alt = ratio - 0.5f;
        const float nor = 0.5f;
        const float up = sqrt(1.0-nor*nor);
        glColor3f(1.0-ratio, 0.2f , ratio);
        glNormal3f(nor*c, up, nor*s);
        glVertex3f(r1*c, alt, r1*s);
        glVertex3f(r2*c, alt+0.05f, r2*s);
    }
    glEnd();
}

Surface_mesh& Algorithm::result_mesh()
{
    return *(result_mesh_.get());
}

void Algorithm::init_mesh_members()
{
    v_points_ = input_mesh_->get_vertex_property<Point>(kSurfMeshPropVertexPoint);
}

void Algorithm::compute_face_point(Point& face_point, const Surface_mesh::Face& face)
{
    // init result with zero
    face_point = Point(0);
    // declare and initialize circulators
    Surface_mesh::Vertex_around_face_circulator vc, vc_end;
    vc = input_mesh_->vertices(face);
    vc_end = vc;
    int i = 0;
    // loop over all incident vertices
    do {
        face_point += v_points_[*vc];
        ++i;
    } while (++vc != vc_end);
    if (i != 0)
        face_point /= i;
}

void Algorithm::mid_edge(Point& mid_edge, const Surface_mesh::Edge& edge)
{
    Surface_mesh::Vertex edge_vertex0, edge_vertex1;
    edge_vertex0 = input_mesh_->vertex(edge, 0);
    edge_vertex1 = input_mesh_->vertex(edge, 1);
    mid_edge = v_points_[edge_vertex0] + v_points_[edge_vertex1];
    mid_edge /= 2;
}

} // namespace subdivision
