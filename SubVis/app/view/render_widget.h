#ifndef SUBVIS_VIEW_RENDER_WIDGET_H
#define SUBVIS_VIEW_RENDER_WIDGET_H

#include "QGLViewer/qglviewer.h"

namespace SubVis {
namespace View {

class RenderWidget : public QGLViewer
{
public:
    RenderWidget(QWidget *parent);
};

} // namespace View
} // namespace SubVis
#endif // SUBVIS_VIEW_RENDER_WIDGET_H
