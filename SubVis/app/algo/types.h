#ifndef SUBVIS_ALGO_TYPES_H
#define SUBVIS_ALGO_TYPES_H

#include <string>

namespace SubVis {
namespace Algo {

/**
 * @brief kSurfMeshPropVertexPoint key to access surface mesh property that stores the vertex coordinates
 */
const std::string kSurfMeshPropVertexPoint = "v:point";

/**
 * @brief kSurfMeshPropVertexPointUpdated key to access surface mesh property that stores the updated vertex coordinates (after subdivision)
 */
const std::string kSurfMeshPropVertexPointUpdated = "v:point_updated";

/**
 * @brief kSurfMeshPropFacePoint key to access surface mesh property that stores the face point coordinates
 */
const std::string kSurfMeshPropFacePoint = "f:point";

/**
 * @brief kSurfMeshPropEdgePoint key to access surface mesh property that stores the edge point coordinates
 */
const std::string kSurfMeshPropEdgePoint = "e:point";

/**
 * @brief kSurfMeshPropVertexIndexSubMesh index of the corresponding vertex in the new result mesh (subdivided mesh)
 * This is necessary to map vertices from origin mesh to subdivided mesh.
 */
const std::string kSurfMeshPropVertexIndexSubMeshV = "v:subdivision_mesh_vertex_index";
const std::string kSurfMeshPropVertexIndexSubMeshE = "e:subdivision_mesh_vertex_index";
const std::string kSurfMeshPropVertexIndexSubMeshF = "ff:subdivision_mesh_vertex_index";

} // namespace Algo
} // namespace SubVis

#endif // SUBVIS_ALGO_TYPES_H
