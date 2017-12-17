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
*/
CBrick::CBrick(short x_, short y_, char color_, unsigned char direction_){
     x = x_;
     y = y_;
     color = color_ % (COLOR_NUM+1);
     direction = direction_;
     return;
}
void CBrick::stop(){
     return;
}

int CBrick::mvDown(int command){
    int mvSteps=0;
    while ( isProped()==false ) {
          mvSteps++;
          if ( mvSteps==1 ) erase();
          y--;
          if (command==UNDEFINED) break;
    } 
    score += mvSteps;
    if ( mvSteps==0 ) return STOPPED;
    show();
    return MV_DOWN;
}

int CBrick::rotateLeft(){
     return mvDown();
}

int CBrick::rotateRight(){
     return mvDown();
}


CBrick* CBrick::newBrick(){
     CBrick* result=NULL;     
     int color = rand()%COLOR_NUM + 1;
     unsigned short curBrickType = rand()%SHAPE_NUM;
     
     if ( board.getRim(6, 30)&LUTRIANGLE&RUTRIANGLE ) return result;
     switch ( brickType[curBrickType] ) {
     case SQUARE:
     	if(board.getRim(6, 30)){
     		return newBrick();
		 }
		 result = new CSquare(6, 30, color, brickType[curBrickType]);
		 break;
     case HRECTANGLE:
     	if(board.getRim(6, 30) || board.getRim(5, 30) || board.getRim(7, 30)){
     		return newBrick();
		 }
		 result = new CRectangle(6, 30, color, brickType[curBrickType]);
		 break;
     case VRECTANGLE:
          if(board.getRim(6, 30) || board.getRim(6, 29) || board.getRim(6, 28)){
          	return newBrick();
		  }
		  result = new CRectangle(6, 29, color, brickType[curBrickType]);
          break;
     case LUTRIANGLE:
     case RUTRIANGLE:
     case RDTRIANGLE:
     case LDTRIANGLE:
          if ( board.getRim(6, 30)&brickType[curBrickType] ) return newBrick();
          result = new CTriangle(6, 30, color, brickType[curBrickType]);
          break;          
     case HUTRAPEZIUM:
     	if(board.getRim(6, 30) || (board.getRim(5, 30) & RDTRIANGLE) || (board.getRim(7, 30) & LDTRIANGLE)){
     		return newBrick();
		}
		result = new CTrapezium(6, 30, color, brickType[curBrickType]);
		break;
     case HDTRAPEZIUM:
     	if(board.getRim(6, 30) || (board.getRim(5, 30) & RUTRIANGLE) || (board.getRim(7, 30) & LUTRIANGLE)){
     		return newBrick();
		}
		result = new CTrapezium(6, 30, color, brickType[curBrickType]);
		break;
     case VRTRAPEZIUM:
     	if((board.getRim(6, 30) & LDTRIANGLE) || board.getRim(6, 29) || (board.getRim(6, 28) & LUTRIANGLE)){
     		return newBrick();
		}
		result = new CTrapezium(6, 29, color, brickType[curBrickType]);
		break;
     case VLTRAPEZIUM:
     	if((board.getRim(6, 30) & RDTRIANGLE) || board.getRim(6, 29) || (board.getRim(6, 28) & RUTRIANGLE)){
     		return newBrick();
		}
		result = new CTrapezium(6, 29, color, brickType[curBrickType]);
		break;
     default:
           break;
     }
     if (result!=NULL) result->show();
     return result;
}

CBrick::~CBrick() {
}


CSquare::CSquare(short x_, short y_, char color_, unsigned char direction_):CBrick(x_,y_,color_,direction_){
	if(x_ < 1 || x_ > 11){
		x = 6;
	}
	if(y_ < 1 || y_ > 30){
		y = 30;
	}
	if(direction_ != SQUARE){
		x = 0;
		y = 0;
		color = 0;
		direction = 0;
	}
}
int CSquare::mvLeft(){
	bool stopped = board.getRim(x - 1, y);
	if(stopped){
		return mvDown();
	}
	erase();
	x--;
	show();
	return MV_LEFT;
}
int CSquare::mvRight(){
	bool stopped = board.getRim(x + 1, y);
	if(stopped){
		return mvDown();
	}
	erase();
	x++;
	show();
	return MV_RIGHT;
}
int CSquare::rotateLeft(){
	return RT_LEFT;
}
int CSquare::rotateRight(){
	return RT_RIGHT;
}
void CSquare::show(){
	board<<*(new CTriangle(x, y, color, LUTRIANGLE))<< *(new CTriangle(x, y, color, RDTRIANGLE));
}
void CSquare::erase(){
	board>>*(new CTriangle(x, y, color, LUTRIANGLE)) >> *(new CTriangle(x, y, color, RDTRIANGLE));
}
bool CSquare::isProped(){
	bool stopped = board.getRim(x, y - 1);
	return stopped;
}

