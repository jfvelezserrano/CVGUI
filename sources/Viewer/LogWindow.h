#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include <QDialog>

namespace Ui {
class LogWindow;
}

class LogWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LogWindow(QWidget *parent = 0);
    ~LogWindow();
    void addLog(const char *message);
    void addLog(const QString &message);

private:
    Ui::LogWindow *ui;
};

#endif // LOGWINDOW_H
