#ifndef __BL_0942_FACTORY__
#define __BL_0942_FACTORY__
#include "bl0942_interface.hpp"

class BL0942Factory{
public:
    BL0942Factory(){

    }
    virtual BL0942Interface* build()=0;
    virtual ~BL0942Factory(){
        
    }
};
#endif