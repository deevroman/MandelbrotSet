#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QThread>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    bool freeze = false;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage* canvas;
    QImage* startCanvas;
    QLabel* canvasLabel;
    QRubberBand* rubberBand = nullptr;
    QPoint origin;
    long double scale = 1.0;
    long double start_x = 0.0, start_y = 0.0;

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *e);

    void paintEvent(QPaintEvent *event);
};
#endif // MAINWINDOW_H
