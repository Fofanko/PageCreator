#include "Tags.h"

Tags* Tags::p_instance = 0;

std::vector<std::string> Tags::getAllTagsAsStr()
{
    std::vector<std::string> tags;
    for (auto tag: m_table) {
        tags.emplace_back(tag.second);
    }
    return tags;
}

std::vector<Tag> Tags::getAllTags()
{
    std::vector<Tag> tags;
    for (auto tag: m_table) {
        tags.emplace_back(Tag(tag.first, tag.second));
    }
    return tags;
}

std::string Tags::getTagByID(int id)
{
    return m_table.at(id);
}

int Tags::getIdByString(const std::string &intag)
{
    for (auto tag: m_table) {
        if (tag.second == intag)
            return tag.first;
    }
    return 0;
}

bool Tags::isTag(const std::string &str)
{
    std::vector<std::string> tags = getAllTagsAsStr();
    return std::find(tags.begin(), tags.end(), str) != tags.end();
}

Tags::Tags()
{
    QFile file("./assets/tags.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Not found tag file";
        return;
    }
    int id = 0;
    while (!file.atEnd()) {
        std::string line = file.readLine().toStdString();
        utils::trimString(line);
        m_table.insert(std::pair<int, std::string>(id++, line ));
    }
}

