#include "plugins/subdivision/sd_doosabin.h"

namespace SubdivisionPlugin {

const QString SubdivDooSabin::id()
{
    return "de.htwg.ios.subvis.plugin.subdivision.doosabin";
}

const QString SubdivDooSabin::name()
{
    return "Doo-Sabin";
}

void SubdivDooSabin::subdivide_specific_algorithm()
{
    // read from input_mesh_
    // write to result_mesh_
}

} // namespace SubVis
