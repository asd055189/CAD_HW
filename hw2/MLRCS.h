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
void ML_RCS::run(){
    int iteration=1;
    for(int i=0;i<root->getchild().size();i++){
        root->getchild()[i]->setstate(1);
    }
    checkstate();
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
        }
        cout <<endl;
        checkstate();
        iteration++;
    }
    cout <<"#AND: "<<limit_op[0]<<endl;
    cout <<"#OR: "<<limit_op[1]<<endl;
    cout <<"#NOT: "<<limit_op[2]<<endl;
    cout<<"END"<<endl;

}

#endif 