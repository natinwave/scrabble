#ifndef __Trie_INCLUDED__
#define __Trie_INCLUDED__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include "CharArr.h"
using namespace std;

struct Trie {
    map<char, Trie*> children;
    bool isEOW;
    
    bool hasChild(char key);

    bool hasWord(char* input, int length);
    
    void printAll(const string& prefix);
    
    void getAllWords(const string& prefix, vector<string>& results);
    
    void LOP_shift_shell(CharArr board, map<char, int>& rack, vector<CharArr>& output);
    
    void list_of_placements(const CharArr& board_segment, CharArr board, map<char, int>& rack, vector<CharArr>& output, int position, bool attached, bool uses_rack, int start);
    
    void insert(char* input, int length);
    
    Trie();
    
    void read_in_dict();
    
    ~Trie();
};
#endif