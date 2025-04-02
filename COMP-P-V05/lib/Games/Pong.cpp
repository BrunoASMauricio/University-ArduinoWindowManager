#ifndef __Pong_cpp__
#define __Pong_cpp__
#include "Pong.h"

struct ball{
    int height;
    int width;
    int widthspeed;
    int heightspeed;
};

class pongGAME{
  public:
      int barLeftHeight;
      int barRightHeight;
      int prevbarLeftHeight;
      int prevbarRightHeight;
      int nGames;
      int winPLAYER1;
      int winPLAYER2;
      int ControlledDelay;
      unsigned char screen;
      ball bal;
      ball previousbal;
      uchar ScreenWidth;
      uchar ScreenHeight;
      //position of the inferior left corner
  public:
    pongGAME(int n, uchar ScreenWidth, uchar ScreenHeight){
      this->nGames = n;
      this->barLeftHeight = ScreenHeight/2;
      this->barRightHeight = ScreenHeight/2;
      this->winPLAYER1 = 0;
      this->winPLAYER2 = 0;
      this->bal.width =  ScreenWidth/2 - int(ballSize/2);
      this->bal.height = ScreenHeight/2 + int(ballSize/2);
      this->bal.widthspeed = 4;
      this->bal.heightspeed = 0;
      this->ControlledDelay = 100;
      this->screen = 0x00;
      this->ScreenWidth = ScreenWidth;
      this->ScreenHeight = ScreenHeight;
    }
    void SetScreen(unsigned char Screen){
      this->screen = Screen;
    }
    //return 1 if 1 pont player 1 2 if 1 pont player 2 and 0 of no case
    int newBallspeed(){
      int width = this->bal.width;
      int height = this->bal.height;
      //Check if ball is still in play
      if((width < this->ScreenWidth-margin-CursorWidth-ballSize and width > margin+CursorWidth)){
        //bouncing on top
        if(height-1 <= 0){
          this->bal.heightspeed = abs(this->bal.heightspeed);
        }
        //bouncing on bottom
        if(height+ballSize+1 >= this->ScreenHeight  ){
          this->bal.heightspeed = -1*abs(this->bal.heightspeed);
        }
        return 0;
      }
      //Right player Contact
      if((height < this->barRightHeight+CursorHeight) and (height+ballSize > this->barRightHeight) and width+ballSize+1 >= this->ScreenWidth - margin - CursorWidth){
        this->bal.widthspeed = -this->bal.widthspeed;
        //this->bal.widthspeed = random_val()%(MaxBallSpeed-1) +1;
        int n;
        n =(random_val()%(MaxBallSpeed-1) +1);
        int m = random_val()%(2*MaxBallSpeed)-MaxBallSpeed;
        if(m>0){
          this->bal.heightspeed = abs(this->bal.heightspeed)+1;
        }else{
          this->bal.heightspeed = -abs(this->bal.heightspeed)-1;
        }
        if(this->ControlledDelay > 0){
          this->ControlledDelay -= 10;
        }
        return 0;
      }
      //Left Player Contact
      if((height < this->barLeftHeight+CursorHeight) and (height+ballSize > this->barLeftHeight) and width-1 <= margin + CursorWidth){
        //this->bal.widthspeed = -(random_val()%(MaxBallSpeed-1) +1);
        this->bal.widthspeed = -this->bal.widthspeed;
        int n;
        n =(random_val()%(MaxBallSpeed-1) +1);
        int m = random_val()%(2*MaxBallSpeed)-MaxBallSpeed;
        if(m>0){
          this->bal.heightspeed = abs(this->bal.heightspeed)+1;
        }else{
          this->bal.heightspeed = -abs(this->bal.heightspeed)-1;
        }
        if(this->ControlledDelay > 0){
          this->ControlledDelay -= 10;
        }
        return 0;
      }
      //Check if right player scored
      if(width <= CursorWidth + margin){
        return 2;
      }
      //Check if left player scored
      if(width+ballSize > this->ScreenWidth - margin - CursorWidth){
        return 1;
      }
    }
    void newRound(){
      this->barLeftHeight = this->ScreenHeight/2;
      this->barRightHeight = this->ScreenHeight/2;
      this->bal.width =  this->ScreenWidth/2 - int(ballSize/2);
      this->bal.height = this->ScreenHeight/2 ;
      this->bal.heightspeed = 1;
      this->ControlledDelay = 100;
    }
    int newresult(int res){
      if (res==2){
        this->winPLAYER2+=1;
        if(this->winPLAYER2 == this->nGames){
          return 2;
        }
      }
      if(res==1){
        this->winPLAYER1+=1;
        if(this->winPLAYER1 == this->nGames){
          return 1;
        }
      }
      return 0;
    }
    //movplayer 1 =-1,0,1 mediante quer ir para cima parado ou baixo
    int actualizeGAME(int movplayer1,int movplayer2){
      static int PreviousPosition1 = margin;
      static int NewPosition1 = 0;
      static int PreviousPosition2 = margin;
      static int NewPosition2 = 0;
      NewPosition1 = (movplayer1*(this->ScreenHeight-CursorHeight))/1023;
      //right control
      NewPosition2 = (movplayer2*(this->ScreenHeight-CursorHeight))/1023;
      //Keep writing so that the ball doesn't erase the text
      ST7565_Print(0,2,"Player1",this->screen);
      ST7565_Print_Int(2,4,this->winPLAYER1,this->screen);
      ST7565_Print(0,W-10,"Player2",this->screen);
      ST7565_Print_Int(2,W-9,this->winPLAYER2,this->screen);
      ST7565_Print(3,(W/2)-5,"Difficulty",this->screen);
      ST7565_Print_Int(4,(W/2)-2,(100-this->ControlledDelay)/10,this->screen);

      Pong_write(PreviousPosition1,PreviousPosition2,0,this->screen, this->ScreenWidth);
      Pong_write(NewPosition1,NewPosition2,1,this->screen,this->ScreenWidth);

      PreviousPosition1 = NewPosition1;
      PreviousPosition2 = NewPosition2;
      Pong_Paint(this->bal.height,this->bal.width,ballSize,ballSize,0,this->screen);

      //atualize bar positions
      this->barLeftHeight=NewPosition1;
      this->barRightHeight=NewPosition2;
      //saving ball position
      //this->previousbal.width = this->bal.width;
      //this->previousbal.height = this->bal.height;
      //updating ball speed
      int res;
      res = newBallspeed();
      int fin;
      if(res==1 or res==2){
        fin = newresult(res);
        newRound();
      }
      //updating ball positions
      this->bal.width +=this->bal.widthspeed;
      this->bal.height += this->bal.heightspeed;
      Pong_Paint(this->bal.height,this->bal.width,ballSize,ballSize,1,this->screen);
      //ST7565_RefreshEntireScreen();
      delay(this->ControlledDelay);
      //ST7565_Print_Int(5,5,(int)this->screen);
      return fin;
    }
};

void Pong_Paint(int line, int column, int height, int width, int write, unsigned char screen){
  for(int x = column; x < column+width; x++){
    for(int y = line; y < line+height; y++){
      ST7565_write_pixel(y,x, write, screen);
    }
  }
}

void Pong_write(int leftheighty, int rightheighty, int write, unsigned char screen, uchar ScreenWidth){
  Pong_Paint(leftheighty, margin, CursorHeight, CursorWidth, write, screen);
  Pong_Paint(rightheighty, ScreenWidth-margin-CursorWidth, CursorHeight, CursorWidth, write, screen);
}



#endif
