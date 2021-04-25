#include "Application.hh"

int main()
{try{
    Application app;
    return 0;
}catch(const rterr& err){
    std::cerr << err.first << " -> " << err.second.what() << '\n';
    return err.first;
}}
