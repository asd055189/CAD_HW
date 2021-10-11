#ifndef MLRCS_H
#define MLRCS_H
#include <bits/stdc++.h>
using namespace std;


class ML_RCS{
    public:
        ML_RCS(Node *r,map<string,Node*>l,vector<int>A){
            root=r;
            list=l;
            limit_op=A;
            ready_arr.reserve(l.size());
        };
        ~ML_RCS(){};
        void run();
        void checkstate();
        void checkfeasible();
    private:
        Node* root;
        map<string,Node*>list;
        vector<int> limit_op;
        vector<Node*>ready_arr;
};
void ML_RCS::checkstate(){
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
    //cout <<ready_arr.capacity()<<" "<<ready_arr.size()<<endl;
    for(auto i:list){
        if(i.second->getstate()==0){
            //cout <<i.first<<" ";
            if(ready_arr.size()==0){
                //cout <<"insert "<<i.first<<endl;
                ready_arr.push_back(i.second);
            }
            else{
                auto it=ready_arr.begin();
                for(;it!=ready_arr.end();it++){
                    //cout <<i.first<<endl;
                    if((*it)->getpriority()<i.second->getpriority()){
                        break;
                    }
                }
                ready_arr.insert(it,i.second);
                //cout <<"insert "<<i.first<<endl;
            }
        }
    }    
}
void ML_RCS::checkfeasible(){
    if(limit_op[0]==0){
        for(auto i:list){
            if(i.second->getop()==0){
                cout <<"No feasible solution.\nEND";
                exit(0);
            }
        }
    }
    if(limit_op[1]==0){
        for(auto i:list){
            if(i.second->getop()==1){
                cout <<"No feasible solution.\nEND";
                exit(0);
            }
        }
    }
    if(limit_op[2]==0){
        for(auto i:list){
            if(i.second->getop()==2){
                cout <<"No feasible solution.\nEND";
                exit(0);
            }
        }
    }
}

void ML_RCS::run(){
    int iteration=1;
    for(int i=0;i<root->getchild().size();i++){
        root->getchild()[i]->setstate(1);
    }
    checkstate();
    checkfeasible();
    int M_a=0,M_o=0,M_n=0;
    while(ready_arr.size()!=0){
        cout <<iteration<<": ";
        for (int i=0;i<3;i++){
            int usage_op=0;
            cout <<"{";
            bool temp=false;
            for (auto j:ready_arr){
                if(j->getop()==i && usage_op<limit_op[i]){
                    list[j->getname()]->setstate(1);
                    if(temp)
                        cout<<" ";
                    temp=true;
                    cout<<j->getname();
                    usage_op++;
                }
            }
            cout <<"} ";
            if(i==0)
                if(usage_op>M_a)M_a=usage_op;
            if(i==1)
                if(usage_op>M_o)M_o=usage_op;
            if(i==2)
                if(usage_op>M_n)M_n=usage_op;
        }
        cout <<endl;
        checkstate();
        iteration++;
    }
    cout <<"#AND: "<<M_a<<endl;
    cout <<"#OR: "<<M_o<<endl;
    cout <<"#NOT: "<<M_n<<endl;
    cout<<"END"<<endl;

}

#endif 