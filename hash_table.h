#ifndef HASH_TABLE
#define HASH_TABLE

#include <bits/stdc++.h>
#include "hash_functions.h"
#include "storage_node.h"

using namespace std;

class HashTable {
    int hashSpace;
    int virtualizationFactor;
    map<int, StorageNode*> hashTable;
    map<string, StorageNode*> setOfNodes;
    HashFunction* hashFunction;
    int ip_last_num;
    vector<int> primes;
    bool debug;

    void fillPrimes(){
        //fill sufficient number of primes. These will be multiplied with
        //hash value and mod will be taken with hashSpace to get different values for each
        //virtual instance of the node.
        vector<bool> nums (100, true);
        nums[0]=false;
        nums[1]=false;
        for(int i=2; i<100; i++){
            if(nums[i]==true){
                primes.push_back(i);
                for(int j=i*i; j<100; j+=i)
                    nums[j]=false;
            }
        }
        return;
    }

    void TransferDataWhileAdding(StorageNode* from, StorageNode* to, StorageNode* prev, int hashedKeyOfFromNode, int hashedKeyOfToNode, int hashedKeyOfPrevNode){
        if (debug){
            cout << "ADD " << from->GetName() << " " << hashedKeyOfFromNode << " " << to->GetName() << " " << hashedKeyOfToNode << " " << prev->GetName() << " " << hashedKeyOfPrevNode << endl;
        }
        map<int, set<int> > dataPoints = from->GetDataPoints();
        if(dataPoints.size()==0)
            return;
        map<int, set<int> >::iterator dataPointsIterator = dataPoints.begin();
        while(dataPointsIterator != dataPoints.end()){
            int dataHash = dataPointsIterator->first;
            if(hashedKeyOfPrevNode < hashedKeyOfToNode){
                if(dataHash >= hashedKeyOfPrevNode && dataHash < hashedKeyOfToNode){
                    int s = dataPointsIterator->second.size();
                    set<int>::iterator actualDP = dataPointsIterator->second.begin();
                    while(actualDP != dataPointsIterator->second.end()){
                        to->AddDataToNode(dataPointsIterator->first, *actualDP);
                        from->RemoveDataFromNode(dataPointsIterator->first, *actualDP);
                        actualDP++;
                    }
                }
            } else {
                if((dataHash >= hashedKeyOfPrevNode && dataHash < hashSpace) || (dataHash >= 0 && dataHash < hashedKeyOfToNode)){
                    int s = dataPointsIterator->second.size();
                    set<int>::iterator actualDP = dataPointsIterator->second.begin();
                    while(actualDP != dataPointsIterator->second.end()){
                        to->AddDataToNode(dataPointsIterator->first, *actualDP);
                        from->RemoveDataFromNode(dataPointsIterator->first, *actualDP);
                        actualDP++;
                    }
                }
            }
            dataPointsIterator++;
        }
    }

    void TransferDataWhileRemoving(StorageNode* from, StorageNode* to, StorageNode* prev, int hashedKeyOfFromNode, int hashedKeyOfToNode, int hashedKeyOfPrevNode){
        if (debug){
            cout << "REMOVE " << from->GetName() << " " << hashedKeyOfFromNode << " " << to->GetName() << " " << hashedKeyOfToNode << " " << prev->GetName() << " " << hashedKeyOfPrevNode << endl;
        }
        map<int, set<int> > dataPoints = from->GetDataPoints();
        if(dataPoints.size()==0)
            return;
        map<int, set<int> >::iterator dataPointsIterator = dataPoints.begin();
        while(dataPointsIterator != dataPoints.end()){
            int dataHash = dataPointsIterator->first;
            if(hashedKeyOfPrevNode < hashedKeyOfFromNode){
                if(dataHash >= hashedKeyOfPrevNode && dataHash <= hashedKeyOfFromNode){
                    int s = dataPointsIterator->second.size();
                    set<int>::iterator actualDP = dataPointsIterator->second.begin();
                    while(actualDP != dataPointsIterator->second.end()){
                        to->AddDataToNode(dataPointsIterator->first, *actualDP);
                        from->RemoveDataFromNode(dataPointsIterator->first, *actualDP);
                        actualDP++;
                    }
                }
            } else {
                if((dataHash >= hashedKeyOfPrevNode && dataHash < hashSpace) || (dataHash >= 0 && dataHash <= hashedKeyOfFromNode)){
                    int s = dataPointsIterator->second.size();
                    set<int>::iterator actualDP = dataPointsIterator->second.begin();
                    while(actualDP != dataPointsIterator->second.end()){
                        to->AddDataToNode(dataPointsIterator->first, *actualDP);
                        from->RemoveDataFromNode(dataPointsIterator->first, *actualDP);
                        actualDP++;
                    }
                }
            }
            dataPointsIterator++;
        }
    }


    public:
        HashTable(int hashSpace, int virtualizationFactor, HashFunctionType hashFunctionType, bool debug){
            hashTable.clear();
            setOfNodes.clear();
            this->hashSpace = hashSpace;
            this->virtualizationFactor = virtualizationFactor;
            this->debug = debug;
            //Factory method
            this->hashFunction = HashFunction::GetHashFunctionObject(hashFunctionType);
            ip_last_num = 0;
            fillPrimes();
            cout << "Hash Table created. Hash Space = " << hashSpace << ", virtualization factor = " << virtualizationFactor << ", hash function type = " << hashFunctionType << endl;
        }

        int getHashSpace(){
            return hashSpace;
        }

