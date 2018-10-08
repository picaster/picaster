#ifndef QLEVELSLIDER_H
#define QLEVELSLIDER_H

#include <QObject>
#include <QSlider>
#include <QGroupBox>

class QLevelSlider : public QSlider
{
private:
    int top = 7;
    int bottom = -8;
    long double topValue = 1.12;
    long double bottomValue = 0;

public:
    QLevelSlider(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *);

private:
    void addMark(long double value, QString label, QRect* geometry, QPainter* painter);
    QSize sizeHint() const;
};

#endif // QLEVELSLIDER_H
