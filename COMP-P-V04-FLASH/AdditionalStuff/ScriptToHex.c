#include <stdio.h>
#include <stdlib.h>
char c, buffer[10];
FILE *fin, *fout;
int fsize;
char *string;
int main(){
  fin = fopen("./Input","rb");
  fout = fopen("./Output","wb");
  fseek(fin, 0L, SEEK_END);
  fsize = ftell(fin);
  fseek(fin, 0L, SEEK_SET);

  string = (char*)malloc(fsize + 1);
  fread(string, fsize, 1, fin);
  printf("Size:   %d\n", fsize);
  int t = 0;
  for(int i = 0; i < fsize; i++){
    printf("%0.2x ",string[i] & 0xff);
    sprintf(buffer, "%0.2x",string[i]);
    fputc(buffer[0], fout);
    fputc(buffer[1], fout);
    if(t == 6){
      fputc(' ', fout);
      t = 0;
    }else{
      t += 2;
    }
  }
  //printf(string);
  fclose(fout);
  fclose(fin);
}
