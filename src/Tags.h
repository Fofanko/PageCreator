#ifndef TAGS_H
#define TAGS_H

#include <string>
#include <map>
#include <vector>

#include <QFile>
#include <QDebug>

#include "utils.h"

typedef std::pair<int, std::string> Tag;

class Tags
{
public:
    static Tags* getInstance() {
        if(!p_instance)
            p_instance = new Tags();
        return p_instance;
    }

    std::vector<std::string> getAllTagsAsStr();
    std::vector<Tag> getAllTags();

    std::string getTagByID(int id);
    int getIdByString(const std::string &intag);

    bool isTag(const std::string& str);

private:
    std::map<int, std::string>  m_table;

    static Tags * p_instance;
    Tags();
    Tags( const Tags& );
    Tags& operator=( Tags& );
};

#endif // TAGS_H
