#ifndef SUBVIS_SUBVIS_PLUGIN_H
#define SUBVIS_SUBVIS_PLUGIN_H

#include <string>

namespace SubVis {

using std::string;

/**
 * @brief Interface for plugins
 */
class SubVisPlugin
{
public:
    virtual ~SubVisPlugin() {}
    virtual string echo() = 0;
};

#define SUBVIS_PLUGIN_IID "de.htwg.ios.subvis.SubVisPlugin"

} // namespace SubVis

// must be outside namespace
Q_DECLARE_INTERFACE(SubVis::SubVisPlugin, SUBVIS_PLUGIN_IID)

#endif // SUBVIS_SUBVIS_PLUGIN_H

