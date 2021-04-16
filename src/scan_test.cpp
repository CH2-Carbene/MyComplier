#include "scan.hpp"

using namespace std;
using namespace basic_scan;
using namespace c0_scan;
signed main(){
    // system("pwd");
    std::ifstream fin;fin.open("../testfile/scantestfile.txt");
    // while(fin.peek()!=EOF)cout<<(char)fin.get();
    ScanDataLoader<char>dl(fin);
    C_DFA cd{};
    do{
        auto&&p=cd.get_token(dl);
        cout<<C0config::sname[p.first]<<' '<<p.second<<endl;
        if(p.first==C0config::ERR)break;
    }while(1);
}