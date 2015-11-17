#include "matrixfixture.h"

MatrixFixture::MatrixFixture(
        QSize size,
        ColorMode colorMode,
        BrightnessModel *brightnessModel,
        QObject *parent) :
    Fixture(parent),
    colormode(colorMode),
    brightnessModel(brightnessModel),
    size(size)
{
}

QList<QColor> MatrixFixture::getColorStreamForFrame(const QImage frame) const
{
    QList<QColor> colorStream;

    if(frame.isNull()) {
        return colorStream;
    }

    for(int x = 0; x <size.width(); x++) {
        for(int y = 0; y < size.height(); y++) {
            QColor pixel = frame.pixel(x,x%2?size.height()-1-y:y);
            colorStream.append(brightnessModel->correct(pixel));
        }
    }

    return colorStream;
}

int MatrixFixture::getLedCount() const
{
    return size.height()*size.width();
}

QSize MatrixFixture::getSize() const
{
    return size;
}

void MatrixFixture::setSize(QSize newSize)
{
    size = newSize;
}

ColorMode MatrixFixture::getColorMode() const {
    return colormode;
}

void MatrixFixture::setColorMode(ColorMode newColorMode) {
    colormode = newColorMode;
}

BrightnessModel * MatrixFixture::getBrightnessModel() const {
    return brightnessModel;
}
void MatrixFixture::setBrightnessModel(BrightnessModel *newBrightnessModel) {
    brightnessModel = newBrightnessModel;
}