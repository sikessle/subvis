#include "plugins/subdivision/algorithm.h"
#include "plugins/subdivision/types.h"

namespace SubdivisionPlugin {

using surface_mesh::Surface_mesh;
using surface_mesh::Point;

void Algorithm::subdivide(SubVis::MeshData& mesh_data, int steps)
{
    result_mesh_.reset(new Surface_mesh);
    input_mesh_.reset(new Surface_mesh{mesh_data.mesh()});

    for (int i = 0; i < steps; i++) {
        result_mesh_->clear();
        subdivide_specific_algorithm();
        // input mesh is now the previous result mesh
        input_mesh_.reset(new Surface_mesh{*(result_mesh_.get())});
    }

    mesh_data.load(std::move(result_mesh_));

    // free memory
    result_mesh_.reset(nullptr);
    input_mesh_.reset(nullptr);
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

} // namespace SubdivisionPlugin
