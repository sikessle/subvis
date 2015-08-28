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

#ifndef SUBDIVISIONPLUGIN_SD_CATMULL_H
#define SUBDIVISIONPLUGIN_SD_CATMULL_H

// ===============[ includes ]===============
#include "surface_mesh/Surface_mesh.h"
#include "plugins/subdivision/types.h"

#include "plugins/subdivision/algorithm.h"

namespace SubdivisionPlugin {

// ===============[ public prototypes ]===============


// mesh_ is input mesh
class SubdivCatmull : public Algorithm {

public:
    using Surface_mesh = surface_mesh::Surface_mesh;
    using Point = surface_mesh::Point;

    virtual const QString id();
    virtual const QString name();

protected:
    virtual void subdivide_specific_algorithm();

private:
    Surface_mesh::Face_property<Point> f_points_;
    Surface_mesh::Edge_property<Point> e_points_;
    Surface_mesh::Vertex_property<Point> v_points_updated_;

    // vertex index properties to map from origin mesh to subdivision mesh
    Surface_mesh::Vertex_property<Surface_mesh::Vertex> v_index_sub_mesh_v_prop_;
    Surface_mesh::Edge_property<Surface_mesh::Vertex> v_index_sub_mesh_e_prop_;
    Surface_mesh::Face_property<Surface_mesh::Vertex> v_index_sub_mesh_f_prop_;

    void compute_all_face_points();
    void compute_all_edge_points();
    void compute_all_new_vertex_points();

    /**
     * @brief compute_edge_point An edge point is the the average of the two control points on either side of the edge,
     *                           and the face-points of the touching faces
     *                           - all face points have to be computed before usage!
     */
    void compute_edge_point(Point &edge_point, const Surface_mesh::Edge& edge);

    /**
     * @brief compute_new_vertex_point Compute the new vertex coordinates: (Q/n) + (2R/n) + (S(n-3)/n)
     *  n - valence
     *  Q - average of the surrounding face points
     *  R - average of all surround edge midpoints
     *  S - old control point
     * @param vertex old vertex
     */
    void compute_new_vertex_point(Point& new_vertex_point, const Surface_mesh::Vertex& vertex);

    void compute_new_faces(const Surface_mesh::Face& face);

    void avg_face_points(Point& avg_face_points, const Surface_mesh::Vertex& vertex);

    void avg_mid_edges(Point& avg_mid_edges, const Surface_mesh::Vertex& vertex);

    void add_mesh_properties()
    {
        // add properties that are necessary for catmull clark
        input_mesh_->add_face_property<Point>(kSurfMeshPropFacePoint);
        input_mesh_->add_edge_property<Point>(kSurfMeshPropEdgePoint);
        input_mesh_->add_vertex_property<Point>(kSurfMeshPropVertexPointUpdated);
        // (vertex point property with key kSurfMeshPropVertexPoint is maintained by default)

        // index properties
        input_mesh_->add_vertex_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshV);
        input_mesh_->add_edge_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshE);
        input_mesh_->add_face_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshF);
    }

    void init_mesh_members()
    {
        Algorithm::init_mesh_members();
        f_points_ = input_mesh_->get_face_property<Point>(kSurfMeshPropFacePoint);
        e_points_ = input_mesh_->get_edge_property<Point>(kSurfMeshPropEdgePoint);
        v_points_updated_ = input_mesh_->get_vertex_property<Point>(kSurfMeshPropVertexPointUpdated);

        v_index_sub_mesh_f_prop_ = input_mesh_->get_face_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshF);
        v_index_sub_mesh_e_prop_ = input_mesh_->get_edge_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshE);
        v_index_sub_mesh_v_prop_ = input_mesh_->get_vertex_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshV);
    }

    void remove_mesh_properties()
    {
        input_mesh_->remove_face_property(f_points_);
        input_mesh_->remove_edge_property(e_points_);
        input_mesh_->remove_vertex_property(v_points_updated_);
        input_mesh_->remove_face_property(v_index_sub_mesh_f_prop_);
        input_mesh_->remove_edge_property(v_index_sub_mesh_e_prop_);
        input_mesh_->remove_vertex_property(v_index_sub_mesh_v_prop_);
    }
};


// ===============[ public prototypes ]===============

void test_catmull();

} // namespace SubdivisionPlugin
#endif // SUBDIVISIONPLUGIN_SD_CATMULL_H

