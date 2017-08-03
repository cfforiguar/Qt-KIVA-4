#ifndef URLDELEGATE_H
#define URLDELEGATE_H

#include <QAbstractItemDelegate>
#include <QtWidgets>
//Pinta, genera y abre las URL de las palabras clave de KIVA
//Basado en los ejemplos "spinboxdelegate" y "stardelegate"
// y basado en  https://stackoverflow.com/questions/2375763/how-to-open-an-url-in-a-qtableview
class UrlDelegate : public QAbstractItemDelegate
{
     Q_OBJECT
public:
    UrlDelegate(QObject * parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const override;
};

#endif // URLDELEGATE_H
