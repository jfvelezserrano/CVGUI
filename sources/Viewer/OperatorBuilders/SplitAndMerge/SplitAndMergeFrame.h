#ifndef SPLITANDMERGEFRAME_H
#define SPLITANDMERGEFRAME_H

#include <QFrame>

namespace Ui {
class SplitAndMergeFrame;
}

class SplitAndMergeFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SplitAndMergeFrame(QWidget *parent = 0);
    ~SplitAndMergeFrame();

private:
    Ui::SplitAndMergeFrame *ui;
};

#endif // SPLITANDMERGEFRAME_H
