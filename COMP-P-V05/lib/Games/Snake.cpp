#ifndef __Snake_cpp
#define __Snake_cpp
#include "Snake.h"

struct snake_point {
    //position of next point
    //         1
    //     4   X  2
    //         3
    unsigned char nextpoint;
};
struct point {
    int Xposition;
    int Yposition;
};
void Write_Square(int Y, int X, unsigned char screen, unsigned char type){
  for(unsigned char y = 0; y < Ratio; y++){
    for(unsigned char x = 0; x < Ratio; x++){
      ST7565_write_pixel(Ratio*Y+x,Ratio*X+y, type, screen);
    }
  }
}
class snake_Game {
    public:
        snake_point  snake[100];
        int size;
        point tail_point;
        point food_point;
        point current_point;
        unsigned char screen;
        int SnakeScreenHeight;
        int SnakeScreenWidth;
    snake_Game (int SnakeScreenHeight, int SnakeScreenWidth){
      this->SnakeScreenHeight = SnakeScreenHeight;
      this->SnakeScreenWidth = SnakeScreenWidth;
    }
    void Reset(){
      snake[100] = {};
      new_food_point();
      this->current_point.Xposition = this->SnakeScreenWidth/2;
      this->current_point.Yposition = this->SnakeScreenHeight/2;
      this->tail_point.Xposition = this->SnakeScreenWidth/2;
      this->tail_point.Yposition = this->SnakeScreenHeight/2;
      size=0;
    }
    void SetScreen(unsigned char screen){
      this->screen = screen;
    }
    //Need to check if new point is on top of snake (through the screen)
    void new_food_point (){
      this->food_point.Xposition = random_val()%SnakeScreenWidth;
      this->food_point.Yposition = random_val()%SnakeScreenHeight;
      Write_Square(this->food_point.Yposition, this->food_point.Xposition, this->screen, 1);
    }
    void move_to_local (unsigned char mov,int *x,int *y){
      switch(mov){
        case 1:
          *y+=1;
          break;
        case 2:
          *x+=1;
          break;
        case 3:
          *y-=1;
          break;
        case 4:
          *x-=1;
          break;
      }
    }
    //Need to check if snake is moving into itself
    bool check_if_cant_move(unsigned char mov){
        int x = current_point.Xposition;
        int y = current_point.Yposition;
        move_to_local(mov, &x, &y);
        if(x<0 or x>= SnakeScreenWidth-1 or y<0 or y>=SnakeScreenHeight-2){
          return 1;
        }
        if(
          ScreenScreen[y*Ratio][x*Ratio] & 1<<(this->screen) &&
          x != this->food_point.Xposition &&
          y != this->food_point.Yposition
        ){
            return 1;
        }
        return 0;
    }
    // recebo os bool que correspondem ao movimento
    unsigned char game_mov (unsigned char mov) {
        //movese segundo x
        if(check_if_cant_move(mov))
            return this->size+1;

        if(size == 0){
            tail_point.Yposition = current_point.Yposition;
            tail_point.Xposition = current_point.Xposition;
        }

        move_to_local(mov, &current_point.Xposition,&current_point.Yposition);

        Write_Square(current_point.Yposition, current_point.Xposition, this->screen, 1);
        //
        //enviar ordem de escrever head point que esta na posicao  current_point.Xposition,current_point.Yposition;
        //
        if(hit_food_point()){
            //n se apaga a tail comeuse um ponto
            //gera-se novo ponto
            new_food_point();
            snake[size].nextpoint = mov;
            size+=1;
        }else{
            //
            Write_Square(tail_point.Yposition, tail_point.Xposition, this->screen, 0);
            //enviar ordem de apagar tail_point tail_point.Xposition, tail_point.Yposition;
            //
            snake[size].nextpoint = mov;
            move_to_local(snake[0].nextpoint, &tail_point.Xposition, &tail_point.Yposition);
            for(int c = 0;c<size;c++){
                //snake[c] = snake[c+1] ;
                snake[c].nextpoint = snake[c+1].nextpoint;
            }
        }
        return 0;
    }
    bool hit_food_point(){
        if(current_point.Xposition ==  food_point.Xposition and current_point.Yposition== food_point.Yposition)
            return 1;
        return 0;
    }
};

#endif
