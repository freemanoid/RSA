#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    KeyLengthComboBoxValidator = new QIntValidator(8, 1024, this);
    ui->keyLengthComboBox->setValidator(KeyLengthComboBoxValidator);
    NumericSystemComboBoxValidator = new QIntValidator(2, 62, this);
    ui->NumericSystemComboBox->setValidator(NumericSystemComboBoxValidator);
    ui->CryptTextBaseComboBox->setValidator(NumericSystemComboBoxValidator);

    keyring = new Key(8);
    connect(ui->generatePushButton, SIGNAL(clicked()), keyring, SLOT(NewKey()));
    connect(keyring, SIGNAL(PublicKeyUpdate(QString)), ui->PublicKeyTextBrowser, SLOT(setText(QString)));
    connect(keyring, SIGNAL(PrivateKeyUpdate(QString)), ui->PrivateKeyTextBrowser, SLOT(setText(QString)));
    connect(keyring, SIGNAL(PublicKeyUpdate(QString)), ui->PublicKeyTextBrowser_2, SLOT(setText(QString)));
    connect(keyring, SIGNAL(PrivateKeyUpdate(QString)), ui->PrivateKeyTextBrowser_2, SLOT(setText(QString)));
    connect(keyring, SIGNAL(MessageCryptedString(QString)), ui->CryptTextBrowser, SLOT(setText(QString)));
    connect(keyring, SIGNAL(MessageDecryptedString(QString)), ui->CryptTextBrowser, SLOT(setText(QString)));
    connect(keyring, SIGNAL(MessageCrypted(Key::cryptedmsg)), this, SLOT(getCryptedmsgSlot(Key::cryptedmsg)));

    connect(keyring, SIGNAL(NewKeyGenerationStarted()), this, SLOT(DisableGenerateButton()));
    connect(keyring, SIGNAL(NewKeyGenerationStarted()), this, SLOT(setVisibleTrueProgressBars()));
    connect(keyring, SIGNAL(NewKeyGenerationFinished()), this, SLOT(EnableGenerateButton()));
    connect(keyring, SIGNAL(NewKeyGenerationFinished()), this, SLOT(setVisibleFalseProgressBars()));

    setVisibleFalseProgressBars();
    ui->retranslateUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete KeyLengthComboBoxValidator;
    delete keyring;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::getCryptedmsgSlot(Key::cryptedmsg msg)
{
    CurrentCryptedmsg = msg;
}

void MainWindow::EnableGenerateButton()
{
    ui->generatePushButton->setEnabled(true);
}

void MainWindow::DisableGenerateButton()
{
    ui->generatePushButton->setEnabled(false);
}

void MainWindow::setVisibleTrueProgressBars()
{
    ui->PublicKeyProgressBar->setVisible(true);
    ui->PrivateKeyProgressBar->setVisible(true);
    ui->PublicKeyProgressBar_2->setVisible(true);
    ui->PrivateKeyProgressBar_2->setVisible(true);
}

void MainWindow::setVisibleFalseProgressBars()
{
    ui->PublicKeyProgressBar->setVisible(false);
    ui->PrivateKeyProgressBar->setVisible(false);
    ui->PublicKeyProgressBar_2->setVisible(false);
    ui->PrivateKeyProgressBar_2->setVisible(false);
}

void MainWindow::on_generatePushButton_clicked()
{
    keyring->KeyLengthChanged(ui->keyLengthComboBox->getValue());
}

void MainWindow::on_CryptPushButton_clicked()
{
    if(ui->CryptTextBrowser->toPlainText().size() < 1)
    {
        QMessageBox msg;
        msg.setText(tr("Text field can not be empty"));
        msg.exec();
        return;
    }
    if(ui->PrivateKeyTextBrowser->toPlainText().size() == 0)
    {
        QMessageBox msg;
        msg.setText(tr("Private key must be defined"));
        msg.exec();
        return;
    }
    keyring->CryptMessageSlot(ui->CryptTextBrowser->toPlainText(), ui->CryptTextBaseComboBox->getValue());
    ui->CryptPushButton->setEnabled(false);
    ui->DecryptPushButton->setEnabled(true);
    ui->CryptTextBrowser->setReadOnly(true);
}

void MainWindow::on_DecryptPushButton_clicked()
{
    if(ui->CryptTextBrowser->toPlainText().size() < 1)
    {
        QMessageBox msg;
        msg.setText(tr("Text field can not be empty"));
        msg.exec();
        return;
    }
    if(ui->PublicKeyTextBrowser->toPlainText().size() == 0)
    {
        QMessageBox msg;
        msg.setText(tr("Public key must be defined"));
        msg.exec();
        return;
    }
    keyring->DecryptMessageSlot(CurrentCryptedmsg);
    ui->CryptPushButton->setEnabled(true);
    ui->DecryptPushButton->setEnabled(false);
    ui->CryptTextBrowser->setReadOnly(false);
}

void MainWindow::on_keyLengthComboBox_editTextChanged(const QString &arg1)
{
    QString carry1 = arg1;
    int carry2 = 0;
    if(ui->keyLengthComboBox->validator()->validate(carry1, carry2) != QValidator::Acceptable)
        return;
    if(ui->keyLengthComboBox->getValue() > LongKeyLengthWarning)
        ui->statusBar->showMessage(tr("Note: keys generation with length > ")
                                   + QString::number(LongKeyLengthWarning) +
                                   tr(" can take a while on weak systems."));
    else
        ui->statusBar->clearMessage();
}

void MainWindow::on_NumericSystemComboBox_editTextChanged(const QString &arg1)
{
    QString carry1 = arg1;
    int carry2 = 0;
    if(ui->NumericSystemComboBox->validator()->validate(carry1, carry2) == QValidator::Acceptable)
    {
        QString carry;
        carry = Key::QStringBase(ui->PrivateKeyTextBrowser->toPlainText(), arg1.toInt(), keyring->getBase());
        ui->PrivateKeyTextBrowser->setText(carry);
        carry = Key::QStringBase(ui->PublicKeyTextBrowser->toPlainText(), arg1.toInt(), keyring->getBase());
        ui->PublicKeyTextBrowser->setText(carry);
        carry = Key::QStringBase(ui->PrivateKeyTextBrowser_2->toPlainText(), arg1.toInt(), keyring->getBase());
        ui->PrivateKeyTextBrowser_2->setText(carry);
        carry = Key::QStringBase(ui->PublicKeyTextBrowser_2->toPlainText(), arg1.toInt(), keyring->getBase());
        ui->PublicKeyTextBrowser_2->setText(carry);
        keyring->BaseChanged(arg1.toInt());
    }
}

void MainWindow::on_CryptTextBaseComboBox_editTextChanged(const QString &arg1)
{
    static int previousBase = 10;
    QString carry1 = arg1;
    int carry2 = 0;
    if(!ui->DecryptPushButton->isEnabled())
    {
        if(ui->CryptTextBaseComboBox->validator()->validate(carry1, carry2) == QValidator::Acceptable)
            previousBase = arg1.toInt();
        return;
    }
    if(ui->CryptTextBaseComboBox->validator()->validate(carry1, carry2) == QValidator::Acceptable)
    {
        ui->CryptTextBrowser->setText(Key::QStringBase(ui->CryptTextBrowser->toPlainText(), arg1.toInt(), previousBase));
        previousBase = ui->CryptTextBaseComboBox->getValue();
    }
}
