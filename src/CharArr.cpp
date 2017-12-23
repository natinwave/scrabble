 #include "CharArr.h"
 #include <vector>
 using namespace std;
 
// This is a basic char array class, that allows copying and assignment. I made it to store char arrays
// in a vector, since vectors require their containers to be copyable and assignable and stuff.

CharArr::CharArr() {
    length = 0;
    amtFilled = 0;
    head = 0;
}

CharArr::CharArr(int len){
    head = new char[len];
    length = len;
    amtFilled = 0;
    for (int i = 0; i < len; i++) {
        *(head + i) = '-';
    }
}

CharArr::CharArr(string input) {
    head = new char[input.length()];
    length = input.length();
    amtFilled = length;
    for (int i = 0; i < length; i++) {
        *(head + i) = input.at(i);
    }
}

CharArr::CharArr(const CharArr& that) {
    head = new char[that.size()];
    length = that.size();
    amtFilled = that.amtFull();
    for (int i = 0; i < length; i++) {
        head[i] = that.at(i);
    }
}

CharArr::~CharArr() {
    delete head;
}
 
bool CharArr::push_back(char input) {
    if (amtFilled == length) {
        return false;
    }
    *(head + amtFilled) = input;
    amtFilled++;
    return true;
}

string CharArr::substring(int start, int end) const {
    string output;
    for (int i = start; i < end; i++) {
        output += *(head + i);
    }
    return output;
}

char CharArr::operator[](int index) {
    if (index > length) {
        return 0;
    }
    return *(&(*(head + index)));    
}

void CharArr::assign(int index, char input) {
    if (index > length || index < 0) {
        cout << "assign: aborted " << endl;
        return;
    }
    *(head + index) = input;
    if (amtFilled <= index) amtFilled = index + 1;
}

char CharArr::at(int index) const {
    if (index > length) {
        return 0;
    }
    return *(head + index);    
}

int CharArr::size() const {
    return length;
}

int CharArr::amtFull() const {
    return amtFilled;
}

CharArr& CharArr::operator=(const CharArr& newArr) {
    if (this != &newArr) {
        if (newArr.size() != length) {
            length = newArr.size();
            char* temp = new char[length];
            delete head;
            head = temp;
        }
        amtFilled = newArr.amtFull();
        for (int i = 0; i < length; i++) {
            *(head + i) = newArr.at(i);
        }
    }
    return *this;
}

CharArr CharArr::operator+(char input) const {
    CharArr output = CharArr(length + 1);

    for (int i = 0; i < amtFilled; i++) {
        output.assign(i, this->at(i));
    }
    output.push_back(input);
    return output;
}

bool CharArr::operator==(const CharArr& input) {
    if (length != input.size()) {
        return false;
    }
    if (amtFilled != input.amtFull()) {
        return false;
    }
    for (int i = 0; i < length; i++) {
        if (*(head + i) != input.at(i)) {
            return false;
        }
    }
    return true;
}

bool CharArr::operator!=(const CharArr& input) {
    return !(*this == input);
}

ostream& operator<<(ostream &strm, const CharArr &arr) {
    string output;
    for (int i = 0; i < arr.size(); i++) {
        output.push_back(arr.at(i));
    }
    return strm << output;
}

/*
int main() {
    CharArr letters = CharArr(7);
    letters.push_back('a');
    letters.push_back('b');
    letters.push_back('c');
    letters.push_back('d');
    letters.push_back('e');
    letters.push_back('f');
    letters.push_back('g');
    
    CharArr let = CharArr(6);
    let.push_back('q');
    let.push_back('w');
    let.push_back('e');
    let.push_back('r');
    let.push_back('t');
    let.push_back('y');
    
    vector<CharArr> vec;
    vec.push_back(letters);
    vec.push_back(let);
    
    cout << "first entry:" << endl;
    for (int i = 0; i < 7; i++) {
        cout << vec[0][i];
    }
    cout << endl;
    
    cout << "second entry:" << endl;
    for (int i = 0; i < 6; i++) {
        cout << vec[1][i];
    }
    cout << endl;
    
    vec[1] = letters;
    CharArr third = CharArr(letters);
    vec.push_back(third);
    cout << "first entry:" << endl;
    for (int i = 0; i < 7; i++) {
        cout << vec[0][i];
    }
    cout << endl;
    
    cout << "second entry new:" << endl;
    for (int i = 0; i < vec[2].size(); i++) {
        cout << vec[2].at(i);
    }
    cout << endl;
    char p = 'P';
    cout << letters + p << endl;
    
}*/
