#include<iostream>
using namespace std;

int main(){
    cout<<"i++" << endl;
    for(int i = 0; i<10; i++){
        cout << i <<endl;
    }
    cout<<"Now ++i" << endl;
    for(int i = 0; i<10; ++i){
        cout << i <<endl;
    }
    return 0;
}