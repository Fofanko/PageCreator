#include "utils.h"

namespace utils {


void unordered_set_intersection(unordered_set_StyleRecord::iterator b1,
                                                      unordered_set_StyleRecord::iterator e1,
                                                      unordered_set_StyleRecord::iterator b2,
                                                      unordered_set_StyleRecord::iterator e2,
                                                      unordered_set_StyleRecord& out,
                                                      const std::vector<StyleRecord>& used) {
    while (!(b1 == e1)) {
        if( !used.empty() && std::find(used.begin(), used.end(), *b1) != used.end()){
            ++b1;
            continue;
        }
        if (!(std::find(b2, e2, *b1) == e2)) {
            out.insert(*b1);
        }
        ++b1;
    }
}

void ltrimString(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}

void rtrimString(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

void trimString(std::string &s) {
    ltrimString(s);
    rtrimString(s);
}

}


