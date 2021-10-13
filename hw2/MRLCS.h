#ifndef MRLCS_H
#define MRLCS_H
#include <bits/stdc++.h>
using namespace std;


class MR_LCS{
    public:
        MR_LCS(Node *r,map<string,Node*>l,int A){
            root=r;
            list=l;
            limit_time=A;
            ready_arr.reserve(l.size());
        };
        ~MR_LCS(){};
        void checkstate();
        void checkfeasible();
        void run();
    private:
        Node* root;
        map<string,Node*>list;
        int limit_time;
        vector<Node*>ready_arr;
};
void MR_LCS::checkstate(){
    ready_arr.clear();
    for(auto i:list){
        if(i.second->getstate()==-1){
            bool tag=true;
            for(auto j:i.second->getparent()){
                if(j->getstate()!=1){
                    tag=false;
                    break;
                }
            }
            if(tag){
                i.second->setstate(0);
            }
        }
    }
    for(auto i:list){
        if(i.second->getstate()==0){
            if(ready_arr.size()==0){
                ready_arr.push_back(i.second);
            }
            else{
                auto it=ready_arr.begin();
                for(;it!=ready_arr.end();it++){
                    if((*it)->getpriority()<i.second->getpriority()){
                        break;
                    }
                }
                ready_arr.insert(it,i.second);
            }
        }
    }    
}

void MR_LCS::checkfeasible(){
    for(int i=0;i<root->getchild().size();i++){
        root->getchild()[i]->setstate(1);
    }
    for(auto i:list)
        if(i.second->getstate()!=1&& i.second->getpriority()>limit_time){
            cout <<"No feasible solution.\nEND";
            exit(0);
        }
}

void MR_LCS::run(){
    int iteration=1;
    checkstate();
    int M_op[3]={0,0,0};
    while(ready_arr.size()!=0){
        cout <<iteration<<": ";
        for (int i=0;i<3;i++){
            int usage_op=0;
            cout <<"{";
            bool temp=false;
            for (auto j:ready_arr){
                if(j->getop()==i){
                if(M_op[j->getop()]==0)
                    M_op[i]++;
                if( limit_time-j->getpriority()-iteration<0){
                    list[j->getname()]->setstate(1);
                    if(temp)
                        cout<<" ";
                    temp=true;
                    cout<<j->getname();
                    usage_op++;
                }
                else if( usage_op<M_op[i]){
                    list[j->getname()]->setstate(1);
                    if(temp)
                        cout<<" ";
                    temp=true;
                    cout<<j->getname();
                    usage_op++;
                }
                if(usage_op>M_op[i])M_op[i]=usage_op;
                }
            }
            cout <<"} ";
            
        }
        cout <<endl;
        checkstate();
        iteration++;
    }
    cout <<"#AND: "<<M_op[0]<<endl;
    cout <<"#OR: "<<M_op[1]<<endl;
    cout <<"#NOT: "<<M_op[2]<<endl;
    cout<<"END"<<endl;

}

#endif 