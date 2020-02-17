#ifndef FRAMEHOUGHDETECTION_H
#define FRAMEHOUGHDETECTION_H

#include <QFrame>
#include <OperatorConfigurator.h>

namespace Ui {
class FrameHoughDetection;
}

class FrameHoughDetection : public QFrame
{
    Q_OBJECT

public:
    explicit FrameHoughDetection(OperatorConfigurator *builder, QWidget *parent = nullptr);
    ~FrameHoughDetection();

    int getDistance();

    double getAngle();

    int getThreshold();

    double getSRN();

    double getSTN();

    double getMinTheta();

    double getMaxTheta();
private slots:
    void on_distanceResolution_valueChanged(int arg1);

    void on_angleResolution_valueChanged(double arg1);

    void on_threshold_valueChanged(int arg1);

    void on_distanceDivisor_valueChanged(double arg1);

    void on_angleDivisor_valueChanged(double arg1);

    void on_minAngle_valueChanged(const QString &arg1);

    void on_maxAngle_valueChanged(double arg1);

private:
    Ui::FrameHoughDetection *ui;    
    OperatorConfigurator *builder;
};

#endif // FRAMEHOUGHDETECTION_H
