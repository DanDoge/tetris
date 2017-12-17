#include "typedef.h"
/*
class CBoard {
      CDisplay* pDisplay;
      unsigned short pixels[31][13]; 
      vector<CBrick*> vecPTRBricks;
      SRowState rowStates[31];
public:
       CBoard();
       ~CBoard();
       void repaint();
       bool init(CDisplay* pDisplay_);
       unsigned short  getRim(short x, short y);
       CBoard& operator<<(CTriangle triangle);
       CBoard& operator>>(CTriangle triangle);
};
*/

CBoard::CBoard() {
      int i;
      for( i=0; i<13; i++ ) pixels[0][i] = 0x030C;
      for( i=1; i<31; i++ ) pixels[i][0] = pixels[i][12] = 0x030C;
      for( int i=0; i<30; i++ )
          for( int j=0; j<11; j++) pixels[i+1][j+1]=0;
      for( i=1; i<31; i++ ) {
           rowStates[i].linked = false;
           rowStates[i].blankTriangles = 22;
      }
      vecPTRBricks.clear();
      pDisplay = NULL;    
}

bool CBoard::init(CDisplay* pDisplay_) {
     if ( pDisplay != NULL ) return false;
     pDisplay = pDisplay_;
     pDisplay->show(rowStates);
     return true;
}

void CBoard::repaint() {
     int i, j;
     
     for(i=1; i<=30; i++) 
         for(j=1; j<=11; j++){
			pDisplay->show(j, i, pixels[i][j]);
			pDisplay->show(j, i, pixels[i][j]>>8);
		}
     pDisplay->show(rowStates);
     
     return;
}


unsigned short CBoard::getRim(short x, short y) {
     if ( x<1 || x>11 || y<1 || y>30 ) return UNDEFINED; 
     return ((pixels[y][x]>>8)|pixels[y][x])&0xF;
}

CBoard& CBoard::operator<<(CTriangle triangle) {
     int x=triangle.x;
     int y=triangle.y;
     unsigned char color = triangle.color;
     unsigned char direction = triangle.direction;
     
     if ( x<1 || x>11 || y<1 || y>30 ) return (*this);
     switch (((pixels[y][x]>>8)|pixels[y][x])&0xF) {
            case 0xF:
                 return (*this);
            case 0:
                 pixels[y][x] = (color<<4)|direction;
                 break;
            default:
                 if (direction&pixels[y][x]&0xF) return (*this);
                 pixels[y][x] = ((color<<4)|direction)|(pixels[y][x]<<8);
                 break;
     }
     if (pDisplay==NULL) return (*this);
     pDisplay->show(x, y, (color<<4)|direction);
     rowStates[y].blankTriangles--;
     return (*this);      
}

CBoard& CBoard::operator>>(CTriangle triangle) {
     int x=triangle.x;
     int y=triangle.y;
     unsigned char direction = triangle.direction;


     if ( x<1 || x>11 || y<1 || y>30 ) return (*this);
     if ( (direction&0xF)== (pixels[y][x]&0xF) ) {
          triangle.color = (pixels[y][x]&0xF0)>>4;
          pDisplay->erase(x, y, direction);
          pixels[y][x]=pixels[y][x]>>8;
     }
     if ( (direction&0xF)== ((pixels[y][x]>>8)&0xF) ) {
          triangle.color = (pixels[y][x]&0xF000)>>12;
          pDisplay->erase(x, y, direction);
          pixels[y][x]=pixels[y][x]&0xFF;
     }
     rowStates[y].blankTriangles++;
     return (*this);
}


bool compare_by_y(const CBrick* obj1, const CBrick* obj2){
	return obj1->y < obj2->y;
}


int CBoard::tryclear(CBrick* pBrick){
	short result = 0;
	vecPTRBricks.push_back(pBrick);
    sort(vecPTRBricks.begin(), vecPTRBricks.end(), compare_by_y);
    
    for(vector<CBrick*>::iterator i = vecPTRBricks.begin(); i != vecPTRBricks.end(); i += 1){
    	if((*i)->direction == VRECTANGLE || (*i)->direction == VRTRAPEZIUM || (*i)->direction == VLTRAPEZIUM){
    		rowStates[(*i)->y + 1].linked |= 1;//01 ==> linked down
    		rowStates[(*i)->y - 1].linked |= 2;//10 ==> linked up
    		rowStates[(*i)->y].linked |= 3;    //10 ==> linked up and down
		}
	}
    
    int upperbound, lowerbound;
    for(int i = 1; i <= 30; i += 1){
		if(rowStates[i].blankTriangles == 0){
			upperbound = i;
			lowerbound = i;
			while((rowStates[upperbound].linked & 2) && (rowStates[upperbound + 1].linked & 1)){
				upperbound += 1;
			}
			while((rowStates[lowerbound].linked & 1) && (rowStates[lowerbound - 1].linked & 2)){
				lowerbound -= 1;
			}
			bool allfilled = true;
			for(int j = lowerbound; j != upperbound + 1; j += 1){
				if(rowStates[j].blankTriangles != 0){
					allfilled = false;
					break;
				}
			}
			if(allfilled == true){
				for(vector<CBrick*>::iterator it = vecPTRBricks.begin(); it != vecPTRBricks.end(); ){
					if((*it)->y <= upperbound && (*it)->y >= lowerbound){
						result += (*it)->score;
						(*it)->erase();
						delete (*it);
						it = vecPTRBricks.erase(it);
					}else{
						it++;
					}
				}
				for(vector<CBrick*>::iterator it = vecPTRBricks.begin(); it != vecPTRBricks.end(); ){
					if((*it)->y > upperbound){
						(*it)->erase();
						(*it)->y -= upperbound - lowerbound + 1;
						(*it)->show();
						it++;
					}else{
						it++;
					}
				}
				break;
			}
		}
	}
	
	for(int i = 0; i < 31; i += 1){
		rowStates[i].linked = 0;
	}
	for(vector<CBrick*>::iterator i = vecPTRBricks.begin(); i != vecPTRBricks.end(); i += 1){
    	if((*i)->direction == VRECTANGLE || (*i)->direction == VRTRAPEZIUM || (*i)->direction == VLTRAPEZIUM){
    		rowStates[(*i)->y - 1].linked |= 1;
    		rowStates[(*i)->y + 1].linked |= 2;
    		rowStates[(*i)->y].linked |= 3;
		}
	}
	return result;
}

int CBoard::clear(){
      for( int i=0; i<13; i++ ) pixels[0][i] = 0x030C;
      for( int i=1; i<31; i++ ) pixels[i][0] = pixels[i][12] = 0x030C;
      for( int i=0; i<30; i++ )
          for( int j=0; j<11; j++) pixels[i+1][j+1]=0;
      for( int i=1; i<31; i++ ) {
           rowStates[i].linked = 0;
           rowStates[i].blankTriangles = 22;
      }
      vecPTRBricks.clear();
      pDisplay->show(rowStates);
      return 1;
}

CBoard::~CBoard() {
         
}
