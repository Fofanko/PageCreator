#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <cctype>
#include <locale>

#include <string>
#include <unordered_set>
#include <algorithm>
#include <vector>
#include <iterator>

#include <QDebug>

namespace utils {


//------------------------Стиль---------------------------
struct StyleRecord {
    std::string m_name; // Имя атрибута
    std::string m_value; // Значение
    StyleRecord(const std::string &name, const std::string &value):
        m_name(name), m_value(value) {};

    bool operator==(const StyleRecord& rec) const{
        return (this->m_name == rec.m_name && this->m_value == rec.m_value);
    }

};

struct HashStyleRecordFunc {
public:
    size_t operator()(const StyleRecord & record) const {
        int size = record.m_name.length() + record.m_value.length();
        return std::hash<int>()(size);
    }
};

typedef std::unordered_set<StyleRecord, HashStyleRecordFunc> unordered_set_StyleRecord;
//----------------------------------------------------------

struct TagStyleClass {
    std::string className;
    unordered_set_StyleRecord styles;
    TagStyleClass(const std::string& _name, unordered_set_StyleRecord _styles): className(_name), styles(_styles){};
    TagStyleClass(const std::string& _name): className(_name){};
};

void unordered_set_intersection(unordered_set_StyleRecord::iterator b1,
                                                      unordered_set_StyleRecord::iterator e1,
                                                      unordered_set_StyleRecord::iterator b2,
                                                      unordered_set_StyleRecord::iterator e2,
                                                      unordered_set_StyleRecord& out,
                                                      const std::vector<StyleRecord>& used);


void ltrimString(std::string &s);

void rtrimString(std::string &s);

void trimString(std::string &s);


}



#endif // UTILS_H
