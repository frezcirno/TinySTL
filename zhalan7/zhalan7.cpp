#include<iostream>
#include<queue>
#include<vector>
#include<algorithm>

using namespace std;

int main(){
    priority_queue<int, vector<int>, greater<int> > pq;
    int n;
    cin>>n;
    for(int i=0; i<n; i++){
        int t;
        cin>>t;
        pq.push(t);
    }

    int sum=0;
    while(pq.size() != 1){
        int t1=pq.top();pq.pop();
        int t2=pq.top();pq.pop();
        sum += t1+t2;
        pq.push(t1+t2);
    }
    cout << sum;
    return 0;
}
