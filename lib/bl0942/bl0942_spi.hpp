#ifndef __BL_0942_SPI__
#define __BL_0942_SPI__
#include "bl0942_interface.hpp"
#include "SPI.h"
class BL0942Spi:public BL0942Interface
{
    public:
        BL0942Spi();
        BL0942Spi(SPIClass* spiObject, AC_FREQUENCY frequency, float voltageMeasurementRatio, float shuntValue);
        error_type_t Init() override;
        error_type_t GetRMSVoltage(double&) override;
        error_type_t GetRMSCurrent(double&) override;
        error_type_t GetActivePower(double&) override;
        error_type_t GetEnergy(double&) override;
    private:
        AC_FREQUENCY frequency;
        float voltageMeasurementRatio;
        float shuntValue;
        SPIClass* spiObject;
};
#endif