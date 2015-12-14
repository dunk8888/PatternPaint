#include "patternscrollmodel.h"

#include "patternscrollundocommand.h"

#include <QColor>
#include <QDebug>
#include <QPainter>

#define FRAME_COLOR_DEFAULT    QColor(0, 0, 0, 255)
#define PATTERN_FRAME_SPEED_DEFAULT_VALUE 20

PatternScrollModel::PatternScrollModel(QSize size, QObject *parent) :
    PatternModel(parent),
    frameSize(size),
    frameSpeed(PATTERN_FRAME_SPEED_DEFAULT_VALUE)
{
    undoStack.setUndoLimit(50);

    // TODO: How to handle 0-sized image?
    image = QImage(frameSize, QImage::Format_ARGB32_Premultiplied);
    image.fill(FRAME_COLOR_DEFAULT);
}

int PatternScrollModel::rowCount(const QModelIndex &) const
{
    return image.width();
}

Qt::ItemFlags PatternScrollModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

Qt::DropActions PatternScrollModel::supportedDropActions() const
{
    return 0;
}

void PatternScrollModel::pushUndoState()
{
    // TODO: Implement me!
    undoStack.push(new PatternScrollUndoCommand(this));

    if (modified != true) {
        modified = true;

        QVector<int> roles;
        roles.append(Modified);
        emit dataChanged(this->index(0), this->index(rowCount()-1), roles);
    }
}

void PatternScrollModel::applyUndoState(QImage newImage, QSize newSize)
{
    // TODO: Handle the whole state, not just the frames...
    image = newImage;
    frameSize = newSize;

    modified = true;

    QVector<int> roles;
    roles.append(FrameSize);
    roles.append(FrameImage);
    roles.append(Modified);
    emit dataChanged(this->index(0), this->index(rowCount()-1), roles);
}

QVariant PatternScrollModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= rowCount() || index.row() < 0)
        return QVariant();

    if (role == FrameImage || role == Qt::EditRole) {
        // TODO: Handle splits!
        QImage frame(frameSize, QImage::Format_ARGB32_Premultiplied);
        QPainter painter;
        painter.begin(&frame);

        if (index.row() < image.width() - frameSize.width()) {
            painter.drawImage(0, 0, image, index.row(), 0, frameSize.width(), frameSize.height());
        } else {
            painter.drawImage(0, 0,
                              image,
                              index.row(), 0,
                              image.width() - index.row(), frameSize.height());
            painter.drawImage(image.width() - index.row(), 0,
                              image,
                              0, 0,
                              frameSize.width() - image.width() + index.row(), frameSize.height());
        }

        painter.end();
        return frame;
    } else if (role == EditImage) {
        return image;
    } else if (role == FrameSize) {
        return frameSize;
    } else if (role == FrameSpeed) {
        return frameSpeed;
    } else if (role == FileName) {
        return fileInfo;
    } else if (role == Modified) {
        return modified;
    }

    return QVariant();
}

