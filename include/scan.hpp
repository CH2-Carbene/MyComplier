#pragma once
#include "base.hpp"
namespace basic_scan{
template<typename C>
class ScanDataLoader{
    std::ifstream&fin;
    int nowpos;
    /**
     * @brief Construct a new Scan Data Loader object, based on a input file stream
     * 
     * @param _fin 
     */
    ScanDataLoader(std::ifstream&_fin);
  public:
    /**
     * @brief try to get next character
     * 
     * @return C 
     */
    C try_get();
    /**
     * @brief having found a token, lock down 
     * 
     */
    void lock();
    /**
     * @brief rollback to last token
     * 
     */
    void rollback();
};

template<class Config>
class DFA{
    class DFA_Node;
    using C=typename Config::C;
    using State=typename Config::State;
    using Token=std::pair<State,std::string>;
    using ptrNode=std::unique_ptr<DFA_Node>;
    class DFA_Node{
        using Checker=std::function<bool(C)>;
        using ptrNext=DFA_Node*;
        using Rule=std::pair<Checker,ptrNext>;
        std::vector<Rule>ruleList;
        
      public:
        State st=State::NIL;
        /**
         * @brief Get the next DFA state according to a character
         * 
         * @param c 
         * @return DFA_Node* 
         */
        DFA_Node* get_next(const C&c);
        
    };
    
    std::vector<ptrNode>stateList;
    /**
     * @brief insert a string to DFA, only used when building DFA
     * 
     * @param t 
     */
    void ins_string(const Token&t);
    
  public:
    DFA();
    /**
     * @brief Get the token from loader
     * 
     * @param dl 
     * @return Token 
     */
    Token get_token(ScanDataLoader<C>&dl)const;
    
};
}

namespace c0_scan{

struct C0config{
    using C=char;
    enum State{
        NIL,ERR,NUM,ID,IF,ELSE,FOR,WHILE,SET,EQ,GQ,LQ
    };
    using ScanList=std::unordered_map<std::string,State>;
    static ScanList reservedList;
    static ScanList signs;
};

class C_DFA:public basic_scan::DFA<C0config>{
  public:
    C_DFA();
};
}
