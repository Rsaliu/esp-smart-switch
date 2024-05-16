/*
 * Connect the SD card to the following pins:
 *
 * SD Card | ESP32
 *    D2       -
 *    D3       SS
 *    CMD      MOSI
 *    VSS      GND
 *    VDD      3.3V
 *    CLK      SCK
 *    VSS      GND
 *    D0       MISO
 *    D1       -
 */
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <bl0942_spi.hpp>
#define SPI_CLK 800000
#define READ_CMD 0x58
#define WRITE_CMD 0xA8
#define I_WAVE_ADD 0x03
#define V_WAVE_ADD 0x04
#define DUMMY_DATA 0x01
#define RESET_DATA 0xFF
#define PIN_4 4
#define ELECTRIC_DATA_WIDTH 25
#define COMMON_WIDTH 3
#define MAX_RMS_VAL (1<<23)


void setup(){

    Serial.begin(115200);
    pinMode(PIN_4, OUTPUT);
    digitalWrite(PIN_4,HIGH);
    SPI.begin();

}
int32_t voltage_val=0;
int32_t current_val=0;
uint8_t electric_data[ELECTRIC_DATA_WIDTH];
uint8_t current_data[COMMON_WIDTH + 1];
uint8_t voltage_data[COMMON_WIDTH + 1];
double v_rms;
void loop(){

    Serial.println("Starting");
    Serial.println("SPI set");
    SPI.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
    for(int k=0; k< 6;k++){
        SPI.transfer(RESET_DATA);
    }
    SPI.endTransaction();
    SPI.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));    
    SPI.transfer(READ_CMD);
    SPI.transfer(I_WAVE_ADD);
    for(int x  =0; x<=COMMON_WIDTH;x++){
        current_data[x] = SPI.transfer(DUMMY_DATA);
    }
    SPI.endTransaction();

    SPI.beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE1));
    delay(1);
    SPI.transfer(READ_CMD);
    SPI.transfer(V_WAVE_ADD);
    for(int x  =0; x<=COMMON_WIDTH;x++){
        voltage_data[x] = SPI.transfer(DUMMY_DATA);
    }
    SPI.endTransaction();
    uint32_t a = ((voltage_data[0]&&0x7F)<<16);
    uint16_t b = (voltage_data[1]<<8);
    uint8_t c = (voltage_data[2]);
    voltage_val = a|b|c;
    if(voltage_data[0] > 0x7F){
        voltage_val = voltage_val*-1;
    }
    v_rms = (1218*voltage_val)*1.0/MAX_RMS_VAL;
    a = ((current_data[0]&&0x7F)<<16);
    b = (current_data[1]<<8);
    c = (current_data[2]);
    current_val = a|b|c;
    if(current_data[0] > 0x7F){
        current_val = current_val*-1;
    }
    // uint32_t iwave_data;
    // iwave_data = electric_data[0];
    // iwave_data = iwave_data << 8;
    // iwave_data = iwave_data  || electric_data[1];
    // iwave_data = iwave_data << 8;
    // iwave_data = iwave_data  || electric_data[2];
    // iwave_data = iwave_data >> 4;
    // for(int k =0; k<3;k++){
    //     uint8_t data = SPI.transfer(DUMMY_DATA);
    //     iwave_data = iwave_data << SPI.transfer(DUMMY_DATA);
    // }
    //iwave_data = SPI.transfer(DUMMY_DATA);

    
    //Serial.printf("iwave data read is: %ld",iwave_data);
    Serial.printf("Hello world %p and %p",voltage_data,current_data);
    delay(1);
}
