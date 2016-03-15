#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QVector>
#include <QMainWindow>
#include <vector>
#include "spline.h"
#include <ParticleSettings.h>

using std::vector;
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setStatusBarText(const QString&);
    void graphClicked(QMouseEvent*);

private slots:
    void on_actionExit_triggered();

    void on_actionNew_Map_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_wl_direction_x_editingFinished();

    void on_wl_direction_y_editingFinished();

    void on_wl_direction_z_editingFinished();

    void on_pushButton_3_clicked();

    void on_btnUpdate_clicked();

    void on_btnChooseStartColor_clicked();

    void on_btnChooseEndColor_clicked();

    void on_btnChooseTexture_clicked();

	void on_btnSaveSystem_clicked();

	void on_btnOpenSystem_clicked();

private:
    vector<double> spline_points_x;
    vector<double> spline_points_y;
    QVector<double> graph_x;
    QVector<double> graph_y;
    tk::spline test_spline;
    void UpdateWorldLightDirection();
	ParticleSettings GetParticleSettingsFromUI();
    Ui::MainWindow *ui;
};

#endif
