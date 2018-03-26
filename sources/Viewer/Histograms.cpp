#include "Histograms.h"
#include "ui_Histograms.h"
#include <qgraphicsview.h>
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>

#include <QGraphicsPolygonItem>
#include <algorithm>

Histograms::Histograms(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Histograms)
{
    ui->setupUi(this);
}

Histograms::~Histograms()
{
    delete ui;
}

void Histograms::computeRGBPlanes()
{
    if (bgr_planes.size() == 0)
        cv::split( image, bgr_planes );
}

void Histograms::computeHSVPlanes()
{
    const bool isRGB = (image.type() == CV_8UC3);

    if (isRGB)
    {
        cv::Mat hsvImage;
        cv::cvtColor(image,hsvImage,CV_BGR2HSV);
        if (hsv_planes.size() == 0)
            cv::split( hsvImage, hsv_planes );
    }
}

void Histograms::draw256Range(const int hist_w, const int hist_h, QGraphicsScene *scene)
{
    int val = 0;
    const double step_w = hist_w /8.0;

    for( double pos = 0; pos < 9; pos++)
    {
        const int x = pos * step_w;

        QString text = QString::number(val);
        QGraphicsTextItem *textItem = scene->addText(text);
        textItem->setPos(x-10,hist_h);
        val += 256/8;
    }
}

void Histograms::draw1Range(const int hist_w, const int hist_h, QGraphicsScene *scene)
{
    int val = 0;
    const double step_w = hist_w /8.0;

    for( double pos = 0; pos < 9; pos++)
    {
        const int x = pos * step_w;
        QString text = QString::number(val/80.0,'g',3);
        QGraphicsTextItem *textItem = scene->addText(text);
        textItem->setPos(x-10,hist_h);
        val += 10;
    }
}

void Histograms::draw365Range(const int hist_w, const int hist_h, QGraphicsScene *scene)
{
    int val = 0;
    const double step_w = hist_w /8.0;

    for( double pos = 0; pos < 9; pos++)
    {
        const int x = pos * step_w;

        QString text = QString::number(val);
        QGraphicsTextItem *textItem = scene->addText(text);
        textItem->setPos(x-10,hist_h);
        val += 365/8;
    }
}

void Histograms::drawHueRange(const int hist_w, const int hist_h, QGraphicsScene *scene)
{
    for( double pos = 0; pos <= hist_w; pos++)
    {
        QColor color;
        color.setHsv(pos*255/hist_w,255,255);
        QPen pen(color);
        scene->addLine(pos,hist_h+4,pos,hist_h+12,pen);
    }
}

void Histograms::drawValueRange(const int hist_w, const int hist_h, QGraphicsScene *scene)
{
    for( double pos = 0; pos <= hist_w; pos++)
    {
        const int bright = pos * 255 / hist_w;
        QPen pen(QColor(bright,bright,bright));
        scene->addLine(pos,hist_h+4,pos,hist_h+12,pen);
    }
}

void Histograms::drawGrid(const int hist_w,const int hist_h, QGraphicsScene *scene)
{
    QPen gray;
    gray.setColor(QColor(200,200,200));

    const double step_w = hist_w /8.0;
    const double step_h = hist_h /8.0;
    for( double pos = 0; pos < 9; pos++)
    {
        const int x = pos * step_w;
        const int y = pos * step_h;
        scene->addLine(x,0,x ,hist_h,gray);
        scene->addLine(0,y,hist_w, y,gray);
    }

    const int index = ui->scaleComboBox->currentIndex();

    if (index == 0)
        drawValueRange(hist_w, hist_h, scene);
    else if (index == 1)
        draw256Range(hist_w, hist_h, scene);
    else if (index == 2)
        drawHueRange(hist_w, hist_h, scene);
    else if (index == 3)
        draw365Range(hist_w, hist_h, scene);
    else
        draw1Range(hist_w, hist_h, scene);
}

