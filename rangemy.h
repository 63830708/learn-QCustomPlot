#ifndef RANGEMY_H
#define RANGEMY_H


#include "global.h"

class QCP_LIB_DECL QCPRange
{
public:
    double lower, upper;

    QCPRange();
    QCPRange(double l, double u);

    double size() const;
    double center() const;
    void normalize();
    QCPRange sanitizedForLogScale() const;
    QCPRange sanitizedForLinScale() const;
    bool contains(double value) const;

    static bool validRange(double lower, double upper);
    static bool validRange(const QCPRange &range);
    static const double minRange;
    static const double maxRange;
};
Q_DECLARE_TYPEINFO(QCPRange, Q_MOVABLE_TYPE);

#endif // RANGEMY_H
