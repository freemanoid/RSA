#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QValidator>
#include <key.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    enum {
        LongKeyLengthWarning = 128
    };
    QIntValidator *KeyLengthComboBoxValidator;
    QIntValidator *NumericSystemComboBoxValidator;
    Ui::MainWindow *ui;
    Key *keyring;
    Key::cryptedmsg CurrentCryptedmsg;
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    void changeEvent(QEvent *e);
    
private slots:
    void getCryptedmsgSlot(Key::cryptedmsg);
    void EnableGenerateButton();
    void DisableGenerateButton();
    void setVisibleTrueProgressBars();
    void setVisibleFalseProgressBars();

    void on_generatePushButton_clicked();
    void on_NumericSystemComboBox_editTextChanged(const QString &arg1);
    void on_CryptPushButton_clicked();
    void on_DecryptPushButton_clicked();
    void on_keyLengthComboBox_editTextChanged(const QString &arg1);

    void on_CryptTextBaseComboBox_editTextChanged(const QString &arg1);
};

#endif // MAINWINDOW_H
