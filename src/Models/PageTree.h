#ifndef PAGETREE_H
#define PAGETREE_H

#include <cstdio>

#include <string>
#include <stack>
#include <queue>
#include <map>

#include <QAbstractItemModel>
#include <QDebug>

#include "../TreeNode.h"
#include "../StateMachine.h"

#include "../DOMElement.h"

#include "../utils.h"

typedef TreeNode<DOMElement> HTMLNode;

class BFSIterator;

class PageTree: public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit PageTree(const DOMElement &data, QObject *parent = nullptr);
    ~PageTree();

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool setData(const QModelIndex &index, const QVariant &value,
                     int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    HTMLNode* getItem(const QModelIndex &index) const;

    BFSIterator begin();
    BFSIterator end();

    std::pair<std::string, std::string> getSerializeData();
    std::string getSerializeData(HTMLNode* node, unsigned level, std::map<HTMLNode*, std::vector<utils::TagStyleClass*>> styleMap);

    void readFromString(const std::string& stream);

private:
    void setupModelData(HTMLNode *parent);

    void setRoot(HTMLNode* const node);

private:

    HTMLNode *m_root;
};


class BFSIterator
{
public:
    BFSIterator(HTMLNode* root)
    {
        m_currentNode = root;
        for(HTMLNode* children: m_currentNode->getChilds()) {
            m_bfsQueue.push(root);
        }
    }

    void next()
    {
        if ( !m_bfsQueue.empty() ){
            m_currentNode = m_bfsQueue.front();
            m_bfsQueue.pop();
            for(HTMLNode* children: m_currentNode->getChilds()) {
                m_bfsQueue.push(children);
            }
        }
    }

    BFSIterator operator++() {
        this->next();
        return *this;
    }

    BFSIterator operator++(int) {
        this->next();
        return *this;
    }

    bool operator==(const BFSIterator &rgt)
    {
        return m_currentNode == rgt.m_currentNode;
    }

    bool operator!=(const BFSIterator &rgt)
    {
        return m_currentNode != rgt.m_currentNode;
    }

    HTMLNode* getCurrent() {
        return m_currentNode;
    }

private:
    std::queue<HTMLNode*> m_bfsQueue;
    std::queue<HTMLNode*> m_walkChild;
    HTMLNode* m_currentNode;
};


std::istream& operator>>(std::istream &input, PageTree *pageTree);

std::ostream& operator<<(std::ostream &output, PageTree &pageTree);

#endif // PAGETREE_H
