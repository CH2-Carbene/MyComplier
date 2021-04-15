#include "scan.hpp"

using namespace std;
using namespace basic_scan;
using namespace c0_scan;
signed main(){
    std::ifstream fin;fin.open("../testfile/scantest.txt");

    ScanDataLoader<char>dl(fin);
    C_DFA cd{};
    do{
        auto&&p=cd.get_token(dl);
        if(p.first==C0config::ERR)break;
        cout<<p.first<<' '<<p.second<<endl;
    }while(1);

}