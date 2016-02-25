#include "vectorwidget.h"

Vector2Widget::Vector2Widget(QWidget* parent)
   : QWidget(parent)
{
    texts = new QLineEdit[2];
    layout = new QGridLayout(this);
    groupBox = new QGroupBox(this);

    texts[0].setText("0.0");
    texts[1].setText("0.0");

    texts[0].setFixedWidth(50);
    texts[1].setFixedWidth(50);


    layout->setColumnMinimumWidth(0, 50);
    layout->setColumnMinimumWidth(1, 50);
    layout->addWidget(&texts[0]);
    layout->addWidget(&texts[1]);

    groupBox->setLayout(layout);

   // groupBox->setTitle();
}

Vector2Widget::Vector2Widget(const Vector2Widget &other)
{

}

Vector2Widget::~Vector2Widget()
{
    if (texts)
        delete [] texts;

    if (layout)
        delete layout;

    if (groupBox)
        delete groupBox;
}

float Vector2Widget::myVecX()
{
    return texts[0].text().toFloat();
}

float Vector2Widget::myVecY()
{
    return texts[1].text().toFloat();
}
