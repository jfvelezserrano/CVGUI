#ifndef FRAMEGABORFILTER_H
#define FRAMEGABORFILTER_H

#include <QFrame>
#include <opencv2/core/core.hpp>

class GaborFilterBuilder;

namespace Ui {
class FrameGaborFilter;
}

class FrameGaborFilter : public QFrame
{
    Q_OBJECT

public:
    explicit FrameGaborFilter(GaborFilterBuilder *builder, QWidget *parent = 0);
    ~FrameGaborFilter();

    double getSigma() const;
    double getTheta() const;
    double getLambda() const;
    double getGamma() const;
    double getPsi() const;
    double getKernelRadious() const;

private slots:
    void on_thetaSlider_valueChanged(int value);

    void on_psiSlider_valueChanged(int value);

    void on_gammaValue_valueChanged(double arg1);

    void on_sigmaValue_valueChanged(double arg1);

    void on_kernelRadious_valueChanged(int arg1);

    void on_splitter_splitterMoved(int pos, int index);

    void on_lambdaValue_valueChanged(int arg1);

private:
    Ui::FrameGaborFilter *ui;
    int kernelRadious = 15;
    double sigma = 1;
    double theta = 0;
    double lambda = 1;
    double gamma = 0.02;
    double psi = CV_PI/2;
    GaborFilterBuilder *builder;
    double updateMatrix() const;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *);
};

#endif // FRAMEGABORFILTER_H
