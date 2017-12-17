#include <windows.h>
#include <windowsx.h>
#include <vector>
#include <fstream>   
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

using namespace std;

#define UNDEFINED  0xFF
#define STOPPED    0
#define MV_DOWN    1
#define MV_RIGHT   2
#define MV_LEFT    3
#define RT_RIGHT   4
#define RT_LEFT    5
#define NEW_GAME   10
#define OPEN_GAME  11
#define SAVE_GAME  12
#define EXIT_GAME  13
#define SQUARE     0x0F     //square: 0000 1111  
#define HRECTANGLE 0xAA     //horizonal rectangle: 1010 1010
#define VRECTANGLE 0x55     //vertical rectangle: 0101 0101
#define LUTRIANGLE 0x0C     //triangle on the left-up conner: 0000 1100
#define RUTRIANGLE 0x06     //triangle on the right-up conner: 0000 0110
#define RDTRIANGLE 0x03     //triangle on the right-down conner: 0000 0011
#define LDTRIANGLE 0x09     //triangle on the left-down conner: 0000 1001
#define HUTRAPEZIUM 0x39    //horizonal trapezium shrinking up: 0011 1001
#define VRTRAPEZIUM 0xC9    //vertical trapezium shrinking right: 1100 1001
#define HDTRAPEZIUM 0xC6    //horizonal trapezium shrinking down: 1100 0110
#define VLTRAPEZIUM 0x36    //vertical trapezium shrinking left: 0011 0110

#define BUTTON_HEIGHT 35
#define BUTTON_WIDTH 100
#define BUTTON_SPACE 10

#define COLOR_NUM 5
#define SHAPE_NUM 11
#define TIMER_ID  1000
#define MV_SPEED  500

class CBrick;
class CTriangle;
class CPlayer;
struct SRowState {
       int blankTriangles;
       int linked;
};

class CDisplay {
      HWND hWnd,
           hBnNew, //child-windown for new-button
           hBnSave, //child-windown for save-button
           hBnOpen, //child-windown for open-button           
           hBnExit, //child-windown for exit-button           
           hBnMvLeft, //child-windown for mvLeft-button
           hBnMvRight, //child-windown for mvRight-button
           hBnMvDown, //child-windown for mvDown-button
           hBnRtClck, //child-windown for rotate-clockwise-button
           hBnRtAntiClck;//child-windown for rotate-anti-clockwise-button

      HDC  hDC;
      COLORREF colorRef[COLOR_NUM+7];      
      RECT  boardRect, scoreRect;
      int   winWidth, winHeight, boardLegend;
      bool fillTriangle(int color, int x, int y, int triangle) const;
public:
       CDisplay(HWND hWnd_);
       ~CDisplay();
       void refresh(int score);
       int  parseCommand(LPARAM hButton) const;
       bool show(int x, int y, int triangle) const;
       bool erase(int x, int y, int triangle) const;
       void show(int arg) const; 
       void show(char arg[])const; 
       void show(SRowState arg[]) const; 
       void show(CBrick& arg) const;
};

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
       
       int tryclear(CBrick*);
       int clear();
       
friend class CPlayer;
};

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
friend class CBoard;
friend class CPlayer;
friend bool compare_by_y(const CBrick*, const CBrick*);
friend int CBoard::tryclear(CBrick* pBrick);
};

class CPlayer {
      int  score;
      char gameFilePath[MAX_PATH];
      CDisplay* pDisplay;
      CBoard&   board;
      
      HWND hWnd; //main window 
      UINT_PTR hTimer;
      int  command;
      CBrick* pBrick;//CTrapezium* pTrapezium;
      void newGame();
      void saveGame();
      void openGame();
public:
       CPlayer(CBoard& board_);
       void init(HWND hWnd_);
       void repaint();
       bool clickButton(LPARAM hButton);
       void executeCommand();
       ~CPlayer();
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

class CSquare:public CBrick{
	virtual void erase();
	virtual void show();
	virtual bool isProped();
friend class CBoard;
public:
	CSquare(short x_, short y_, char color_, unsigned char direction_);
	virtual int mvLeft();
	virtual int mvRight();
	virtual int rotateLeft();
	virtual int rotateRight();
};

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

class CTrapezium:public CBrick{
	virtual void erase();
	virtual void show();
	virtual bool isProped();
friend class CBoard;
public:
	CTrapezium(short x_, short y_, char color, unsigned char direction_);
	virtual int mvLeft();
	virtual int mvRight();
	virtual int rotateLeft();
	virtual int rotateRight();
};
