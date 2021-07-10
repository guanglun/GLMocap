#ifndef MUITEMDELEGATE_H
#define MUITEMDELEGATE_H

#include <QPainter>
#include <QStyledItemDelegate>
#include "MuItemCamData.h"

class MuItemCam : public QStyledItemDelegate
{
public:
    MuItemCam(QObject *parent = nullptr);

    // painting
    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif
