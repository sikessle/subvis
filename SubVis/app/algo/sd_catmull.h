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

#ifndef SUBVIS_SD_CATMULL_H
#define SUBVIS_SD_CATMULL_H

// ===============[ includes ]===============
#include "surface_mesh/Surface_mesh.h"
#include "algo/types.h"

#include "algo/sd_base.h"

namespace SubVis {

using surface_mesh::Surface_mesh;
using surface_mesh::Point;


// ===============[ public prototypes ]===============

class SubdivCatmull : public SubdivBase {

public:

    SubdivCatmull(Surface_mesh mesh) : SubdivBase(mesh) {
        this->add_mesh_properties(mesh_);
        this->init_mesh_members();
    }

    void subdivide();
    void subdivide(unsigned char steps);

    /**
     * @brief get_subdivision_mesh get the result mesh after calling subdivision
     * @return The subdivided mesh (result of subdivide() method)
     */
    Surface_mesh get_subdivision_mesh() {
        return mesh_;
    }

private:
    Surface_mesh::Vertex_property<Point> v_points_;
    Surface_mesh::Face_property<Point> f_points_;
    Surface_mesh::Edge_property<Point> e_points_;
    Surface_mesh::Vertex_property<Point> v_points_updated_;

    // vertex index properties to map from origin mesh to subdivision mesh
    Surface_mesh::Vertex_property<Surface_mesh::Vertex> v_index_sub_mesh_v_prop_;
    Surface_mesh::Edge_property<Surface_mesh::Vertex> v_index_sub_mesh_e_prop_;
    Surface_mesh::Face_property<Surface_mesh::Vertex> v_index_sub_mesh_f_prop_;

    /**
     * @brief compute_face_point A face point is the average of all the points of the face.
     *                           The Face Point is stored as surface mesh property of every face
     */
    void compute_face_point(const Surface_mesh::Face& face);

    /**
     * @brief compute_edge_point An edge point is the the average of the two control points on either side of the edge,
     *                           and the face-points of the touching faces
     *                           - all face points have to be computed before usage!
     */
    void compute_edge_point(const Surface_mesh::Edge& edge);


    /**
     * @brief compute_new_vertex_point Compute the new vertex coordinates: (Q/n) + (2R/n) + (S(n-3)/n)
     *  n - valence
     *  Q - average of the surrounding face points
     *  R - average of all surround edge midpoints
     *  S - old control point
     *  The result is stored as property in the mesh (kSurfMeshPropVertexPointUpdated)
     * @param vertex old vertex
     */
    void compute_new_vertex_point(const Surface_mesh::Vertex& vertex);

    void compute_new_faces(Surface_mesh& result_mesh, const Surface_mesh::Face& face);

    void avg_face_points(Point& avg_face_points, const Surface_mesh::Vertex& vertex);

    void avg_mid_edges(Point& avg_mid_edges, const Surface_mesh::Vertex& vertex);

    void mid_edge(Point& mid_edge, const Surface_mesh::Edge &edge);

    void add_mesh_properties(Surface_mesh& mesh) {
        // add properties that are necessary for catmull clark
        mesh.add_face_property<Point>(kSurfMeshPropFacePoint);
        mesh.add_edge_property<Point>(kSurfMeshPropEdgePoint);
        mesh.add_vertex_property<Point>(kSurfMeshPropVertexPointUpdated);
        // (vertex point property with key kSurfMeshPropVertexPoint is maintained by default)

        // index properties
        mesh.add_vertex_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshV);
        mesh.add_edge_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshE);
        mesh.add_face_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshF);
    }

    void init_mesh_members() {
        v_points_ = mesh_.get_vertex_property<Point>(kSurfMeshPropVertexPoint);
        f_points_ = mesh_.get_face_property<Point>(kSurfMeshPropFacePoint);
        e_points_ = mesh_.get_edge_property<Point>(kSurfMeshPropEdgePoint);
        v_points_updated_ = mesh_.get_vertex_property<Point>(kSurfMeshPropVertexPointUpdated);

        v_index_sub_mesh_v_prop_ = mesh_.get_vertex_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshV);
        v_index_sub_mesh_e_prop_ = mesh_.get_edge_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshE);
        v_index_sub_mesh_f_prop_ = mesh_.get_face_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshF);
    }
};


// ===============[ public prototypes ]===============

void test_catmull();

} // namespace SubVis
#endif // SUBVIS_SD_CATMULL_H

