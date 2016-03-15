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
#include "ParticleSettings.h"
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

	//connect(ui->btnSaveSystem, SIGNAL(clicked()), this, SLOT(on_btnSaveSystem_clicked()));

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

ParticleSettings MainWindow::GetParticleSettingsFromUI()
{
	ParticleSettings settings;

	settings.system_name = ui->txtParticleSystemName->text().toStdString();
	settings.texture_name = "..\\..\\Assets\\Textures\\" + ui->txtTextureName->text().toStdString();
	settings.particles_per_second = std::stof(ui->txtParticlesPerSecond->text().toStdString());
	settings.max_particles = stof(ui->txtMaxParticles->text().toStdString());

	if (ui->cmbBlendMode->currentText() == "Alpha")
	{
		settings.blend_state = BLEND_STATE::Alpha;
	}
	else if (ui->cmbBlendMode->currentText() == "Additive")
	{
		settings.blend_state = BLEND_STATE::Additive;
	}

	settings.geometry_scale = Vector2(ui->geometry_scale_x->value(), ui->geometry_scale_y->value());

	settings.min_position = Vector3(ui->min_position_offset_x->value(), ui->min_position_offset_y->value(), ui->min_position_offset_z->value());
	settings.max_position = Vector3(ui->max_position_offset_x->value(), ui->max_position_offset_y->value(), ui->max_position_offset_z->value());

	settings.min_velocity = Vector3(ui->min_velocity_x->value(), ui->min_velocity_y->value(), ui->min_velocity_z->value());
	settings.max_velocity = Vector3(ui->max_velocity_x->value(), ui->max_velocity_y->value(), ui->max_velocity_z->value());

	settings.min_acceleration = Vector3(ui->min_acceleration_x->value(), ui->min_acceleration_y->value(), ui->min_acceleration_z->value());
	settings.max_acceleration = Vector3(ui->max_acceleration_x->value(), ui->max_acceleration_y->value(), ui->max_acceleration_z->value());

	settings.min_color = Color(ui->startColorWidget->palette().background().color().redF(),
		ui->startColorWidget->palette().background().color().greenF(),
		ui->startColorWidget->palette().background().color().blueF(),
		ui->alpha_start->value());

	settings.max_color = Color(ui->endColorWidget->palette().background().color().redF(),
		ui->endColorWidget->palette().background().color().greenF(),
		ui->endColorWidget->palette().background().color().blueF(),
		ui->alpha_end->value());

	float life_min = 0.0f;
	float life_max = 0.0f;
	life_min = ui->life_min->value();
	life_max = ui->life_max->value();

	settings.duration = life_max;
	settings.duration_randomness = life_max - life_min;

	settings.start_size = Vector2(ui->start_size_x->value(), ui->start_size_y->value());
	settings.end_size = Vector2(ui->end_size_x->value(), ui->end_size_y->value());

	return settings;
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
	auto settings = GetParticleSettingsFromUI();

    ui->viewport_2->GetGraphics()->GetParticleEngine()->Clear();
    ui->viewport_2->GetGraphics()->GetParticleEngine()->Add(ui->viewport_2->GetGraphics()->GetD3D()->GetDevice(), ui->viewport_2->GetGraphics()->GetD3D()->GetDeviceContext(), vector<ParticleSettings>( { settings } ));
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

void MainWindow::on_btnSaveSystem_clicked()
{
	auto settings = GetParticleSettingsFromUI();

	string save_path = QFileDialog::getSaveFileName(this, tr("Save System"), "..\\..\\Assets\\ParticleSystems", tr("Particle Systems (*.ps)")).toStdString();
	
	settings.WriteToFile(save_path);
}

void MainWindow::on_btnOpenSystem_clicked()
{
	string file_path = QFileDialog::getOpenFileName(this, tr("Open System"), "..\\..\\Assets\\ParticleSystems", tr("Particle Systems (*.ps)")).toStdString();

	auto settings = ParticleSettings(file_path);

	ui->viewport_2->GetGraphics()->GetParticleEngine()->Clear();
	ui->viewport_2->GetGraphics()->GetParticleEngine()->Add(ui->viewport_2->GetGraphics()->GetD3D()->GetDevice(), ui->viewport_2->GetGraphics()->GetD3D()->GetDeviceContext(), vector<ParticleSettings>({ settings }));
}
