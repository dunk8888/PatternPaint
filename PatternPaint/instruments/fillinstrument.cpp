#include "fillinstrument.h"

#include "frameeditor.h"

#include <QPen>
#include <QPainter>
#include <QDebug>

FillInstrument::FillInstrument(QObject *parent) :
    CustomCursorInstrument(":/instruments/images/instruments-icons/cursor_fill.png", parent)
{
    drawing = false;
}

void FillInstrument::mousePressEvent(QMouseEvent *event, FrameEditor &editor, const QPoint &pt)
{
    if (event->button() == Qt::LeftButton) {
        drawing = true;
        toolPreview = editor.getPatternAsImage();

        mStartPoint = mEndPoint = pt;
        paint(editor);
    }
}

void FillInstrument::mouseMoveEvent(QMouseEvent *, FrameEditor &, const QPoint &)
{
}

void FillInstrument::mouseReleaseEvent(QMouseEvent *, FrameEditor &editor, const QPoint &)
{
    editor.applyInstrument(toolPreview);
    drawing = false;
}

void FillInstrument::paint(FrameEditor &editor)
{
    // Make a copy of the image
    toolPreview = editor.getPatternAsImage();

    QColor switchColor = editor.getPrimaryColor();
    QRgb pixel(toolPreview.pixel(mStartPoint));

    QColor oldColor(pixel);

    if (switchColor.rgb() != oldColor.rgb())
        fill(mStartPoint, switchColor.rgb(), pixel, toolPreview);
}

QList<QPoint> neighbors(const QPoint &pt, const QImage &img)
{
    QList<QPoint> res;
    if (pt.x() > 0) res << QPoint(pt.x()-1, pt.y());
    if (pt.y() > 0) res << QPoint(pt.x(), pt.y()-1);
    if (pt.x() < img.width() - 1) res << QPoint(pt.x()+1, pt.y());
    if (pt.y() < img.height() - 1) res << QPoint(pt.x(), pt.y()+1);
    return res;
}

void FillInstrument::fill(const QPoint &pt, QRgb newColor, QRgb oldColor, QImage &pattern)
{
    if (pt.x() >= pattern.width() || pt.y() >= pattern.height())
        return;

    if (pattern.pixel(pt) != oldColor)
        return;
    pattern.setPixel(pt, newColor);

    foreach (const QPoint &p, neighbors(pt, pattern))
        if (pattern.pixel(p) == oldColor) fill(p, newColor, oldColor, pattern);
}
