#include "typedef.h"

/*
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
       void refresh();
       int  parseCommand(LPARAM hButton) const;
       bool show(int x, int y, int triangle) const;
       bool erase(int x, int y, int triangle) const;
       void show(int arg) const; 
       void show(char arg[])const; 
       void show(CBrick& arg) const;
       void show(SRowState arg[]) const; 
};
*/


CDisplay::CDisplay(HWND hWnd_) {
     int i;
     RECT  rcWin, rcClient;//, rcDisplay, rcScore;
     int   bnX, bnY;
     HINSTANCE hInst;// = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);
      
     colorRef[0] = 0x00000000;
     colorRef[1] = 0x00FF0000;
     colorRef[2] = 0x0000FF00;
     colorRef[3] = 0x000000FF;
     colorRef[4] = 0x00FFFF00;
     colorRef[5] = 0x0000FFFF;
     colorRef[6] = 0x00FFFFFF;
     hWnd = hWnd_;
      
     GetWindowRect(GetDesktopWindow(), &rcWin);
     SetWindowPos(hWnd, HWND_TOP,0,0,rcWin.right,rcWin.bottom, (UINT)NULL);
     /*
      BOOL WINAPI SetWindowPos(
      __in      HWND hWnd,
      __in_opt  HWND hWndInsertAfter,
      __in      int left_side,
      __in      int top,
      __in      int width,
      __in      int height,
      __in      UINT uFlags
      );
      */
     GetClientRect(hWnd, &rcClient);
     boardLegend = rcClient.bottom/32;
     
     winWidth = 14*boardLegend + BUTTON_WIDTH*2 + BUTTON_SPACE +
               (rcWin.right - rcClient.right );
     winHeight = 32*boardLegend + rcWin.bottom - rcClient.bottom;
     
     boardRect.top = boardLegend;
     boardRect.left  = boardLegend;
     boardRect.bottom  = 31*boardLegend; 
     boardRect.right = 12*boardLegend;
     scoreRect.left = boardRect.right+boardLegend;
     scoreRect.right = boardRect.right + boardLegend+BUTTON_SPACE+BUTTON_WIDTH*2;
     scoreRect.top = boardRect.top+BUTTON_SPACE*4+BUTTON_HEIGHT*4;
     scoreRect.bottom = boardRect.bottom-BUTTON_SPACE*3-BUTTON_HEIGHT*3;

     hInst = (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE);

     SetWindowPos(hWnd, HWND_TOP,0,0,winWidth, winHeight, (UINT)NULL);
     bnX = boardRect.right+boardLegend;
     bnY = boardRect.bottom-BUTTON_SPACE-BUTTON_HEIGHT*2;
     hBnNew = CreateWindowEx(0,"BUTTON","New",BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_BORDER,
            bnX,bnY,BUTTON_WIDTH,BUTTON_HEIGHT,hWnd, NULL,hInst,NULL);
     /*
      HWND WINAPI CreateWindowEx(
      __in      DWORD dwExStyle,
      __in_opt  LPCTSTR lpClassName,
      __in_opt  LPCTSTR lpWindowName,
      __in      DWORD dwStyle,
      __in      int x,
      __in      int y,
      __in      int nWidth,
      __in      int nHeight,
      __in_opt  HWND hWndParent,
      __in_opt  HMENU hMenu,
      __in_opt  HINSTANCE hInstance,
      __in_opt  LPVOID lpParam
      );
      */
     bnX = boardRect.right+boardLegend+BUTTON_SPACE+BUTTON_WIDTH;
     hBnOpen = CreateWindowEx(0,"BUTTON","Open",BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_BORDER,
            bnX,bnY,BUTTON_WIDTH,BUTTON_HEIGHT,hWnd, NULL,hInst,NULL);
     bnX = boardRect.right+boardLegend;
     bnY = boardRect.bottom-BUTTON_HEIGHT;             
     hBnSave = CreateWindowEx(0,"BUTTON","Save",BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_BORDER,
            bnX,bnY,BUTTON_WIDTH,BUTTON_HEIGHT,hWnd, NULL,hInst,NULL);
     bnX = boardRect.right+boardLegend+BUTTON_SPACE+BUTTON_WIDTH;
     hBnExit = CreateWindowEx(0,"BUTTON","Exit",BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_BORDER,
            bnX,bnY,BUTTON_WIDTH,BUTTON_HEIGHT,hWnd, NULL,hInst,NULL);
             
     bnX = boardRect.right+boardLegend;
     bnY = boardRect.top;
     hBnRtAntiClck = CreateWindowEx(0,"BUTTON","rotateAnti-Clk",BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_BORDER,
            bnX,bnY,BUTTON_WIDTH,BUTTON_HEIGHT,hWnd, NULL,hInst,NULL);
     bnX = boardRect.right+boardLegend+BUTTON_SPACE+BUTTON_WIDTH;
     hBnRtClck = CreateWindowEx(0,"BUTTON","rotateClk",BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_BORDER,
            bnX,bnY,BUTTON_WIDTH,BUTTON_HEIGHT,hWnd, NULL,hInst,NULL);
     bnX = boardRect.right+boardLegend;
     bnY = boardRect.top+BUTTON_SPACE+BUTTON_HEIGHT;
     hBnMvLeft = CreateWindowEx(0,"BUTTON","moveLeft",BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_BORDER,
            bnX,bnY,BUTTON_WIDTH,BUTTON_HEIGHT,hWnd, NULL,hInst,NULL);
     bnX = boardRect.right+boardLegend+BUTTON_SPACE+BUTTON_WIDTH;
     hBnMvRight = CreateWindowEx(0,"BUTTON","moveRight",BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD|WS_BORDER,
            bnX,bnY,BUTTON_WIDTH,BUTTON_HEIGHT,hWnd, NULL,hInst,NULL);
     bnX = boardRect.right+boardLegend;
     bnY = boardRect.top+BUTTON_SPACE*2+BUTTON_HEIGHT*2;
     hBnMvDown = CreateWindowEx(0,"BUTTON","moveDown",BS_PUSHBUTTON|WS_VISIBLE|WS_CHILD,
            bnX,bnY,BUTTON_WIDTH*2+BUTTON_SPACE,BUTTON_HEIGHT,hWnd, NULL,hInst,NULL);
      
    return; 
}

