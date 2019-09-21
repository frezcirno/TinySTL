#include <iostream>
using namespace std;

template <typename T,int max_size>
class queue{
public:
    queue():m_front(0),m_rear(0){}
    T& dequeue(){
        m_front=(m_front+1)%max_size;
        return m_data[m_front-1];
    }
    void enqueue(T&elem){
        m_data[m_rear]=elem;
        m_rear=(m_rear+1)%max_size;
    }
    bool empty(){
        return m_front==m_rear;
    }
private:
    T m_data[max_size];
    int m_front,m_rear;
};


int main(){
    queue<int,500>qa,qb;
    int n,x;
    cin>>n;
    for(int i=0;i<n;i++){
        cin>>x;
        if(x%2!=0)qa.enqueue(x);
        else qb.enqueue(x);
    }
    bool nf=false;int emp;
    while(true){
        emp=0;
        if(nf)cout<<" ";    nf=true;
        if(!qa.empty())cout<<qa.dequeue()<<" "<<qa.dequeue();
        else emp++;
        if(!qb.empty())cout<<(emp==1?"":" ")<<qb.dequeue();
        else emp++;
        if(emp==2)break;
    }
    return 0;
}
