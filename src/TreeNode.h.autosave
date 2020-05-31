#ifndef TREENODE_H
#define TREENODE_H

#include <list>
#include <assert.h>
#include <queue>
#include <vector>

#include <QtAlgorithms>

#include <QDebug>

template <class T>
class TreeNode
{
public:
    explicit TreeNode(const T &data, TreeNode<T>* parent = nullptr): m_data(data), m_parent(parent){}

    ~TreeNode() {
        qDeleteAll(m_childs);
    }

    // Add new child at the end of childs of this node
    void appendChild(TreeNode<T> *newChild)
    {
        m_childs.push_back(newChild);
    }

    // get pointer to child element
    TreeNode<T> *child(int row)
    {
        if (row < 0 || row >= static_cast<int>(m_childs.size())) {
            return nullptr;
        }
        // icrement i while we finde neded element

        int i = 0;
        for (auto itr = m_childs.begin(); itr!=m_childs.end(); ++itr, ++i) {
            if (i == row) return *itr;
        }
    }

    // get no-depth count children of this node
    int childCount() const
    {
        return m_childs.size();
    }

    // get positon of node in list of parent node
    int row() const
    {
        if (!m_parent)
            return 0;
        return m_parent->getChildrenPos(const_cast<TreeNode<T>*>(this));
    }

    // return child position in list of child
    int getChildrenPos(TreeNode<T> *child) const
    {
        int i = 0;
        for (auto itr = m_childs.begin(); itr!=m_childs.end(); itr++, i++) {
            if (*itr == child) return i;
        }
        return -1;
    }

    // get data from node
    T& data()
    {
        return m_data;
    }

    // get parent of this node
    TreeNode<T> *parent() const
    {
        return m_parent;
    }

    //
    int columnCount() const
    {
        return m_childs.size();
    }

    void setData(T newData) {
        m_data = newData;
    };


    bool insertChildren(int position, int count, int columns)
    {
        if (position < 0 || position > m_childs.size()) {
            return false;
        }

        for (int row = 0; row < count; ++row) {
            T data;
            auto item = new TreeNode<T>(data, this);
            m_childs.insert(m_childs.begin() + position, item);
        }

        return true;
    }


    bool removeChildren(int position, int count)
    {
        if (position < 0 || position + count > m_childs.size()) {
            return false;
        }

        for(int row = 0; row < count; ++row) {
            m_childs.erase(m_childs.begin() + position);
        }

        return true;
    }

    bool insertColumns(int position, int columns)
    {
        return true;
    }


    bool removeColumns(int position, int columns)
    {
        return true;
    }

    std::vector<TreeNode<T>*> getChilds(){
        return m_childs;
    }

private:
    // id for searching (be cool with gui work)
    int m_unicID;

    // data of node
    T m_data;

    // nodes parent
    TreeNode<T> *m_parent;

    // childs of this node
    std::vector<TreeNode<T>*> m_childs;

};

#endif // TREENODE_H