void CDisplay::refresh(int score) {
     int i;
     char str_score[] = "SCORE";
     RECT  rect;
     hDC  = GetDC(hWnd);
     Rectangle(hDC, scoreRect.left, scoreRect.top, scoreRect.right, scoreRect.bottom);
     rect = scoreRect;
     rect.top = scoreRect.top + BUTTON_SPACE; //scoreRect.top += BUTTON_SPACE;
     rect.bottom = scoreRect.bottom - BUTTON_SPACE;//scoreRect.bottom -= BUTTON_SPACE;
     DrawTextEx(hDC,str_score, -1, &rect, DT_CENTER|DT_TOP, NULL);//DrawTextEx(hDC,"SCORE", -1, &scoreRect, DT_CENTER|DT_TOP, NULL);

     Rectangle(hDC, boardRect.left, boardRect.top, boardRect.right+1, boardRect.bottom);
     for ( i=0; i<30; i++ ) {
          MoveToEx(hDC,boardRect.left,boardRect.top+i*boardLegend,NULL);
          LineTo(hDC,boardRect.right,boardRect.top+i*boardLegend);
     } 
     for ( i=0; i<11; i++ ) {
          MoveToEx(hDC,boardRect.left+i*boardLegend,boardRect.top,NULL);
          LineTo(hDC,boardRect.left+i*boardLegend,boardRect.bottom);
    }
    
    show(score);
    return; 
}

int CDisplay::parseCommand(LPARAM hButton)const {
     if ( hButton == (LPARAM)hBnExit) return EXIT_GAME;
     if ( hButton == (LPARAM)hBnNew) return NEW_GAME;
     if ( hButton == (LPARAM)hBnSave) return SAVE_GAME;
     if ( hButton == (LPARAM)hBnOpen) return OPEN_GAME;
     if ( hButton == (LPARAM)hBnMvDown) return MV_DOWN;
     if ( hButton == (LPARAM)hBnMvLeft) return MV_LEFT;
     if ( hButton == (LPARAM)hBnMvRight) return MV_RIGHT;
     if ( hButton == (LPARAM)hBnRtAntiClck) return RT_LEFT;
     if ( hButton == (LPARAM)hBnRtClck) return RT_RIGHT;
     return UNDEFINED;
}

