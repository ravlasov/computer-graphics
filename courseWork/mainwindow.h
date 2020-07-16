#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QColor>
#include <QColorDialog>
#include <QGLWidget>
#include <QGL>

namespace Ui {
    class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_lightButton_clicked();

    void upd();

protected:

    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

private:

    int frameRate = 30;
    QRect screenGeometry;
    Ui::MainWindow *ui;
    QColor lightColor;
    QTimer* timer;
    bool pressedW;
    bool pressedS;
    bool pressedA;
    bool pressedD;
};

#endif // MAINWINDOW_H
