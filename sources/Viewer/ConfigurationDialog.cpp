#include "ConfigurationDialog.h"
#include "ui_ConfigurationDialog.h"

#include <QDesktopServices>
#include <QUrl>

ConfigurationDialog::ConfigurationDialog(Navigator *navigator, QWidget *parent):
    QDialog(parent),
    ui(new Ui::ConfigurationDialog)
{
    ui->setupUi(this);
    this->navigator = navigator;
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(slotCloseTab(int)));
}

ConfigurationDialog::~ConfigurationDialog()
{
    delete ui;
}


void ConfigurationDialog::on_applyButton_clicked()
{
    QTabWidget *container = ui->tabWidget;
    if (container->currentIndex() != -1)
    {
        QWidget *frame = container->currentWidget();
        OperatorConfigurator *builder = tabs[frame];
        builder->apply();
    }
}

void ConfigurationDialog::addOperation(OperatorConfigurator *conf)
{
    QFrame *frame = conf->getConfigPanel();
    QTabWidget *container = ui->tabWidget;

    if (container->indexOf(frame) == -1)
    {
        std::vector<std::string> description = conf->description();
        tabs[frame] = conf;
        container->addTab(frame,description[description.size()-1].c_str());
        container->setCurrentWidget(frame);
    }
}

void ConfigurationDialog::slotCloseTab(int index)
{
    QTabWidget *container = ui->tabWidget;
    container->removeTab(index);
    if (container->currentIndex() == -1)
    {
        ui->applyButton->setEnabled(false);
        ui->helpButton->setEnabled(false);
    }
}

void ConfigurationDialog::on_helpButton_clicked()
{
    QTabWidget *container = ui->tabWidget;
    if (container->currentIndex() != -1)
    {
        QWidget *frame = container->currentWidget();
        OperatorConfigurator *builder = tabs[frame];
        QString url = builder->helpURL().c_str();
        QDesktopServices::openUrl(QUrl(url));
    }
}


void ConfigurationDialog::updatePanelToImage(const cv::Mat &image)
{
    if (isVisible())
    {
        QTabWidget *container = ui->tabWidget;
        QWidget *frame = container->currentWidget();
        if (frame != NULL)
        {
            OperatorConfigurator *builder = tabs[frame];
            const bool compatibleImage = builder->isCompatible();
            ui->applyButton->setEnabled(compatibleImage);
            frame->setEnabled(compatibleImage);
            ui->helpButton->setEnabled(true);
            builder->updatePreview();
        }
        else
        {
            ui->applyButton->setEnabled(false);
            ui->helpButton->setEnabled(false);
        }
    }
}
void ConfigurationDialog::on_tabWidget_currentChanged(int index)
{
    const cv::Mat image = navigator->getImage();
    updatePanelToImage(image);
}
