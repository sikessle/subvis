#include <memory>
#include <QtGlobal>

#include "subvis_app.h"
#include "plugins/subvis_plugin.h"
#include "plugins/subdivision/subdivision_algorithms_plugin.h"

int main(int argc, char* argv[]) {
  using PluginPtr = std::unique_ptr<subvis::SubVisPlugin>;
  using SubdivPlugin = subdivision::SubdivisionAlgorithmsPlugin;

  qSetMessagePattern("[%{type}] (%{file}:%{line}) - %{message}");

  subvis::SubVisApp subvis_app{argc, argv};

  // Register your plugins here:
  subvis_app.register_plugin(std::move(PluginPtr {new SubdivPlugin}));
  // end registering plugins

  return subvis_app.run();
}