void Histograms::drawLines(QGraphicsScene *scene, const int hist_w, const int hist_h)
{
    QPen red,green,blue,hue,saturation,value;

    red.setColor(QColor(200,0,0));
    green.setColor(QColor(0,200,0));
    blue.setColor(QColor(0,0,200));
    hue.setColor(QColor(200,0,200));
    saturation.setColor(QColor(255,150,0));
    value.setColor(QColor(0,0,0));

    const double step_w = std::max(1.0, (double) hist_w/ (double)histSize);

    const bool drawRed = (ui->redCheckBox->isChecked() && (r_hist.rows > 0));
    const bool drawGreen = (ui->greenCheckBox->isChecked() && (g_hist.rows > 0));
    const bool drawBlue = (ui->blueCheckBox->isChecked() && (b_hist.rows > 0));
    const bool drawHue = (ui->hueCheckBox->isChecked() && (h_hist.rows > 0));
    const bool drawSaturation = (ui->saturationCheckBox->isChecked() && (s_hist.rows > 0));
    const bool drawValue = (ui->valueCheckBox->isChecked() && (v_hist.rows > 0));

    if (drawRed || drawGreen || drawBlue || drawHue || drawSaturation || drawValue)
    {
        for( double x = 0; x < hist_w; x+= step_w)
        {
            int pos1 = std::min(255.0,std::max(0.0,x*255.0 / hist_w));
            int pos2 = std::min(255.0,std::max(0.0,(x+step_w)*255.0 / hist_w));

            if (drawRed)
                scene->addLine(x,hist_h - hist_h * r_hist.at<float>(pos1),x+step_w, hist_h - hist_h*r_hist.at<float>(pos2),red);
            if (drawGreen)
                scene->addLine(x,hist_h - hist_h * g_hist.at<float>(pos1),x+step_w, hist_h - hist_h*g_hist.at<float>(pos2),green);
            if (drawBlue)
                scene->addLine(x,hist_h - hist_h * b_hist.at<float>(pos1),x+step_w, hist_h - hist_h*b_hist.at<float>(pos2),blue);
            if (drawHue)
            {
                int pos1 = std::min(180.0,std::max(0.0,x*180.0 / hist_w));
                int pos2 = std::min(180.0,std::max(0.0,(x+step_w)*180.0 / hist_w));

                scene->addLine(x,hist_h - hist_h * h_hist.at<float>(pos1),x+step_w, hist_h - hist_h*h_hist.at<float>(pos2),hue);
            }
            if (drawSaturation)
                scene->addLine(x,hist_h - hist_h * s_hist.at<float>(pos1),x+step_w, hist_h - hist_h*s_hist.at<float>(pos2),saturation);
            if (drawValue)
                scene->addLine(x,hist_h - hist_h * v_hist.at<float>(pos1),x+step_w, hist_h - hist_h*v_hist.at<float>(pos2),value);
        }
    }
}

bool Histograms::isBitonal()
{
    if (v_hist.size().width == 0)
        return false;

    for( double x = 1; x < 254; x++)
    {
        const float val = v_hist.at<float>(x);
        if (val!=0)
            return false;
    }
    return true;
}


void Histograms::drawBars(QGraphicsScene *scene, const int hist_w, const int hist_h)
{
    if (v_hist.size().width > 0)
    {
        const int bar1 = hist_h *v_hist.at<float>(0);
        const int bar2 = hist_h *v_hist.at<float>(255);
        QPen pen(QColor(0,0,0));

        scene->addRect(0, hist_h -bar1, hist_w/2, bar1, pen);
        scene->addRect(hist_w/2, hist_h-bar2, hist_w/2, bar2, pen);

        const float sum = v_hist.at<float>(0) + v_hist.at<float>(255);
        QString black = QString::number(100*v_hist.at<float>(0)/sum,'f',1)+"%";
        QString white = QString::number(100*v_hist.at<float>(255)/sum,'f',1)+"%";

        scene->addText(black)->setPos(hist_w/4-MARGIN,hist_h);
        scene->addText(white)->setPos(3*hist_w/4-MARGIN,hist_h);
    }
}

void Histograms::drawHistograms()
{
    const int hist_w = ui->graphicsView->width()  - MARGIN*2;
    const int hist_h = ui->graphicsView->height() - MARGIN*2;

    QGraphicsScene *scene = new QGraphicsScene(0,0,hist_w,hist_h);
    ui->graphicsView->setScene(scene);

    const bool isGray = (image.type() == CV_8U);

    if (isGray && isBitonal())
    {
        drawBars(scene, hist_w, hist_h);
    }
    else
    {
        drawGrid(hist_w, hist_h, scene);
        drawLines(scene, hist_w, hist_h);
    }
}

