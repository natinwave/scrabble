#include "Trie.h"
#include "CharArr.h"
using namespace std;

bool Trie::hasChild(char key){
    map<char, Trie*>::iterator result = children.find(key);
    return result != children.end();      
}

bool Trie::hasWord(char* input, int length){
    if (length == 0){
        return isEOW;
    }
    
    if (!hasChild(input[0])){
        return false;
    }
    
    return (children[input[0]])->hasWord(input+1, length-1);
}
    
void Trie::printAll(const string& prefix) {
    if (isEOW){
        cout << prefix << endl;
    }
    
    for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
        it->second->printAll(prefix + it->first);
    }
}

void Trie::getAllWords(const string& prefix, vector<string>& results){
    if (isEOW){
        results.push_back(prefix);
    }
        
    for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++){
        it->second->getAllWords(prefix + it->first, results);
    }
}
    
void Trie::LOP_shift_shell(CharArr board, CharArr last_board, CharArr next_board, map<char, int>& rack, vector<CharArr>& output) {
    // This function runs once, and runs list_of_placements multiple times, to get the possible words
    // you could make starting from different points on the board.
    CharArr board_segment = CharArr("");
    int i = 0;
    int start = 0;
    // The maximum number of letter chunks possible on the board is 8. Each one is dealt with 
    // separately; i is incremented throughout to divide the letter chunks. 
    for (int k = 0; k < 8; k++) { 
        // the first while loop runs through until it hits a letter; i indicates the position of 
        // the first letter on the board it finds, starting from either the beginning (start == 0)
        // or from the end of the last letter chunk (start == i as we go into subsequent loops).
        // this is important because we only want to find possible words until this point; past it, the
        // words would no longer be attached to that letter chunk and we consider the next chunk.
        while (board[i] == '-' && i < 15) i++;
        for (int j = start; j <= i; j++) {
            this->list_of_placements(board_segment, board, last_board, next_board, rack, output, j, false, false, j);
        }
        // Moves i to the end of the letter chunk
        while (board[i] != '-' && i < 15) i++;
        // adds one to i to prevent '--k-pp---'' from becoming '--kapple-'' later on
        i++;
        start = i;
    }
}

void Trie::list_of_placements(const CharArr& board_segment, CharArr board, CharArr last_board, CharArr next_board, map<char, int>& rack, vector<CharArr>& output, int position, bool attached, bool uses_rack, int start) {
    // isEOW- see Trie diagram in Andy's notes. Indicates if it's a complete word or not.
    // attached- attached becomes true on line 93, once the word you're making runs into a letter
    //           on the board. before that, I don't want them.
    // board[position] - if there's a chunk of many letters on the board, I need to keep going 
    //                   until I see a '-' in the next spot. e.g., if there's a --tr-- on the board, 
    //                   I can't say that catr-- is fine just because 'cat' is a word.
    // position == 15 - an exception to the prior case; if you're at the edge of the board, it's ok if
    //                  there's not a '-' in the next spot. 
    if (isEOW && attached && uses_rack && ((board[position] == '-') || position == 15)) {
        // makes a new output_segment, and sets it equal to the board.
        CharArr output_segment = board;
        // assigns the word you've found into the output_segment.
        for (int i = 0; i < board_segment.size(); i++) {
            output_segment.assign(i + start, board_segment.at(i));
        }
        // This next if-else pair is to prevent duplicate output. since duplicate output happens 
        // consecutively, I can just check if it equals the last entry to decide if I should add it.

        if (output.empty()) {
            output.push_back(output_segment);
        }
        else {
            if (output_segment != output.back()) {
                output.push_back(output_segment);
            }
        }
    }
    // If you're at the end of the board, it won't look for more letters.
    if (position == 15) return;
    // If the board[position] is a '-', that means I can populate it with any letter from the rack.
    if (board[position] == '-') {
        uses_rack = true;
        // The iterator starts at the first child (Trie diagram is helpful here), and moves until 
        // it gets to the last child.
        for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++) {
            if (rack.find(it->first) != rack.end() && rack[it->first] > 0) {
                // removes the letter you just used when it does the recursive call.
                // like how you would put down a letter on the board to try making a word, and 
                // when you do that you temporarily remove that letter from your rack bc you can't
                // use the same tile twice.
                rack[it->first] -=1;
                // Board_segment + it->first --means-- prefix of word we're making + latest valid character = new prefix for the next layer.
                it->second->list_of_placements(board_segment + it->first, board, rack, output, position + 1, attached, uses_rack, start);
                rack[it->first] +=1;
            }
        }
    }
    // If board[position] is a letter, that means you only have one option for growth: check if there's a next
    // child that matches the letter on the board, and if there's not, then that's the end of the road.
    else if (last_board.amtFull() != 0 && last_board[position] > 58) {
        attached = true;
    }
    else if (next_board.amtFull() != 0 && next_board[position] > 58) {
        attached = true;
    }
    else {
        attached = true;
        for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++) {
            // it->first is the char value of a child, it->second is a pointer to that child.
            if (it->first == board[position]) {
                it->second->list_of_placements(board_segment + board[position], board, rack, output, position + 1, attached, uses_rack, start);
            }
        }
    }
}

