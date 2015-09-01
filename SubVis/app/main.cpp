#include "subvis_app.h"
#include "plugins/subdivision/subdivision_algorithms_plugin.h"

int main(int argc, char* argv[])
{
    subvis::SubVisApp subvis_app{argc, argv};

    // Register your plugins here:
    subvis_app.register_plugin(new subdivision::SubdivisionAlgorithmsPlugin);
    // end registering plugins

    return subvis_app.run();
}
