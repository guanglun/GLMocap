#include "MuItemPoint.h"

MuItemPoint::MuItemPoint(QObject *parent) : QStyledItemDelegate(parent)
{
}

void MuItemPoint::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.isValid())
    {
        painter->save();
        QVariant var = index.data(Qt::UserRole + 1);
        MuItemPointData itemData = var.value<MuItemPointData>();

        // item 矩形区域
        QRectF rect;
        rect.setX(option.rect.x());
        rect.setY(option.rect.y());
        rect.setWidth(option.rect.width() - 1);
        rect.setHeight(option.rect.height() - 1);

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
        if (option.state.testFlag(QStyle::State_MouseOver))
        {
            painter->setPen(QPen(QColor("#ebeced")));
            painter->setBrush(QColor("#ebeced"));
            painter->drawPath(path);
        }
        if (option.state.testFlag(QStyle::State_Selected))
        {
            painter->setPen(QPen(QColor("#e3e3e5")));
            painter->setBrush(QColor("#e3e3e5"));
            painter->drawPath(path);
        }

        QRectF iconRect = QRect(rect.left() + 5, rect.top() + 5, 60, 60);

        QRectF xRect = QRect(iconRect.right() + 5, iconRect.top(), (rect.width() - 10 - iconRect.width()) / 3, 20);
        QRectF yRect = QRect(xRect.right() + 5, xRect.top(), (rect.width() - 10 - iconRect.width()) / 3, 20);
        QRectF zRect = QRect(yRect.right() + 5, yRect.top(), (rect.width() - 10 - iconRect.width()) / 3, 20);

        QRectF typeRect = QRect(xRect.left(), xRect.bottom(), (rect.width() - 10 - iconRect.width()) / 2, 20);
        QRectF waitTimeRect = QRect(typeRect.right(), xRect.bottom(), (rect.width() - 10 - iconRect.width()) / 2, 20);

        QRectF statusRect = QRect(typeRect.left(), typeRect.bottom(), rect.width() - 10 - iconRect.width(), 20);

        painter->drawImage(iconRect, QImage(QString(":/image/position.jpg")));

        painter->setPen(QPen(Qt::black));
        painter->setFont(QFont("Microsoft Yahei", 9));

        // mlog->show("pos : " +
        //            QString::number(itemData.x, 'f', 2) + "\t" +
        //            QString::number(itemData.y, 'f', 2) + "\t" +
        //            QString::number(itemData.z, 'f', 2));

        painter->drawText(xRect, QString::number(itemData.x, 'f', 2));
        painter->drawText(yRect, QString::number(itemData.y, 'f', 2));
        painter->drawText(zRect, QString::number(itemData.z, 'f', 2));
        painter->drawText(typeRect, itemData.typeString);
        painter->drawText(waitTimeRect, QString::number(itemData.waitTime));

        painter->setPen(QPen(Qt::gray));
        painter->drawText(statusRect, itemData.stateString);

        painter->restore();
    }
}

QSize MuItemPoint::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index)
    return QSize(option.rect.width(), 70);
}
