#include "StateMachine.h"

namespace dsm {

StateMachine::StateMachine()
{
    refreshMachine();

    // set settings for state machine
    m_beginState.insert(STATE::BEGIN);


    m_endState.insert(STATE::TAG_ATTR);
    m_endState.insert(STATE::ATTR_VAL);
    m_endState.insert(STATE::EQ);

    m_NonTerminal.insert('\n');
    m_NonTerminal.insert('\t');
    m_NonTerminal.insert('\r');
    m_NonTerminal.insert(' ');
    m_NonTerminal.insert('\0');

    std::vector<char> alphabet;
    std::vector<char> numeric{'0','1','2','3','4','5','6','7','8','9'};
    std::vector<char> extra{'_',';', ' '};
    for(char i = 'a'; i<='z'; ++i) alphabet.push_back(i);
    for(char i = 'A'; i<='Z'; ++i) alphabet.push_back(i);

    // from begin to tag_attr
    for (char sym: alphabet) m_translationTable.insert(outputSignal(inputSignal(STATE::BEGIN, sym), STATE::TAG_ATTR));
    // from tag_attr to tag_attr
    for (char sym: alphabet) m_translationTable.insert(outputSignal(inputSignal(STATE::TAG_ATTR, sym), STATE::TAG_ATTR));
    // from begin to eq
    m_translationTable.insert(outputSignal(inputSignal(STATE::BEGIN, '='), STATE::EQ));
    // from begin to b_attr_val
    m_translationTable.insert(outputSignal(inputSignal(STATE::BEGIN, '"'), STATE::B_ATTR_VAL));
    m_translationTable.insert(outputSignal(inputSignal(STATE::BEGIN, '\''), STATE::B_ATTR_VAL));
    // from b_attr_val to attr_val
    for (char sym: alphabet) m_translationTable.insert(outputSignal(inputSignal(STATE::B_ATTR_VAL, sym), STATE::ATTR_VAL));
    for (char sym: numeric) m_translationTable.insert(outputSignal(inputSignal(STATE::B_ATTR_VAL, sym), STATE::ATTR_VAL));
    for (char sym: extra) m_translationTable.insert(outputSignal(inputSignal(STATE::B_ATTR_VAL, sym), STATE::ATTR_VAL));
    // from attr_val to attr_val
    for (char sym: alphabet) m_translationTable.insert(outputSignal(inputSignal(STATE::ATTR_VAL, sym), STATE::ATTR_VAL));
    for (char sym: numeric) m_translationTable.insert(outputSignal(inputSignal(STATE::ATTR_VAL, sym), STATE::ATTR_VAL));
    for (char sym: extra) m_translationTable.insert(outputSignal(inputSignal(STATE::ATTR_VAL, sym), STATE::ATTR_VAL));
    // from attr_val to e_attr_val
    m_translationTable.insert(outputSignal(inputSignal(STATE::ATTR_VAL, '"'), STATE::E_ATTR_VAL));
    m_translationTable.insert(outputSignal(inputSignal(STATE::ATTR_VAL, '\''), STATE::E_ATTR_VAL));
}

void StateMachine::refreshMachine()
{
    m_curState = STATE::BEGIN;
    m_buffer.clear();
}

void StateMachine::step(char symbol)
{   
    auto new_state = m_translationTable.find(inputSignal(m_curState, symbol));
    bool isNon = m_NonTerminal.find(symbol) != m_NonTerminal.end();
    bool isNewState = new_state != m_translationTable.end();
    bool isEndState = m_endState.find(m_curState) != m_endState.end();

    if (m_curState == STATE::BEGIN && isNon){
        return;
    }

    if ( (!isNewState && isNon) ) {
        createToken();
        refreshMachine();
        return;
    } else if (isEndState && !isNewState) {
        createToken();
        refreshMachine();
        step(symbol);
        return;
    } else if (isNewState) {
        m_buffer += symbol;
        m_curState = new_state->second;
        return;
    }
    refreshMachine();
    qDebug() << "fail";

}


std::list<Token> *StateMachine::getMemory()
{
    return &m_memory;
}

void StateMachine::createToken()
{
    if (m_curState == STATE::TAG_ATTR) {
        if (Tags::getInstance()->isTag(m_buffer)) {
            m_memory.push_back(Token(m_buffer, TYPE::T_TAG));
        } else {
            m_memory.push_back(Token(m_buffer, TYPE::T_ATTR_NAME));
        }
    } else if(m_curState == STATE::E_ATTR_VAL) {
        m_memory.push_back(Token(m_buffer, TYPE::T_ATTR_VAL));
    } else if(m_curState == STATE::EQ) {
        m_memory.push_back(Token(m_buffer, TYPE::T_EQ));
    }
}

}

