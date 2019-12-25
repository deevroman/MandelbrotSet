#include "mainwindow.h"
#include <QtWidgets>
#include <QPainter>
#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QColor>
#include <QRubberBand>
#include "complex.h"
#include <QDebug>
#include <QThread>
#include <QVector>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent) {
    this->setFixedSize(1000, 1000);
    canvas = new QImage(this->width(), this->height(), QImage::Format_RGB32);
}

MainWindow::~MainWindow() {
}


void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (freeze)
        return;
    freeze = true;
    origin = event->pos();
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    rubberBand->hide();
    QPoint end = event->pos();
    if (origin.x() > end.x()) {
        std::swap(origin, end);
    }
    if (origin.y() > end.y()) {
        auto tmp = origin.y();
        origin.setY(end.y());
        end.setY(tmp);
    }
    int x = end.x() - origin.x();
    int y = end.y() - origin.y();
    if(std::max(x, y) <= 3){
        return;
    }
    start_x += 3.0 * origin.x() / this->width() * scale;
    start_y += 3.0 * origin.y() / this->height() * scale;
    if((long double)x / this->width() < (long double)y / this->height()){
        scale *= (long double)x / this->width();
    } else {
        scale *= (long double)y / this->height();
    }
    this->update();
    freeze = false;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_Escape:
            this->update();
            scale = 1.0;
            start_x = 0.0;
            start_y = 0.0;
            break;
    }
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if(freeze) {
        painter.drawImage(0, 0, *canvas);
        return;
    }
    freeze = true;
    const int blockSize = 100;
    QVector<QThread*> threads;
    long double w = canvas->width(), h = canvas->height();
    auto draw = [&](int x, int y, int xw, int yh){
        const int MAX_ITERS = 448;
        for (int i = x; i < xw; i++) {
            for (int j = y; j < yh; j++) {
                complex<long double> c(scale * i * 3 / std::min(w, h) - 1.5 + start_x,
                                       scale * j * 3 / std::min(w, h) - 1.5 + start_y);
                complex<long double> z(0.0, 0.0);
                QColor test(0, 0, 0);
                for (int it = 0; it < MAX_ITERS; it++) {
                    z = z * z + c;
                    if (z.norm() >= 4.0) {
                        int color = (255 * it) / 48;
                        if (color >= 256) {
                            color = 255;
                        }
                        test = QColor(color, 0, 0);
                        break;
                    }
                }
                canvas->setPixelColor(i, j, test);
            }
        }
    };
    for (int x = 0; x < w; x += blockSize) {
        for (int y = 0; y < h; y += blockSize) {
            QThread* thread = QThread::create(draw, x, y, x + blockSize, y + blockSize);
            thread->start();
            threads.push_back(thread);
        }
    }
    for (auto t : threads) {
        t->wait();
    }
    painter.drawImage(0, 0, *canvas);
    freeze = false;
    for(auto &thread : threads){
        delete thread;
    }
}