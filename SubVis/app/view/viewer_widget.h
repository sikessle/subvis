#ifndef SUBVIS_VIEWER_WIDGET_H
#define SUBVIS_VIEWER_WIDGET_H

#include "QGLViewer/qglviewer.h"

#include "controller/draw_controller.h"

namespace subvis {

class ViewerWidget : public QGLViewer {
 public:
  ViewerWidget(QWidget* parent = 0);

  virtual void set_draw_controller(DrawController& draw_controller);

 protected:
  virtual void draw() override = 0;
  virtual void init() override = 0;

 protected:
  DrawController* draw_controller_{nullptr};
};

} // namespace subvis
#endif // SUBVIS_VIEWER_WIDGET_H
