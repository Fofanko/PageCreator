#ifndef STYLEDELEGATE_H
#define STYLEDELEGATE_H

#include <QDebug>

#include <QStyledItemDelegate>
#include <QLineEdit>
#include <QFrame>

#include <QHBoxLayout>

#include "src/Widgets/StyleWidget.h"

class StyleDelegate: public QStyledItemDelegate{
    Q_OBJECT
public:
    using QStyledItemDelegate::QStyledItemDelegate;
//    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const{
//        Q_UNUSED(painter)
//        Q_UNUSED(option)
//        Q_UNUSED(index)
//    }
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const{
        Q_UNUSED(option)
        Q_UNUSED(index)
        StyleWidget *editor = new StyleWidget(parent);
        return editor;
    }
    void setEditorData(QWidget *editor, const QModelIndex &index) const{

    }
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const{

    }

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const{
        Q_UNUSED(index)
        editor->setGeometry(option.rect);
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QSize s =  index.data(Qt::SizeHintRole).toSize();



        // Find correct way to return editor size
        return QSize(60, 50);

        return s.isValid() ? s: QStyledItemDelegate::sizeHint(option, index);
    }
};


#endif // STYLEDELEGATE_H
