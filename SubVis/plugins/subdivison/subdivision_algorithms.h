#ifndef SUBDIVISON_ALGORITHMS_H
#define SUBDIVISON_ALGORITHMS_H

#include <QObject>
#include <string>
#include "subvis_plugin.h"

namespace SubdivisonPlugin {

using namespace SubVis;
using std::string;

class SubdivisonAlgorithms : public QObject, SubVisPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SUBVIS_PLUGIN_IID FILE "subdivision.json")
    // fully scoped name required in macro
    Q_INTERFACES(SubVis::SubVisPlugin)

public:
     string echo() override;
};

} // namespace SubdivisonPlugin
#endif // SUBDIVISON_ALGORITHMS_H
