#include "urldelegate.h"

UrlDelegate::UrlDelegate(QObject *parent):QAbstractItemDelegate(parent)
{
}

void paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index)
{

/*
    QString text = index.data(Qt::DisplayRole).toString();
    if (text.isEmpty())
        return;

    painter->save();
*/
    if (option.state & QStyle::State_MouseOver)
    {
        QFont font = option.font;
        font.setUnderline(true);
        painter->setFont(font);
        painter->setPen(option.palette.link().color());
    }
    painter->drawText(option.rect, Qt::AlignLeft | Qt::AlignVCenter, QString("ebrio"));

    painter->restore();
}

QSize sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index)
{
    return QSize(20, 20);//QAbstractItemDelegate::sizeHint(option, index);
}

