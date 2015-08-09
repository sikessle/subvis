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

    surface_mesh::Surface_mesh::Vertex_property<surface_mesh::Point> v_points_;
    surface_mesh::Surface_mesh::Face_property<surface_mesh::Point> f_points_;
    surface_mesh::Surface_mesh::Edge_property<surface_mesh::Point> e_points_;
    surface_mesh::Surface_mesh::Vertex_property<surface_mesh::Point> v_points_updated_;

    surface_mesh::Surface_mesh::Face_property<bool> f_points_is_set_;
    surface_mesh::Surface_mesh::Edge_property<bool> e_points_is_set_;

    /**
     * @brief compute_face_point A face point is the average of all the points of the face.
     *                           The Face Point is stored as surface mesh property of every face
     */
    void compute_face_point(const surface_mesh::Surface_mesh::Face& face);

    /**
     * @brief compute_edge_point An edge point is the the average of the two control points on either side of the edge,
     *                           and the face-points of the touching faces
     *                           - all face points have to be computed before usage!
     */
    void compute_edge_point(const surface_mesh::Surface_mesh::Edge& edge);


    /**
     * @brief compute_new_vertex_point Compute the new vertex coordinates: (Q/n) + (2R/n) + (S(n-3)/n)
     *  n - valence
     *  Q - average of the surrounding face points
     *  R - average of all surround edge midpoints
     *  S - old control point
     *  The result is stored as property in the mesh (kSurfMeshPropVertexPointUpdated)
     * @param vertex old vertex
     */
    void compute_new_vertex_point(const surface_mesh::Surface_mesh::Vertex& vertex);

    void compute_new_faces(surface_mesh::Surface_mesh& result_mesh, const surface_mesh::Surface_mesh::Face& face);

    void avg_face_points(surface_mesh::Point& avg_face_points, const surface_mesh::Surface_mesh::Vertex& vertex);

    void avg_mid_edges(surface_mesh::Point& avg_mid_edges, const surface_mesh::Surface_mesh::Vertex& vertex);

    void mid_edge(surface_mesh::Point& mid_edge, const surface_mesh::Surface_mesh::Edge &edge);

    void add_mesh_properties(surface_mesh::Surface_mesh& mesh) {
        // add properties that are necessary for catmull clark
        mesh.add_face_property<surface_mesh::Point>(kSurfMeshPropFacePoint);
        mesh.add_edge_property<surface_mesh::Point>(kSurfMeshPropEdgePoint);
        mesh.add_vertex_property<surface_mesh::Point>(kSurfMeshPropVertexPointUpdated);
        // (vertex point property with key kSurfMeshPropVertexPoint is maintained by default)
    }

public:
    SubdivCatmull (surface_mesh::Surface_mesh mesh) : mesh_(mesh) {
        this->add_mesh_properties(mesh_);
        // init properties
        v_points_ = mesh_.get_vertex_property<surface_mesh::Point>(kSurfMeshPropVertexPoint);
        f_points_ = mesh_.get_face_property<surface_mesh::Point>(kSurfMeshPropFacePoint);
        e_points_ = mesh_.get_edge_property<surface_mesh::Point>(kSurfMeshPropEdgePoint);
        v_points_updated_ = mesh_.get_vertex_property<surface_mesh::Point>(kSurfMeshPropVertexPointUpdated);
    }

    void subdivide(int steps);
};


// ===============[ public prototypes ]===============

void test_surface_mesh_read();

} // namespace Algo
} // namespace SubVis

#endif // SD_CATMULL_H

