#include "subvis_app.h"

int main(int argc, char *argv[])
{
    using SubVis::SubVisApp;

    SubVisApp subvis_app{argc, argv};

    return subvis_app.run();
}
