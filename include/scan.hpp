#pragma once
#include "base.hpp"
namespace basic_scan{
template<typename C>
class ScanDataLoader{
    fin f;
    ScanDataLoader(const ifstream&if);
  public:
    C get();
};

template<class Config>
class DFA{
    using C=typename Config::C;
    using State=typename Config::State;

    class DFA_Node{
        using Checker=std::function<bool(C)>;
        using ptrNext=const DFA_Node*;
        using Rule=pair<Checker,ptrNext>;
        std::vector<Rule>ruleList;
        
      public:
        State st;
        DFA_Node* get_next(const C&c);
        
    };
    using ptrNode=std::unique_ptr<DFA_Node>;
    std::vector<ptrNode>stateList;
    void ins_string(const std::string&s);

  public:
    using Token=std::pair<State,std::string>;
    Token get_token(ScanDataLoader<C>&dl)const;
    
};
}

namespace c0_scan{

struct C0config{
    using C=char;
    enum State{
        ERR,NUM,ID,IF,ELSE,FOR,WHILE,EQ,GQ,LQ
    };
    using ScanList=std::unordered_map<std::string,State>;
    static ScanList reservedList;
    static ScanList signs;
};
auto C0config::reservedList{
    {"if",IF},
    {"else",ELSE},
    {"for",FOR},
    {"while",WHILE}
};
auto C0config::signs{
    {"=",EQ},
    {">",GQ},
    {"<",LQ}

};

class C_DFA:public basic_scan::DFA<C0config>{
  public:
    C_DFA();
};
}
