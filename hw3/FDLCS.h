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
        void calculate_force();
        double calculate_successor_force(int time,Node* start);
        void calculate_constant();
    private:
        vector<string>calculated;
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
            ready_arr[i.second->getop()].push_back(i.second);
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
    
}

//mark suit_to_be_place to true if current iteration is at least force
void FD_LCS::calculate_force(){
    
    for (auto i=0;i<3;i++){
        for (auto j:ready_arr[i]){
            int alap=j->getALAP();
            int asap=iteration;
            double delta=1.0/(alap-asap+1);
            vector<double>SF;
            vector<double>PS_F;
            //cout<<"\n===============node==============="<<j->getname()<<endl;
            for (auto it=iteration;it<=alap;it++){
                //cout <<"iteration "<<it<<endl;
                double successor_force=0.0;
                //cout <<endl;
                calculated.clear();
                successor_force=calculate_successor_force(it,j);
                //cout <<"successor_force "<<successor_force<<endl;
                PS_F.push_back(successor_force);
                double self_force=0.0;
                for(auto k=iteration;k<=alap;k++){
                    if(k==it){
                        //cout<<" + "<<constant[i][k]*delta;
                        self_force+=constant[i][k]*delta;
                    }
                    else{
                        //cout<<" - "<<constant[i][k]*delta;
                        self_force-=constant[i][k]*delta;
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
            cout <<endl<<"node "<<j->getname()<<" : \n";
            cout <<"ASAP"<<j->getASAP()<<" ALAP"<<j->getALAP()<<endl;
            cout <<"iteration"<<iteration<<endl;
            for(int a=0;a<total_force.size();a++){
                cout <<SF[a]<<"+"<<PS_F[a]<<"="<<total_force[a]<<"\n";
            }
            //cout<<"=====done====="<<endl;
            *****************************************/
            double minimal=total_force[0];
            //cout <<"minimal set to "<<minimal<<endl;
            j->set_suit_to_be_place(iteration);
            int num=0;
            bool flag=true;
            //cout <<endl;
            for(auto k:total_force){
            //cout <<k<<endl;
                if(k<minimal){
                   // cout <<"yes!!"<<endl;
                    flag=false;
                    break;
                }
                num++;
            }
            if(!flag){
                j->set_suit_to_be_place(iteration+num);
                //cout<< j->getname()<<" can be place at"<<iteration+num<<endl;
            }
        }

    }
}


double FD_LCS::calculate_successor_force(int time,Node* start){
    double force=0.0;
    //cout <<"call for "<<start->getname()<<"\'s child"<<"time="<<time<<"\n";
    for (auto i:start->getchild()){
        if(time>=i->getASAP()&&find(calculated.begin(),calculated.end(),i->getname())==calculated.end()){
            calculated.push_back(i->getname());
            double add=0.0,minus=0.0;
            //cout <<i->getname()<<" ASAP"<<i->getASAP()<<" ALAP"<<i->getALAP()<<endl;
            for(int j=time;j<=i->getALAP();j++){
                minus+=constant[i->getop()][j];
                if(j>time)
                    add+=constant[i->getop()][j];
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
    
}
#endif 