/*
class CRectangle:public CBrick{
	virtual void erase();
	virtual void show();
	virtual bool isProped();
friend class CBoard;
public:
	CRectangle(short x_, short y_, char color, unsigned char direction_);
	virtual int mvLeft();
	virtual int mvRight();
	virtual int rotateLeft();
	virtual int rotateRight();
};
*/

CRectangle::CRectangle(short x_, short y_, char color_, unsigned char direction_):CBrick(x_,y_,color_,direction_){
	if(x_ < 1 || x_ > 11){
		x = 6;
	}
	if(y_ < 1 || y_ > 30){
		y = 30;
	}
	if(direction_ != HRECTANGLE && direction_ != VRECTANGLE){
		x = 0;
		y = 0;
		color = 0;
		direction = 0;
	}
}

void CRectangle::erase(){
	CTriangle* parray[6];
	switch(direction){
		case HRECTANGLE:
			for(int i = 0; i < 3; i += 1){
				parray[2 * i] = new CTriangle(x - 1 + i, y, color, LUTRIANGLE);
				parray[2 * i + 1] = new CTriangle(x - 1 + i, y, color, RDTRIANGLE);
			}
			for(int i = 0; i < 6; i += 1){
				board >> *parray[i];
			}
			break;
		case VRECTANGLE:
			for(int i = 0; i < 3; i += 1){
				parray[2 * i] = new CTriangle(x, y - 1 + i, color, LUTRIANGLE);
				parray[2 * i + 1] = new CTriangle(x, y - 1 + i, color, RDTRIANGLE);
			}
			for(int i = 0; i < 6; i += 1){
				board >> *parray[i];
			}
			break;
	}
	for(int i = 0; i < 6; i += 1){
		delete parray[i];
	}
}

void CRectangle::show(){
	CTriangle* parray[6];
	switch(direction){
		case HRECTANGLE:
			for(int i = 0; i < 3; i += 1){
				parray[2 * i] = new CTriangle(x - 1 + i, y, color, LUTRIANGLE);
				parray[2 * i + 1] = new CTriangle(x - 1 + i, y, color, RDTRIANGLE);
			}
			for(int i = 0; i < 6; i += 1){
				board << *parray[i];
			}
			break;
		case VRECTANGLE:
			for(int i = 0; i < 3; i += 1){
				parray[2 * i] = new CTriangle(x, y - 1 + i, color, LUTRIANGLE);
				parray[2 * i + 1] = new CTriangle(x, y - 1 + i, color, RDTRIANGLE);
			}
			for(int i = 0; i < 6; i += 1){
				board << *parray[i];
			}
			break;
	}
	for(int i = 0; i < 6; i += 1){
		delete parray[i];
	}
}

bool CRectangle::isProped(){
	switch(direction){
		case HRECTANGLE:
			return board.getRim(x - 1, y - 1) || board.getRim(x, y - 1) || board.getRim(x + 1, y - 1);
		case VRECTANGLE:
			return board.getRim(x, y - 2);
	}
}

int CRectangle::mvLeft(){
	bool stopped;
	switch(direction){
		case HRECTANGLE:
			stopped = board.getRim(x - 2, y);
			break;
		case VRECTANGLE:
			stopped = board.getRim(x - 1, y + 1) || board.getRim(x - 1, y) || board.getRim(x - 1, y - 1);
			break;
		default:
			return UNDEFINED;
	}
	if(stopped){
		return mvDown();
	}
	erase();
	x--;
	show();
	return MV_LEFT;
}

int CRectangle::mvRight(){
	bool stopped;
	switch(direction){
		case HRECTANGLE:
			stopped = board.getRim(x + 2, y);
			break;
		case VRECTANGLE:
			stopped = board.getRim(x + 1, y + 1) || board.getRim(x + 1, y) || board.getRim(x + 1, y - 1);
			break;
		default:
			return UNDEFINED;
	}
	if(stopped){
		return mvDown();
	}
	erase();
	x++;
	show();
	return MV_LEFT;
}

