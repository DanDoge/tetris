#include "typedef.h"
/*
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
*/
CPlayer::CPlayer(CBoard& board_):board(board_) {
      hTimer = 0;
      pDisplay = NULL;
      pBrick = NULL;
      command = UNDEFINED;
      return; 
}

void CPlayer::init(HWND hWnd_) {
     
     score = 0;
     hTimer = 0;
     hWnd = hWnd_;
     gameFilePath[0] = 0;
     pDisplay = new CDisplay(hWnd);
     board.init(pDisplay);
     return; 
}

void CPlayer::repaint() {
     
     pDisplay->refresh(score);
     board.repaint();     
     return; 
}

void CPlayer::newGame() {
     /*************************************************************************/
     /*                                                                       */
     /*      添加代码，清楚当前游戏进度，重置为开始玩游戏的状态               */
     /*                                                                       */
     /*************************************************************************/
     score = 0;	 
	 if(pBrick){
	 	pBrick->erase();
	 	delete pBrick;
	 }
	 for(vector<CBrick*>::iterator i = board.vecPTRBricks.begin(); i != board.vecPTRBricks.end(); i += 1){
      	(*i)->erase();
	 }// compile error if this code is put in board.clear()
     int wellclear = board.clear();
     if(!wellclear){
     	return ;
	 };

     
     pBrick = CBrick::newBrick();//newBrick();
     if ( pBrick==NULL ) return;
     command = UNDEFINED;
     if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
     hTimer = SetTimer(hWnd, TIMER_ID, MV_SPEED, NULL);
     pDisplay->show(score);

     return;      
}

void CPlayer::executeCommand() {
     short result;
     char  str_gameover[] = "Game Over";
     if (pBrick==NULL) return;
     switch ( command ) {
     case MV_LEFT:
          result = pBrick->mvLeft();
          break;
     case MV_RIGHT:
          result = pBrick->mvRight();
          break;
     case RT_LEFT:
          result = pBrick->rotateLeft();
          break;
     case RT_RIGHT:
          result = pBrick->rotateRight();
          break;
     default:
          result = pBrick->mvDown(command);
          break;     
     }
     command = UNDEFINED;
     if ( result != STOPPED ) return;
     
     score += board.tryclear(pBrick);
     repaint();
     pBrick = CBrick::newBrick();
     if ( pBrick==NULL ) pDisplay->show(str_gameover);
     return;
}

CPlayer::~CPlayer() {
      if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
      if (pDisplay != NULL) delete pDisplay;
      if (pBrick!=NULL) delete pBrick;
      return; 
}

bool CPlayer::clickButton(LPARAM hButton) {
     
     command = pDisplay->parseCommand(hButton);
     switch ( command ) {
            case NEW_GAME:
                 newGame();
                 break;
            case SAVE_GAME:
                 saveGame();
                 break;
            case OPEN_GAME:
                 openGame();
                 break;
            case EXIT_GAME:
                 return false;
            default:
                    break;
     }
     return true;
}

void CPlayer::saveGame() {
	
	if(pBrick){
    	int result = pBrick->mvDown(command);
     	score += board.tryclear(pBrick);
     	repaint();
	}
	
     OPENFILENAME ofnGame;
          
     if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
         
     ZeroMemory(&ofnGame, sizeof(ofnGame));
     ofnGame.lStructSize = sizeof(ofnGame);
     ofnGame.hwndOwner = hWnd;
     ofnGame.lpstrFilter = "brick-game files (*.brk)\0*.brk\0\0";
     ofnGame.lpstrFile = gameFilePath;
     ofnGame.nMaxFile = MAX_PATH;
     ofnGame.lpstrDefExt = "brk";
     ofnGame.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY |
         OFN_OVERWRITEPROMPT;

     while (gameFilePath[0] == 0) 
         if (GetOpenFileName(&ofnGame)==false) {
         hTimer = SetTimer(hWnd, TIMER_ID, 500, NULL);
         return;
         }
     ofstream gameFile(gameFilePath, ios::out);
     /*************************************************************************/
     /*                                                                       */
     /*           添加代码，将游戏进度保存到文件"gameFile"中                  */
     /*                                                                       */
     /*************************************************************************/
     
    gameFile << score << endl;
    for(int i = 0; i < 31; i += 1){
     	for(int j = 0; j < 13; j += 1){
     		gameFile << board.pixels[i][j] << ' ';
		}
		gameFile << endl;
	}
	for(int i = 0; i < 31; i += 1){
		gameFile << board.rowStates[i].blankTriangles << ' ' << board.rowStates[i].linked << endl;
	}
	gameFile << endl;
	vector<CBrick*>::iterator it = board.vecPTRBricks.begin();
	gameFile << board.vecPTRBricks.size() << endl;
	while(it != board.vecPTRBricks.end()){
		gameFile << (*it)->x << ' ' << (*it)->y << ' ' << (*it)->color << ' ' << (*it)->direction << ' ' << (*it)->score << endl;
		it++;
	}
    gameFile.close();	
	
	if(pBrick){
		delete pBrick;
	}
	pBrick = CBrick::newBrick();
	command = UNDEFINED;
    if ( pBrick==NULL ) return;
    pDisplay->show(score);
    
	hTimer = SetTimer(hWnd, TIMER_ID, 500, NULL);
     return;
}

