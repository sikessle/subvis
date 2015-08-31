#ifndef SUBVIS_VIEWER_PLUGIN_WIDGET_H
#define SUBVIS_VIEWER_PLUGIN_WIDGET_H

#include "plugins/subvis_plugin.h"
#include "view/viewer_widget.h"

namespace subvis {

class ViewerPluginWidget : public ViewerWidget
{
    Q_OBJECT

public:
    ViewerPluginWidget(QWidget* parent = 0);

protected:
    void draw() override;

private:
    SubVisPlugin* drawing_plugin_{nullptr};

public slots:
    void set_drawing_plugin(SubVisPlugin* plugin);
};

} // namespace subvis
#endif // SUBVIS_VIEWER_PLUGIN_WIDGET_H
