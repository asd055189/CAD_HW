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
            constant.resize(3);
            constant[0].resize(A+1);
            constant[1].resize(A+1);
            constant[2].resize(A+1);
            iteration=1;
        };
        ~FD_LCS(){};
        void checkfeasible();
        void run();
        void calculate_force(Node * n);
        double calculate_successor_force(int time,Node* start);
        double calculate_predecessor_force(int time,Node* start);
        void calculate_constant();
    private:
        vector<string>calculated;
        int iteration;
        vector<vector<double>>constant;
        Node* root;
        map<string,Node*>list;
        int limit_time;
        vector<Node*>ready_arr;
};

void FD_LCS::checkfeasible(){
    for(int i=0;i<root->getchild().size();i++){
        root->getchild()[i]->setstate(1);
    }
    for(auto i:list){
        if(i.second->getstate()!=1&& i.second->getALAP()==0){
            cout <<"No feasible solution.\nEND\n";
            exit(0);
        }
    }
}

void FD_LCS::run(){
    iteration=1;
    calculate_constant();
    int M_op[3]={0,0,0};
    vector<vector<vector<string>>>output;
    output.resize(limit_time);
    for(int i=0;i<output.size();i++)
        output[i].resize(3);
    Node *select=nullptr;
    int min_slack=INT_MAX;
    for(auto i:list){//select a node that has minimal (ALAP-ASAP)
        if(i.second->getALAP()-i.second->getASAP()<min_slack && i.second->getop()!=-1){
            min_slack=i.second->getALAP()-i.second->getASAP();
            select=i.second;
            
        }
    }
    while(select!=nullptr){
        calculate_force(select);
        list[select->getname()]->setstate(1);
        select->setALAP(select->get_suit_to_be_place());
        select->setASAP(select->get_suit_to_be_place());
        

        queue<Node *>q;
        q.push(select);
        while(!q.empty()){
            for(auto i:q.front()->getchild()){
                if(i->getASAP()<=select->getASAP()){
                    i->setASAP(select->getASAP()+1);//set the child ASAP;
                    q.push(i);
                }
            }
            q.pop();
        }
        q.push(select);
        while(!q.empty()){
            for(auto i:q.front()->getparent()){
                if(i->getALAP()>=select->getALAP()){
                    i->setALAP(select->getALAP()-1);//set the child ASAP;
                    q.push(i);
                }
            }
            q.pop();
        }

        output[select->get_suit_to_be_place()-1][select->getop()].push_back(select->getname());
        select=nullptr;
        min_slack=INT_MAX;
        for(auto i:list){//select a node state!=done and has minimal (ALAP-ASAP)
            if(i.second->getALAP()-i.second->getASAP()<min_slack && i.second->getstate()!=1&& i.second->getop()!=-1){
                min_slack=i.second->getALAP()-i.second->getASAP();
                select=i.second;
            }
        }
        calculate_constant();
    }

    int it=1;
    int ops[3]={0,0,0};
    for(auto i:output){
        cout <<it<<": ";
        int op=0;
        for(auto j:i){
            if(ops[op]<j.size()){
                ops[op]=j.size();
            }
            bool tag1=false;
            cout <<"{";
            for(auto k:j){
                if(tag1)
                    cout <<" "<<k;
                else{
                    tag1=true;
                    cout <<k;
                }
            }
            if(op<2)
                cout <<"} ";
            else
                cout <<"}";
            op++;
        }
        cout <<endl;
        it++;
    }
    cout <<"#AND: "<<ops[0]<<endl;
    cout <<"#OR: "<<ops[1]<<endl;
    cout <<"#NOT: "<<ops[2]<<endl;
    cout <<"END"<<endl;

}
    

void FD_LCS::calculate_force(Node * n){

        int alap=n->getALAP();
        int asap=n->getASAP();

        double delta=1.0/(alap-asap+1);
        vector<double>SF;
        vector<double>PS_F;
        for (auto it=asap;it<=alap;it++){
            double successor_force=0.0;
            double predecessor_force=0.0;
            calculated.clear();
            successor_force=calculate_successor_force(it,n);
            calculated.clear();
            predecessor_force=calculate_predecessor_force(it,n);
            PS_F.push_back(successor_force+predecessor_force);
            double self_force=0.0;
            for(auto k=asap;k<=alap;k++){
                if(k==it){
                    self_force+=constant[n->getop()][k]*delta;
                }
                else{
                    self_force-=constant[n->getop()][k]*delta;
                }

            }
            SF.push_back(self_force);
        }
        vector<double>total_force;
        for (int ii=0;ii<PS_F.size();ii++){
            total_force.push_back(PS_F[ii]+SF[ii]);
        }
        double minimal=total_force[0];
        n->set_suit_to_be_place(asap);

        int num=0;
        int minnum;
        bool flag=true;
        for(auto k:total_force){
            if(k<minimal){
                minnum=num;
                flag=false;
                minimal=k;
            }
            num++;
        }
        if(!flag){
            n->set_suit_to_be_place(asap+minnum);
        }
}

double FD_LCS::calculate_predecessor_force(int time,Node* start){
    double force=0.0;
    for (auto i:start->getparent()){
        if(time<=i->getALAP()&&find(calculated.begin(),calculated.end(),i->getname())==calculated.end()&&i->getop()!=-1){
            calculated.push_back(i->getname());
            double add=0.0,minus=0.0;
            for(int j=i->getASAP();j<=i->getALAP();j++){
                minus+=constant[i->getop()][j];
                if(j<time){
                    add+=constant[i->getop()][j];
                }
            }
            
            force+=add/(time-i->getASAP())-minus/(i->getALAP()-i->getASAP()+1);
            force+=calculate_predecessor_force(time-1,i);
        }
    }
    return force;
}

double FD_LCS::calculate_successor_force(int time,Node* start){
    double force=0.0;
    for (auto i:start->getchild()){
        if(time>=i->getASAP()&&find(calculated.begin(),calculated.end(),i->getname())==calculated.end()&&i->getop()!=-1){
            calculated.push_back(i->getname());
            double add=0.0,minus=0.0;
            for(int j=i->getASAP();j<=i->getALAP();j++){
                minus+=constant[i->getop()][j];
                if(j>time){
                    add+=constant[i->getop()][j];
                }
            }
            force+=add/(i->getALAP()-time)-minus/(i->getALAP()-i->getASAP()+1);
            force+=calculate_successor_force(time+1,i);
        }
    }
    return force;
}

void FD_LCS::calculate_constant(){
    constant[0].assign(constant[0].size(),0.0);
    constant[1].assign(constant[1].size(),0.0);
    constant[2].assign(constant[2].size(),0.0);

    for(auto i:list){
        if(i.second->getop()!=-1){
            int alap=i.second->getALAP();
            int asap=i.second->getASAP();
            for(auto j=asap;j<=alap;j++){
                constant[i.second->getop()][j]+=1.0/(alap-asap+1);
            }
        }
    }
}
#endif 