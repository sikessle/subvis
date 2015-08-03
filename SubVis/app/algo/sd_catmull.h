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

namespace SubVis {
namespace Algo {

// ===============[ public prototypes ]===============

void test_surface_mesh();

void test_surface_mesh_read();

void sd_catmull(surface_mesh::Surface_mesh& mesh);

} // namespace Algo
} // namespace SubVis

#endif // SD_CATMULL_H

