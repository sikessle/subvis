
// ===============[ includes ]===============

#include <QDebug>

#include "surface_mesh/Surface_mesh.h"
#include "surface_mesh/IO.h"

#include "sd_catmull.h"
#include "utils.h"

namespace SubVis {
namespace Algo {

// ===============[ private prototypes ]===============

/**
 * @brief compute_face_point A face point is the average of all the points of the face
 * @param face_point computed face point (result)
 * @param face
 * @param mesh
 */
static void compute_face_point(surface_mesh::Point& face_point, const surface_mesh::Surface_mesh::Face& face, const surface_mesh::Surface_mesh& mesh);

/**
 * @brief compute_edge_point An edge point is the the average of the two control points on either side of the edge,
 *                           and the face-points of the touching faces
 * @param edge_point computed edge point (result)
 * @param edge
 * @param mesh
 */
static void compute_edge_point(surface_mesh::Point& edge_point, const surface_mesh::Surface_mesh::Edge& edge, const surface_mesh::Surface_mesh& mesh);

// ===============[ public implementation ]===============

void test_surface_mesh_read() {
    surface_mesh::Surface_mesh mesh;
    std::string path = kRootPathToObjFiles + kObjDemoFilesString[kCube];
    surface_mesh::read_mesh(mesh, path);
    utils_q_debug_mesh(mesh, QString("Cube Mesh"));
    sd_catmull(mesh);
}


void sd_catmull(surface_mesh::Surface_mesh& mesh) {
    // instantiate iterator
    surface_mesh::Surface_mesh::Face_iterator fit;
    // loop over all vertices
    for (fit = mesh.faces_begin(); fit != mesh.faces_end(); ++fit)
    {
        // TODO subdivision
        surface_mesh::Point face_point;
        compute_face_point(face_point, *fit, mesh);

        // TODO ...
    }
}

// ===============[ private implementation ]===============

void compute_face_point(surface_mesh::Point& face_point, const surface_mesh::Surface_mesh::Face& face, const surface_mesh::Surface_mesh& mesh) {
    // init result with zero
    face_point = surface_mesh::Point(0);
    // get (pre-defined) property storing vertex positions
    surface_mesh::Surface_mesh::Vertex_property<surface_mesh::Point> points = mesh.get_vertex_property<surface_mesh::Point>("v:point");
    // declare and initialize circulators
    surface_mesh::Surface_mesh::Vertex_around_face_circulator vc, vc_end;
    vc = mesh.vertices(face);
    vc_end = vc;
    int i = 0;
    // loop over all incident vertices
    do {
        face_point += points[*vc];
        ++i;
    } while (++vc != vc_end);
    if (i != 0)
        face_point /= i;
}

void compute_edge_point(surface_mesh::Point& edge_point, const surface_mesh::Surface_mesh::Edge& edge, const surface_mesh::Surface_mesh& mesh) {
    // init result with zero
    edge_point = surface_mesh::Point(0);
    // TODO


}


} // namespace Algo
} // namespace SubVis
