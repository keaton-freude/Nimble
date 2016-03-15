#include <d3drenderwidget.h>
#include <iostream>
#include <QKeyEvent>
#include <QCoreApplication>
#include <Windows.h>
#include <QTimer>
#include <cstdio>
#include <ctime>
#include <QGuiApplication>
#include <fpscalc.h>
#include <QStatusBar>
#include <SimpleMath.h>
#include "Logger.h"
#include "ShaderManager.h"

using std::cout;
using std::endl;

using DirectX::SimpleMath::Ray;


void PrintMatrix(Matrix m);

D3DRenderWidget::D3DRenderWidget(QWidget* parent = nullptr)
    : QDockWidget(parent), _prev_mouse_pos(Vector3::Zero)
{
    // DRAW TIMER
    timer = QSharedPointer<QTimer>::create(this);
    connect(timer.data(), SIGNAL(timeout()), this, SLOT(update()));
    timer->start(17);

    this->setMouseTracking(true);

    // FPS_UPDATE_TIMER
    fps_update_timer = QSharedPointer<QTimer>::create(this);

    connect(fps_update_timer.data(), SIGNAL(timeout()), this, SLOT(D3DRenderWidget::UpdateFPSTitle()));
    fps_update_timer->start(250);


    fps_calc = QSharedPointer<FPSCalc>::create(0.25f);

    // Get a pointer to the Graphics object
    graphics = make_shared<Graphics>();

    if (graphics->Init(this->geometry().width(), this->geometry().height(),
                       (HWND)this->winId(), false, false, 1000.0f, 0.5f))
    {
        cout << "Graphics initialized correctly." << endl;
    }
    else
    {
        cout << "Graphics initialization failed." << endl;
    }

    setAttribute(Qt::WA_PaintOnScreen, true);
    setAttribute(Qt::WA_NativeWindow, true);
    setAttribute(Qt::WA_DeleteOnClose);

    _mouse_speed = 0.2f;
}


D3DRenderWidget::~D3DRenderWidget()
{
    LOG_INFO("D3DRenderWidget destruct!");

}

void D3DRenderWidget::SetAmbientLight(QColor color)
{
    graphics->SetAmbientLight(color.redF(), color.greenF(), color.blueF());


}

void D3DRenderWidget::SetDiffuseColor(QColor color)
{
    graphics->SetDiffuseColor(color.redF(), color.greenF(), color.blueF());
}

void D3DRenderWidget::SetLightDirection(float x, float y, float z)
{
    graphics->SetLightDirection(x, y, z);
}

shared_ptr<Graphics> D3DRenderWidget::GetGraphics()
{
    return this->graphics;
}

void D3DRenderWidget::paintEvent(QPaintEvent* evt)
{
    this->UpdateFPSTitle();

    float dt = fps_calc->Tick();

    graphics->Draw(RENDER_MODE::Viewport, dt);
    fps_calc->Tock();

    // each frame, lets ask the Graphics layer for a diagnostics message.
    // will be expanded to require much more than a Status Bar,
    // but for now we will cram the info into there
    QString graphics_message = QString(graphics->GetStatistics().c_str());

    emit(statusEvent(graphics_message));
}

void D3DRenderWidget::resizeEvent(QResizeEvent * evt)
{
    if (this->objectName() == "minimap")
        return;

    graphics->GetD3D()->ReleaseRenderTarget();
    graphics->GetSwapChain()->ResizeBuffers(1, width(), height(),
                                            graphics->GetSwapChainDesc().BufferDesc.Format, 0);
    graphics->GetD3D()->CreateRenderTarget(width(), height());

    graphics->GetD3D()->UpdateProjectionMatrix(width(), height(), 0.1f, 1000.0f);
}

