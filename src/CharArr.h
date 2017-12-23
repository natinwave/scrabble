#ifndef __Arr_INCLUDED__
#define __Arr_INCLUDED__

#include <iostream>
#include <cstdlib>
using namespace std;

// This is a basic char array class, that allows copying and assignment. I made it to store char arrays
// in a vector, since vectors require their containers to be copyable and assignable and stuff.
class CharArr {
    char * head;
    int amtFilled;
    int length;
    public:
        CharArr();
        
        CharArr(int len);
        
        CharArr(string input);
        
        CharArr(const CharArr& that);
    
        ~CharArr();
    
        int size() const;
        
        int amtFull() const;
        
        bool push_back(char input);
    
        string substring(int start, int end) const;
    
        char operator[](int index);
        
        void assign(int index, char input);
        
        char at(int index) const;
    
        CharArr& operator=(const CharArr& newArr);
        
        CharArr operator+(char input) const;
        
        bool operator==(const CharArr& input);
        
        bool operator!=(const CharArr& input);
        
        friend ostream& operator<<(ostream &strm, const CharArr &arr);
    
};

#endif