CDisplay::~CDisplay() {
     ReleaseDC(hWnd, hDC);
}

void CDisplay::show(CBrick& arg) const{
     char strBrick[MAX_PATH];
     sprintf(strBrick, "x=%d y=%d\ndirection=0x%X  score=%d", arg.x, arg.y, arg.direction, arg.score);
     show(strBrick);
}

void CDisplay::show(char arg[]) const{
     RECT strRect = scoreRect;
     strRect.top = (strRect.top+strRect.bottom)/2;
     strRect.left += BUTTON_SPACE;
     strRect.right -= 1;
     HRGN hrgn=CreateRectRgn(strRect.left, strRect.top, strRect.right, strRect.bottom);
     HBRUSH hbr = CreateSolidBrush(colorRef[6]);      
     FillRgn( hDC, hrgn,hbr);
     DeleteObject(hbr);
     DeleteObject(hrgn);
     DrawTextEx(hDC,arg, -1, &strRect,DT_CENTER|DT_WORDBREAK, NULL);
}

void CDisplay::show(int arg) const{
     RECT intRect = scoreRect;
     char strInt[MAX_PATH];
     intRect.bottom = (intRect.top+intRect.bottom)/2;
     intRect.top = (intRect.top+intRect.bottom)/2;
     intRect.left += BUTTON_SPACE;
     intRect.right -= BUTTON_SPACE;
     HRGN hrgn=CreateRectRgn(intRect.left, intRect.top, intRect.right, intRect.bottom);
     HBRUSH hbr = CreateSolidBrush(colorRef[6]);      
     FillRgn( hDC, hrgn,hbr);
     DeleteObject(hbr);
     DeleteObject(hrgn);
     sprintf(strInt, "%d", arg);
     DrawTextEx(hDC,strInt, -1, &intRect,DT_CENTER|DT_WORDBREAK, NULL);
}

void CDisplay::show(SRowState arg[]) const{
     RECT rect;
     char str[MAX_PATH], str_null[]="   ";
     
     for ( int i=1; i<=30; i++ ) {
         rect.top = boardRect.bottom - i*boardLegend;
         rect.bottom = rect.top + boardLegend;
         rect.left = boardRect.left-boardLegend;
         rect.right = boardRect.left;
         sprintf(str, "%d", arg[i].blankTriangles);
         DrawTextEx(hDC,str_null, -1, &rect,DT_CENTER|DT_WORDBREAK, NULL);
         DrawTextEx(hDC,str, -1, &rect,DT_CENTER|DT_WORDBREAK, NULL);
         
         rect.left = boardRect.left+boardLegend*11+1;
         rect.right = rect.left+boardLegend-1;
         if (arg[i].linked != 0) sprintf(str, "l");
         else sprintf(str, "u");
         DrawTextEx(hDC,str_null, -1, &rect,DT_CENTER|DT_WORDBREAK, NULL);
         DrawTextEx(hDC,str, -1, &rect,DT_CENTER|DT_WORDBREAK, NULL);
     }
}        

