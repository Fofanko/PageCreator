#include "DOMElement.h"

DOMElement::DOMElement(int tagID): m_tagID(tagID)
{

}

std::string DOMElement::getTagAsString() const
{
    return Tags::getInstance()->getTagByID(m_tagID);
}

int DOMElement::getTagID() const
{
    return m_tagID;
}

void DOMElement::setTag(int tagID)
{
    m_tagID = tagID;
}

void DOMElement::addStyle(const std::string &name, const std::string &value)
{
    if (m_styles.insert(utils::StyleRecord(name, value)).second == false){
        //raise error
    }
}

utils::unordered_set_StyleRecord *DOMElement::getStyles()
{
    return &m_styles;
}

void DOMElement::addAttribute(const std::string &name, const std::string &value)
{
    if (m_attributes.insert(utils::StyleRecord(name, value)).second == false){
        //raise error
    }
}

utils::unordered_set_StyleRecord *DOMElement::getAttributes()
{
    return &m_attributes;
}

std::string DOMElement::getText()
{
    for(utils::StyleRecord record: m_attributes){
        if (record.m_name == "text")
            return record.m_value;
    }
    return std::string("");
}

std::string DOMElement::getAttributesAsString()
{
    std::string data;
    for (utils::StyleRecord attr: m_attributes ) {
        if (attr.m_name != "text")
            data += attr.m_name + "='" + attr.m_value + "' ";
    }
    return data;
}
