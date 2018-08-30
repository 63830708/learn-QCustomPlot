
#include "layoutmy.h"

#include "core.h"


QCPMarginGroup::QCPMarginGroup(QCustomPlot *parentPlot):
    QObject(parentPlot),
    mParentPlot(parentPlot)
{
    mChildren.insert(QCP::msLeft, QList<QCPLayoutElement*>());
    mChildren.insert(QCP::msRight, QList<QCPLayoutElement*>());
    mChildren.insert(QCP::msTop, QList<QCPLayoutElement*>());
    mChildren.insert(QCP::msBottom, QList<QCPLayoutElement*>());
}


QCPMarginGroup::~QCPMarginGroup()
{
    clear();
}

