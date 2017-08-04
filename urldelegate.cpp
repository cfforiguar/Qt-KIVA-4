#include "urldelegate.h"
/*
UrlDelegate::UrlDelegate(QObject *parent):QStyledItemDelegate(parent)
{
}
*/


void UrlDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,const QModelIndex &index) const
{

/*
    QString text = index.data(Qt::DisplayRole).toString();
    if (text.isEmpty())
        return;
*/

    painter->save();
    if (option.state & QStyle::State_MouseOver)
    {
        QFont font = option.font;
        font.setUnderline(true);
        painter->setFont(font);
        painter->setPen(option.palette.link().color());
    }
    painter->drawText(option.rect, Qt::AlignLeft | Qt::AlignVCenter,index.data(Qt::DisplayRole).toString());

    painter->restore();
}


QSize UrlDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    return QSize(20, 20);//QAbstractItemDelegate::sizeHint(option, index);
}


