#ifndef SUBDIVISIONPLUGIN_SD_LOOP_H
#define SUBDIVISIONPLUGIN_SD_LOOP_H

#include "plugins/subdivision/algorithm.h"

namespace subdivision {

class SubdivLoop : public Algorithm
{
public:
    using Surface_mesh = surface_mesh::Surface_mesh;
    using Point = surface_mesh::Point;

    virtual const QString id() override;
    virtual const QString name() override;

protected:
    virtual void subdivide_specific_algorithm() override;

};

} // namespace subdivision

#endif // SUBDIVISION_SD_LOOP_H
