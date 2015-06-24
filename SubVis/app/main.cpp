#include "view/mainwindow.h"
#include "surface_mesh/Surface_mesh.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SubVis::View::MainWindow w;
    w.show();

    surface_mesh::Surface_mesh mesh;

    mesh.add_vertex(surface_mesh::Point(0,0,0));

    return a.exec();
}
