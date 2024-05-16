#ifndef __BL_0942_INTERFACE_SPI__
#define __BL_0942_INTERFACE_SPI__

#include <common_headers.hpp>
#include <stdint.h>
enum class AC_FREQUENCY{
    FIFTY_HZ,
    SIXTY_HZ
};
class BL0942Interface{
    public:
        BL0942Interface();
        virtual error_type_t Init();
        virtual error_type_t GetRMSVoltage(double&);
        virtual error_type_t GetRMSCurrent(double&);
        virtual error_type_t GetActivePower(double&);
        virtual error_type_t GetEnergy(double&);
    protected:
        static const float DEFAULT_SHUNT;
        static const uint32_t DEFAULT_VOLTAGE_RATIO;
};
        const float DEFAULT_SHUNT = 0.001;
        const uint32_t DEFAULT_VOLTAGE_RATIO = ((1000000+510)/510);
#endif