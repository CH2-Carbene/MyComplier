#pragma once
#include "base.hpp"
namespace basic_scan{

template<typename C>class ScanDataLoader;
template<class Config>class DFA;

template<typename C>
class ScanDataLoader{
    std::ifstream&fin;
    int nowpos;
    /**
     * @brief Construct a new Scan Data Loader object, based on a input file stream
     * 
     * @param _fin 
     */

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
        ScanDataLoader(std::ifstream&_fin);
};

template<class Config>
class DFA{
    
    class DFA_Node;
    friend Config;
    using C=typename Config::C;
    using State=typename Config::State;
    using Token=typename Config::Token;

    using ptrNode=std::unique_ptr<DFA_Node>;
    using tmpNode=DFA_Node*;
    struct DFA_Node{
        using Checker=std::function<bool(C)>;
        using Rule=std::pair<Checker,tmpNode>;
        std::vector<Rule>ruleList;
        State st=State::NIL;
        /**
         * @brief Get the next DFA state according to a character
         * 
         * @param c 
         * @return tmpNode 
         */
       tmpNode get_next(const C&c);
        
    };
    /**
     * @brief Root node of DFA
     * 
     */
    tmpNode rt;
    /**
     * @brief State list of DFA
     * 
     */
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
struct C0config;
using C_DFA=basic_scan::DFA<C0config>;

struct C0config{
    using C=char;
    enum State{
        NIL,ERR,NUM,ID,IF,ELSE,FOR,WHILE,SET,EQ,GQ,LQ,E,G,L,LE,GE,BL,BR,MBL,MBR,GBL,GBR
    };
    static char const* sname[];
    using Token=std::pair<State,std::string>;
    using ScanList=std::vector<Token>;
    using ScanMap=std::map<std::string,State>;
    static ScanList reservedList;
    static ScanMap reservedMap;
    static ScanList signs;

    static void init(C_DFA&dfa);
    static bool isblank(C c);
    static void reco_blank(C_DFA&dfa);
    static void reco_numbers(C_DFA&dfa);
    static void reco_signs(C_DFA&dfa);
    static void reco_id(C_DFA&dfa);
    static Token confirm_token(const Token&t);
};
}

#include "scan.cpp"