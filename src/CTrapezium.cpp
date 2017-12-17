#include "typedef.h"
/*
class CTrapezium {
      static CBoard &board;
      short  x,y;
      unsigned char color, direction;
      void show();
      void erase();
public:
      CTrapezium(short x_, short y_, char color_, unsigned char direction_);
      void start();
      int  mvDown();
      int  mvLeft();
      int  mvRight(); 
      int  rotateLeft();
      int  rotateRight();
};
*/

/*
CTrapezium::CTrapezium(short x_, short y_, char color_, unsigned char direction_):CBrick(x_,y_,color_,direction_){
     switch ( direction_ ) {
            case HUTRAPEZIUM:
                 if ( x_<2 || x_>10 ) x = 6;
                 if ( y_>30 || y_<1 ) y = 30;
                 break;
            case HDTRAPEZIUM:
                 if ( x_<2 || x_>10 ) x = 6;
                 if ( y_>30 || y_<1 ) y = 30;
                 break;
            case VRTRAPEZIUM:
                 if ( x_<1 || x_>11 ) x = 6;
                 if ( y_>29 || y_<2 ) y = 29;
                 break;
            case VLTRAPEZIUM:
                 if ( x_<1 || x_>11 ) x = 6;
                 if ( y_>29 || y_<2 ) y = 29;
                 break;
            default:
                 x=0;
                 y=0;
                 color = 0;
                 direction = 0;
                 return;
     }
     return;
}
bool CTrapezium::isProped(){
     bool result;
     switch ( direction ) {
     case HUTRAPEZIUM:
          result = board.getRim(x,y-1)|board.getRim(x-1,y-1)|board.getRim(x+1,y-1);
          break;
     case VRTRAPEZIUM:
          result = (board.getRim(x,y-1)&RDTRIANGLE)|(board.getRim(x,y-2)&LUTRIANGLE);
          break;
     case HDTRAPEZIUM:
          result = (board.getRim(x-1,y)&LDTRIANGLE)|board.getRim(x,y-1);
          result = result || (board.getRim(x+1,y)&RDTRIANGLE);
          result = result || (board.getRim(x-1,y-1)&RUTRIANGLE);
          result = result || (board.getRim(x+1,y-1)&LUTRIANGLE);
          break;
     case VLTRAPEZIUM:
          result = (board.getRim(x,y-1)&LDTRIANGLE)|(board.getRim(x,y-2)&RUTRIANGLE);
          break;
     }
     return result;
}

int CTrapezium::mvLeft(){
     bool  stopped;

     switch ( direction ) {
            case HUTRAPEZIUM:
                 stopped = (board.getRim(x-2,y)&RDTRIANGLE)|(board.getRim(x-1,y)&LUTRIANGLE);
                 break;
            case VRTRAPEZIUM:
                 stopped = board.getRim(x-1,y-1)|board.getRim(x-1,y)|board.getRim(x-1,y+1);
                 break;
            case HDTRAPEZIUM:
                 stopped = (board.getRim(x-2,y)&RUTRIANGLE)|(board.getRim(x-1,y)&LDTRIANGLE);
                 break;
            case VLTRAPEZIUM:
                 stopped = board.getRim(x-1,y);
                 stopped = stopped || ((board.getRim(x,y+1)&LUTRIANGLE)|(board.getRim(x-1,y+1)&RDTRIANGLE));
                 stopped = stopped || ((board.getRim(x,y-1)&LDTRIANGLE)|(board.getRim(x-1,y-1)&RUTRIANGLE));
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
int CTrapezium::mvRight(){
     bool  stopped;

     switch ( direction ) {
            case HUTRAPEZIUM:
                 stopped = (board.getRim(x+2,y)&LDTRIANGLE)|(board.getRim(x+1,y)&RUTRIANGLE);
                 break;
            case VRTRAPEZIUM:
                 stopped = board.getRim(x+1,y);
                 stopped = stopped || ((board.getRim(x,y+1)&RUTRIANGLE)|(board.getRim(x+1,y+1)&LDTRIANGLE));
                 stopped = stopped || ((board.getRim(x,y-1)&RDTRIANGLE)|(board.getRim(x+1,y-1)&LUTRIANGLE));
                 break;
            case HDTRAPEZIUM:
                 stopped = (board.getRim(x+2,y)&LUTRIANGLE)|(board.getRim(x+1,y)&RDTRIANGLE);
                 break;
            case VLTRAPEZIUM:
                 stopped = board.getRim(x+1,y-1)|board.getRim(x+1,y)|board.getRim(x+1,y+1);
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

int CTrapezium::rotateLeft(){
     bool  stopped=true;
     char lPixel, rPixel;
     
     switch ( direction ) {
            case HUTRAPEZIUM:
                 if ( board.getRim(x-1,y-1) ) break;
                 if ( board.getRim(x,y-1)) break;
                 if ( board.getRim(x+1, y+1) ) break;
                 if ( board.getRim(x+1,y)&RUTRIANGLE) break;
                 if ( board.getRim(x,y+1)&RDTRIANGLE ) break;
                 stopped = false;
                 break;
            case VRTRAPEZIUM:
                 if ( board.getRim(x-1,y+1) ) break;
                 if ( board.getRim(x-1,y)) break;
                 if ( board.getRim(x, y-1)&RDTRIANGLE ) break;
                 if ( board.getRim(x+1,y-1)) break;
                 if ( board.getRim(x+1,y)&LDTRIANGLE ) break;
                 stopped = false;
                 break;
            case HDTRAPEZIUM:
                 if ( board.getRim(x+1,y+1) ) break;
                 if ( board.getRim(x,y+1)) break;
                 if ( board.getRim(x-1, y)&LDTRIANGLE ) break;
                 if ( board.getRim(x-1,y-1)) break;
                 if ( board.getRim(x,y-1)&LUTRIANGLE ) break;
                 stopped = false;
                 break;
            case VLTRAPEZIUM:
                 if ( board.getRim(x+1,y-1) ) break;
                 if ( board.getRim(x+1,y)) break;
                 if ( board.getRim(x, y+1)&LUTRIANGLE ) break;
                 if ( board.getRim(x-1,y+1)) break;
                 if ( board.getRim(x-1,y)&RUTRIANGLE ) break;
                 stopped = false;
                 break;
            default:
                 return UNDEFINED;
     } 
     if ( stopped ) return mvDown();
     erase();
     lPixel = direction>>4;
     rPixel = direction&0xF;
     rPixel = ((rPixel&7)<<1)|(rPixel>>3);
     lPixel = ((lPixel&7)<<1)|(lPixel>>3);
     direction = (rPixel<<4)|lPixel;
     show();
     return RT_LEFT;
}

int CTrapezium::rotateRight(){
     return RT_RIGHT;
}


void  CTrapezium::show(){
     board<<CTriangle(x, y, color, LUTRIANGLE)<<CTriangle(x, y, color, RDTRIANGLE);;
     switch ( direction ) {
            case HUTRAPEZIUM:
                 board<<CTriangle(x-1, y, color, RDTRIANGLE);
                 board<<CTriangle(x+1, y, color, LDTRIANGLE);
                 break;
            case HDTRAPEZIUM:
                 board<<CTriangle(x-1, y, color, RUTRIANGLE);
                 board<<CTriangle(x+1, y, color, LUTRIANGLE);
                 break;
            case VRTRAPEZIUM:
                 board<<CTriangle(x, y-1, color, LUTRIANGLE);
                 board<<CTriangle(x, y+1, color, LDTRIANGLE);
                 break;
            case VLTRAPEZIUM:
                 board<<CTriangle(x, y-1, color, RUTRIANGLE);
                 board<<CTriangle(x, y+1, color, RDTRIANGLE);
                 break;
            default:
                 break;
     }
     
     return;
}

void  CTrapezium::erase(){
     board>>CTriangle(x, y,0, LUTRIANGLE)>>CTriangle(x, y,0, RDTRIANGLE);
     switch ( direction ) {
            case HUTRAPEZIUM:
                 board>>CTriangle(x-1, y,0, RDTRIANGLE);
                 board>>CTriangle(x+1, y,0, LDTRIANGLE);
                 break;
            case VRTRAPEZIUM:
                 board>>CTriangle(x, y+1,0, LDTRIANGLE);
                 board>>CTriangle(x, y-1,0, LUTRIANGLE);
                 break;
            case HDTRAPEZIUM:
                 board>>CTriangle(x-1, y,0, RUTRIANGLE);
                 board>>CTriangle(x+1, y,0, LUTRIANGLE);
                 break;
            case VLTRAPEZIUM:
                 board>>CTriangle(x, y+1,0, RDTRIANGLE);
                 board>>CTriangle(x, y-1,0, RUTRIANGLE);
                 break;
            default:
                 break;
     } 
     return;
}
*/
