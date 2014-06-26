#ifndef FRAKTALWIDGET_H
#define FRAKTALWIDGET_H

#include <QPixmap>
#include <QWidget>

#include "renderthread.h"

class FraktalWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FraktalWidget(QWidget *parent = 0);
    ~FraktalWidget();

    inline QPixmap* getPixmap()
    {
        return &pixmap;
    }

    inline void setAlgebraParserPath(QString path)
    {
        thread.setAlgebraParserPath(path);
    }

    void render();

signals:
    void centerXChanged(double x);
    void centerYChanged(double y);
    void scaleChanged(double s);

    void renderBegin();
    void renderFinished();

public slots:
    void setCenterX(double x);
    void setCenterY(double y);
    void setScale(double s);
    void setZoomFaktor(double f);
    void setFraktal(Fraktal* fraktal);
    void setAntialiasing(bool b);
    void setMaxIterations(int i);
    void setLimit(int i);
    void stopRendering();

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private slots:
    void updatePixmap(const QImage &image, double scaleFactor);

private:
    void zoom(double zoomFactor);
    void scroll(int deltaX, int deltaY);

    RenderThread thread;
    QPixmap pixmap;
    QPoint pixmapOffset;
    QPoint lastDragPos;
    double centerX;
    double centerY;
    double pixmapScale;
    double curScale;
    double zoomFaktor;
    Fraktal* currentFraktal;

    bool antialiasing;
};
#endif // FRAKTALWIDGET_H
