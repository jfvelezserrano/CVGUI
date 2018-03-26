#ifndef FRAMEMORPHOLOGICALFILTER_H
#define FRAMEMORPHOLOGICALFILTER_H

#include <QFrame>
#include <opencv2/core/core.hpp>

class MorphologyFilterBuilder;
class QInteractiveGraphicsScene;

namespace Ui {
class FrameMorphologicalFilter;
}

class FrameMorphologicalFilter : public QFrame
{
    Q_OBJECT

    QString lastLoadedFileName;
    QString lastSavedFileName;

public:
    explicit FrameMorphologicalFilter(MorphologyFilterBuilder *builder, QWidget *parent = 0);
    ~FrameMorphologicalFilter();

    cv::Mat_<uint8_t> getElement();

    bool isDilate();
private slots:
    void on_pushButton_newElement_clicked();

    void on_pushButton_LoadElement_clicked();

    void on_pushButton_SaveElement_clicked();

    void on_radioButton_dilate_toggled(bool checked);

private:
    Ui::FrameMorphologicalFilter *ui;

    MorphologyFilterBuilder *builder;

    virtual void resizeEvent(QResizeEvent *event);

    QInteractiveGraphicsScene *scene;

    void setNewScene(QInteractiveGraphicsScene *newScene);
};

#endif // FRAMEMORPHOLOGICALFILTER_H
