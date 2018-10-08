#include "qlevelslider.h"

#include <iostream>

#include <QPainter>

QLevelSlider::QLevelSlider(QWidget *parent) : QSlider(parent)
{

}

void
QLevelSlider::paintEvent(QPaintEvent *ev)
{
    QSlider::paintEvent(ev);
    QRect geometry = this->geometry();
    QPainter painter(this);
    QFont font = painter.font();
    font.setPointSize(static_cast<int>(font.pointSize() / 1.4));
    painter.setFont(font);

    addMark(1.11212562118l, "  +3", &geometry, &painter);
    addMark(1.00000000000l, "   0", &geometry, &painter);
    addMark(0.89917899646l, "  -3", &geometry, &painter);
    addMark(0.80852286768l, "  -6", &geometry, &painter);
    addMark(0.70170382867l, " -10", &geometry, &painter);
    addMark(0.60899732444l, " -14", &geometry, &painter);
    addMark(0.49238826317l, " -20", &geometry, &painter);
    addMark(0.39810717055l, " -26", &geometry, &painter);
    addMark(0.29986313485l, " -34", &geometry, &painter);
    addMark(0.20309176209l, " -45", &geometry, &painter);
    addMark(0.10000000000l, " -65", &geometry, &painter);
    addMark(0.00000000000l, "-inf", &geometry, &painter);
}

void
QLevelSlider::addMark(long double value, QString label, QRect* geometry, QPainter* painter)
{
    long double percentValue = static_cast<long double>(value - bottomValue) / static_cast<long double>(topValue - bottomValue);
    long double invertedPercentValue = 1.0l - percentValue;
    int pixelValue = static_cast<int>(top + ((geometry->height() + bottom) - top) * invertedPercentValue);

    painter->save();
    painter->setPen(QPen(this->palette().dark().color()));
    painter->drawLine(6 + geometry->width() / 2, pixelValue, 9 + geometry->width() / 2, pixelValue);
    //painter->drawLine(geometry->width() - 5, pixelValue, geometry->width() - 2, pixelValue);

    QFont font = painter->font();
    QFontMetrics fm(font);
    painter->setPen(QPen(this->palette().foreground().color()));
    painter->drawText(0, pixelValue + (fm.height() / 2) - 2, label);
    painter->restore();
}

QSize
QLevelSlider::sizeHint() const
{
    QSize size = QSlider::sizeHint();
    size.setWidth(size.width() * 2);

    return size;
}
