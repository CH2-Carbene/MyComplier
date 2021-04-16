#include"bits/stdc++.h"

// using Vec=std::vector<int>;
namespace arrop{
template<class T1,class T2>
auto zip(const std::vector<T1>&v1,const std::vector<T2>&v2){
    std::vector<std::pair<T1,T2>>vout;
    int n=std::min(v1.size(),v2.size());
    vout.reserve(n);
    for(int i=0;i<n;++i)vout.push_back(std::make_pair(v1[i],v2[i]));
    return vout;
}
template<class T1,class T2>
auto first(const std::vector<std::pair<T1,T2>>&v){
    std::vector<T1>ans;ans.reserve(v.size());
    for(auto&&p:v)ans.push_back(p.first);
    return ans;
}
template<class T1,class T2>
auto second(const std::vector<std::pair<T1,T2>>&v){
    std::vector<T2>ans;ans.reserve(v.size());
    for(auto&&p:v)ans.push_back(p.second);
    return ans;
}
template<class T1,class T2>
std::map<T1,T2>make_map(std::vector<std::pair<T1,T2>>v1){
    std::map<T1,T2>ans;
    for(auto&&p:v1)ans.insert(p);
    return ans;
}
}