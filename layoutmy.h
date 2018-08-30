
#ifndef LAYOUTMY_H
#define LAYOUTMY_H

#include "global.h"
#include "layermy.h"

class QCPLayout;
class QCPLayoutElement;
class QCustomPlot;

class QCP_LIB_DECL QCPMarginGroup : public QObject
{
    Q_OBJECT
public:
    QCPMarginGroup(QCustomPlot *parentPlot);
    ~QCPMarginGroup();

    QList<QCPLayoutElement*> elements(QCP::MarginSide side) const { return mChildren.value(side);}

protected:
    int commonMargin(QCP::MarginSide side) const;
    void addChild(QCP::MarginSide side, QCPLayoutElement *element);
    void removeChild(QCP::MarginSide side, QCPLayoutElement *element);

    QCustomPlot *mParentPlot;
    QHash<QCP::MarginSide, QList<QCPLayoutElement*> > mChildren;
private:
    Q_DISABLE_COPY(QCPMarginGroup)

    friend class QCPLayoutElement;
};

#endif // LAYOUTMY_H

