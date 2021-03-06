#include "patterncollectionlistview.h"
#include "defaults.h"

#include <QDebug>
#include <QSettings>

PatternCollectionListView::PatternCollectionListView(QWidget *parent) :
    QListView(parent)
{
}

PatternCollectionListView::~PatternCollectionListView()
{
}

// void PatternCollectionListView::dragEnterEvent(QDragEnterEvent *event)
// {
//// If this drag is coming from inside, then it's a re-order
// if(event->source() == this) {
// event->acceptProposedAction();
// }

//// If this drag has URLs, then try to load them as new patterns
// if(event->mimeData()->hasUrls()) {
// event->acceptProposedAction();
// }
// }

// void PatternCollectionListView::dropEvent(QDropEvent *event)
// {
// if(event->source() == this) {
//// Note: Implement this by hand on Windows, to work around a bug in QT 5.4.1
// #if defined(Q_OS_WIN)
// if(event->mimeData()->formats().at(0) == "application/x-qabstractitemmodeldatalist") {
// QByteArray itemData =
// event->mimeData()->data("application/x-qabstractitemmodeldatalist");
// QDataStream stream(&itemData, QIODevice::ReadOnly);

// int row, column;
// QMap<int, QVariant> v;
// stream >> row >> column >> v;

//// Insert the item in an appropriate place
// QModelIndex dropIndex = indexAt(event->pos());

// if(dropIndex.row() == row) {
// return;
// }

// QListWidgetItem* item = takeItem(row);
// if(dropIndex.row() == -1) {
// addItem(item);
// }
// else {
// int destinationRow = dropIndex.row();

// if(dropIndex.row() > row) {
// destinationRow--;
// }
// insertItem(destinationRow, item);
// }
// setCurrentItem(item);
// }
// #else
// QListWidget::dropEvent(event);
// #endif
// }

//// If this drop has URLs, then try to load them as new patterns
//// TODO: Fix the data model and allow dropMimeData
// else if(event->mimeData()->hasUrls()) {
// QList<QUrl> droppedUrls = event->mimeData()->urls();
// int droppedUrlCnt = droppedUrls.size();
// for(int i = 0; i < droppedUrlCnt; i++) {
// QString localPath = droppedUrls[i].toLocalFile();
// QFileInfo fileInfo(localPath);

//// TODO: OS X Yosemite hack for /.file/id= references

// if(fileInfo.isFile()) {
//// Create a patternItem, and attempt to load the file
//// TODO: pass this to mainwindow for proper handling!
// QSettings settings;
// QSize displaySize;
// displaySize.setWidth(settings.value("Options/displayWidth", DEFAULT_FIXTURE_WIDTH).toUInt());
// displaySize.setHeight(settings.value("Options/displayHeight", DEFAULT_FIXTURE_HEIGHT).toUInt());

// Pattern* patternItem = new Pattern(displaySize, 1);

// if(!patternItem->load(fileInfo)) {
// continue;
// }

//// Insert the item in an appropriate place
// QModelIndex dropIndex = indexAt(event->pos());
// if(dropIndex.row() == -1) {
// addItem(patternItem);
// }
// else {
// insertItem(dropIndex.row(), patternItem);
// }
// setCurrentItem(patternItem);
// }
// }
// }
// }
