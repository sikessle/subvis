#include "plugins/subdivision/sd_loop.h"

namespace SubdivisionPlugin {

const QString SubdivLoop::id()
{
    return "de.htwg.ios.subvis.plugin.subdivision.loop";
}

const QString SubdivLoop::name()
{
    return "Loop";
}

void SubdivLoop::subdivide_specific_algorithm()
{

}

} // namespace SubVis
