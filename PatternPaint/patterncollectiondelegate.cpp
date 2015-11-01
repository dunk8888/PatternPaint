#include "patterncollectiondelegate.h"
#include "patterncollectionmodel.h"

#include "colors.h"

#include <QDebug>

#define ITEM_HEIGHT 120
#define ITEM_WIDTH 150
#define ITEM_LETTERBOX 10
#define FRAME_SIZE 2

PatternCollectionDelegate::PatternCollectionDelegate(QObject* parent) :
    QStyledItemDelegate(parent)
{
}

void PatternCollectionDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex & index ) const {
    painter->save();
    QImage img = qvariant_cast<QImage>(index.data(PatternCollectionModel::PreviewImage));

    QImage scaled = img.scaled(ITEM_WIDTH - 2*ITEM_LETTERBOX, ITEM_HEIGHT - 2*ITEM_LETTERBOX);

//    QStyledItemDelegate::paint(painter, option, index);


    // If this pattern is active, draw it as selected

    QColor frameColor = COLOR_FRAME_UNSELECTED;

    if(option.state & QStyle::State_Selected) {
        painter->fillRect(option.rect, COLOR_BACKGROUND_CHECKED);
        frameColor = COLOR_FRAME_SELECTED;
    }

    // Draw the frame
    painter->fillRect(QRect(option.rect.x()+ITEM_LETTERBOX-FRAME_SIZE,
                            option.rect.y()+ITEM_LETTERBOX-FRAME_SIZE,
                            scaled.width()+2*FRAME_SIZE,
                            scaled.height()+2*FRAME_SIZE), frameColor);


    // Draw the image
    painter->drawImage(option.rect.x()+ITEM_LETTERBOX, option.rect.y()+ITEM_LETTERBOX, scaled);

    painter->restore();
}

QSize PatternCollectionDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex & ) const {
    return QSize(ITEM_WIDTH, ITEM_HEIGHT);
}

