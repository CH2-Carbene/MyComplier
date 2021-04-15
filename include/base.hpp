#include"bits/stdc++.h"

template<class T1,class T2>
auto zip(const std::vector<T1>&v1,const std::vector<T2>&v2){
    std::vector<std::pair<T1,T2>>vout;
    int n=std::min(v1.size(),v2.size());
    vout.reserve(n);
    for(int i=0;i<n;++i)vout.push_back(std::make_pair(v1[i],v2[i]));
    return vout;
}
