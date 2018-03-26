#ifndef HISTOGRAMS_H
#define HISTOGRAMS_H

#include "ImageListener.h"

#include <QDialog>
#include <QGraphicsScene>

namespace Ui {
class Histograms;
}

class Histograms : public QDialog, public ImageListener
{
    Q_OBJECT

    Ui::Histograms *ui;
    cv::Mat b_hist, g_hist, r_hist,h_hist, s_hist, v_hist;
    const int histSize = 256;
    const int MARGIN = 20;
    cv::Mat image;
    std::vector<cv::Mat> bgr_planes,hsv_planes;


    // ImageListener interface
    void computeRedHistogram();
    void computeGreenHistogram();
    void computeBlueHistogram();
    void computeValueHistogram();
    void computeHueHistogram();
    void computeSaturationHistogram();
    void computeRGBPlanes();
    void computeHSVPlanes();
    void drawGrid(const int hist_w, const int hist_h, QGraphicsScene *scene);
    void draw1Range(const int hist_w, const int hist_h, QGraphicsScene *scene);
    void draw365Range(const int hist_w, const int hist_h, QGraphicsScene *scene);
    void drawHueRange(const int hist_w, const int hist_h, QGraphicsScene *scene);
    void drawValueRange(const int hist_w, const int hist_h, QGraphicsScene *scene);
    void draw256Range(const int hist_w, const int hist_h, QGraphicsScene *scene);
    void drawLines(QGraphicsScene *scene, const int hist_w, const int hist_h);
    bool isBitonal();
    void drawBars(QGraphicsScene *scene, const int hist_w, const int hist_h);
public:
    explicit Histograms(QWidget *parent = 0);
    ~Histograms();
    void updatePanelToImage(const cv::Mat &image);
    void drawHistograms();
    void computeHistograms();
private slots:
    void on_redCheckBox_stateChanged(int);
    void on_saturationCheckBox_stateChanged(int);
    void on_hueCheckBox_stateChanged(int);
    void on_valueCheckBox_stateChanged(int);
    void on_blueCheckBox_stateChanged(int);
    void on_greenCheckBox_stateChanged(int);

    // QWidget interface
    void on_scaleComboBox_currentIndexChanged(int index);

protected:
    void resizeEvent(QResizeEvent *);
    void showEvent(QShowEvent *);
};

#endif // HISTOGRAMS_H
