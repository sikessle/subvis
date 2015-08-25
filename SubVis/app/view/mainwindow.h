#ifndef SUBVIS_MAINWINDOW_H
#define SUBVIS_MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include "view/viewer_mesh_widget.h"
#include "view/viewer_plugin_widget.h"
#include "controller/draw_controller.h"

namespace SubVis {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(DrawController &draw_controller);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    const QString kStatusText {"Build: " __DATE__ " " __TIME__};
    const QString kTabViewerMeshText {"Mesh"};
    const QString kTabViewerPluginText {"Plugin specific"};

    // memory managed by Qt's parent-relationship (ui)
    QLabel *status_label;
    ViewerMeshWidget *viewer_mesh_widget;
    ViewerPluginWidget *viewer_plugin_widget;

    void setup_status_bar();
    void setup_viewer_tabs(DrawController &draw_controller);
    void setup_plugin_tabs();
};


} // namespace SubVis
#endif // SUBVIS_MAINWINDOW_H
