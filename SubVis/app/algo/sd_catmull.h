/**
 *
 * @brief Implementation of the catmull-clark subdivision algorithm.
 *
 * 1. create face point for each face
 * 2. create edge point for each edge
 * 3. update vertex point coordinates
 * (http://rosettacode.org/wiki/Catmull%E2%80%93Clark_subdivision_surface)
 *
 * @author Felix Born
 *
 */

#ifndef SD_CATMULL_H
#define SD_CATMULL_H

// ===============[ includes ]===============
#include "surface_mesh/Surface_mesh.h"

#include "types.h"

namespace SubVis {
namespace Algo {

// ===============[ public prototypes ]===============

class SubdivCatmull {
private:
    surface_mesh::Surface_mesh mesh_;

    /**
     * @brief compute_face_point A face point is the average of all the points of the face.
     *                           The Face Point is stored as surface mesh property of every face
     * @param face_point computed face point (result)
     * @param face
     */
    void compute_face_point(surface_mesh::Point& face_point, const surface_mesh::Surface_mesh::Face& face);

    /**
     * @brief compute_edge_point An edge point is the the average of the two control points on either side of the edge,
     *                           and the face-points of the touching faces
     * @param edge_point computed edge point (result)
     * @param edge
     * @param mesh
     */
    void compute_edge_point(surface_mesh::Point& edge_point, const surface_mesh::Surface_mesh::Edge& edge);

public:
    SubdivCatmull (surface_mesh::Surface_mesh mesh) : mesh_(mesh) {
        // add properties that are necessary for catmull clark
        mesh_.add_face_property<surface_mesh::Point>(kSurfMeshPropFacePoint);
        mesh_.add_edge_property<surface_mesh::Point>(kSurfMeshPropEdgePoint);
        mesh_.add_vertex_property<surface_mesh::Point>(kSurfMeshPropVertexPointUpdated);
        // (vertex point property with key kSurfMeshPropVertexPoint is maintained by default)
    }

    void subdivide(int steps);
};


// ===============[ public prototypes ]===============

void test_surface_mesh_read();

} // namespace Algo
} // namespace SubVis

#endif // SD_CATMULL_H

