
#ifndef SUBVIS_PLUGINS_SUBDIVISION_UTILS_H
#define SUBVIS_PLUGINS_SUBDIVISION_UTILS_H

#include <string>
#include <QString>
#include "surface_mesh/Surface_mesh.h"

namespace subdivision {

// uncomment to disable debug output
#define DEBUG_SUBDIV

/// Print basic information (Vertices, Edges and Faces) of the @c mesh with QDebug.
void debug_mesh(const surface_mesh::Surface_mesh& mesh,
                const QString& title);

/// Print the coordinates of the 3-D @c point with QDebug.
void debug_point(const surface_mesh::Point point, const QString& title);

#ifdef DEBUG_SUBDIV

#define DEBUG_MESH(mesh, name)      debug_mesh(mesh, name);
#define DEBUG_POINT(point, name)    debug_point(point, name);

#else

#define DEBUG_MESH(mesh, name)
#define DEBUG_POINT(point, name)

#endif

} // namespace subdivision
#endif // SUBVIS_PLUGINS_SUBDIVISION_UTILS_H

