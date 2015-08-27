#ifndef SUBDIVISIONPLUGIN_SD_BASE_H
#define SUBDIVISIONPLUGIN_SD_BASE_H

#include "surface_mesh/Surface_mesh.h"

namespace SubdivisionPlugin {

class SubdivBase {
public:
    virtual ~SubdivBase() {}
    SubdivBase(surface_mesh::Surface_mesh mesh) : mesh_(mesh) {}

protected:
    surface_mesh::Surface_mesh mesh_;
};

} // namespace SubdivisionPlugin

#endif // SUBDIVISIONPLUGIN_SD_BASE_H
