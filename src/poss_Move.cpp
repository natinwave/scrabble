#include <fstream>
#include "CharArr.h"
using namespace std;


class poss_Move{
    int direction; //0 means vertical, 1 means horizontal
    int score;
    int indexX;
    int indexY;
    CharArr line; 
    
    poss_Move(int d, int x, int y);
    
    ~poss_Move();
    
    
    void updateScore();
    
    
};