bool CDisplay::show(int x, int y, int triangle) const { 
      int color = (triangle & 0xF0)>>4;
      if (color==0 || color>COLOR_NUM ) return false;
      if ( x<1 || x>11 ) return false;
      if ( y<1 || y>30 ) return false;
      return fillTriangle(color, x, y, triangle);      
}
bool CDisplay::erase(int x, int y, int triangle) const {
      if ( x<1 || x>11 ) return false;
      if ( y<1 || y>30 ) return false;
      if (fillTriangle(6, x, y, triangle)==false) return false;

      switch ( triangle&0xF ) {
      case LUTRIANGLE:
           MoveToEx(hDC,boardRect.left + x*boardLegend, boardRect.bottom - y*boardLegend,NULL);
           LineTo(hDC,boardRect.left + (x-1)*boardLegend,boardRect.bottom - y*boardLegend);
           LineTo(hDC,boardRect.left + (x-1)*boardLegend,boardRect.bottom - (y-1)*boardLegend);
           break;
      case LDTRIANGLE:
           MoveToEx(hDC,boardRect.left + (x-1)*boardLegend, boardRect.bottom - y*boardLegend,NULL);
           LineTo(hDC,boardRect.left + (x-1)*boardLegend,boardRect.bottom - (y-1)*boardLegend);
           LineTo(hDC,boardRect.left + x*boardLegend,boardRect.bottom - (y-1)*boardLegend);
           break;
      case RUTRIANGLE:
           MoveToEx(hDC,boardRect.left + (x-1)*boardLegend, boardRect.bottom - y*boardLegend,NULL);
           LineTo(hDC,boardRect.left + x*boardLegend,boardRect.bottom - y*boardLegend);
           LineTo(hDC,boardRect.left + x*boardLegend,boardRect.bottom - (y-1)*boardLegend);
           break;
      case RDTRIANGLE:
           MoveToEx(hDC,boardRect.left + x*boardLegend, boardRect.bottom - y*boardLegend,NULL);
           LineTo(hDC,boardRect.left + x*boardLegend,boardRect.bottom - (y-1)*boardLegend);
           LineTo(hDC,boardRect.left + (x-1)*boardLegend,boardRect.bottom - (y-1)*boardLegend);
           break;
      }      

      return true;
}

bool CDisplay::fillTriangle(int color, int x, int y, int triangle) const {
      POINT vTriangle[3];

      switch ( triangle&0xF ) {
      case LUTRIANGLE:
           vTriangle[0].x = boardRect.left + (x-1)*boardLegend;
           vTriangle[0].y = boardRect.bottom - (y)*boardLegend;
           vTriangle[1].x = boardRect.left + (x)*boardLegend;
           vTriangle[1].y = boardRect.bottom - (y)*boardLegend;
           vTriangle[2].x = boardRect.left + (x-1)*boardLegend;
           vTriangle[2].y = boardRect.bottom - (y-1)*boardLegend;           
           break;
      case LDTRIANGLE:
           vTriangle[0].x = boardRect.left + (x-1)*boardLegend;
           vTriangle[0].y = boardRect.bottom - (y-1)*boardLegend;
           vTriangle[1].x = boardRect.left + (x-1)*boardLegend;
           vTriangle[1].y = boardRect.bottom - (y)*boardLegend;
           vTriangle[2].x = boardRect.left + (x)*boardLegend;
           vTriangle[2].y = boardRect.bottom - (y-1)*boardLegend;
           break;
      case RUTRIANGLE:
           vTriangle[0].x = boardRect.left + (x)*boardLegend;
           vTriangle[0].y = boardRect.bottom - (y)*boardLegend;
           vTriangle[1].x = boardRect.left + (x-1)*boardLegend;
           vTriangle[1].y = boardRect.bottom - (y)*boardLegend;
           vTriangle[2].x = boardRect.left + (x)*boardLegend;
           vTriangle[2].y = boardRect.bottom - (y-1)*boardLegend;
           break;
      case RDTRIANGLE:
           vTriangle[0].x = boardRect.left + (x)*boardLegend;
           vTriangle[0].y = boardRect.bottom - (y-1)*boardLegend;
           vTriangle[1].x = boardRect.left + (x)*boardLegend;
           vTriangle[1].y = boardRect.bottom - (y)*boardLegend;
           vTriangle[2].x = boardRect.left + (x-1)*boardLegend;
           vTriangle[2].y = boardRect.bottom - (y-1)*boardLegend;
           break;
      default:
           return false;
      }
      HBRUSH hbr = CreateSolidBrush(colorRef[color]);
      HRGN hrgn = CreatePolygonRgn(vTriangle, 3, WINDING);
      FillRgn( hDC, hrgn, hbr);
      DeleteObject(hbr);
      DeleteObject(hrgn);
      return true;

}
