#ifndef SUBDIVISONPLUGIN_TYPES_H
#define SUBDIVISONPLUGIN_TYPES_H

#include <string>

namespace SubdivisonPlugin {

using std::string;

/**
 * @brief kSurfMeshPropVertexPoint key to access surface mesh property that stores the vertex coordinates
 */
const string kSurfMeshPropVertexPoint = "v:point";

/**
 * @brief kSurfMeshPropVertexPointUpdated key to access surface mesh property that stores the updated vertex coordinates (after subdivision)
 */
const string kSurfMeshPropVertexPointUpdated = "v:point_updated";

/**
 * @brief kSurfMeshPropFacePoint key to access surface mesh property that stores the face point coordinates
 */
const string kSurfMeshPropFacePoint = "f:point";

/**
 * @brief kSurfMeshPropEdgePoint key to access surface mesh property that stores the edge point coordinates
 */
const string kSurfMeshPropEdgePoint = "e:point";

/**
 * @brief kSurfMeshPropVertexIndexSubMesh index of the corresponding vertex in the new result mesh (subdivided mesh)
 * This is necessary to map vertices from origin mesh to subdivided mesh.
 */
const string kSurfMeshPropVertexIndexSubMeshV = "v:subdivision_mesh_vertex_index";
const string kSurfMeshPropVertexIndexSubMeshE = "e:subdivision_mesh_vertex_index";
const string kSurfMeshPropVertexIndexSubMeshF = "ff:subdivision_mesh_vertex_index";

} // namespace SubdivisonPlugin

#endif // SUBDIVISONPLUGIN_TYPES_H
