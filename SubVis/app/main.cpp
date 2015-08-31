#include "subvis_app.h"
#include "plugins/subdivision/subdivision_algorithms.h"

int main(int argc, char* argv[])
{
    using subvis::SubVisApp;

    SubVisApp subvis_app{argc, argv};

    // Register your plugins here:
    subvis_app.register_plugin(new subdivision::SubdivisionAlgorithms);
    // end registering plugins

    return subvis_app.run();
}
