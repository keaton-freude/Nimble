#ifndef VECTORWIDGET
#define VECTORWIDGET

#include <QLineEdit>
#include <QGroupBox>
#include <QSharedPointer>
#include <QLayout>

class Vector2Widget : public QWidget
{
    Q_OBJECT
public:
    Vector2Widget(QWidget* parent);
    Vector2Widget(const Vector2Widget& other);
    ~Vector2Widget();

    float myVecX();
    float myVecY();
private:
    // one for x and one for y
    QLineEdit* texts;
    QGroupBox* groupBox;
    QGridLayout* layout;

};


#endif // VECTORWIDGET

