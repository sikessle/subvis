/**
 * @class SdTriangle "plugins/subdivision/sd_triangle.h"
 *
 * @brief Base class for all triangle subdivision algorithms.
 *
 * This class bunches methods that are used by subdivision algorithms that operate
 * on triangle meshes like Loop and Butterfly.
 * It provides methods to initializes all necessary properties.
 *
 * @author Felix Born
 *
 */


#ifndef SUBVIS_PLUGINS_SUBDIVISION_SD_TRIANGLE_H
#define SUBVIS_PLUGINS_SUBDIVISION_SD_TRIANGLE_H

#include "plugins/subdivision/sd_algorithm.h"

namespace subdivision {


class SdTriangle : public SdAlgorithm {
 protected:
  void subdivide_input_mesh_write_output_mesh() override = 0;
  void init_mesh_members() override;
  void deinit_mesh_members() override;


  /// Property key to store the index of the corresponding output mesh vertex points.
  const ::std::string kPropVertexIndexOutputV =
    "v:result_mesh_vertex_index";

  /// Property key to store the index of the corresponding output mesh edge points.
  const ::std::string kPropVertexIndexOutputE =
    "e:result_mesh_vertex_index";


  /// Property with the key @c kPropVertexIndexOutputV.
  /// Use @c init_mesh_members() and @c deinit_mesh_members() to initialize and release.
  Surface_mesh::Vertex_property<Surface_mesh::Vertex> v_index_output_v_prop_;

  /// Property with the key @c kPropVertexIndexOutputE.
  /// Use @c init_mesh_members() and @c deinit_mesh_members() to initialize and release.
  Surface_mesh::Edge_property<Surface_mesh::Vertex> v_index_output_e_prop_;


  /// Loop over all faces, split them and add the splitted faces to the output mesh.
  /// @sa add_splitted_face_to_output_mesh(const Surface_mesh::Face& face)
  void add_all_faces_output_mesh();

  /// Split the @c face (triangle face) and add the splitted faces to the output mesh.
  void add_splitted_face_to_output_mesh(const Surface_mesh::Face& face);

  /// Get the outer vertex of the triangle face.
  /// If triagnel face with vertieces A, B and C and the @c halfedge connects A to B
  /// the vertex C is returned.
  surface_mesh::Surface_mesh::Vertex get_outer_vertex_triangle(
    const Surface_mesh::Halfedge& halfedge) const;
};


} // namespace subdivision

#endif // SUBVIS_PLUGINS_SUBDIVISION_SD_TRIANGLE_H


