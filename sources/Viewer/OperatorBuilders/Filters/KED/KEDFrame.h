#ifndef KEDFRAME_H
#define KEDFRAME_H

#include <QFrame>

namespace Ui {
class KEDFrame;
}

class KEDFrame : public QFrame
{
    Q_OBJECT

public:
    explicit KEDFrame(QWidget *parent = 0);
    ~KEDFrame();
    int getWindowSize();

private:
    Ui::KEDFrame *ui;
};

#endif // KEDFRAME_H
