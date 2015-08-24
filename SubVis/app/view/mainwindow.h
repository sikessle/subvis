#ifndef SUBVIS_MAINWINDOW_H
#define SUBVIS_MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include "view/viewer_mesh_widget.h"
#include "view/viewer_plugin_widget.h"

namespace SubVis {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    const QString STATUS_TEXT {"Build: " __DATE__ " " __TIME__};
    const QString TAB_VIEWER_MESH_TEXT {"Mesh"};
    const QString TAB_VIEWER_PLUGIN_TEXT {"Plugin specific"};

    QLabel *status_label;
    ViewerMeshWidget *viewer_mesh_widget;
    ViewerPluginWidget *viewer_plugin_widget;

    void setup_status_bar();
    void setup_viewer_tabs();
    void setup_plugin_tabs();
};


} // namespace SubVis
#endif // SUBVIS_MAINWINDOW_H
