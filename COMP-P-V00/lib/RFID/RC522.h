#ifndef __RC522_h__
#define __RC522_h__


#include <SPI.h>
/* Include the RFID library */
#include <RFID.h>
/*SDA (SS) and RST (reset) pins. */
#define SDA_DIO 9
#define RESET_DIO 8

RFID RC522(SDA_DIO, RESET_DIO);
#endif
