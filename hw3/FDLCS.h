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
        void checkstate();
        void checkfeasible();
        void run();
        void calculate_force(Node * n);
        double calculate_successor_force(int time,Node* start);
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
    for(auto i:list)
        if(i.second->getstate()!=1&& i.second->getALAP()==0){
            cout <<"No feasible solution.\nEND\n";
            exit(0);
        }
}

void FD_LCS::checkstate(){
    ready_arr.clear();
    //select the unready(-1) node and set the node to ready state(0) while its all parent are done(1)
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
            ready_arr.push_back(i.second);
        }
    }    
}


void FD_LCS::run(){
    iteration=1;
    checkstate();
    calculate_constant();
    int M_op[3]={0,0,0};
    vector<vector<vector<string>>>output;
    output.resize(limit_time);
    for(int i=0;i<output.size();i++)
        output[i].resize(3);
    while(ready_arr.size()!=0){
        //cout <<"\niteration:"<<iteration<<endl;
        Node *select=nullptr;
        int min_slack=INT_MAX;

        for(auto i:ready_arr){
            if(i->getALAP()-i->getASAP()<min_slack){
                min_slack=i->getALAP()-i->getASAP();
                select=i;
            }
        }
        //for (auto i:ready_arr){
        while(select!=nullptr){
            calculate_force(select);
            list[select->getname()]->setstate(1);
            select->setALAP(select->get_suit_to_be_place());
            select->setASAP(select->get_suit_to_be_place());
            for (auto i:select->getchild()){
                if(i->getASAP()<=select->get_suit_to_be_place())
                    i->setASAP(select->get_suit_to_be_place()+1);
            }
            output[select->get_suit_to_be_place()-1][select->getop()].push_back(select->getname());
            select=nullptr;
            min_slack=INT_MAX;
            checkstate();
            for(auto i:ready_arr){
                if(i->getALAP()-i->getASAP()<min_slack && i->getstate()!=1){
                    min_slack=i->getALAP()-i->getASAP();
                    select=i;
                }
            }
            calculate_constant();
        }
        //}
        //iteration++;
    }
    int it=1;
    int ops[3]={0,0,0};
    //int s_maxop[3]{0,0,0};
    for(auto i:output){
        cout <<it<<": ";
        int op=0;
        for(auto j:i){
            if(ops[op]<j.size()){
                ops[op]=j.size();
                //s_maxop[op]=it;
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
    

//mark suit_to_be_place to true if current iteration is at least force
void FD_LCS::calculate_force(Node * n){

        int alap=n->getALAP();
        int asap=n->getASAP();

        /*
        cout<< n->getname()<<"iteration "<<iteration<<endl;
        cout<<endl<< n->getname()<<"alap "<<alap<<endl;
        cout<< n->getname()<<"asap "<<n->getASAP()<<endl;
        */
        double delta=1.0/(alap-asap+1);
        vector<double>SF;
        vector<double>PS_F;
        //cout<<"\n===============node==============="<<j->getname()<<endl;
        for (auto it=asap;it<=alap;it++){
            //cout <<"iteration "<<it<<endl;
            double successor_force=0.0;
            //cout <<endl;
            calculated.clear();
            successor_force=calculate_successor_force(it,n);
            //cout <<"successor_force "<<successor_force<<endl;
            PS_F.push_back(successor_force);
            double self_force=0.0;
            for(auto k=asap;k<=alap;k++){
                if(k==it){
                    //cout<<" + "<<constant[n->getop()][k]*delta;
                    self_force+=constant[n->getop()][k]*delta;
                }
                else{
                    //cout<<" - "<<constant[n->getop()][k]*delta;
                    self_force-=constant[n->getop()][k]*delta;
                }

            }
            //cout<<"\nself_force "<<self_force<<endl;

            SF.push_back(self_force);
            //if(it==2)
                //  exit(0);

        }
        vector<double>total_force;
        for (int ii=0;ii<PS_F.size();ii++){
            total_force.push_back(PS_F[ii]+SF[ii]);
        }
        /***************************************
        cout <<"node "<<n->getname()<<" : \n";
        cout <<"ASAP"<<n->getASAP()<<" ALAP"<<n->getALAP()<<endl;
        cout <<"iteration"<<iteration<<endl;
        for(int a=0;a<total_force.size();a++){
            cout <<SF[a]<<"+"<<PS_F[a]<<"="<<total_force[a]<<"\n";
        }
        //cout<<"=====done====="<<endl;
        /*****************************************/
        double minimal=total_force[0];
        //cout <<"minimal set to "<<minimal<<endl;
        n->set_suit_to_be_place(asap);

        int num=0;
        int minnum;
        bool flag=true;
        //cout <<endl;
        for(auto k:total_force){
            //cout <<k<<endl;
            if(k<minimal){
                minnum=num;
                flag=false;
                minimal=k;
            }
            num++;
        }
        if(!flag){
            n->set_suit_to_be_place(asap+minnum);
        //cout<< n->getname()<<"iteration "<<iteration<<" num "<<num<<endl;
        }
        
        /*
        cout <<"ASAP"<<n->getASAP()<<" ALAP"<<n->getALAP()<<endl;
        for(int a=0;a<total_force.size();a++){
            cout <<SF[a]<<"+"<<PS_F[a]<<"="<<total_force[a]<<"\n";
        }
        cout<< n->getname()<<" can be place at"<<n->get_suit_to_be_place()<<endl;
        */
}


double FD_LCS::calculate_successor_force(int time,Node* start){
    double force=0.0;
   // cout <<"call for "<<start->getname()<<"\'s child"<<"time="<<time<<"\n";
    for (auto i:start->getchild()){
        if(time>=i->getASAP()&&find(calculated.begin(),calculated.end(),i->getname())==calculated.end()){
            calculated.push_back(i->getname());
            double add=0.0,minus=0.0;
            //cout <<i->getname()<<" ASAP"<<i->getASAP()<<" ALAP"<<i->getALAP()<<endl;
            for(int j=time;j<=i->getALAP();j++){
                minus+=constant[i->getop()][j];
                //cout<<constant[i->getop()][j]<<endl;
                if(j>time){
                    add+=constant[i->getop()][j];
                    //cout<<"---"<<constant[i->getop()][j]<<endl;
                }
            }
            //cout <<i->getname()<<" "<<add<<"/"<<(i->getALAP()-time)<<"-"<<minus<<"/"<<(i->getALAP()-i->getASAP()+1);
            //cout <<"="<<add/(i->getALAP()-time)-minus/(i->getALAP()-i->getASAP()+1)<<endl;
            
            force+=add/(i->getALAP()-time)-minus/(i->getALAP()-i->getASAP()+1);
            vector<string>a;
            force+=calculate_successor_force(time+1,i);
        }
    }
    //cout <<"return for "<<start->getname()<<"\'s child";
    //cout <<" ="<<force<<endl;

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
                //cout <<"op"<<i.second->getop()<<" : + "<<"1.0/"<<(alap-asap+1)<<endl;
                constant[i.second->getop()][j]+=1.0/(alap-asap+1);
            }

            //cout <<"node : "<<i.first<<" //asap : "<<asap<<" //alap : "<<alap<<"//op : "<<i.second->getop()<<endl;
        }
    }
    /*for (auto i:constant[0])
    {
        cout <<i<<endl;
    }*/
}
#endif 