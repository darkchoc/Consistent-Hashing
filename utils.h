#ifndef UTILS
#define UTILS

#include <bits/stdc++.h>
using namespace std;

vector<string> tokenize(string input, char delimiter){
    vector<string> tokens;
    string word = "";
    for(int i=0; i<input.length(); i++){
        if(input[i]==delimiter){
            if(word.length() > 0)
                tokens.push_back(word);
            word = "";
        } else {
            word += input[i];
        }
    }
    if(word.length() > 0)
        tokens.push_back(word);
    return tokens;
}

#endif