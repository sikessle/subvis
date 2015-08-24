#ifndef SUBVIS_VIEWER_WIDGET_H
#define SUBVIS_VIEWER_WIDGET_H

#include "QGLViewer/qglviewer.h"

namespace SubVis {

class ViewerWidget : public QGLViewer
{
public:
    ViewerWidget(QWidget *parent);

protected:
    // must be implemented by subclass
    virtual void draw() = 0;
};

} // namespace SubVis
#endif // SUBVIS_VIEWER_WIDGET_H
