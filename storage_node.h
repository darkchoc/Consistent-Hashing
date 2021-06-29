#ifndef STORAGE_NODE
#define STORAGE_NODE

#include <bits/stdc++.h>
using namespace std;

class StorageNode{
    map<int, set<int> > dataPoints; // the collision is resolved by chaining, although duplicate dataPoints are not allowed
    string nodeName;
    string ip;
    public:
        StorageNode(string nodeName, string allotedIP){
            this->nodeName = nodeName;
            this->ip = allotedIP;
        }

        void printData(){
            cout << "\t\tPrinting data in node " << nodeName << " : " << ip << endl;
            for(map<int, set<int> >::iterator it=dataPoints.begin(); it!=dataPoints.end(); it++){
                cout << "\t\t\tHash value: " << it->first << ", DataPoints: ";
                for(set<int>::iterator it2=(it->second).begin(); it2!=(it->second).end(); it2++){
                    cout << (*it2) << " ";
                }
                cout << endl;
            }
            cout << endl;
        }

        void AddDataToNode(int hashValue, int data){
            dataPoints[hashValue].insert(data);
            cout << "Added data=" << data << " to node " << nodeName << endl; 
            return;
        }

        void RemoveDataFromNode(int hashValue, int data){
            if(dataPoints.find(hashValue)==dataPoints.end())
                cout << "Data=" << data << " not present in node " << nodeName << endl;
            else {
                dataPoints[hashValue].erase(data);
                if(dataPoints[hashValue].size() == 0)
                    dataPoints.erase(hashValue);
                cout << "Data=" << data << " removed from node " << nodeName << endl;
            }
            return;
        }

        map<int, set<int> > & GetDataPoints(){
            return dataPoints;
        }

        string GetName(){
            return nodeName;
        }

        string GetIp(){
            return ip;
        }
};

#endif