#include <iostream>
#include "color.h"


int main(){
    mdcl::HSBColor c(200.0f,0.8f,0.8f);
    auto rgb = c.getRGBA();
    char names[4] = {'r','g','b','a'};
    for(int i = 0; i < 4; i++){
        std::cout << names[i] << ':' << (double)rgb[i] << std::endl;
    }
    return 0;
}