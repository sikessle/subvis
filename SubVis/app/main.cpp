
// ===============[ includes ]===============

#include <QApplication>

#include "QGLViewer/qglviewer.h"

#include "view/mainwindow.h"
#include "algo/sd_catmull.h"

// ===============[ main routine ]===============

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SubVis::View::MainWindow w;
    w.show();

    //SubVis::Algo::test_surface_mesh();
    //SubVis::Algo::test_surface_mesh_read();

    return a.exec();
}
