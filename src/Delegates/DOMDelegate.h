#ifndef DOMDELEGATE_H
#define DOMDELEGATE_H

#include <QDebug>

#include <QStyledItemDelegate>
#include <QComboBox>
#include "../DOMElement.h"

class DOMDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    DOMDelegate(QObject *parent = nullptr);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;


    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                          const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const override;

};

#endif // DOMDELEGATE_H
