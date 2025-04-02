#include <Arduino.h>
#include <string.h>
//int nd = 1414;
//const int ne = 2008;
int ready = 0;
char arr[100];
char buffer[3];

int buffernumber;
unsigned int Beggining;
unsigned int NVMUnlock (unsigned int nvmop){

  unsigned int status;
  //Suspend or Disable all Interrupts
  asm volatile ("di %0" : "=r" (status));
  //Enable Flas Write/Erase Operations and Select
  //Flash operation to perform
  NVMCON = nvmop;

  //Write Keys
  NVMKEY = 0XAA996655;
  NVMKEY = 0X556699AA;

  //Start the operation using the Set Register
  NVMCONSET = 0X8000;

  //Wait for operation to complete
  while(NVMCON & 0x8000);

  //Restore Interrupts
  if(status & 0x00000001)
    asm volatile ("ei");
  else
    asm volatile ("di");

  //Disable NVM write Enable
  NVMCONCLR = 0x0004000;

  //Return WRERR and LVDERR Error Status Bit
  return (NVMCON & 0X300);
}
unsigned int NVMWriteWord(void* address, unsigned int data){
  //Write 4 bytes
  unsigned int res;

  //Load data into NVMDATA Register
  NVMDATA = data;

  //Load address to program into NVMADDR Register
  NVMADDR = (unsigned int) address;

  //Unlock and Write Word
  res = NVMUnlock (0x4001);

  return res;
  //Addres is 23
}
unsigned int NVMErasePage(void* address){
  unsigned int res;

  //Set NVMADDR to the Start Address of page to erase
  NVMADDR = (unsigned int) address;

  //Unlock and Erase page
  res = NVMUnlock(0x4004);

  //Return Result
  return res;
}

