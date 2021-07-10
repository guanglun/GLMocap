
#include "MuItemCam.h"

MuItemCam::MuItemCam(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

void MuItemCam::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid()) {
        painter->save();
        QVariant var = index.data(Qt::UserRole+1);
        MuItemCamData itemData = var.value<MuItemCamData>();

        // item 矩形区域
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width()-1);
        rect.setHeight(option.rect.height()-1);

        QPainterPath path;
        path.moveTo(rect.topRight());
        path.lineTo(rect.topLeft());
        path.quadTo(rect.topLeft(), rect.topLeft());
        path.lineTo(rect.bottomLeft());
        path.quadTo(rect.bottomLeft(), rect.bottomLeft());
        path.lineTo(rect.bottomRight());
        path.quadTo(rect.bottomRight(), rect.bottomRight());
        path.lineTo(rect.topRight());
        path.quadTo(rect.topRight(), rect.topRight());

        // 鼠标悬停或者选中时改变背景色
        if (option.state.testFlag(QStyle::State_MouseOver)) {
            painter->setPen(QPen(QColor("#ebeced")));
            painter->setBrush(QColor("#ebeced"));
            painter->drawPath(path);
        }
        if (option.state.testFlag(QStyle::State_Selected)) {
            painter->setPen(QPen(QColor("#e3e3e5")));
            painter->setBrush(QColor("#e3e3e5"));
            painter->drawPath(path);
        }

        QRectF iconRect = QRect(rect.left()+5, rect.top()+5, 40, 40);
        QRectF singerRect = QRect(iconRect.right()+5, iconRect.top(), rect.width()-10-iconRect.width(), 20);
        QRectF songNbRect = QRect(singerRect.left(), singerRect.bottom()+5, rect.width()-10-iconRect.width(), 20);

        painter->drawImage(iconRect, QImage(QString(":/image/camera.png")));
        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Microsoft Yahei", 10));
        painter->drawText(singerRect, itemData.id);

        painter->setPen(QPen(Qt::gray));
        painter->drawText(songNbRect, itemData.status);

        painter->restore();
    }
}

QSize MuItemCam::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 50);
}
