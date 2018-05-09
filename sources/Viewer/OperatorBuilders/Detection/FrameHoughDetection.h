#ifndef FRAMEHOUGHDETECTION_H
#define FRAMEHOUGHDETECTION_H

#include <QFrame>

namespace Ui {
class FrameHoughDetection;
}

class FrameHoughDetection : public QFrame
{
    Q_OBJECT

public:
    explicit FrameHoughDetection(QWidget *parent = 0);
    ~FrameHoughDetection();

private:
    Ui::FrameHoughDetection *ui;
};

#endif // FRAMEHOUGHDETECTION_H
