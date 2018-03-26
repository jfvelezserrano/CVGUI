#include "LogWindow.h"
#include "ui_LogWindow.h"

LogWindow::LogWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogWindow)
{
    ui->setupUi(this);
}

LogWindow::~LogWindow()
{
    delete ui;
}

void LogWindow::addLog(const char *message)
{
    ui->logText->insertPlainText(message);
}

void LogWindow::addLog(const QString &message)
{
    ui->logText->insertPlainText(message);
}
