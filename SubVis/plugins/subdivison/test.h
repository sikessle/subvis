#ifndef TEST_H
#define TEST_H

#include "algorithm.h"

namespace SubdivisionPlugin {

using surface_mesh::Surface_mesh;

class test : public Algorithm
{

protected:
    void subdivide_specific_algorithm(Surface_mesh& mesh) override;
};

}

#endif // TEST_H
