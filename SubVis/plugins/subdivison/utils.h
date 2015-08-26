#ifndef SUBDIVISONPLUGIN_UTILS_H
#define SUBDIVISONPLUGIN_UTILS_H

#include <string>
#include "surface_mesh/Surface_mesh.h"

namespace SubdivisonPlugin {

using std::string;
using surface_mesh::Surface_mesh;
using surface_mesh::Point;

enum ObjDemoFiles {
    kBigGuy = 0,
    kCube,
    kMonsterFrog,
    kTetra
};
const string kObjDemoFilesString[4] = {"bigguy.obj", "cube.obj", "monsterfrog.obj", "tetra.obj"};
const string kRootPathToObjFiles = "./../../SubVis/objs/";


/**
 * @brief utils_q_debug_mesh Print basic information (Vertices, Edges and Faces) of mesh with QDebug.
 * @param mesh
 * @param title
 */
void utils_debug_mesh(const Surface_mesh& mesh, const QString& title);
void utils_debug_point(const Point point, const QString& title);

} // namespace SubdivisonPlugin
#endif // SUBDIVISONPLUGIN_UTILS_H

