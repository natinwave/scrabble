#include "Trie.h"
#include "CharArr.h"
#include "Scrabble.h"
using namespace std;

Scrabble::Scrabble() {
    read_in_board();
    dict_root.read_in_dict();
    horizontals.resize(15);
    verticals.resize(15);
    letter_values['a'] = 1;
    letter_values['b'] = 3;
    letter_values['c'] = 3;
    letter_values['d'] = 2;
    letter_values['e'] = 1;
    letter_values['f'] = 4;
    letter_values['g'] = 2;
    letter_values['h'] = 4;
    letter_values['i'] = 1;
    letter_values['j'] = 8;
    letter_values['k'] = 5;
    letter_values['l'] = 1;
    letter_values['m'] = 3;
    letter_values['n'] = 1;
    letter_values['o'] = 1;
    letter_values['p'] = 3;
    letter_values['q'] = 10;
    letter_values['r'] = 1;
    letter_values['s'] = 1;
    letter_values['t'] = 1;
    letter_values['u'] = 1;
    letter_values['v'] = 4;
    letter_values['w'] = 4;
    letter_values['x'] = 8;
    letter_values['y'] = 4;
    letter_values['z'] = 10;
    letter_values['*'] = 0;
    
}

Scrabble::~Scrabble() {
    
}

void Scrabble::read_in_board() {
    string key;
    ifstream myReadFile;
    myReadFile.open("board.txt");
    myReadFile >> key;
    for (int i = 0; i < key.length(); i++) {
        char lower = key.at(i);
        if (key.at(i) > 64 && key.at(i) < 91) {
            lower += 32;
        }
        rack[lower]++;
    }
    for (int i = 0; i < 15; i++) {
        myReadFile >> key;
        for (int j = 0; j < 15; j++) {
            char formatted = key.at(j);
            if (key.at(j) > 64 && key.at(j) < 91) {
                formatted += 32;
            }
            board[i][j] = formatted;
        }
    }
    myReadFile.close();
}

