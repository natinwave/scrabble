#ifndef __PossMove_INCLUDED__   
#define __PossMove_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <map>
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
#endif