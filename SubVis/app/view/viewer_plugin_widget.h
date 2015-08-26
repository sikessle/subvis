#ifndef SUBVIS_VIEWER_PLUGIN_WIDGET_H
#define SUBVIS_VIEWER_PLUGIN_WIDGET_H

#include "view/viewer_widget.h"
#include "subvis_plugin.h"

namespace SubVis {

class ViewerPluginWidget : public ViewerWidget
{
    Q_OBJECT

public:
    ViewerPluginWidget(QWidget *parent = 0);

protected:
    void draw() override;

private:
    SubVisPlugin *drawing_plugin{nullptr};

public slots:
    void set_drawing_plugin(SubVisPlugin *plugin);
};

} // namespace SubVis
#endif // SUBVIS_VIEWER_PLUGIN_WIDGET_H
