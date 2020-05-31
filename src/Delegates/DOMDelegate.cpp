#include "../Delegates/DOMDelegate.h"

DOMDelegate::DOMDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{

}

QWidget *DOMDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    qDebug() << "parent: " << parent << " QStyleOptionViewItem: " << option << " QModelIndex: " << index;
    QComboBox *editor = new QComboBox(parent);
    std::vector<Tag> tags = Tags::getInstance()->getAllTags();
    for(Tag tag: tags){
        editor->addItem(tag.second.c_str(), tag.first);
    }
    return editor;
}

void DOMDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    const int tagID = index.model()->data(index, Qt::EditRole).toInt();
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    comboBox->setCurrentIndex(tagID);
}

void DOMDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    int tagID = comboBox->currentIndex();

    model->setData(index, QVariant(tagID), Qt::EditRole);
}

void DOMDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