void setup(){
  buffer[2] = '\0';
  //pinMode(34, OUTPUT);
  Serial.begin(9600);
  /*
  sprintf(arr, "BMXPUPBA 0x%x", _CP0_GET_EPC());
  Serial.println(arr);
  */
  const char* Programm = "7f454c46 01010100 00000000 00000000 01000800 01000000 00000000 00000000 58010000 0110ff70 34000000 00002800 0f000c00 300000ff 00000000 00000000 00000000 00000000 00000000 22000424 00000008 01000524 ffffff27 1400ffff 01000524 0000000c 22000424 0000000c ff010424 22000424 0000000c 21280000 ff010424 1400ffff 00000008 1800ff27 00474343 3a202863 6869704b 49542920 342e382e 33206368 69704b49 5420436f 6d70696c 65722066 6f722050 49433332 204d4355 73207631 2e343200 410f0000 00676e75 00010700 00000403 002e7379 6d746162 002e7374 72746162 002e7368 73747274 6162002e 74657874 002e6461 7461002e 62737300 2e726567 696e666f 002e6d64 65627567 2e616269 3332002e 72656c2e 74657874 2e736574 7570002e 72656c2e 74657874 2e6c6f6f 70002e63 6f6d6d65 6e74002e 676e752e 61747472 69627574 65730000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 00000000 1b000000 01000000 06000000 00000000 34000000 00000000 00000000 00000000 04000000 00000000 21000000 01000000 03000000 00000000 34000000 00000000 00000000 00000000 01000000 00000000 27000000 08000000 03000000 00000000 34000000 00000000 00000000 00000000 01000000 00000000 2c000000 06000070 00000000 00000000 34000000 18000000 00000000 00000000 04000000 18000000 35000000 01000000 03000000 00000000 4c000000 00000000 00000000 00000000 01000000 00000000 47000000 01000000 06000000 00000000 4c000000 0c000000 00000000 00000000 04000000 00000000 43000000 09000000 00000000 00000000 10050000 08000000 0d000000 06000000 04000000 08000000 57000000 01000000 06000000 00000000 58000000 38000000 00000000 00000000 04000000 00000000 53000000 09000000 00000000 00000000 18050000 20000000 0d000000 08000000 04000000 08000000 62000000 01000000 30000000 00000000 ff000000 3c000000 00000000 00000000 01000000 01000000 6b000000 ffffff6f 00000000 00000000 ff000000 10000000 00000000 00000000 01000000 00000000 11000000 03000000 00000000 00000000 ff000000 7b000000 00000000 00000000 01000000 00000000 01000000 02000000 00000000 00000000 ff030000 30010000 0e000000 0e000000 04000000 10000000 09000000 03000000 00000000 00000000 ff040000 30000000 00000000 00000000 01000000 00000000 00000000 00000000 00000000 00000000 01000000 00000000 00000000 0400ffff 00000000 00000000 00000000 03000100 00000000 00000000 00000000 03000200 00000000 00000000 00000000 03000300 00000000 00000000 00000000 03000100 00000000 00000000 00000000 03000200 00000000 00000000 00000000 03000300 00000000 00000000 00000000 03000400 00000000 00000000 00000000 03000500 00000000 00000000 00000000 03000600 00000000 00000000 00000000 03000800 00000000 00000000 00000000 03000a00 00000000 00000000 00000000 03000b00 0a000000 00000000 0c000000 12000600 10000000 00000000 00000000 10000000 18000000 00000000 38000000 12000800 1d000000 00000000 00000000 10000000 2a000000 00000000 00000000 10000000 006d6169 6e2e6370 70007365 74757000 70696e4d 6f646500 6c6f6f70 00646967 6974616c 57726974 65006465 6c617900 04000000 040f0000 0c000000 04110000 14000000 04120000 20000000 04110000 30000000 04120000 00000000";
  label:
  if(ready){
    for(int i = 0; i < strlen(Programm); i += 9){
      strncpy(buffer, Programm+i, 9);
      sscanf(buffer, "%x", &buffernumber);
      sprintf(arr, "%.8x", buffernumber);
      //Serial.print(arr);
      //Serial.print(" ");

      NVMWriteWord((void*)Beggining+i*4, buffernumber);
    }
    /*
    void *address = 0x10080000;
    goto *address;
    */

    goto *Beggining;
    //asm volatile ("jump %0" : "=r" (Beggining));
  }
  ready = 1;
  asm volatile("nop");
  /*
First, test if address is the correct one.
Then, use assembly comments to outline code to cut
  */
  sprintf(arr, "%.8x", _CP0_GET_EPC());//Obtido (assumido ultimo endereço de _CP0_GET_EPC()) e mais 1 pelo printf (epero 1))   +1
  Serial.print("HERE");//Equivalente a um só beq (assumindo por favor que não seja um bne (50/50))                                    +1
  Beggining = _CP0_GET_EPC()+10;//0 arr vai imprimir o ínicio desta sequência, ficando então a apontar para o início (1 goto -> -1, e um add -> -1)
  //--------mais um por um goto

  asm ("  nop\n" /* Comment #1 */"  nop\n" /* comment #2 */);//First Assembly compiled delinear --------INÍCIO--------
  goto label;
  sprintf(arr, "%.8x", _CP0_GET_EPC());
  Serial.print(arr);
  pinMode(34, OUTPUT);
  while(1){
    digitalWrite(34, HIGH);
    delay(500);
    digitalWrite(34, LOW);
    delay(500);
    asm ("  nop\n" /* Comment #1 */"  nop\n" /* comment #2 */);//Second Assembly compiled delinear
  }
  //asm("jump 0x10080000");
  /*
  sprintf(arr, "BMXPUPBA 0x/asm("jump 0x10080000");%x", _CP0_GET_EPC());
  Serial.println(arr);
  sprintf(arr, "%x", Beggining);
  Serial.println(arr);

  sprintf(arr, "%x", Beggining+16);
  Serial.println(arr);
*/
  /*
   0x9d0088b4

  sprintf(arr, "BMXPUPBA 0x%x", BMXPUPBA);
  Serial.println(arr);

  sprintf(arr, "BMXDKPBA 0x%x\n",BMXDKPBA);
  Serial.println(arr);

  sprintf(arr, "RAM memory size BMXDRMSZ 0x%x (%d KB)\n ",BMXDRMSZ, (BMXDRMSZ>>10));
  Serial.println(arr);

  sprintf(arr, "FLASH memory size BMXPFMSZ 0x%x (%d KB)\n",BMXPFMSZ,(BMXPFMSZ>>10));
  Serial.println(arr);

  sprintf(arr, "BOOT memory size BMXBOOTSZ 0x%x (%d KB)\n",BMXBOOTSZ,(BMXBOOTSZ>>10));
  Serial.println(arr);
  */
  /*
  char arr[10];
  sprintf(arr,"&arr = %p",&arr);
  Serial.println(arr);
  sprintf(arr,"&arr = %p",&(arr[9]));
  Serial.println(arr);
  /*
  int na;
  const int nb = 31415;
  static int nc = 0x1234ABCD;
  sprintf(arr,"&na = %p",&na);
  Serial.println(arr);
  sprintf(arr,"&nb = %p",&nb);
  Serial.println(arr);
  sprintf(arr,"&nc = %p",&nc);
  Serial.println(arr);
  sprintf(arr,"&nd = %p",&nd);
  Serial.println(arr);
  sprintf(arr,"&ne = %p",&ne);
  Serial.println(arr);

*/
}
/*
As a safety measure, a sequence of values must be set on
a couple of registers, in order to be able to write to
flash
*/
void loop(){
  /*
  digitalWrite(34, HIGH);
  delay(500);
  digitalWrite(34, LOW);
  delay(500);
  */
}
/*
int main(){
  while(1){
  }
}
*/
