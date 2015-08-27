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

#include "plugins/subdivision/sd_base.h"

namespace SubdivisionPlugin {

// ===============[ public prototypes ]===============

class SubdivDooSabin : public SubdivBase{
public:

    using Surface_mesh = surface_mesh::Surface_mesh;
    using Point = surface_mesh::Point;

    SubdivDooSabin(Surface_mesh mesh) : SubdivBase(mesh) {
        ;
    }
};


} // namespace SubdivisionPlugin

#endif // SUBDIVISIONPLUGIN_SD_DOO_SABIN_H