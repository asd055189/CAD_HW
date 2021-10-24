#ifndef FDLCS_H
#define FDLCS_H
#include <bits/stdc++.h>
using namespace std;


class FD_LCS{
    public:
        FD_LCS(Node *r,map<string,Node*>l,int A){
            root=r;
            list=l;
            limit_time=A;
            ready_arr.resize(3);
            ready_arr[0].resize(l.size());
            ready_arr[1].resize(l.size());
            ready_arr[2].resize(l.size());
            constant.resize(3);
            constant[0].resize(A+1);
            constant[1].resize(A+1);
            constant[2].resize(A+1);
            iteration=1;
        };
        ~FD_LCS(){};
        void checkstate();
        void checkfeasible();
        void run();
        vector<vector<double>> calculate_self_force();
    private:
        int iteration;
        vector<vector<double>>constant;
        Node* root;
        map<string,Node*>list;
        int limit_time;
        vector<vector<Node*>>ready_arr;
};
void FD_LCS::checkstate(){
    ready_arr[0].clear();
    ready_arr[1].clear();
    ready_arr[2].clear();
    constant[0].assign(constant[0].size(),0.0);
    constant[1].assign(constant[1].size(),0.0);
    constant[2].assign(constant[2].size(),0.0);
    //select the unready(-1) node and set the node to ready state(0) while its parent are done(1)
    for(auto i:list){
        if(i.second->getstate()!=1){
            int alap=(i.second->getALAP()>iteration)?i.second->getALAP():iteration;
            int asap=i.second->getASAP();
            for(auto j=asap;j<=alap;j++){
                //cout <<"op"<<i.second->getop()<<" : + "<<"1.0/"<<(alap-asap+1)<<endl;
                constant[i.second->getop()][j]+=1.0/(alap-asap+1);
            }

            //cout <<"node : "<<i.first<<" //asap : "<<asap<<" //alap : "<<alap<<"//op : "<<i.second->getop()<<endl;
        }

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
    //sort the ready array with ALAP 
    for(auto i:list){
        if(i.second->getstate()==0){
            //if(ready_arr[i.second->getop()].size()==0){
                ready_arr[i.second->getop()].push_back(i.second);
            /*}
            else{
                auto it=ready_arr[i.second->getop()].begin();
                for(;it!=ready_arr[i.second->getop()].end();it++){
                    if((*it)->getALAP()<i.second->getALAP()){
                        break;
                    }
                }
                ready_arr[i.second->getop()].insert(it,i.second);
            }*/
        }
    }    
}

void FD_LCS::checkfeasible(){
    for(int i=0;i<root->getchild().size();i++){
        root->getchild()[i]->setstate(1);
    }
    for(auto i:list)
        if(i.second->getstate()!=1&& i.second->getALAP()==0){
            cout <<"No feasible solution.\nEND\n";
            exit(0);
        }
}

void FD_LCS::run(){
    //int iteration=1;
    checkstate();
    int M_op[3]={0,0,0};
    while(ready_arr[0].size()!=0 || ready_arr[1].size()!=0 || ready_arr[2].size()!=0){
        cout <<iteration<<": ";

        /********************************
        cout <<endl<<"and operation"<<endl;
        for(int i=iteration;i<constant[0].size();i++)
            cout <<"in it : "<<i<<endl<<constant[0][i]<<endl;

        cout <<endl<<"or operation"<<endl;
        for(int i=iteration;i<constant[1].size();i++)
            cout <<"in it : "<<i<<endl<<constant[1][i]<<endl;

        cout <<endl<<"not operation"<<endl;
        for(int i=iteration;i<constant[2].size();i++)
            cout <<"in it : "<<i<<endl<<constant[2][i]<<endl;
        ********************************/
        vector<vector<double>>self_force;
        self_force=calculate_self_force();
        for (int bb=0;bb<3;bb++){
            cout <<"\nop"<<bb<<" : ";
            for (int aa=0;aa<self_force[bb].size();aa++)
                cout<<ready_arr[bb][aa]->getname()<<"  "<<self_force[bb][aa]<<" ";
        }
        for (int i=0;i<3;i++){
            int usage_op=0;
            cout <<"{";
            bool temp=false;
            if(M_op[i]==0)
                M_op[i]++;
            if(M_op[i]>=ready_arr[i].size()){//if max_op >= ready_arr's size (this case not need to select the node to be scheduled)
                for (auto j:ready_arr[i]){
                    list[j->getname()]->setstate(1);
                    if(j==ready_arr[i][ready_arr[i].size()-1])
                        cout<<j->getname();
                    else
                        cout<<j->getname()<<" ";
                }
            }
            else{
                for (auto j:ready_arr[i]){
                    if( j->getALAP()-iteration==0){//critical node (need to be scheduled immediately)
                        list[j->getname()]->setstate(1);
                        if(temp)
                            cout<<" ";
                        temp=true;
                        cout<<j->getname();
                        usage_op++;
                    }
                    else if( usage_op<M_op[i]){
                        
                        vector<double>ps_force; 
                        list[j->getname()]->setstate(1);
                        if(temp)
                            cout<<" ";
                        temp=true;
                        cout<<j->getname();
                        usage_op++;
                    }
                    if(usage_op>M_op[i])
                        M_op[i]=usage_op;
                }
            }
            cout <<"} ";
            
        }
        cout <<endl;
        iteration++;
        checkstate();
    }
    cout <<"#AND: "<<M_op[0]<<endl;
    cout <<"#OR: "<<M_op[1]<<endl;
    cout <<"#NOT: "<<M_op[2]<<endl;
    cout<<"END"<<endl;

}
vector<vector<double>> FD_LCS::calculate_self_force(){
    vector<vector<double>>self_force;
    self_force.resize(3);
    self_force[0].resize(ready_arr[0].size());
    self_force[1].resize(ready_arr[1].size());
    self_force[2].resize(ready_arr[2].size());

    for (auto i=0;i<3;i++){
        int a=0;
        for (auto j:ready_arr[i]){
            int alap=j->getALAP();
            int asap=iteration;
            double delta=1.0/(alap-asap+1);
            //cout<<"asap : "<<asap<<"//alap : "<<alap<<endl;
            //cout <<"delta = 1.0/"<<(alap-asap+1)<<endl;
            double force=constant[i][iteration]*delta;
            //cout <<j->getname()<<endl;
            //cout <<constant[i][iteration]<<" * "<<delta<<endl;
            for(auto k=iteration+1;k<=alap;k++){
                //cout <<" - "<<constant[i][k]<<" * "<<delta<<endl;
                force-=constant[i][k]*delta;
            }
            //cout<<"============\n";
            self_force[i][a]=force;
            a++;
        }
    }
    return self_force;
}

#endif 