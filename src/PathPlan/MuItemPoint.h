#ifndef MUITEMPOINT_H
#define MUITEMPOINT_H

#include <QPainter>
#include <QStyledItemDelegate>
#include "MuItemPointData.h"

class MuItemPoint : public QStyledItemDelegate
{
public:
    MuItemPoint(QObject *parent = nullptr);

    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif
