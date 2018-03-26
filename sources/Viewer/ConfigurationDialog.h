#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include "ImageListener.h"
#include "OperatorBuilder.h"
#include "OperatorConfigurator.h"

#include <QDialog>
#include <QTabWidget>

#include <map>
#include <set>

namespace Ui {
class ConfigurationDialog;
}

class ConfigurationDialog : public QDialog, public ImageListener
{
    Q_OBJECT

public:
    explicit ConfigurationDialog(Navigator *navigator, QWidget *parent = 0);
    ~ConfigurationDialog();

    void addOperation(OperatorConfigurator *conf);
private slots:
    void on_applyButton_clicked();

    void on_helpButton_clicked();

    void on_tabWidget_currentChanged(int index);
    void slotCloseTab(int index);

private:
    Ui::ConfigurationDialog *ui;
    std::map <QWidget*,OperatorConfigurator*> tabs;
    Navigator *navigator;

    // ImageListener interface
public:
    void updatePanelToImage(const cv::Mat &image);
};

#endif // CONFIGURATIONDIALOG_H
