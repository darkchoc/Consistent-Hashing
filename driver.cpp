#include <bits/stdc++.h>
#include "hash_table.h"
#include "utils.h"
using namespace std;

int main(){
    //Take input
    int hashSpace=100, virtualizationFactor=1;
    HashFunctionType hashFunctionType = Default;
    string hashFunctionTypeString;
    bool debug = false;

    cout << "Enter hash space (between 100 and 10000): " << endl;
    cin >> hashSpace;
    cout << "Enter virtualization factor (between 1 and 10): " << endl;
    cin >> virtualizationFactor;
    cout << "Enter hash function type (among Default, Custom1, Custom2): " << endl;
    cin >> hashFunctionTypeString;
    if(hashFunctionTypeString == "Default")
        hashFunctionType = Default;
    else if(hashFunctionTypeString == "Custom1")
        hashFunctionType = Custom1;
    else if(hashFunctionTypeString == "Custom2")
        hashFunctionType = Custom2;
    else{
        cout << "Invalid Hash Function Type. Terminating!" << endl;
        return 0;
    }
    cout << "Run in Debug mode? (1/0)" << endl;
    cin >> debug;
    cin.ignore();
    
    //Create hash table
    HashTable *ht = new HashTable(hashSpace, virtualizationFactor, hashFunctionType, debug);
 
    //Do operations
    string input;
    while(getline(cin, input)){
        if(input == "")
            break;
        vector<string> tokens = tokenize(input, ' ');
        if(tokens.size()==1)
            ht->PrintTable();
        else if(tokens[1] == "NODE"){
            if(tokens[0] == "ADD")
                ht->AddStorageNode(tokens[2]);
            else
                ht->RemoveStorageNode(tokens[2]);
        } else if(tokens[1] == "DATA") {
            if(tokens[0] == "ADD")
                ht->AddDataPoint(stoi(tokens[2]));
            else 
                ht->RemoveDataPoint(stoi(tokens[2]));
        } else {
            cout << "Given input: (" << input << ") is invalid. Try again!" << endl;
        }
    }
    return 0;
}