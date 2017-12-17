#include "typedef.h"
/*
class CBrick {
      const static unsigned char brickType[SHAPE_NUM];
protected:
      static CBoard &board;
      short  x,y;
      unsigned char color, direction;
      virtual void  erase()=0;
      virtual void  show()=0;
      virtual bool isProped()=0;
public:
      short  score;
      CBrick(short x_, short y_, char color_, unsigned char direction_);
      virtual ~CBrick();
      static CBrick* newBrick();
      void stop();
      int  mvDown(int command=UNDEFINED);
      virtual int  mvLeft()=0;
      virtual int  mvRight()=0; 
      virtual int  rotateLeft();
      virtual int  rotateRight();
friend class CDisplay;
};

class CTriangle:public CBrick {
      virtual void  erase();
      virtual void  show();
      virtual bool  isProped();
friend class CBoard;
public:
      CTriangle(short x_, short y_, char color_, unsigned char direction_);
      virtual int  mvLeft();
      virtual int  mvRight(); 
      virtual int  rotateLeft();
      virtual int  rotateRight();
};
*/
CTriangle::CTriangle(short x_, short y_, char color_, unsigned char direction_):CBrick(x_,y_,color_,direction_){
     if ( x_<1 || x_>11 ) x = 6;
     if ( y_>30 || y_<1 ) y = 30;
     switch ( direction_ ) {
            case LUTRIANGLE:
            case RUTRIANGLE:
            case RDTRIANGLE:
            case LDTRIANGLE:
                 break;
            default:
                 x=0;
                 y=0;
                 color = 0;
                 direction = 0;
                 break;
     }
     return;
}

void CTriangle::show(){
     board<<(*this);
     return;
}

void  CTriangle::erase(){
     board>>(*this);
     return;
}

bool CTriangle::isProped(){
     bool result;
     switch ( direction ) {
     case LUTRIANGLE:
          result = (board.getRim(x,y)&RDTRIANGLE)|(board.getRim(x,y-1)&LUTRIANGLE);
          break;
     case RUTRIANGLE:
          result = (board.getRim(x,y)&LDTRIANGLE)|(board.getRim(x,y-1)&RUTRIANGLE);
          break;
     case RDTRIANGLE:
     case LDTRIANGLE:
          result = board.getRim(x,y-1);
          break;
     } 
     return result;

}

int CTriangle::mvLeft(){
    bool  stopped;
    switch ( direction ) {
           case LDTRIANGLE:
           case LUTRIANGLE:
                stopped = board.getRim(x-1,y);
                break;
           case RUTRIANGLE:
                stopped = (board.getRim(x,y)&LDTRIANGLE)|(board.getRim(x-1,y)&RUTRIANGLE);
                break;
           case RDTRIANGLE:
                stopped = (board.getRim(x,y)&LUTRIANGLE)|(board.getRim(x-1,y)&RDTRIANGLE);
                break;
           default:
                return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     x--;
     show();
    return MV_LEFT;
}

int CTriangle::mvRight(){
    bool  stopped;
    switch ( direction ) {
           case RUTRIANGLE:
           case RDTRIANGLE:
                stopped = board.getRim(x+1,y);
                break;
           case LDTRIANGLE:
                stopped = (board.getRim(x,y)&RUTRIANGLE)|(board.getRim(x+1,y)&LDTRIANGLE);
                break;
           case LUTRIANGLE:
                stopped = (board.getRim(x,y)&RDTRIANGLE)|(board.getRim(x+1,y)&LUTRIANGLE);
                break;
           default:
                return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     x++;
     show();
     return MV_RIGHT;
}

int CTriangle::rotateLeft(){
    if(board.getRim(x,y)!=direction) return mvDown(); 
    erase();
    direction = ((direction&0x7)<<1)|(direction>>3);
    show();
    return RT_LEFT;
}

int CTriangle::rotateRight(){
    if(board.getRim(x,y)!=direction) return mvDown(); 
    erase();
    direction = ((direction&0x1)<<3)|(direction>>1);
    show();
    return RT_RIGHT;
}