void D3DRenderWidget::mouseMoveEvent(QMouseEvent *evt)
{
    float pointX, pointY;

    pointX = ((2.0f * (float)evt->x()) / (float)this->width()) - 1.0f;

    pointY = (((2.0f * (float)evt->y()) / (float)this->height()) -1.0f) * -1.0f;

    if (evt->buttons() == Qt::MouseButton::LeftButton)
    {

    }
    else if (evt->buttons() == Qt::MouseButton::RightButton)
    {
        Vector3 curr_pos = Vector3(-evt->x(), 0.0f, evt->y());

        if (_prev_mouse_pos == Vector3::Zero)
            _prev_mouse_pos = curr_pos;

        Vector3 translation = curr_pos - _prev_mouse_pos;
        translation.Normalize();

        // transform by camera rotation
        Vector3 rot = graphics->getCamera()->GetRotation();

        Matrix rotX = Matrix::CreateRotationX(rot.x);
        Matrix rotY = Matrix::CreateRotationY(rot.y);

        Matrix fullRot = rotX * rotY;

        Vector3 transformed_translation = Vector3::Transform(translation, fullRot);

        //transformed_translation.z = transformed_translation.y;
        transformed_translation.y = 0;

        graphics->getCamera()->Translate(transformed_translation * _mouse_speed);

        _prev_mouse_pos = curr_pos;
    }
    else if (evt->buttons() != Qt::MouseButton::RightButton)
    {
        _prev_mouse_pos = Vector3::Zero;
    }
}

void D3DRenderWidget::keyPressEvent(QKeyEvent * evt)
{
    switch (evt->key())
    {
    case Qt::Key_Down:
        this->graphics->getCamera()->Translate(Vector3(0, -1.0f, 0.0f));
        break;
    case Qt::Key_Up:
        this->graphics->getCamera()->Translate(Vector3(0, 1.0f, 0.0f));
        break;
    case Qt::Key_F1:

        break;
    case Qt::Key_F2:

        break;
    case Qt::Key_F3:
        graphics->PrintDebugObjects();
    default:
        //dontcare
        break;
    }
}

void D3DRenderWidget::wheelEvent(QWheelEvent *evt)
{
    cout << evt->delta() << endl;

    if (evt->delta() > 0)
    {
        //roll forward

        this->graphics->getCamera()->Translate(Vector3(0, -1.0f, 0.0f));
    }
    else if (evt->delta() < 0)
    {
        //roll back
        this->graphics->getCamera()->Translate(Vector3(0, 1.0f, 0.0f));
    }
}

void D3DRenderWidget::mousePressEvent(QMouseEvent *evt)
{
    float pointX, pointY;

    pointX = ((2.0f * (float)evt->x()) / (float)this->width()) - 1.0f;

    pointY = (((2.0f * (float)evt->y()) / (float)this->height()) -1.0f) * -1.0f;

    if (evt->buttons() == Qt::MouseButton::LeftButton)
    {

        Matrix projectionMatrix = *graphics->GetD3D()->GetProjectionMatrix();

        Ray ray = graphics->getCamera()->GetMouseRay(Vector2(pointX, pointY), projectionMatrix);

        auto hit = graphics->IsRayIntersectingTerrain(ray);

        if (hit.hit)
        {
            graphics->SetDebugLine(ray.position, hit.hit_location);
            graphics->HeightmapAdd(hit.hit_location, 10.0f, 10.0f);
        }

        //emit statusEvent(message);
    }
}


void D3DRenderWidget::UpdateFPSTitle()
{
    QString window_title = "NimbleEditor <MapName> | FPS: " + QString::number(this->fps_calc->GetFPS());
    //cout << window_title.toStdString() << endl;
    this->window()->setWindowTitle(window_title);
}

void PrintMatrix(Matrix m)
{
    cout << "[" << m._11 << "," << m._12 << "," << m._13 << "," << m._14 << "]" << endl;
    cout << "[" << m._21 << "," << m._22 << "," << m._23 << "," << m._24 << "]" << endl;
    cout << "[" << m._31 << "," << m._32 << "," << m._33 << "," << m._34 << "]" << endl;
    cout << "[" << m._41 << "," << m._42 << "," << m._43 << "," << m._44 << "]" << endl;
}
