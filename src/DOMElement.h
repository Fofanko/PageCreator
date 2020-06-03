#ifndef DOMELEMENT_H
#define DOMELEMENT_H

#include "utils.h"
#include "Tags.h"

#include <QDebug>

#include <string>
#include <list>


//---------------------Элмент DOM----------------------------
class DOMElement
{
public:
    DOMElement(int tagID = 1);

    std::string getTagAsString() const;
    int getTagID() const;
    void setTag(int tagID);

    void addStyle(const std::string &name, const std::string &value);
    utils::unordered_set_StyleRecord *getStyles();

    void addAttribute(const std::string &name, const std::string &value);
    utils::unordered_set_StyleRecord *getAttributes();
    std::string getText();

    std::string getAttributesAsString();

private:
    // Имя тэга
    int m_tagID;

    // id и class тега
    std::string m_id;
    std::string m_class;

    // множество стилей присущие данному тэгу
    utils::unordered_set_StyleRecord m_styles;
    utils::unordered_set_StyleRecord m_attributes;
};

#endif // DOMELEMENT_H
