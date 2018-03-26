#ifndef FRAMECANNYFILTER_H
#define FRAMECANNYFILTER_H

#include <OperatorConfigurator.h>
#include <QFrame>

namespace Ui {
class FrameCannyFilter;
}

class FrameCannyFilter : public QFrame
{
    Q_OBJECT

public:
    explicit FrameCannyFilter(OperatorConfigurator *builder, QWidget *parent = 0);
    ~FrameCannyFilter();

    int getT1();
    int getT2();
    int getAperture();
    bool getGradient();

private slots:
    void on_t1SB_valueChanged(int arg1);

    void on_t2SB_valueChanged(int arg1);

    void on_apertureSB_valueChanged(int arg1);

    void on_L2GradientFlag_stateChanged(int arg1);

    void on_apertureCB_currentIndexChanged(int index);

    void on_apertureCB_currentTextChanged(const QString &arg1);

private:
    Ui::FrameCannyFilter *ui;
    OperatorConfigurator *builder;
};

#endif // FRAMECANNYFILTER_H