void Trie::insert(char* input, int length){
    if (length == 0){
        isEOW = true;
        return;
    }
    
    if (!hasChild(input[0])){
        Trie* newTrie = new Trie();
        children[input[0]] = newTrie;
    }
        (children[input[0]])->insert(input+1, length-1);
}

Trie::Trie() {
    isEOW = false;
}

void Trie::read_in_dict() {
    string key;
    // READING DICTIONARY INTO TRIE
    ifstream myReadFile;
    myReadFile.open("dict1.txt");
    while (!myReadFile.eof()) {
        myReadFile >> key;
        char charArr[key.length()];
        for (int i = 0; i < key.length(); i++) {
            charArr[i] = key.at(i);
        }
        this->insert(&charArr[0], key.length());
    }
    
    myReadFile.close();
    ifstream myReadFile2;
    myReadFile2.open("dict2.txt");
    while (!myReadFile2.eof()) {
        myReadFile2 >> key;
        char charArr2[key.length()];
        for (int i = 0; i < key.length(); i++) {
            charArr2[i] = key.at(i);
        }
        this->insert(&charArr2[0], key.length());
    }
    myReadFile2.close();
}

Trie::~Trie() {
    for(map<char, Trie*>::iterator it = children.begin(); it != children.end(); it++) {
        delete it->second;
    }
}

/*
int main() {
    Trie root;
    root.read_in_dict();
    
    map<char, int> rack;
    rack['e'] = 1;
    rack['s'] = 1;
    rack['a'] = 1;
    rack['l'] = 1;
    rack['p'] = 2;
    rack['t'] = 1;
    // output is the vector containing all the valid placements of words you could make with your rack.
    vector<CharArr> output;
    // Creating a sample row that might be taken from the board; blanks are dashes.
    CharArr board = CharArr(15);
    board.push_back('b');
    for (int i = 0; i < 7; i++) {
        board.push_back('-');
    }
    board.push_back('p');
    board.push_back('-');
    board.push_back('e');
    board.push_back('a');
    for (int i = 0; i < 3; i++) {
        board.push_back('-');
    }
    // Adds the sample row to the output so you can see what you start with.
    output.push_back(board);
    // prints out the letters you have in your rack.
    cout << "letters in your rack: " << endl;
    for(map<char, int>::iterator it = rack.begin(); it != rack.end(); it++) {
        for (int i = 0; i < (it->second); i++) {
            cout << it->first;
        }
    }
    cout << endl;
    
    // LOP_shift_shell is the important function, that takes in the rack, sample board, and the output vector.
    // It is not recursive; it's a "shell" to run the real recursive list_of_placements function.
    root.LOP_shift_shell(board, rack, output);
    
    // Prints the output, which is a vector full of the possible board placements.
    cout << "First line is the board segment you begin with." << endl;
    for(vector<CharArr>::iterator it = output.begin(); it!= output.end(); it++) {
        for (int i = 0; i < (*it).size(); i++) {
            cout << (*it)[i];
        }
        cout << endl;
    }
}
*/