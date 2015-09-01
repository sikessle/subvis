#ifndef SUBVIS_VIEWER_WIDGET_H
#define SUBVIS_VIEWER_WIDGET_H

#include "QGLViewer/qglviewer.h"

#include "controller/draw_controller.h"

namespace subvis {

class ViewerWidget : public QGLViewer {
  Q_OBJECT

 public:
  ViewerWidget(QWidget* parent = 0);

  void set_draw_controller(DrawController* draw_controller);

 protected:
  virtual void draw() = 0;

 protected:
  DrawController* draw_controller_{nullptr};

 public slots:
  void enforce_redraw();
};

} // namespace subvis
#endif // SUBVIS_VIEWER_WIDGET_H
