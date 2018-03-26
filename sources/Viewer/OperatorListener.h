#ifndef OPERATORLISTENER_H
#define OPERATORLISTENER_H

#include "OperatorBuilder.h"
#include "OperatorConfigurator.h"
#include "ViewerWindow.h"

#include <QObject>

class OperatorListener : public QObject
{
    Q_OBJECT

    OperatorBuilder *builder;
    ViewerWindow *viewer;

public:
    explicit OperatorListener(OperatorBuilder *builder, ViewerWindow *viewer);

signals:

public slots:
    void action();
};

#endif // OPERATORLISTENER_H
