#include "PageTree.h"

PageTree::PageTree(const DOMElement &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    m_root = new HTMLNode(data);
    DOMElement *div = new DOMElement(Tags::getInstance()->getIdByString("html"));
    HTMLNode *beginNode = new HTMLNode(*div, m_root);
    m_root->appendChild(beginNode);

//    HTMLNode *test2Node = new HTMLNode(DOMElement(1), testNode);
//    test2Node->data().addStyle("margin", "4px");
//    test2Node->data().addStyle("float", "left");
//    testNode->appendChild(test2Node);

//    test2Node = new HTMLNode(DOMElement(2), testNode);
//    test2Node->data().addStyle("margin", "4px");
//    testNode->appendChild(test2Node);

//    HTMLNode *test3Node = new HTMLNode(DOMElement(3), test2Node);
//    test2Node->appendChild(test3Node);
}

PageTree::~PageTree()
{
    delete m_root;
}

Qt::ItemFlags PageTree::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    
    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

QVariant PageTree::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section)
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return QVariant();
    }
    return QVariant();
}

QModelIndex PageTree::index(int row, int column, const QModelIndex &parent) const
{
    if ( !hasIndex(row, column, parent) )
        return QModelIndex();

    HTMLNode *parentNode;

    if(!parent.isValid())
        parentNode = m_root;
    else
        parentNode = static_cast<HTMLNode*>(parent.internalPointer());

    HTMLNode* childNode = parentNode->child(row);
    if (childNode) {
        return createIndex(row, column, childNode);
    }
    return QModelIndex();
}

QModelIndex PageTree::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    HTMLNode *childrenNode = static_cast<HTMLNode*>(index.internalPointer());
    HTMLNode *parentNode = childrenNode->parent();

    if (parentNode == m_root)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int PageTree::rowCount(const QModelIndex &parent) const
{
    HTMLNode *parentNode;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentNode = m_root;
    else
        parentNode = static_cast<HTMLNode*>(parent.internalPointer());

    return parentNode->childCount();
}

int PageTree::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return static_cast<HTMLNode*>(parent.internalPointer())->columnCount();
    }
    return 1;
}

bool PageTree::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!(index.isValid() && role == Qt::EditRole)) {
        return false;
    }

    HTMLNode *node = static_cast<HTMLNode*>(index.internalPointer());

    // TODO
    node->data().setTag( value.toInt() );
    
    return true;
}

bool PageTree::insertColumns(int position, int columns, const QModelIndex &parent)
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = m_root->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool PageTree::insertRows(int position, int rows, const QModelIndex &parent)
{
    HTMLNode *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginInsertRows(parent, position, position + rows - 1);
    const bool success = parentItem->insertChildren(position,
                                                    rows,
                                                    m_root->columnCount());
    endInsertRows();

    return success;
}

bool PageTree::removeColumns(int position, int columns, const QModelIndex &parent)
{
    beginRemoveColumns(parent, position, position + columns - 1);
    const bool success = m_root->removeColumns(position, columns);
    endRemoveColumns();

    if (m_root->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool PageTree::removeRows(int position, int rows, const QModelIndex &parent)
{
    HTMLNode *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

HTMLNode *PageTree::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        HTMLNode *item = static_cast<HTMLNode*>(index.internalPointer());
        if (item)
            return item;
    }
    return m_root;
}

BFSIterator PageTree::begin()
{
    return  BFSIterator(m_root);
}

BFSIterator PageTree::end()
{
    HTMLNode* cur = m_root;
    while (cur->childCount()) {
        cur = cur->getChilds().back();
    }
    return BFSIterator(cur);
}

std::string PageTree::getSerializeData()
{
    std::vector<utils::StyleRecord> usedStyles;
    utils::unordered_set_StyleRecord allStyles;
    std::map<HTMLNode*, std::vector<utils::TagStyleClass*>> styleMap;

    for(BFSIterator it = begin(); it != end(); ++it) {
        for(utils::StyleRecord record: *it.getCurrent()->data().getStyles()) {
            allStyles.insert(record);
        }
    }

    std::vector<utils::TagStyleClass*> styleClasses;

    while (true) {
        utils::TagStyleClass *tagStyleClass = new utils::TagStyleClass("style_" + std::to_string(styleClasses.size() ));
        utils::unordered_set_StyleRecord interseption;
        std::copy(allStyles.begin(), allStyles.end(), std::inserter(interseption, interseption.begin()));
        for(BFSIterator it = begin(); it != end(); ++it) {
            utils::unordered_set_StyleRecord tmp;
            utils::unordered_set_StyleRecord *cur = it.getCurrent()->data().getStyles();
            utils::unordered_set_intersection(interseption.begin(), interseption.end(),
                                              cur->begin(), cur->end(),
                                              tmp, usedStyles);
            if(!tmp.empty()){
                interseption = tmp;
                styleMap[it.getCurrent()].push_back(tagStyleClass);
            }

        }
        if (allStyles.size() == usedStyles.size()) {
            delete tagStyleClass;
            break;
        }
        styleClasses.push_back(tagStyleClass);
        styleClasses.back()->styles = interseption;
        for(utils::StyleRecord record :interseption) {
            usedStyles.push_back(record);
        }
    }

    std::string htmlCode = getSerializeData(m_root, 0, styleMap);

    std::string cssCode = "<style>\n";
    for (const utils::TagStyleClass *styleClass: styleClasses){
        cssCode += "." + styleClass->className + "{\n";
        for (utils::StyleRecord record : styleClass->styles){
            cssCode += "\t" + record.m_name + ": " + record.m_value + ";";
        }
        cssCode += "\n}\n\n";
    }
    cssCode += "</style>";

    qDebug() << (htmlCode + cssCode).c_str();
    return htmlCode + cssCode;
}



void PageTree::test() {
    int c=0;
    for(BFSIterator it = begin(); it != end(); ++it) {
        c++;
    }
    qDebug() << "count is " << c;
}



std::string PageTree::getSerializeData(HTMLNode *node, unsigned level, const std::map<HTMLNode*, std::vector<utils::TagStyleClass*>> styleMap)
{
    std::string data;
    std::string tabs;
    std::string style;
    for(unsigned i=0; i < level; ++i) tabs+="\t";
    auto styleMapIt = styleMap.find(node);
    if( styleMapIt != styleMap.end() ){
        for(const utils::TagStyleClass *styleClass : styleMapIt->second){
            style += styleClass->className + " ";
        }
    }
    data += tabs + "<" +
            node->data().getTagAsString() + " " +
            (!style.empty()?(" style='" + style + "'"):std::string(""))
            + node->data().getAttributesAsString() + ">\n";
    for( HTMLNode* child: node->getChilds() ) {
        data += getSerializeData(child, level + 1, styleMap);
    }
    data += tabs + "</" + node->data().getTagAsString() + ">\n";
    return data;
}

QVariant PageTree::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }


    HTMLNode *node = static_cast<HTMLNode*>(index.internalPointer());
    if (role == Qt::EditRole){
        return QVariant(node->data().getTagID());
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    return QVariant( node->data().getTagAsString().c_str() );
}

