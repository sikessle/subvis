#ifndef TYPES_H
#define TYPES_H

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

} // namespace Algo
} // namespace SubVis

#endif // TYPES_H
