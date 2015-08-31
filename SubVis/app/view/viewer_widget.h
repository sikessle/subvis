#ifndef SUBVIS_VIEWER_WIDGET_H
#define SUBVIS_VIEWER_WIDGET_H

#include "QGLViewer/qglviewer.h"
#include "surface_mesh/Surface_mesh.h"

#include "controller/draw_controller.h"

namespace subvis {

using surface_mesh::Surface_mesh;

class ViewerWidget : public QGLViewer
{
    Q_OBJECT

public:
    ViewerWidget(QWidget* parent = 0);

    void set_draw_controller(DrawController* draw_controller);

protected:
    virtual void draw() = 0;

protected:
    DrawController *draw_controller_{nullptr};

public slots:
    void enforce_redraw();
};

} // namespace SubVis
#endif // SUBVIS_VIEWER_WIDGET_H
