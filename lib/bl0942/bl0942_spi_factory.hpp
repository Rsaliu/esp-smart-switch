#ifndef __BL_0942_SPI_FACTORY__
#define __BL_0942_SPI_FACTORY__
#include "bl0942_spi.hpp"
#include "bl0942_factory.hpp"

class BL0942SpiFactory:public BL0942Factory{
public:
    BL0942SpiFactory(SPIClass* spiObject, AC_FREQUENCY frequency, float voltageMeasurementRatio, float shuntValue):
    BL0942Factory(){

    }

    BL0942Interface* build()override{
        return new BL0942Spi(spiObject,frequency,voltageMeasurementRatio,shuntValue);
    }
private:
    SPIClass* spiObject;
    AC_FREQUENCY frequency;
    float voltageMeasurementRatio;
    float shuntValue;
    // BL0942SpiFactory* build(){
    //     return new BL0942SpiFactory(SPIClass* spiObject, AC_FREQUENCY frequency, float voltageMeasurementRatio, float shuntValue);
    // }
};
#endif