void Histograms::computeRedHistogram()
{
    if ((r_hist.rows == 0) && (image.rows > 0))
    {
        computeRGBPlanes();
        float range[] = {0,256};
        const float* histRange = { range };
        const bool uniform = true;
        const bool accumulate = false;
        cv::calcHist( &bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );
        cv::normalize(r_hist, r_hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
    }
}

void Histograms::computeGreenHistogram()
{
    if ((g_hist.rows == 0) && (image.rows > 0))
    {
        computeRGBPlanes();
        float range[] = {0,256};
        const float* histRange = { range };
        const bool uniform = true;
        const bool accumulate = false;
        cv::calcHist( &bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
        cv::normalize(g_hist, g_hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
    }
}

void Histograms::computeBlueHistogram()
{
    if ((b_hist.rows == 0) && (image.rows > 0))
    {
        computeRGBPlanes();
        float range[] = {0,256};
        const float* histRange = { range };
        const bool uniform = true;
        const bool accumulate = false;
        cv::calcHist( &bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
        cv::normalize(b_hist, b_hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
    }
}


void Histograms::computeHueHistogram()
{
    if ((h_hist.rows == 0) && (image.rows > 0))
    {
        computeHSVPlanes();
        float range[] = {0,180};
        const float* histRange = { range };
        const bool uniform = true;
        const bool accumulate = false;
        cv::calcHist( &hsv_planes[0], 1, 0, cv::Mat(), h_hist, 1, &histSize, &histRange, uniform, accumulate );
        cv::normalize(h_hist, h_hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
    }
}

void Histograms::computeSaturationHistogram()
{
    if ((s_hist.rows == 0) && (image.rows > 0))
    {
        computeHSVPlanes();
        float range[] = {0,256};
        const float* histRange = { range };
        const bool uniform = true;
        const bool accumulate = false;
        cv::calcHist( &hsv_planes[1], 1, 0, cv::Mat(), s_hist, 1, &histSize, &histRange, uniform, accumulate );
        cv::normalize(s_hist, s_hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
    }
}

void Histograms::computeValueHistogram()
{
    const bool isGray = (image.type() == CV_8U);

    if ((v_hist.rows == 0) && (image.rows > 0) && isGray)
    {
        float range[] = {0,256};
        const float* histRange = { range };
        const bool uniform = true;
        const bool accumulate = false;
        cv::calcHist( &image, 1, 0, cv::Mat(), v_hist, 1, &histSize, &histRange, uniform, accumulate );
        cv::normalize(v_hist, v_hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
    }
    else if ((v_hist.rows == 0) && (image.rows > 0))
    {
        computeHSVPlanes();
        float range[] = {0,256};
        const float* histRange = { range };
        const bool uniform = true;
        const bool accumulate = false;
        cv::calcHist( &hsv_planes[2], 1, 0, cv::Mat(), v_hist, 1, &histSize, &histRange, uniform, accumulate );
        cv::normalize(v_hist, v_hist, 0, 1, cv::NORM_MINMAX, -1, cv::Mat() );
    }
}

void Histograms::computeHistograms()
{
    const bool isRGB = ((image.type() == CV_8UC3) && (image.rows > 0));
    const bool isGray = ((image.type() == CV_8U) && (image.rows > 0));

    if ((image.rows > 0) && (isRGB || isGray))
    {
        if (ui->redCheckBox->isChecked() && (r_hist.rows == 0) && isRGB)
            computeRedHistogram();
        if (ui->greenCheckBox->isChecked() && (g_hist.rows == 0) && isRGB)
            computeGreenHistogram();
        if (ui->blueCheckBox->isChecked() && (b_hist.rows == 0) && isRGB)
            computeBlueHistogram();
        if (ui->hueCheckBox->isChecked() && (h_hist.rows == 0) && isRGB)
            computeHueHistogram();
        if (ui->saturationCheckBox->isChecked() && (s_hist.rows == 0) && isRGB)
            computeSaturationHistogram();
        if (ui->valueCheckBox->isChecked() && (v_hist.rows == 0))
            computeValueHistogram();
    }
}

void Histograms::updatePanelToImage(const cv::Mat &image)
{
    this->image = image;

    if (this->isVisible())
    {
        b_hist = cv::Mat();
        g_hist = cv::Mat();
        r_hist = cv::Mat();
        h_hist = cv::Mat();
        s_hist = cv::Mat();
        v_hist = cv::Mat();

        bgr_planes = std::vector<cv::Mat>();
        hsv_planes = std::vector<cv::Mat>();

        computeHistograms();
        drawHistograms();
    }
}

void Histograms::on_redCheckBox_stateChanged(int)
{
    const bool isRGB = (image.type() == CV_8UC3);

    if (isRGB)
    {
        if (ui->redCheckBox->isChecked() && (r_hist.rows == 0))
            computeRedHistogram();
        drawHistograms();
    }
}
void Histograms::on_greenCheckBox_stateChanged(int)
{
    const bool isRGB = (image.type() == CV_8UC3);

    if (isRGB)
    {
        if (ui->greenCheckBox->isChecked() && (g_hist.rows == 0))
            computeGreenHistogram();
        drawHistograms();
    }
}
void Histograms::on_blueCheckBox_stateChanged(int)
{
    const bool isRGB = (image.type() == CV_8UC3);

    if (isRGB)
    {
        if (ui->blueCheckBox->isChecked() && (b_hist.rows == 0))
            computeBlueHistogram();
        drawHistograms();
    }
}
void Histograms::on_hueCheckBox_stateChanged(int)
{
    const bool isRGB = (image.type() == CV_8UC3);

    if (isRGB)
    {
        if (ui->hueCheckBox->isChecked() && (h_hist.rows == 0))
            computeHueHistogram();
        drawHistograms();
    }
}
void Histograms::on_saturationCheckBox_stateChanged(int)
{
    const bool isRGB = (image.type() == CV_8UC3);

    if (isRGB)
    {
        if (ui->saturationCheckBox->isChecked() && (s_hist.rows == 0))
            computeSaturationHistogram();
        drawHistograms();
    }
}
void Histograms::on_valueCheckBox_stateChanged(int)
{
    const bool isRGB = (image.type() == CV_8UC3);
    const bool isGray = (image.type() == CV_8U);

    if (isRGB || isGray)
    {
        if (ui->valueCheckBox->isChecked() && (v_hist.rows == 0))
            computeValueHistogram();
        drawHistograms();
    }
}

void Histograms::resizeEvent(QResizeEvent *)
{
    computeHistograms();
    drawHistograms();
}


void Histograms::showEvent(QShowEvent *)
{
    computeHistograms();
    drawHistograms();
}

void Histograms::on_scaleComboBox_currentIndexChanged(int index)
{
    drawHistograms();
}