void Scrabble::populate_h_v() {
    //Uses the List of Placements function in Trie.cpp to first populate the horizontal and vertical vectors.
    for (int i = 0; i < 15; i++) {
        CharArr curr_row = CharArr(15);
        CharArr curr_column = CharArr(15);
        CharArr last_row = CharArr();
        CharArr next_row = CharArr();
        CharArr last_col = CharArr();
        CharArr next_col = CharArr();
        // Fills both the curr_row and curr_column CharArr with the same loop.
        for (int j = 0; j < 15; j++) {
            char row = board[i][j];
            char col = board[j][i];
            if (i > 0) {
                char last_row = board[i - 1][j];
            }
            if (j > 0) {
                char last_col = board[j - 1][i];
            }
            char next_row = board[i + 1][j];
            char next_col = board[j + 1][i];
            //replaces any numbers it finds with dashes, to not screw up the LOP function. Numbers are still available on the board.
            if (row < 97 || row > 122) {
                row = '-';
            }
            if (col < 97 || col > 122) {
                col = '-';
            }
            if (last_row < 97 || last_row > 122) {
                last_row = '-';
            }
            if (last_col < 97 || last_col > 122) {
                last_col = '-';
            }
            if (next_row < 97 || next_row > 122) {
                next_row = '-';
            }
            if (next_col < 97 || next_col > 122) {
                next_col = '-';
            }
            curr_row.push_back(row);
            curr_column.push_back(col);
            curr_row.push_back(row);
            curr_column.push_back(col);
            curr_row.push_back(row);
            curr_column.push_back(col);
        }
        // The first CharArr in each vector is the untouched board.
        horizontals[i].push_back(curr_row);
        verticals[i].push_back(curr_column);
        // Runs LOP function 15 times in all, inserting 15 different vectors of 
        // CharArr word possibilities into the horizontals and verticals attributes.
        dict_root.LOP_shift_shell(curr_row, rack, horizontals[i]);
        dict_root.LOP_shift_shell(curr_column, rack, verticals[i]);
    }
    
    // Checks what sideways words are made from each horizontal placement, and removes ones that don't fit.
    // !!!!!NOTE: There are elements of this next code which will be useful for scoring: isolating 
    // what is "different" about the new placement, and knowing where the "start of the difference" is. 
    int k = 0;
    for (vector<vector<CharArr> >::iterator kit = horizontals.begin(); kit != horizontals.end(); kit++) {
        CharArr board_row = CharArr((*kit)[0]);
        for(vector<CharArr>::iterator it = (*kit).begin(); it != (*kit).end(); it++) {
            
            CharArr curr_row = CharArr(*it);
            string difference = "";
            int start_of_d = 0;
            bool word_hit = false;    
            for (int a = 0; a < (*it).size(); a++) {
                if (board_row[a] != curr_row[a]) {
                    word_hit = true;
                    if (start_of_d == 0) {
                        start_of_d = a;
                    }
                    difference += curr_row[a];
                }
                else if (word_hit && curr_row[a] > 58) {
                    difference += '#';
                }
                else if (word_hit && curr_row[a] < 58) {
                    word_hit = false;
                }
            }
            cout << "board_row: " << board_row << endl;
            cout << "curr_row: " << curr_row << endl;
            cout << "difference: " << difference << endl;
            cout << "start_of_d: " << start_of_d << endl;
            
            int word_length = 0;
            for (int j = 0; j < difference.length(); j++) {
                if (difference.at(j) != '#') {
                    int w = k - 1;
                    // > 58 means it's a letter.
                    if (board[w][start_of_d + j] > 58 && w >= 0) { 
                        
                        string sideways_word = "";
                        
                        while (board[w][start_of_d + j] > 58 && w > 0) {
                            w--;
                        }
                        w++;
                        while (board[w][start_of_d + j] > 58 && w < 15) {
                            sideways_word += board[w][start_of_d + j];
                            w++;
                        }
                        sideways_word += difference.at(j);
                        w++;
                        while (board[w][start_of_d + j] > 58 && w < 15) {
                            sideways_word += board[w][start_of_d + j];
                            w++;
                        }
                        char * side_ptr = new char[sideways_word.length()];
                        for (int s = 0; s < sideways_word.length(); s++) {
                            *(side_ptr + s) = sideways_word.at(s);
                        }
                        // checks if this is a word
                        if (!dict_root.hasWord(side_ptr, sideways_word.length())) {
                            (*kit).erase(it);
                            it--;
                            // If not, must delete this element
                            break;
                        }
                        delete side_ptr;
                    }
                }
            }
        }
        k++;
    }
    
    // Checks what sideways words are made from each vertical placement, and removes ones that don't fit.
    int m = 0;
    for (vector<vector<CharArr> >::iterator mit = verticals.begin(); mit != verticals.end(); mit++) {
        CharArr board_col = CharArr((*mit)[0]);
        for(vector<CharArr>::iterator it = (*mit).begin(); it != (*mit).end(); it++) {
            CharArr curr_col = CharArr(*it);
            string difference = "";
            int start_of_d = 0;
            bool word_hit = false;    
            for (int a = 0; a < (*it).size(); a++) {
                if (board_col[a] != curr_col[a]) {
                    word_hit = true;
                    if (start_of_d == 0) {
                        start_of_d = a;
                    }
                    difference += curr_col[a];
                }
                else if (word_hit && curr_col[a] > 58) {
                    difference += '#';
                }
                else if (word_hit && curr_col[a] < 58) {
                    word_hit = false;
                }
            }

            int word_length = 0;
            for (int j = 0; j < difference.length(); j++) {
                if (difference.at(j) != '#') {
                    int w = k - 1;
                    // > 58 means it's a letter.
                    if (board[start_of_d + j][w] > 58 && w >= 0) { 
                        
                        string sideways_word = "";
                        
                        while (board[start_of_d + j][w] > 58 && w > 0) {
                            w--;
                        }
                        w++;
                        while (board[start_of_d + j][w] > 58 && w < 15) {
                            sideways_word += board[start_of_d + j][w];
                            w++;
                        }
                        sideways_word += difference.at(j);
                        w++;
                        while (board[start_of_d + j][w] > 58 && w < 15) {
                            sideways_word += board[start_of_d + j][w];
                            w++;
                        }
                        char * side_ptr = new char[sideways_word.length()];
                        for (int s = 0; s < sideways_word.length(); s++) {
                            *(side_ptr + s) = sideways_word.at(s);
                        }
                        // checks if this is a word
                        if (!dict_root.hasWord(side_ptr, sideways_word.length())) {
                            (*mit).erase(it);
                            it--;
                            // If not, must delete this element
                            break;
                        }
                        delete side_ptr;
                    }
                }
            }
        }
        m++;
    }
    
}

int main() {
    Scrabble game = Scrabble();
    game.populate_h_v();
    cout << "letters in your rack: " << endl;
    for(map<char, int>::iterator it = game.rack.begin(); it != game.rack.end(); it++) {
        for (int i = 0; i < (it->second); i++) {
            cout << it->first;
        }
    }
    cout << endl;
    
    // Prints the output, which is a vector full of the possible board placements.
    cout << "these are the horizontal possibilities: "<< endl;
    for (int k = 0; k < 15; k++) {
        cout << "row " << k << ": " << endl;
        for(vector<CharArr>::iterator it = game.horizontals[k].begin(); it!= game.horizontals[k].end(); it++) {
            for (int i = 0; i < (*it).size(); i++) {
                cout << (*it)[i];
            }
            cout << endl;
        }
    }
    cout << "these are the vertical possibilities: "<< endl;
    for (int k = 0; k < 15; k++) {
        cout << "column " << k << ": " << endl;
        for(vector<CharArr>::iterator it = game.verticals[k].begin(); it!= game.verticals[k].end(); it++) {
            for (int i = 0; i < (*it).size(); i++) {
                cout << (*it)[i];
            }
            cout << endl;
        }
    }
    cout << "main" << endl;
}
