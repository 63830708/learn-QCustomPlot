#include "rangemy.h"


const double QCPRange::minRange = 1e-280;
const double QCPRange::maxRange = 1e+250;


QCPRange::QCPRange():
    lower(0),
    upper(0)
{}


QCPRange::QCPRange(double l, double u):
    lower(l),
    upper(u)
{
    normalize();
}


double QCPRange::size() const
{
    return upper - lower;
}


double QCPRange::center() const
{
    return (upper+lower) * 0.5;
}


void QCPRange::normalize()
{
    if( lower>upper)
        qSwap(lower, upper);
}


QCPRange QCPRange::sanitizedForLogScale() const
{
    double rangeFac = 1e-3;
    QCPRange sanitizedRange(lower, upper);
    sanitizedRange.normalize();

    if(sanitizedRange.lower == 0.0 && sanitizedRange.upper != 0.0)
    {
        if(rangeFac < sanitizedRange.upper*rangeFac)
            sanitizedRange.lower = rangeFac;
        else
            sanitizedRange.lower = sanitizedRange.upper*rangeFac;
    }
    else if(sanitizedRange.lower != 0.0 && sanitizedRange.upper == 0.0)
    {
        if (-rangeFac > sanitizedRange.lower*rangeFac)
          sanitizedRange.upper = -rangeFac;
        else
          sanitizedRange.upper = sanitizedRange.lower*rangeFac;
    }
    else if( sanitizedRange.lower < 0 && sanitizedRange > 0)
    {
        if( -sanitizedRange.lower > sanitizedRange.upper)
        {
            if( -rangeFac > sanitizedRange.lower*rangeFac)
                sanitizedRange.upper = -rangeFac;
            else
                sanitizedRange.upper = sanitizedRange.lower*rangeFac;
        }
        else
        {
            if(rangeFac<sanitizedRange.upper*rangeFac)
                sanitizedRange.lower = rangeFac;
            else
                sanitizedRange.lower = sanitizedRange.upper*rangeFac;
        }
    }

    return sanitizedRange;
}

QCPRange QCPRange::sanitizedForLinScale() const
{
    QCPRange sanitizedRange(lower, upper);
    sanitizedRange.normalize();
    return sanitizedRange;
}


bool QCPRange::contains(double value) const
{
    return value>=lower && value<=upper;
}


bool QCPRange::validRange(double lower, double upper)
{
    return (lower > -maxRange
            && upper < maxRange
            && qAbs(lower-upper) > minRange
            && qAbs(lower-upper) < maxRange);
}


bool QCPRange::validRange(const QCPRange &range)
{
    return (range.lower > -maxRange &&
            range.upper < maxRange &&
            qAbs(range.lower-range.upper) > minRange &&
            qAbs(range.lower-range.upper) < maxRange);
}