int CRectangle::rotateLeft(){
	bool stopped;
	switch(direction){
		case HRECTANGLE:
			stopped = board.getRim(x + 1, y + 1) || board.getRim(x, y + 1) || board.getRim(x - 1, y + 1)
				|| board.getRim(x - 1, y - 1) || board.getRim(x, y - 1) || board.getRim(x + 1, y - 1);
				break;
		case VRECTANGLE:
			stopped = board.getRim(x + 1, y + 1) || board.getRim(x + 1, y) || board.getRim(x + 1, y - 1)
				|| board.getRim(x - 1, y + 1) || board.getRim(x - 1, y) || board.getRim(x - 1, y - 1);
				break;
	}
	if(stopped){
		return mvDown();
	}
	erase();
	direction = 0xFF - direction;
	show();
	return RT_LEFT;
}

int CRectangle::rotateRight(){
	bool stopped;
	switch(direction){
		case HRECTANGLE:
			stopped = board.getRim(x + 1, y + 1) || board.getRim(x, y + 1) || board.getRim(x - 1, y + 1)
				|| board.getRim(x - 1, y - 1) || board.getRim(x, y - 1) || board.getRim(x + 1, y - 1);
				break;
		case VRECTANGLE:
			stopped = board.getRim(x + 1, y + 1) || board.getRim(x + 1, y) || board.getRim(x + 1, y - 1)
				|| board.getRim(x - 1, y + 1) || board.getRim(x - 1, y) || board.getRim(x - 1, y - 1);
				break;
	}
	if(stopped){
		return mvDown();
	}
	erase();
	direction = 0xFF - direction;
	show();
	return RT_RIGHT;
}


// test
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
	bool stopped = true;
	char lPixel = ' ';
	char rPixel = ' ';

	switch (direction){
		case HUTRAPEZIUM: 
			if(board.getRim(x + 1, y - 1)){
				break;
			}
			if(board.getRim(x, y - 1)){
				break;
			}
			if(board.getRim(x - 1, y) & LUTRIANGLE){
				break;
			}
			if(board.getRim(x - 1, y + 1)){
				break;
			}
			if(board.getRim(x, y + 1) & LDTRIANGLE){
				break;
			}
			stopped = false;//没有卡住 
			break;
		case VRTRAPEZIUM:
			if(board.getRim(x - 1, y - 1)){
				break;
			}
			if(board.getRim(x - 1, y)){
				break;
			}
			if(board.getRim(x, y + 1) & RUTRIANGLE){
				break;
			}
			if(board.getRim(x + 1, y + 1)){
				break;
			}
			if(board.getRim(x + 1, y) & LUTRIANGLE){
				break;
			}
			stopped = false;
			break;
		case HDTRAPEZIUM:
			if(board.getRim(x - 1, y + 1)){
				break;
			}
			if(board.getRim(x, y + 1)){
				break;
			}
			if(board.getRim(x + 1, y) & RDTRIANGLE){
				break;
			}
			if(board.getRim(x + 1, y - 1)){
				break;
			}
			if(board.getRim(x, y - 1) & RUTRIANGLE){
				break;
			}
			stopped = false;
			break;
		case VLTRAPEZIUM:
			if(board.getRim(x - 1, y - 1)){
				break;
			}
			if(board.getRim(x, y - 1) & LDTRIANGLE){
				break;
			}
			if(board.getRim(x - 1, y) & RDTRIANGLE){
				break;
			}
			if(board.getRim(x + 1, y + 1)){
				break;
			}
			if(board.getRim(x + 1, y)){
				break;
			}
			stopped = false;
			break;
		default:
			return UNDEFINED;
	}
	if(stopped){
		return mvDown();//卡住，尝试下移 
	}
	erase();
	lPixel = direction >> 4;//取左字节 
	rPixel = direction & 0xF;//取右字节 
	rPixel = (rPixel >> 1)|((rPixel & 1) << 3);//前三位后移，第四位置首 
	lPixel = (lPixel >> 1)|((lPixel & 1) << 3);
	direction = (rPixel << 4) | lPixel;//左右对调 
	show();
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


