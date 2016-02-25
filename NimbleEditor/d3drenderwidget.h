#ifndef D3DRENDERWIDGET
#define D3DRENDERWIDGET
#include <memory>
#include <QSharedPointer>
#include <QDockWidget>
#include "mainwindow.h"
#include "Graphics.h"
#include "Camera.h"
#include "QTimer"
#include "fpscalc.h"

using std::shared_ptr;

class D3DRenderWidget : public QDockWidget
{
    //Q_DISABLE_COPY(D3DRenderWidget)
    Q_OBJECT
public:
    D3DRenderWidget(QWidget* parent);
    virtual ~D3DRenderWidget();

    // Tell QT to let us handle drawing.
    virtual QPaintEngine* paintEngine() const { return NULL; }
    void widgetDestroyed(QObject* obj);
    void NewTerrain(int width, int depth);

    void SetAmbientLight(QColor color);
    void SetDiffuseColor(QColor color);
    void SetLightDirection(float x, float y, float z);
    shared_ptr<Graphics> GetGraphics();

protected:
    virtual void resizeEvent(QResizeEvent* evt);
    virtual void paintEvent(QPaintEvent* evt);
    virtual void mouseMoveEvent(QMouseEvent* evt);
    virtual void keyPressEvent(QKeyEvent * evt);
    virtual void wheelEvent(QWheelEvent* evt);
    virtual void mousePressEvent(QMouseEvent* evt);
public slots:
    void UpdateFPSTitle();

signals:
    void statusEvent(const QString& message);

private:
    shared_ptr<Graphics> graphics;
    QSharedPointer<QTimer> timer;
    QSharedPointer<QTimer> fps_update_timer;
    QSharedPointer<FPSCalc> fps_calc;

    Vector3 _prev_mouse_pos;
    float _mouse_speed;

private:
    int GetAverageFPS();

};

#endif
