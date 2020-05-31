#ifndef ATTRIBUTEMODEL_H
#define ATTRIBUTEMODEL_H

#include <QString>
#include <QAbstractListModel>
#include <QList>
#include <vector>
#include <unordered_set>
#include "../DOMElement.h"


template <class Container, class Element>
class AttributeModel : public QAbstractListModel
{
public:
    explicit AttributeModel(Container *attributes, QObject *parent = 0) {
        m_attributes = attributes;
        m_cache = nullptr;
    }

    ~AttributeModel() {
        m_attributes = nullptr;
        if (m_cache)
            delete m_cache;
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override{
        if (!index.isValid())
            return Qt::NoItemFlags;

        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        return m_attributes->size();
    }

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override {
        return QVariant();
    }

    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid())
            return QVariant();

        if (index.row() >= static_cast<int>(m_attributes->size()))
            return QVariant();

        if (role != Qt::DisplayRole) {
            return QVariant();
        }

        auto record = std::next(m_attributes->begin(), index.row());
        QList<QVariant> data = { QVariant(record->m_name.c_str()), QVariant(record->m_value.c_str()) };
        return QVariant( (record->m_name + std::string(": ") + record->m_value).c_str() );
    }


    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override {
        if (!m_cache)
            return false;

        beginInsertRows(parent, 0, rowCount());
        m_attributes->insert(*m_cache);
        endInsertRows();

        return true;
    }

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override {
        if ( row < 0 && row >= rowCount() ) {
            return false;
        }

        auto record = std::next(m_attributes->begin(), row);
        beginRemoveRows(parent, 0, rowCount());
        m_attributes->erase(*record);
        endRemoveRows();

        return true;
    }

    void cachedDataForNewItem(const QString &name, const QString &val) {
        if (m_cache)
            delete m_cache;
        qDebug() << "aaa";
        m_cache = new Element(name.toStdString(), val.toStdString());
    }

private:
    Container *m_attributes;

    Element *m_cache;
};

#endif // ATTRIBUTEMODEL_H
