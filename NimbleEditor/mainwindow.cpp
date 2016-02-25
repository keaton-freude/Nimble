#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <vector>
#include <QColorDialog>
#include "spline.h"
#include <QVector>
#include <regex>
#include <QColorDialog>
#include <QFileDialog>
using std::vector;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->viewport_2, SIGNAL(statusEvent(QString)),
            this, SLOT(setStatusBarText(QString)));

    ui->cmbBlendMode->addItem("Alpha");
    ui->cmbBlendMode->addItem("Additive");
    ui->cmbBlendMode->setCurrentIndex(0);

    //connect(ui->graph, SIGNAL(mousePress(QMouseEvent*)),
//            this, SLOT(graphClicked(QMouseEvent*)));

    //ui->graph->addGraph();
    //ui->graph->xAxis->setLabel("x");
    //ui->graph->yAxis->setLabel("y");

    //ui->graph->xAxis->setRange(0, 1);
    //ui->graph->yAxis->setRange(0, 1);

    //ui->graph->replot();

    spline_points_x.push_back(0.0);
    spline_points_y.push_back(0.0);

    this->tabifyDockWidget(ui->toolBox, ui->ParticleEditorDock);
    //ui->statusBar->showMessage
    //int first_active = ui->viewport_2->GetGraphics()->GetParticleSystem()->first_active_particle;
    //int first_free = ui->viewport_2->GetGraphics()->GetParticleSystem()->first_free_particle;
    //int first_new = ui->viewport_2->GetGraphics()->GetParticleSystem()->first_new_particle;
    //int first_retired = ui->viewport_2->GetGraphics()->GetParticleSystem()->first_retired_particle;

    //QString message = "Active: " + QString::number(first_active) + " Free: " + QString::number(first_free) + " New: " + QString::number(first_new) + " Retired: " + QString::number(first_retired);
    //ui->statusBar->showMessage(message);
}

