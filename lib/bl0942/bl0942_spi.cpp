#include "bl0942_spi.hpp"

BL0942Spi::BL0942Spi():BL0942Spi(&SPI, AC_FREQUENCY::FIFTY_HZ, DEFAULT_VOLTAGE_RATIO, DEFAULT_SHUNT)
{

}
BL0942Spi::BL0942Spi(SPIClass* spiObject, AC_FREQUENCY frequency, float voltageMeasurementRatio, float shuntValue):
    BL0942Interface()
{

}
error_type_t BL0942Spi::Init()
{
    return error_type_t::OK;
}
error_type_t BL0942Spi::GetRMSVoltage(double& rmsVoltage)
{
    return error_type_t::OK;
}
error_type_t BL0942Spi::GetRMSCurrent(double& rmsCurrent){
    return error_type_t::OK;
}
error_type_t BL0942Spi::GetActivePower(double& activePower){
    return error_type_t::OK;
}
error_type_t BL0942Spi::GetEnergy(double& energy){
     return error_type_t::OK;   
}