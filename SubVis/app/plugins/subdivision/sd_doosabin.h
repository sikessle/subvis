/**
 *
 * @brief Implementation of the Doo Sabin subdivision algorithm.
 *
 * 1. create face point for each face
 * 2. create edge point for each edge
 * 3. compute new vertex: average of the four points (face point, two edge points, vertex)
 * (http://yoshihitoyagi.com/projects/mesh/subdiv/doo/index.html)
 *
 *
 */

#ifndef SUBDIVISIONPLUGIN_SD_DOO_SABIN_H
#define SUBDIVISIONPLUGIN_SD_DOO_SABIN_H

// ===============[ includes ]===============
#include "surface_mesh/Surface_mesh.h"
#include "plugins/subdivision/types.h"

#include "plugins/subdivision/algorithm.h"

namespace SubdivisionPlugin {

// ===============[ public prototypes ]===============

class SubdivDooSabin : public Algorithm
{
public:
    using Surface_mesh = surface_mesh::Surface_mesh;
    using Point = surface_mesh::Point;

    virtual const QString id() override;
    virtual const QString name() override;

protected:
    virtual void subdivide_specific_algorithm() override;

private:
    Surface_mesh::Face_property<Point> f_points_;
    Surface_mesh::Edge_property<Point> e_points_;

    // vertex index properties to map from origin mesh to subdivision mesh
    Surface_mesh::Face_property<Surface_mesh::Vertex> v_index_sub_mesh_f_prop_;
    Surface_mesh::Edge_property<Surface_mesh::Vertex> v_index_sub_mesh_e_prop_;

    void compute_all_face_points();
    void compute_all_edge_points();

    void add_mesh_properties()
    {
        // add properties that are necessary for doo sabin
        input_mesh_->add_face_property<Point>(kSurfMeshPropFacePoint);
        input_mesh_->add_edge_property<Point>(kSurfMeshPropEdgePoint);

        // index properties
        input_mesh_->add_face_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshF);
        input_mesh_->add_edge_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshE);

    }

    void init_mesh_members()
    {
        Algorithm::init_mesh_members();
        f_points_ = input_mesh_->get_face_property<Point>(kSurfMeshPropFacePoint);
        e_points_ = input_mesh_->get_edge_property<Point>(kSurfMeshPropEdgePoint);

        v_index_sub_mesh_f_prop_ = input_mesh_->get_face_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshF);
        v_index_sub_mesh_e_prop_ = input_mesh_->get_edge_property<Surface_mesh::Vertex>(kSurfMeshPropVertexIndexSubMeshE);
    }
};


} // namespace SubdivisionPlugin

#endif // SUBDIVISIONPLUGIN_SD_DOO_SABIN_H
