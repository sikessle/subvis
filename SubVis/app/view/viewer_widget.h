#ifndef SUBVIS_VIEWER_WIDGET_H
#define SUBVIS_VIEWER_WIDGET_H

#include "QGLViewer/qglviewer.h"
#include "surface_mesh/Surface_mesh.h"
#include "controller/draw_controller.h"

namespace SubVis {

using surface_mesh::Surface_mesh;

class ViewerWidget : public QGLViewer
{
    Q_OBJECT

public:
    ViewerWidget(QWidget *parent, DrawController &draw_ctrl);

protected:
    virtual void draw_mesh(Surface_mesh &mesh) = 0;

private:
    void draw();

    DrawController &draw_controller;

public slots:
    void redraw_required();
};

} // namespace SubVis
#endif // SUBVIS_VIEWER_WIDGET_H