void CPlayer::openGame() {
	
	score = 0;	 
	if(pBrick){
	 	pBrick->erase();
	 	delete pBrick;
	}
    int wellclear = board.clear();
    if(!wellclear){
     	return ;
	};
	
     OPENFILENAME ofnGame;
     
     if ( hTimer!=0 ) KillTimer(hWnd, TIMER_ID);
      
     ZeroMemory(&ofnGame, sizeof(ofnGame));
     ofnGame.lStructSize = sizeof(ofnGame);
     ofnGame.hwndOwner = hWnd;
     ofnGame.lpstrFilter = "brick-game files (*.brk)\0*.brk\0\0";
     ofnGame.lpstrFile = gameFilePath;
     ofnGame.nMaxFile = MAX_PATH;
     ofnGame.lpstrDefExt = "brk";
     ofnGame.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;

     gameFilePath[0] = 0;
     while (gameFilePath[0] == 0) 
         if (GetOpenFileName(&ofnGame)==false) {
         hTimer = SetTimer(hWnd, TIMER_ID, 500, NULL);
         return;
         }
         
     ifstream gameFile(gameFilePath, ios::in);
     /*************************************************************************/
     /*                                                                       */
     /*           添加代码，从文件"gameFile"中读入游戏进度                    */
     /*                                                                       */
     /*************************************************************************/
     
    gameFile >> score;
    for(int i = 0; i < 31; i += 1){
     	for(int j = 0; j < 13; j += 1){
     		gameFile >> board.pixels[i][j];
		}
	}
	for(int i = 0; i < 31; i += 1){
		gameFile >> board.rowStates[i].blankTriangles >> board.rowStates[i].linked;
	}
	int size = 0;
	gameFile >> size;
	vector<CBrick*>::iterator it;
	while(size--){
		int x, y, score;
		unsigned char color, direction;
		gameFile >> x >> y >> color >> direction >> score;
		switch(direction){
			case SQUARE:
				board.vecPTRBricks.push_back(new CSquare(x, y, color, direction));
				it = board.vecPTRBricks.end() - 1;
				break;
     		case HRECTANGLE:
     		case VRECTANGLE:
     			board.vecPTRBricks.push_back(new CRectangle(x, y, color, direction));
     			it = board.vecPTRBricks.end() - 1;
				break;
     		case LUTRIANGLE:
    		case RUTRIANGLE:
     		case RDTRIANGLE:
     		case LDTRIANGLE:
     			board.vecPTRBricks.push_back(new CTriangle(x, y, color, direction));
     			it = board.vecPTRBricks.end() - 1;
				break;
     		case HUTRAPEZIUM:
     		case HDTRAPEZIUM:
     		case VRTRAPEZIUM:
     		case VLTRAPEZIUM:
     			board.vecPTRBricks.push_back(new CTrapezium(x, y, color, direction));
     			it = board.vecPTRBricks.end() - 1;
     			break;
		}
		
		(*it)->score = score;
		(*it)->show();
	}
	gameFile.close();
	
	pBrick = CBrick::newBrick();
	command = UNDEFINED;
    if ( pBrick==NULL ) return;
    pDisplay->show(score);

	hTimer = SetTimer(hWnd, TIMER_ID, 500, NULL);
     return;
}
