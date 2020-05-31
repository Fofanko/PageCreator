#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Tags.h"

#include <vector>
#include <list>
#include <set>
#include <map>
#include <string>
#include <QDebug>

namespace dsm {

enum STATE { BEGIN = 0, TAG_ATTR, EQ, B_ATTR_VAL, ATTR_VAL, E_ATTR_VAL };

enum TYPE { T_TAG, T_ATTR_NAME, T_ATTR_VAL, T_EQ };

enum BASE_TYPE { OPEN_TAG, CLOSE_TAG, TEXT };

struct CommonToken {
    std::string data;
    BASE_TYPE type;
    CommonToken (const std::string &indata, BASE_TYPE intype): data(indata), type(intype) {};
};

struct Token {
    std::string data;
    TYPE type;
    Token (const std::string &indata, TYPE intype): data(indata), type(intype) {};
};

typedef std::pair<STATE, char> inputSignal;
typedef std::pair<inputSignal, STATE> outputSignal;

class StateMachine
{
public:
    StateMachine();

    void refreshMachine();

    void step(char symbol);

    std::list<Token> *getMemory();

private:
    STATE m_curState;
    std::string m_buffer;

    // set of begin state
    std::set<STATE> m_beginState;
    // set of end state
    std::set<STATE> m_endState;
    // set of non-terminal symbol
    std::set<char> m_NonTerminal;
    // set translation map
    std::map<inputSignal, STATE> m_translationTable;

    // output stream
    std::list<Token> m_memory;

    void createToken();
};

}



#endif // STATEMACHINE_H
