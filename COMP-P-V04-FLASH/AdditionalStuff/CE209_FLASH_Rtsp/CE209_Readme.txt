
		Readme File for Code Example:
              CE209 - Flash RTSP code example 
             ----------------------------------------

This file contains the following sections:
1. Code Example Description
2. Folder Contents
3. Suggested Development Resources
4. Reconfiguring the project for a different PIC24H device
5. Revision History


1. Code Example Description:
----------------------------
In this example, a page of Flash memory (512 instructions or 8 rows of 64 instruction) is read first.
Then the page is erased fully. One row of the page is modified and written back to the flash.

Following RTSP Application Program Interface (APIs) are used to perform the operation.

// Flash Memory is organised into ROWs of 64 instructions or 192 bytes
// RTSP allows the user to erage a PAGE of memory which consists of EIGHT ROWs (512 instructions or 1536byts) at a time.
// RTSP allows the user to program a ROW (64 instructions or 192 bytes) at a time

;-------------------------------------
/*
 * FLASH PAGE READ
 *
 * Parameters Definition:
 * nvmAdru:	Selects the upper 8bits of the location to read in program flash memory
 * nvmAdr:  Selects the location to read in program flash memory
 *          It must be aligned to 512 instruction boundary, LSB 10bits of address must be zero
 * pageBufPtr: Pointer to the data array in which read data will be stored

 
 * Return Value:
 * Function returns ERROREE (or -1), if it is not successful
 * Function return ZERO, if successful
*/

extern int flashPageRead(u16 nvmAdru, u16 nvmAdr, i16 *pageBufPtr);
;----------------------------------------

;----------------------------------------
/*
 * FLASH PAGE MODIFY
 *
 * Parameters Definition:
 * row:		Selects the row in the the Flash page that will be modified
 * rowBuf:  Selects the location to read in program flash memory
 * pageBufPtr: Pointer to the page data array, that will be modified
 
 * Return Value:
 * Function returns ERROREE (or -1), if it is not successful
 * Function return ZERO, if successful
*/

extern int flashPageModify(u16 row, u16 size, i16 *rowBuf, i16 *pageBufPtr);
;----------------------------------------

;----------------------------------------
/*
 * FLASH PAGE ERASE
 *
 * Parameters Definition:
 * nvmAdru:	Selects the upper 8bits of the location to program or erase in program flash memory
 * nvmAdr:  Selects the location to program or erase in program flash memory
*           It must be aligned to 512 instruction boundary, LSB 10bits of address must be zero
 
 * Return Value:
 * Function returns ERROREE (or -1), if it is not successful
 * Function return ZERO, if successful
*/
extern int flashPageErase(u16 nvmAdru, u16 nvmAdr);
;----------------------------------------

;----------------------------------------
/*
 * FLASH PAGE WRITE
 *
 * Parameters Definition:
 * nvmAdru:	Selects the upper 8bits of the location to program or erase in program flash memory
 * nvmAdr:  Selects the location to program or erase in program flash memory
 *          It must be aligned to 512 instruction boundary, LSB 10bits of address must be zero
 * pageBufPtr: Pointer to the data array that needs to be programmed 


 * Return Value:
 * Function returns ERROREE (or -1), if it is not successful
 * Function return ZERO, if successful
*/
extern int flashPageWrite(u16 nvmAdru, u16 nvmAdr, i16 *pageBufPtr);
;----------------------------------------


2. Folder Contents:
-------------------
This folder contains the following sub-folders:
a. C:\Program Files\Microchip\MPLAB C30\support\gld
        This folder will have the device GLD file, it is used for building the project. 
	This file was provided with the MPLAB� C30 toolsuite.

b. C:\Program Files\Microchip\MPLAB C30\support\h
        This folder contains C header files useful in building this
        project. Device register and bit definitions are provided in
        the *.h file that follows the device name. These files were provided
        with the MPLAB� C30 toolsuite.

c. C:\Program Files\Microchip\MPLAB C30\lib
        This folder contains library archive files, which are a
        collection of precompiled object files. The file
        named "libpic30-coff.a" contains the C run-time start-up
        library. These file were provided with the
        MPLAB� C30 toolsuite.

d. hex
        This folder contains three file types - coff, hex and map.
        These are files generated by the MPLAB� C30 toolsuite on build
        operation performed within MPLAB� IDE. The *.map file contains
        details on memory allocation for various variables, constants
        and dsPIC instructions specified in the source and library
        code. The *.hex file contains a binary file that may be
        programmed into the dsPIC device. The *.coff file contains
        a binary file that is used by MPLAB� IDE for simulation.

e. h
        This folder contains include files for the code example.


f. src
        This folder contains all the C and Assembler source files (*.c,
        *.s) used in demonstrating the described example. This folder
        also contains a sub-folder named "obj" that stores compiled
        object files generated when the project is built.


3. Suggested Development Resources:
-----------------------------------
        a. Explorer 16 Demo board with PIC24HJ256GP610 controller

4. Reconfiguring the project for a different PIC24H device:
-------------------------------------------------------------
The Project/Workspace can be easily reconfigured for any PIC24H device.
Please use the following general guidelines:
        a. Change device selection within MPLAB� IDE to a PIC24H device of
        your choice by using the following menu option:
        MPLAB IDE>>Configure>>Select Device

        b. Provide the correct device linker script and header file for your
        device. Device linker scripts and header files are available in your
        MPLAB� C30 installation folder under:
        Device Linker Script-
                YourDrive:>Program Files\Microchip\MPLAB C30\support\gld
        Device C Header file-
                YourDrive:>Program Files\Microchip\MPLAB C30\support\h
        Device ASM Include file-
                YourDrive:>Program Files\Microchip\MPLAB C30\support\inc

        c. Provide the appropriate path to your MPLAB C30 support file locations
        using the menu option:
        MPLAB IDE>>Project>>Build Options>>Project

        d. Chose the development board applicable to your device. Some options
        are provided below:


        e. Re-build the MPLAB� project using the menu option:
        MPLAB IDE>>Project>>Build All

        f. Download the hex file into the device and run.

5. Revision History :
---------------------
        04/01/2006 - Initial Release of the Code Example
