#include "scan.hpp"
namespace basic_scan{
template<typename C>
ScanDataLoader<C>::ScanDataLoader(std::ifstream&_fin):fin(_fin){
    nowpos=fin.tellg();
}
template<typename C>
C ScanDataLoader<C>::try_get()
{
    char result;
    fin.get(result);
    return result;
}

template<typename C>
void ScanDataLoader<C>::lock() 
{
    nowpos=fin.tellg();
}

template<typename C>
void ScanDataLoader<C>::rollback() 
{
    fin.seekg(nowpos);
}

template<class Config>
typename DFA<Config>::DFA_Node* DFA<Config>::DFA_Node::get_next(const C&c) 
{
    for(auto&& [checker,ptrnext]:ruleList){
        if(checker(c))return ptrnext;
    }
    return nullptr;
}

template<class Config>
void DFA<Config>::ins_string(const DFA::Token&t) 
{
    auto&& [sst,s]=t;
    DFA_Node* now=stateList[0].get();
    for(auto&& c:s){
        DFA_Node* next=now->get_next(c);
        if(next!=nullptr)now=next;
        else{
            auto next=std::make_unique<ptrNode>();
            now->ruleList.emplace(
                [=](const C& _c){
                    return c==_c;
                },
                next.get()
            );
            now=next.get();
            stateList.emplace(std::move(next));
        }
    }
    if(now.st)std::clog<<"conflict state!"<<std::endl;
    now.st=sst;
}


template<class Config>
typename DFA<Config>::Token DFA<Config>::get_token(ScanDataLoader<C>&dl) const
{
    DFA_Node* now=stateList[0].get(),q=nullptr;
    std::string ts;
    int lap=0;
    while(true){
        C c=dl.try_get();ts.push_back(c);
        now=now->get_next(c);
        if(now==nullptr){
            dl.rollback();
            if(q)return Token(q->st,ts.substr(lap));
            else return Token(State::ERR,"");
        }
        else if(now->st!=State::NIL){
            dl.lock();
            q=now;
            lap=ts.size();
        }
    };
}

}

namespace c0_scan{

C0config::ScanList C0config::reservedList={
    {"if",IF},
    {"else",ELSE},
    {"for",FOR},
    {"while",WHILE}
};
C0config::ScanList C0config::signs={
    {"=",SET},
    {"==",EQ},
    {">",GQ},
    {"<",LQ}
};
}
