
#include "plugins/subdivision/types.h"
#include "plugins/subdivision/sd_butterfly.h"


namespace SubdivisionPlugin {

const QString SubdivButterfly::id()
{
    return "de.htwg.ios.subvis.plugin.subdivision.butterfly";
}

const QString SubdivButterfly::name()
{
    return "Butterfly";
}

void SubdivButterfly::subdivide_specific_algorithm()
{

}

} // namespace SubVis
