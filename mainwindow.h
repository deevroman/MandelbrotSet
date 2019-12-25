#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QThread>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    bool freeze = false;
    struct snapshot{
        long double start_x, start_y, scale;
    };
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QImage* canvas;
    QRubberBand* rubberBand = nullptr;
    QPoint origin;
    QStack<snapshot> history;
    QShortcut* keyCtrlZ;
    QShortcut* keyEsc;
    long double scale = 1.0;
    long double start_x = 0.0, start_y = 0.0;

    void mouseMoveEvent(QMouseEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseReleaseEvent(QMouseEvent *event);

    void paintEvent(QPaintEvent *event);

    void slotShortcutCtrlZ();

    void slotShortcutEcs();
};
#endif // MAINWINDOW_H
