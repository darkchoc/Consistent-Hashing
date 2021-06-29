#ifndef HASH_FUNCTIONS
#define HASH_FUNCTIONS

#include <bits/stdc++.h>
using namespace std;

enum HashFunctionType{
    Default = 1,
    Custom1 = 2,
    Custom2 = 3
};

class HashFunction {
    public:
        virtual int getHashedValue(string str) = 0;
        /*
            CONCEPT: virtual keyword is used to define a function in the base class, and override it in children class.
            When we create an object of the child class, and use a pointer of the base class to refer to it, then we can
            call the virtual function of that object and what will be executed is the version present in child class.

            So basically we ensure that that version of the function is called which is present in the object, and not the pointer class's version.
            This is called "RUNTIME POLYMORPHISM". 

            https://www.geeksforgeeks.org/virtual-function-cpp/

            The " = 0 " at the end is to enforce implementation of this function in the derived classes. If it is not present, then we'd have to
            create full fledged function here also, and at runtime, for an object belong to a child class which doesn't override this virtual function, 
            the base class's virtual function will be called. 

            QUESTION: What if I use = 0, and not define the virtual function and just declare it, and then create an object of this base class
            and make a function call?
            Ans: Well, it won't compile in that case and give error that no definition for pure virtual function.
            If we skip =0, and don't define the function and yet call it, again it will give compilation error: linker error. Basically, meaning that
            it couldn't link the function to any definition, as there was no definition at the top level. 
        */

        virtual int getHashedValue(int key) = 0;

        static HashFunction* GetHashFunctionObject(HashFunctionType hashFunctionType);
        /*
            CONCEPT: This is basically called Factory method.
            https://www.geeksforgeeks.org/design-patterns-set-2-factory-method/

        */
};

class DefaultHashFunction: public HashFunction {
    private:
        int getHashedValue(string input){
            int l = input.length();
            int a = 0;
            for(int i=0; i<l; i++)
                a += (input[i] - 'a');
            l = l*l*l + a*a*a;
            return l;
        }

        int getHashedValue(int data){
            data += 17;
            data = data*data;
            return data;
        }    
};

class CustomHashFunction1: public HashFunction {
    private:
        int getHashedValue(string input){
            int l = input.length();
            int a = 0;
            for(int i=0; i<l; i++)
                a += (input[i] - 'a');
            l = l*l + a*a;
            return l;
        }

        int getHashedValue(int data){
            data += 17;
            data = data*data;
            return data;
        }
};

class CustomHashFunction2: public HashFunction {
    private:
        int getHashedValue(string input){
            int l = input.length();
            int a = 0;
            for(int i=0; i<l; i++)
                a += (input[i] - 'a');
            return l + a;
        }

        int getHashedValue(int data){
            data += 17;
            data = data*data;
            return data;
        }
};

HashFunction* HashFunction::GetHashFunctionObject(HashFunctionType hashFunctionType){
    if(hashFunctionType == Default)
        return new DefaultHashFunction();
    if(hashFunctionType == Custom1)
        return new CustomHashFunction1();
    if(hashFunctionType == Custom2)
        return new CustomHashFunction2();
    return NULL;
}


#endif