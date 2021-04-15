#include "scan.hpp"
namespace basic_scan{

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
template<typename C>
ScanDataLoader<C>::ScanDataLoader(std::ifstream&_fin):fin(_fin){
    nowpos=fin.tellg();
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
    DFA_Node* now=rt;
    for(auto&& c:s){
        DFA_Node* next=now->get_next(c);
        if(next!=nullptr)now=next;
        else{
            auto next=std::make_unique<DFA_Node>();
            now->ruleList.emplace_back(
                [=](const C& _c){
                    return c==_c;
                },
                next.get()
            );
            now=next.get();
            stateList.emplace_back(std::move(next));
        }
    }
    if(now->st)std::clog<<"conflict state!"<<std::endl;
    now->st=sst;
}

template<class Config>
DFA<Config>::DFA() 
{
    stateList.emplace(std::make_unique<DFA_Node>());
    rt=stateList[0].get();
    Config::init(*this);
}


template<class Config>
typename DFA<Config>::Token DFA<Config>::get_token(ScanDataLoader<C>&dl) const
{
    DFA_Node* now=rt,q=nullptr;
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
void C0config::init(C_DFA&dfa) 
{
    reco_blank(dfa);
    reco_numbers(dfa);
    reco_signs(dfa);
    reco_id(dfa);
}

void C0config::reco_blank(C_DFA&dfa) 
{
    dfa.rt->ruleList.emplace_back([](C c){
        return c==' '||c=='\r'||c=='\n'||c=='\t';
    },dfa.rt);
}

void C0config::reco_numbers(C_DFA&dfa) 
{
    auto num_state=std::make_unique<C_DFA::DFA_Node>();
    num_state->ruleList.emplace_back(isdigit,num_state.get());
    num_state->st=NUM;
    dfa.rt->ruleList.emplace_back(isdigit,num_state.get());
    dfa.stateList.emplace_back(std::move(num_state));
}

void C0config::reco_signs(C_DFA&dfa) 
{
    for(auto&& sign:signs){
        dfa.ins_string(sign);
    }
}

void C0config::reco_id(C_DFA&dfa)
{
    auto id_state=std::make_unique<C_DFA::DFA_Node>();
    id_state->ruleList.emplace_back([](C c){
        return isalpha(c)||isdigit(c)||c=='_';
    },id_state.get());
    id_state->st=ID;
    dfa.rt->ruleList.emplace_back([](C c){
        return isalpha(c)||c=='_';
    },id_state.get());
    dfa.stateList.emplace_back(std::move(id_state));
}


C0config::ScanList C0config::reservedList={
    {IF,"if"},
    {ELSE,"else"},
    {FOR,"for"},
    {WHILE,"while"}
};
C0config::ScanList C0config::signs=
    zip(std::vector<State>{SET,E,L,G,LE,GE,BL,BR,MBL,MBR,GBL,GBR},
    std::vector<std::string>{"=","==","<",">","<=","(",")","[","]","{","}"});
}