bool PatternScrollModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    pushUndoState();

    if (role == FrameImage || role == Qt::EditRole) {
////TODO: enforce size scaling here?

// QPainter painter;
// painter.begin(&image);

// if(index.row() < image.width() - frameSize.width())
// painter.drawImage(index.row(),0,
// value.value<QImage>(),
// 0,0,
// frameSize.width(),frameSize.height());

// else {
// painter.drawImage(index.row(),0,
// value.value<QImage>(),
// 0,0,
// image.width() - index.row(),frameSize.height());

// painter.drawImage(0,0,
// value.value<QImage>(),
// image.width()-index.row(),0,
// frameSize.width() - image.width() + index.row(),frameSize.height());
// }

// painter.end();

// QVector<int> roles;
// roles.append(FrameImage);
// emit dataChanged(index, index, roles);
// return true;
        return false;
    } else if (role == EditImage) {
        // TODO: enforce size scaling here?

        QPainter painter;
        painter.begin(&image);
        painter.drawImage(0, 0, value.value<QImage>());
        painter.end();

        QVector<int> roles;
        roles.append(FrameImage);
        emit dataChanged(index, index, roles);
        return true;
    } else if (role == FrameSize) {
        // TODO: Implement me

        frameSize = value.toSize();
        QImage newImage;
        bool scale = true;      // Enforce scaling...

        if (scale) {
            newImage = image.scaled(image.width(), frameSize.height());
        } else {
            newImage = QImage(image.width(), frameSize.height(),
                              QImage::Format_ARGB32_Premultiplied);
            newImage.fill(FRAME_COLOR_DEFAULT);

            QPainter painter(&newImage);
            painter.drawImage(0, 0, image);
        }

        image = newImage;

        QVector<int> roles;
        roles.append(FrameSize);
        roles.append(FrameImage);
        roles.append(EditImage);

        emit dataChanged(this->index(0), this->index(rowCount()-1), roles);
        return true;
    } else if (role == FrameSpeed) {
        frameSpeed = value.toFloat();

        QVector<int> roles;
        roles.append(FrameSpeed);
        emit dataChanged(this->index(0), this->index(rowCount()-1), roles);
        return true;
    } else if (role == FileName) {
        fileInfo = value.toString();

        QVector<int> roles;
        roles.append(FileName);
        emit dataChanged(this->index(0), this->index(rowCount()-1), roles);
        return true;
    } else if (role == Modified) {
        modified = value.toBool();

        QVector<int> roles;
        roles.append(Modified);
        emit dataChanged(this->index(0), this->index(rowCount()-1), roles);
        return true;
    }

    return false;
}

bool PatternScrollModel::insertRows(int position, int rows, const QModelIndex &)
{
    pushUndoState();
    beginInsertRows(QModelIndex(), position, position+rows-1);

    QImage newImage(image.width()+rows, frameSize.height(), QImage::Format_ARGB32_Premultiplied);

    newImage.fill(FRAME_COLOR_DEFAULT);

    QPainter painter;
    painter.begin(&newImage);

    if (position == 0) {
        painter.drawImage(rows, 0, image, 0, 0, image.width(), frameSize.height());
    } else if (position == image.width()) {
        painter.drawImage(0, 0, image, 0, 0, image.width(), frameSize.height());
    } else {
        painter.drawImage(0, 0, image, 0, 0, position, frameSize.height());
        painter.drawImage(position + rows, 0, image,
                          position, 0,
                          image.width() - position, frameSize.height());
    }

    painter.end();

    image = newImage;
    endInsertRows();

    // TODO: what does 'data changed' mean in these circumstances?
    QVector<int> roles;
    roles.append(FrameImage);
    emit dataChanged(this->index(position), this->index(position+rows), roles);
    return true;
}

bool PatternScrollModel::removeRows(int position, int rows, const QModelIndex &)
{
    pushUndoState();
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    QImage newImage(image.width()-rows, image.height(), QImage::Format_ARGB32_Premultiplied);
    newImage.fill(FRAME_COLOR_DEFAULT);

    // If we have any rows remaining, copy them into the new image
    if (newImage.width() > 0) {
        QPainter painter;
        painter.begin(&newImage);

        if (position == 0) {
            painter.drawImage(0, 0, image, rows, 0, image.width()-rows, frameSize.height());
        } else if (position == image.width()) {
            painter.drawImage(0, 0, image, 0, 0, image.width()-rows, frameSize.height());
        } else {
            painter.drawImage(0, 0, image, 0, 0, position, frameSize.height());
            painter.drawImage(position, 0, image,
                              position + rows, 0,
                              image.width() - position - rows, frameSize.height());
        }

        painter.end();
    }

    image = newImage;
    endRemoveRows();

    // TODO: what does 'data changed' mean in these circumstances?
    QVector<int> roles;
    roles.append(FrameImage);
    emit dataChanged(this->index(0), this->index(rowCount()-1), roles);
    return true;
}