        int getVirtualizationFactor(){
            return virtualizationFactor;
        }

        void PrintTable(){
            cout << "Printing Table" << endl;
            for(map<int, StorageNode*>::iterator hashedNodes = hashTable.begin(); hashedNodes != hashTable.end(); hashedNodes++){
                cout << "\tKey: " << hashedNodes->first << " , node name: " << hashedNodes->second->GetName() << endl;
            }
            cout << "Now print data in each node" << endl;
            for(map<string, StorageNode*>::iterator nodes = setOfNodes.begin(); nodes != setOfNodes.end(); nodes++){
                cout << "\tNode name: " << nodes->first << endl;
                nodes->second->printData();
            }
            return;
        }

        void AddStorageNode(string nodeName){
            //currently not supporting collision resolution for hash nodes.
            //get hash of node.
            int hashedValue = hashFunction->getHashedValue(nodeName);
            //create node, and create 'virtualizationFactor' number of pointers and enter into hash table
            int newIP = ip_last_num;
            ip_last_num++;
            string nodeIp = "127.0.0." + to_string(newIP);
            StorageNode* node = new StorageNode(nodeName, nodeIp);
            bool ableToAdd = false;
            for(int i=0; i<virtualizationFactor; i++){
                int hashedKey = (hashedValue * primes[i]) % hashSpace;
                if(hashTable.find(hashedKey)!=hashTable.end())
                    cout << "Collision while inserting virtual instance of node. Skipping this." << endl;
                else{
                    ableToAdd = true;
                    hashTable[hashedKey] = node;
                    //Reorganize keys that should have been in this server (take from all other virtual servers on right)
                    //get node on right
                    map<int, StorageNode*>::iterator currentNodeIterator = hashTable.find(hashedKey);
                    map<int, StorageNode*>::iterator forwardNodeIterator = hashTable.upper_bound(hashedKey);
                    map<int, StorageNode*>::iterator previousNodeIterator = hashTable.lower_bound(hashedKey);
                    if(forwardNodeIterator == hashTable.end())
                        forwardNodeIterator = hashTable.begin();
                    if(previousNodeIterator == hashTable.begin())
                        previousNodeIterator = --hashTable.end();
                    else
                        previousNodeIterator--;

                    if(forwardNodeIterator->second != node){
                        //we only need to reorganize when forwardNode is not the same as currentnode.
                        TransferDataWhileAdding(forwardNodeIterator->second, node, previousNodeIterator->second, forwardNodeIterator->first, hashedKey, previousNodeIterator->first);
                    }
                }
            }
            if(ableToAdd){
                setOfNodes[nodeName] = node;
            } else {
                //resetting
                ip_last_num--;
                delete(node);
                return;
            }
            cout << "Storage node added!" << endl;
            return;
        }

        void RemoveStorageNode(string nodeName){
            //get hash of node, take mod with hash space.
            int hashedValue = hashFunction->getHashedValue(nodeName);            
            StorageNode* node = setOfNodes[nodeName];
            if(setOfNodes.size()==1){
                hashTable.clear();
                setOfNodes.clear();
                delete(node);
                cout << "Storage node removed!" << endl;
                return;
            }
            //Reorganize keys, transfering them to server to right.
            for(int i=0; i<virtualizationFactor; i++){
                int hashedKey = (hashedValue * primes[i]) % hashSpace;
                if(hashTable.find(hashedKey)!=hashTable.end() && hashTable[hashedKey] == node){
                    //collision might have caused hashedKey to not have the current node. 
                    map<int, StorageNode*>::iterator forwardNodeIterator = hashTable.upper_bound(hashedKey);
                    map<int, StorageNode*>::iterator previousNodeIterator = hashTable.lower_bound(hashedKey);
                    if(forwardNodeIterator == hashTable.end())
                        forwardNodeIterator = hashTable.begin();
                    if(previousNodeIterator == hashTable.begin())
                        previousNodeIterator = --hashTable.end();
                    else
                        previousNodeIterator--;
                    if(forwardNodeIterator->second != node){
                        TransferDataWhileRemoving(node, forwardNodeIterator->second, previousNodeIterator->second, hashedKey, forwardNodeIterator->first, previousNodeIterator->first);
                    }
                    //remove all virtual nodes associated with it from hash table.
                    hashTable.erase(hashedKey);
                }
            }
            //Also remove from set of nodes.
            setOfNodes.erase(nodeName);
            delete(node);
            cout << "Storage node removed!" << endl;
            return;
        }

        void AddDataPoint(int data){
            if(setOfNodes.size()==0){
                cout << "No data server added, can't add data." << endl;
                return;
            }
            //Get hash of data point, take mod with hash space. 
            int hashKey = (hashFunction->getHashedValue(data))%hashSpace;
            //Add data point to node to the right.
            map<int, StorageNode*>::iterator it = hashTable.upper_bound(hashKey);
            if(it == hashTable.end())
                it = hashTable.begin();
            it->second->AddDataToNode(hashKey, data);
            return;
        }

        void RemoveDataPoint(int data){
            //Get hash of data point, take mod with hash space.
            if(setOfNodes.size()==0){
                cout << "No data served added. So no data points present." << endl;
                return;
            }
            int hashKey = (hashFunction->getHashedValue(data))%hashSpace;
            //Search and remove data point from node to the right.
            map<int, StorageNode*>::iterator it = hashTable.upper_bound(hashKey);
            if(it == hashTable.end())
                it = hashTable.begin();
            it->second->RemoveDataFromNode(hashKey, data);
            return;
        }
};

#endif
