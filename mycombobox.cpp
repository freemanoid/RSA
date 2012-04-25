#include "mycombobox.h"
#include <QDebug>

MyComboBox::MyComboBox(QWidget *parent) :
    QComboBox(parent)
{
    this->setFocusPolicy(Qt::StrongFocus);
    this->setInputMethodHints(Qt::ImhDigitsOnly);
}

quint32 MyComboBox::getValue() const
{
    return currentText().toUInt();
}

MyComboBox::~MyComboBox()
{
}

void MyComboBox::focusOutEvent(QFocusEvent *e)
{
    if(this->validator() && (currentText().toInt() < dynamic_cast<const QIntValidator*>(validator())->bottom()))
        setEditText(QString::number(dynamic_cast<const QIntValidator*>(validator())->bottom()));
}
