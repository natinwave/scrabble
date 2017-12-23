#ifndef __Scrabble_INCLUDED__   
#define __Scrabble_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include "Trie.h"
#include "CharArr.h"
using namespace std;

struct Scrabble {
    Trie dict_root;
    map<char, int> rack;
    char board[15][15];
    map<char, int> letter_values;
    vector<vector<CharArr> > verticals;
    vector<vector<CharArr> > horizontals;
    vector<poss_Move> Moves;
    
        Scrabble();
        
        ~Scrabble();
    
        void read_in_board();
    
        void populate_h_v();

};

#endif