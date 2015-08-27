#ifndef SUBVIS_SD_BASE_H
#define SUBVIS_SD_BASE_H

#include "surface_mesh/Surface_mesh.h"

namespace SubVis {
namespace algo {

class SubdivBase {
public:
    virtual ~SubdivBase() {}
    SubdivBase(surface_mesh::Surface_mesh mesh) : mesh_(mesh) {}

protected:
    surface_mesh::Surface_mesh mesh_;
};

} // namespace algo
} // namespace SubVis

#endif // SUBVIS_SD_BASE_H
