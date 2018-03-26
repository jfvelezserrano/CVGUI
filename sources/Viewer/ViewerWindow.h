#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QLabel>
#include <QTime>

#include "ConfigurationDialog.h"
#include "ImageListener.h"
#include "LogWindow.h"
#include "OperatorBuilder.h"

#include <map>

namespace Ui {
class ViewerWindow;
}

class Navigator;
class Histograms;
class MultiSelectorBuilder;

class ViewerWindow : public QMainWindow
{
    Q_OBJECT

    QString lastLoadedFileName;
    QString lastSavedFileName;
    QString lastLoadedSelectionFileName;

    Navigator *navigator;
    Histograms *histograms;
    LogWindow *logWindow;
    ConfigurationDialog *configurator;
    QTime time;
    std::map <OperatorBuilder*,QAction *> actions;
    std::list <ImageListener*> imageListeners;
    MultiSelectorBuilder *selectionBuilder;

public:
    explicit ViewerWindow(QWidget *parent = 0);
    ~ViewerWindow();
    void applyOperator(OperatorBuilder *o);

    void updateButtons();

    void startCrono();

    void endCrono();

    Navigator *getNavigator() const;

    ConfigurationDialog *getConfigurator() const;

    ImageViewerWithSelector *getViewer() const;

    ImageViewer *getPreviewer() const;

    FileManager * getManager(QStringList fileNames);

    LogWindow *getLogWindow();

private slots:
    void on_actionOpen_file_triggered();

    void on_actionSave_polygon_layer_triggered();

    void on_actionLoad_polygon_layer_triggered();

    void on_actionSave_Image_triggered();

    void on_actionLearnd_objects_triggered();

    void closeEvent ( QCloseEvent * event );

    void on_actionHistogram_triggered();

    void on_actionPolygonalSelection_triggered();

    void on_actionRectSelection_triggered();

    void on_actionPointSelection_triggered();

    void on_actionZoom100_triggered();

    void on_actionZoomFit_triggered();

    void on_actionLog_triggered();

    void on_tabWidget_currentChanged(int index);

private:
    Ui::ViewerWindow *ui;
    void startProcess();
    void endProcess();
    void initOperators();
    bool notify(QObject *receiver, QEvent *even);
    void addVBorders(cv::Mat_<uchar> image, cv::Mat_<uchar> output);
    void addHBorders(cv::Mat_<uchar> image, cv::Mat_<uchar> output);
};



#endif // VIEWERWINDOW_H