void MainWindow::graphClicked(QMouseEvent* evt)
{/*
    static int num_added = 1;
    //int x = evt->x();
    //int y = evt->y();
    int width = 700;
    int height = 969;
    //int graphx = x - 56;
    //int graphy = y - 32;
    //graphy = 1021 - graphy -32 - 38;

    //if (graphx < 0.0)
     //   graphx = 0.0;
    //if (graphy < 0.0)
    //    graphy = 0.0;


    //double normal_x = (double)graphx / (double)width;
    double normal_y = (double)graphy / (double)height;

    cout << "(" << normal_x << ", " << normal_y << ")" << endl;

    spline_points_x.push_back(normal_x);
    spline_points_y.push_back(normal_y);

    num_added++;

    if (num_added > 2)
    {
        test_spline.set_points(spline_points_x, spline_points_y);

        graph_x.clear();
        graph_y.clear();

        for(double i = 0.0; i < 1.0; i += .01)
        {
            graph_x.push_back(i);
            graph_y.push_back(test_spline(i));
        }

  //      this->ui->graph->graph(0)->setData(graph_x, graph_y);
   //     this->ui->graph->replot();
    }
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStatusBarText(const QString & message)
{
    ui->statusBar->showMessage(message);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionNew_Map_triggered()
{
    setStatusBarText("Test!");
}

void MainWindow::on_pushButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white);
    ui->viewport_2->SetAmbientLight(color);
}

void MainWindow::on_pushButton_2_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white);
    ui->viewport_2->SetDiffuseColor(color);
}

void MainWindow::on_wl_direction_x_editingFinished()
{
    this->UpdateWorldLightDirection();
}

void MainWindow::UpdateWorldLightDirection()
{
    float x = ui->wl_direction_x->text().toFloat();
    float y = ui->wl_direction_y->text().toFloat();
    float z = ui->wl_direction_z->text().toFloat();

    ui->viewport_2->SetLightDirection(x,y,z);
}

void MainWindow::on_wl_direction_y_editingFinished()
{
    this->UpdateWorldLightDirection();
}

void MainWindow::on_wl_direction_z_editingFinished()
{
    this->UpdateWorldLightDirection();
}

void MainWindow::on_pushButton_3_clicked()
{
    cout << "\n\nX: " << endl;
    // save current set of spline points
    // print it out
    for (auto iter = spline_points_x.begin(); iter != spline_points_x.end(); ++iter)
    {
        cout << *iter << ", ";
    }

    cout << "\n\nY: " << endl;

    for (auto iter = spline_points_y.begin(); iter != spline_points_y.end(); ++iter)
    {
        cout << *iter << ", ";
    }

}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

string Prepare_String(string text)
{
    // parse out using the following format
    //1. <value_x, value_y, value_z>
    //value_x, value_y, value_z
    //value_x,value_y,value_z

    text.pop_back();
    text.erase(text.begin());
    text.erase(remove_if(text.begin(), text.end(), isspace), text.end());

    return text;
}

bool GetMinMaxVector3(string text, Vector3& min, Vector3& max)
{
    if (text.front() != '<' || text.back() != '>')
    {
        return false;
    }
    text = Prepare_String(text);
    //format
    //<(min,max),(min,max),(min,max)>
    std::smatch m;
    std::regex e("\\([-+]?\\d*[.]?\\d+,[-+]?\\d*[.]?\\d+\\)", std::regex_constants::ECMAScript);
    vector<string> components;
    while (std::regex_search(text, m, e))
    {
        for (auto x : m)
        {
            std::cout << x << " ";
            components.push_back(x);
        }
        std::cout << std::endl;
        text = m.suffix().str();
    }

    if (components.size() != 3)
    {
        cout << "Incorrect number of components in dynamic range." << endl;
        return false;
    }

    components[0].pop_back();
    components[0].erase(components[0].begin());

    components[1].pop_back();
    components[1].erase(components[1].begin());

    components[2].pop_back();
    components[2].erase(components[2].begin());

    auto tokens_x = split(components[0], ',');
    auto tokens_y = split(components[1], ',');
    auto tokens_z = split(components[2], ',');

    float min_x = 0.0f;
    float min_y = 0.0f;
    float min_z = 0.0f;

    float max_x = 0.0f;
    float max_y = 0.0f;
    float max_z = 0.0f;

    try
    {
        min_x = std::stof(tokens_x[0]);
        max_x = std::stof(tokens_x[1]);

        min_y = std::stof(tokens_y[0]);
        max_y = std::stof(tokens_y[1]);

        min_z = std::stof(tokens_z[0]);
        max_z = std::stof(tokens_z[1]);
    }
    catch (invalid_argument ex)
    {
        cout << "One of the components of the text->vector is not a float." << endl;
        return false;
    }
    catch (out_of_range ex)
    {
        cout << "One of the comonents of the text->vector is out of range." << endl;
        return false;
    }

    min = Vector3(min_x, min_y, min_z);
    max = Vector3(max_x, max_y, max_z);

    return true;
}

bool GetMinMaxVector2(string text, Vector2& min, Vector2& max)
{
    if (text.front() != '<' || text.back() != '>')
    {
        return false;
    }
    text = Prepare_String(text);
    //format
    //<(min,max),(min,max),(min,max)>
    std::smatch m;
    std::regex e("\\([-+]?\\d*[.]?\\d+,[-+]?\\d*[.]?\\d+\\)", std::regex_constants::ECMAScript);
    vector<string> components;
    while (std::regex_search(text, m, e))
    {
        for (auto x : m)
        {
            std::cout << x << " ";
            components.push_back(x);
        }
        std::cout << std::endl;
        text = m.suffix().str();
    }

    if (components.size() != 2)
    {
        cout << "Incorrect number of components in dynamic range." << endl;
        return false;
    }

    auto tokens_x = split(components[0], ',');
    auto tokens_y = split(components[1], ',');

    float min_x = 0.0f;
    float min_y = 0.0f;

    float max_x = 0.0f;
    float max_y = 0.0f;

    try
    {
        min_x = std::stof(tokens_x[0]);
        max_x = std::stof(tokens_x[1]);

        min_y = std::stof(tokens_y[0]);
        max_y = std::stof(tokens_y[1]);
    }
    catch (invalid_argument ex)
    {
        cout << "One of the components of the text->vector is not a float." << endl;
        return false;
    }
    catch (out_of_range ex)
    {
        cout << "One of the comonents of the text->vector is out of range." << endl;
        return false;
    }

    min = Vector2(min_x, min_y);
    max = Vector2(max_x, max_y);

    return true;
}

bool GetVector2(string text, Vector2& vector)
{
    if (text.front() != '<' || text.back() != '>')
    {
        return false;
    }

    text = Prepare_String(text);

    auto tokens = split(text, ',');

    if (tokens.size() != 2)
    {
        return false;
    }

    float x = 0.0f;
    float y = 0.0f;

    try
    {
        x = std::stof(tokens[0]);
        y = std::stof(tokens[1]);
    }
    catch (invalid_argument ex)
    {
        cout << "One of the components of the text->vector is not a float." << endl;
        return false;
    }
    catch (out_of_range ex)
    {
        cout << "One of the comonents of the text->vector is out of range." << endl;
        return false;
    }

    vector = Vector2(x, y);
    return true;
}

bool GetVector3(string text, Vector3& vector)
{
    if (text.front() != '<' || text.back() != '>')
    {
        return false;
    }

    text = Prepare_String(text);

    auto tokens = split(text, ',');

    if (tokens.size() != 3)
    {
        return false;
    }

    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;


    try
    {
        x = std::stof(tokens[0]);
        y = std::stof(tokens[1]);
        z = std::stof(tokens[2]);
    }
    catch (invalid_argument ex)
    {
        cout << "One of the components of the text->vector is not a float." << endl;
        return false;
    }
    catch (out_of_range ex)
    {
        cout << "One of the comonents of the text->vector is out of range." << endl;
        return false;
    }

    vector = Vector3(x, y, z);
    return true;
}




void MainWindow::on_btnUpdate_clicked()
{
/*
    ParticleSystemConfig config;
    config.name = ui->txtParticleSystemName->text().toStdString();
    config.texture_name = "..\\..\\Assets\\Textures\\" + ui->txtTextureName->text().toStdString();
    config.particles_per_second = std::stoi(ui->txtParticlesPerSecond->text().toStdString());
    config.blend_mode = ui->cmbBlendMode->currentText().toStdString();

    if (GetMinMaxVector3(ui->txtPositionOffset->text().toStdString(), config.position_offset_min, config.position_offset_max))
    {
    }
    else
    {
        // try for a regular Vector3
        if (GetVector3(ui->txtPositionOffset->text().toStdString(), config.position_offset_min))
        {
            // do a copy into our max
            config.position_offset_max = config.position_offset_min;
        }
        else
        {
            cout << "Input invalid!" << endl;
            return;
        }
    }

    if (GetMinMaxVector3(ui->txtVelocity->text().toStdString(), config.velocity_min, config.velocity_max))
    {
    }
    else
    {
        // try for a regular Vector3
        if (GetVector3(ui->txtVelocity->text().toStdString(), config.velocity_min))
        {
            // do a copy into our max
            config.velocity_max = config.velocity_min;
        }
        else
        {
            cout << "Input invalid!" << endl;
            return;
        }
    }

    if (GetMinMaxVector3(ui->txtAcceleration->text().toStdString(), config.acceleration_min, config.acceleration_max))
    {
    }
    else
    {
        // try for a regular Vector3
        if (GetVector3(ui->txtAcceleration->text().toStdString(), config.acceleration_min))
        {
            // do a copy into our max
            config.acceleration_max = config.acceleration_min;
        }
        else
        {
            cout << "Input invalid!" << endl;
            return;
        }
    }

    auto start_color = ui->startColorWidget->palette().background().color();
    auto end_color = ui->endColorWidget->palette().background().color();

    Vector2 alpha;

    if (GetVector2(ui->txtAlpha->text().toStdString(), alpha))
    {
    }
    else
    {
        cout << "Input invalid!" << endl;
        return;
    }

    if (GetVector2(ui->txtSize->text().toStdString(), config.size))
    {
    }
    else
    {
        cout << "Input invalid!" << endl;
        return;
    }

    config.start_color = Color(start_color.redF(), start_color.greenF(), start_color.blueF(), alpha.x);
    config.end_color = Color(end_color.redF(), end_color.greenF(), end_color.blueF(), alpha.y);
    config.gravity = Vector3(0.0f, -9.81f, 0.0f);

    if (GetVector2(ui->txtLife->text().toStdString(), config.life))
    {
    }
    else
    {
        cout << "Input invalid!" << endl;

        return;
    }

    config.max_particles = std::stoi(ui->txtMaxParticles->text().toStdString());

    //ui->viewport_2->UpdateParticle()
    auto D3D = ui->viewport_2->GetGraphics()->GetD3D().get();

    ui->viewport_2->GetGraphics()->GetParticleEngine()->SetParticleSystem(D3D->GetDevice(), D3D->GetDeviceContext(), config);
    */
}



void MainWindow::on_btnChooseStartColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white);
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    ui->startColorWidget->setPalette(palette);
}

void MainWindow::on_btnChooseEndColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white);
    QPalette palette;
    palette.setColor(QPalette::Background, color);
    ui->endColorWidget->setPalette(palette);
}

void MainWindow::on_btnChooseTexture_clicked()
{
    string file_path = QFileDialog::getOpenFileName(this, tr("Open Image"), "..\\..\\Assets\\Textures", tr("Image Files (*.tga)")).toStdString();
    cout << file_path << endl;
    vector<string> tokens = split(file_path, '/');
    cout << tokens.back() << endl;
    string name = tokens.back();
    ui->txtTextureName->setText(QString::fromStdString(name));
}