void PageTree::readFromString(const std::string& stream)
{
    std::vector<dsm::CommonToken> baseStream;
    bool open_tag = false;
    bool check_sl = false;
    std::string buffer;

    for(char ch: stream) {
        if (check_sl){
            if( ch == '/' ){
                open_tag = false;
                check_sl = false;
                continue;
            } else {
                open_tag = true;
                check_sl = false;
            }
        }

        if (ch == '>') {
            utils::trimString(buffer);
            if (open_tag)
                baseStream.push_back(dsm::CommonToken(buffer, dsm::BASE_TYPE::OPEN_TAG));
            else
                baseStream.push_back(dsm::CommonToken(buffer, dsm::BASE_TYPE::CLOSE_TAG));
            buffer.clear();
            continue;
        }

        if (ch == '<') {
            check_sl = true;
            utils::trimString(buffer);
            if ( buffer.length() ){
                baseStream.push_back(dsm::CommonToken(buffer, dsm::BASE_TYPE::TEXT));
                buffer.clear();
            }
            continue;
        }
        buffer += ch;
    }



    HTMLNode *root = new HTMLNode(DOMElement());
    std::stack<HTMLNode*> tagStack;
    tagStack.push(root);
    setRoot(root);



    for(dsm::CommonToken comTok: baseStream) {
        if(comTok.type == dsm::BASE_TYPE::OPEN_TAG){
            // open
            dsm::StateMachine stateMachine;
            for(char ch: comTok.data) {
                stateMachine.step(ch);
            }
            stateMachine.step('\0');
            std::list<dsm::Token> *tokenStream = stateMachine.getMemory();
            auto it = tokenStream->begin();
            if (it->type != dsm::TYPE::T_TAG){
                qDebug() << "Unexecpted tag";
                return;
            }
            qDebug() << "Add " << it->data.c_str();
            HTMLNode *node = new HTMLNode(DOMElement( Tags::getInstance()->getIdByString(it->data) ), tagStack.top());
            tagStack.top()->appendChild(node);
            // add attribute
            bool getName = false; it++;
            while( it!=tokenStream->end() ){
                std::string name, value;
                if (it->type == dsm::TYPE::T_ATTR_NAME){
                    name = it->data; it++;
                } else { qDebug() << "Unexecpted attribute " << it->data.c_str(); return; }
                if ( it!=tokenStream->end() && it->type == dsm::TYPE::T_EQ ) {
                    value = (++it)->data;
                    node->data().addAttribute(name, value);
                } else {
                    node->data().addAttribute(name, "");
                }
                it++;
            }

            tagStack.push(node);

        } else if (comTok.type == dsm::BASE_TYPE::TEXT) {
            // text
        } else {
            dsm::StateMachine stateMachine;
            for(char ch: comTok.data) {
                stateMachine.step(ch);
            }
            stateMachine.step('\0');
            std::list<dsm::Token> *tokenStream = stateMachine.getMemory();
            auto it = tokenStream->begin();
//            if (it->type != dsm::TYPE::T_TAG || it->data.c_str() != tagStack.top()->data().getTagAsString().c_str()) {
//                qDebug() << "Unexecpted closed tag, should be a '" << tagStack.top()->data().getTagAsString().c_str() <<
//                            "'. but expected '" << it->data.c_str() << "'";
//                return;
//            }
            tagStack.pop();
        }
    }

    qDebug() << stream.c_str();

}

void PageTree::setRoot(HTMLNode *const node)
{
    m_root = node;
}


std::istream& operator>>(std::istream &input, PageTree &pageTree)
{

    return input;
}

std::ostream& operator<<(std::ostream &output, PageTree &pageTree)
{
    output << pageTree.getSerializeData().c_str();
    return output;
}
