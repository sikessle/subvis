#ifndef SUBVIS_VIEW_MAINWINDOW_H
#define SUBVIS_VIEW_MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>

namespace SubVis {
namespace View {

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
    QLabel *status_label;
    const QString status_text {"HTWG Konstanz"};
    void setup_status_bar();
};


} // namespace View
} // namespace SubVis
#endif // SUBVIS_VIEW_MAINWINDOW_H
