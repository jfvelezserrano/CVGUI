#ifndef FRAMELEARNOBJECTS_H
#define FRAMELEARNOBJECTS_H

#include <Navigator.h>
#include <QFrame>

namespace Ui {
class FrameLearnObjects;
}

class FrameLearnObjects : public QFrame
{
    Q_OBJECT

public:
    explicit FrameLearnObjects(Navigator *navigator, QWidget *parent = 0);
    ~FrameLearnObjects();

private slots:


    void on_newLearner_clicked();

    void on_loadLearner_clicked();

    void on_addLayer_clicked();

private:
    Ui::FrameLearnObjects *ui;
    QString lastFileName;
    Navigator *navigator;
};

#endif // FRAMELEARNOBJECTS_H
