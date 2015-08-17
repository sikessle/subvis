#ifndef SUBVIS_VIEW_VIEWER_WIDGET_H
#define SUBVIS_VIEW_VIEWER_WIDGET_H

#include "QGLViewer/qglviewer.h"

namespace SubVis {
namespace View {

class ViewerWidget : public QGLViewer
{
public:
    ViewerWidget(QWidget *parent);
};

} // namespace View
} // namespace SubVis
#endif // SUBVIS_VIEW_VIEWER_WIDGET_H
