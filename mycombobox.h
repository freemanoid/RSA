#ifndef MYCOMBOBOX_H
#define MYCOMBOBOX_H

#include <QComboBox>
#include <QValidator>

class MyComboBox : public QComboBox
{
    Q_OBJECT

protected:
    void focusOutEvent(QFocusEvent *e);

public:
    explicit MyComboBox(QWidget *parent = 0);
    quint32 getValue() const;
    ~MyComboBox();

};

#endif // MYCOMBOBOX_H
