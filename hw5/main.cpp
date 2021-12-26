#include "bits/stdc++.h"

using namespace std;

struct Macro{
    string name;
    long long int width;
    long long int height;
    long long int position_x=0;
    long long int position_y=0;
    vector<string> list;
};

pair<long long int,long long int> get_bound(map<string,Macro>nodelist){
    long long int width,height;
    width=height=0;
    for (auto i:nodelist){
        if(i.second.position_x+i.second.width>width)
            width=i.second.position_x+i.second.width;
        if(i.second.position_y+i.second.height>height)
            height=i.second.position_y+i.second.height;   
    }
    return make_pair(width,height);
}

void draw_image(map<string,Macro>nodelist,vector<string>polishexp){
    ofstream outfile("result.gp",std::ifstream::out);
    int _ID=1;
    if(!outfile.is_open()){
        cout <<"out file error\n";
        exit(0);
    }
    outfile<<"reset\n";
    outfile<<"set title \"result\"\n";
    outfile<<"set xlabel \"X\"\n";
    outfile<<"set ylabel \"Y\"\n";
    long long int height=0,width=0;
    for (auto i:nodelist){
        if(i.second.position_x+i.second.width>width)
            width=i.second.position_x+i.second.width;
        if(i.second.position_y+i.second.height>height)
            height=i.second.position_y+i.second.height;
        outfile<<"set object "<<to_string(_ID++)<<" rect from "<<i.second.position_x<<","<<i.second.position_y<<" to "<<i.second.position_x+i.second.width<<","<<i.second.position_y+i.second.height<<" lw 1 fs empty\n";
        outfile<<"set label \""<<i.first<<"\" at "<<(i.second.position_x+i.second.position_x+i.second.width)/2<<","<<(i.second.position_y+i.second.position_y+i.second.height)/2<<" center\n";
    }

    outfile<<"set xtics "<<max(height,width)/20<<"\n";//tick = max(width,height)/10
    outfile<<"set ytics "<<max(height,width)/20<<"\n";//tick = max(width,height)/10
    outfile<<"set size square\n";
    outfile<<"set grid  \n";
    outfile<<"plot [0:"<<max(height,width)<<"][0:"<<max(height,width)<<"] 0\n";//max(width,height)
    outfile<<"set terminal png size 1024,768\n";
    outfile<<"set output \"result.png\"\n";
    outfile<<"replot\n";
    outfile<<"set terminal qt persist size 1024,768\n";//linux=x11 macos=qt
    outfile<<"replot\n";
}

void calc_position(map<string,Macro>&nodelist,vector<string>polishexp){
    stack<Macro*>ss;
    int _ID=0;
    for(auto &i:nodelist){
        i.second.position_x=i.second.position_y=0;
    }
    for(auto i:polishexp){
        if(i!="V" && i!="H"){
            ss.push(&nodelist[i]);
        }else{
            Macro *tmp,*macro1,*macro2;
            macro2=ss.top();
            ss.pop();

            macro1=ss.top();
            ss.pop();

            tmp=new Macro;
            tmp->name="bind"+to_string(_ID++);
            if(i=="V"){
                macro2->position_x=macro1->position_x+macro1->width;
                macro2->position_y=macro1->position_y;

                for (auto j:macro2->list){
                    nodelist[j].position_x+=macro1->width;
                }

                tmp->position_x=macro1->position_x;
                tmp->position_y=macro1->position_y;
                tmp->width=macro1->width+macro2->width;
                tmp->height=max(macro1->height,macro2->height);
                if(macro1->list.size()==0)
                    tmp->list.push_back(macro1->name);
                else{
                    for (auto j:macro1->list)
                        tmp->list.push_back(j);
                }
                if(macro2->list.size()==0)
                    tmp->list.push_back(macro2->name);
                else{
                    for (auto j:macro2->list)
                        tmp->list.push_back(j);
                }
            }
            if(i=="H"){
                macro2->position_y=macro1->position_y+macro1->height;
                macro2->position_x=macro1->position_x;
                for (auto j:macro2->list){
                    nodelist[j].position_y+=macro1->height;
                }
                tmp->position_x=macro1->position_x;
                tmp->position_y=macro1->position_y;
                tmp->width=max(macro1->width,macro2->width);
                tmp->height=macro1->height+macro2->height;
                if(macro1->list.size()==0)
                    tmp->list.push_back(macro1->name);
                else{
                    for (auto j:macro1->list)
                        tmp->list.push_back(j);
                }
                if(macro2->list.size()==0)
                    tmp->list.push_back(macro2->name);
                else{
                    for (auto j:macro2->list)
                        tmp->list.push_back(j);
                }
            }
            ss.push(tmp);
            if(nodelist.find(macro1->name)==nodelist.end()){
                delete macro1;
            }
            if(nodelist.find(macro2->name)==nodelist.end()){
                delete macro2;
            }
            /*
            cout<<"macro1"<<endl;
            cout <<macro1->position_x<<",";
            cout <<macro1->position_y<<" ";
            cout <<macro1->width<<",";
            cout <<macro1->height<<" "<<macro1->name<<endl;
            cout<<"macro2"<<endl;

            cout <<macro2->position_x<<",";
            cout <<macro2->position_y<<" ";
            cout <<macro2->width<<",";
            cout <<macro2->height<<" "<<macro2->name<<endl;

            cout<<"new"<<endl;
            cout <<tmp->position_x<<",";
            cout <<tmp->position_y<<" ";
            cout <<tmp->width<<",";
            cout <<tmp->height<<" "<<tmp->name<<endl; 
            */
        }
    }

}
void print_exp(vector<string>polishexp){
    for(auto i:polishexp){
        cout<<i<<" ";
    }

    cout <<endl;
    
}
void simulated_annealing(map<string,Macro>nodelist,vector<string>&polishexp){
    vector<string>*answer=&polishexp;
    
    calc_position(nodelist,polishexp);
    map<string,Macro>S_list=nodelist;
    vector<string>S_exp=polishexp;
    double reject=0;
    double MT=0;
    double T=3000;

    long long int cost=get_bound(nodelist).first*get_bound(nodelist).second;
    long long int best_cost=cost;
    vector<string>best_exp=polishexp;
    
    while(T>1 || (reject/MT)>0.95){
        long long int new_cost;
        for(int i=0;i<7*nodelist.size();i++){
            //print_exp(S_exp);
            map<string,Macro>S_b_list=S_list;
            vector<string>S_b_exp=S_exp;
            //M1 (Operand Swap): Swap two adjacent operands.
            //M2 (Chain Invert): Complement some chain (V = H, H = V).
            //M3 (Operator/Operand Swap): Swap two adjacent operand
            int r=rand()%100;
            if(r<10){
                //cout <<"==========op : M1=========="<<endl;
                //print_exp(S_b_exp);

                int s1=rand()%S_b_exp.size();
                while(S_b_exp[s1]=="V"||S_b_exp[s1]=="H")
                    s1=rand()%S_b_exp.size();

                int s2=rand()%S_b_exp.size();
                while((S_b_exp[s2]=="V"||S_b_exp[s2]=="H")||s1==s2)
                    s2=rand()%S_b_exp.size();

                
                //cout <<S_b_exp[s1]<<" "<<S_b_exp[s2]<<endl;
                swap(S_b_exp[s1],S_b_exp[s2]); 
                //print_exp(S_b_exp);

            }
            else if (r<80){
                //cout <<"==========op : M2=========="<<endl;
                //print_exp(S_b_exp);
                int s=rand()%S_b_exp.size();
                while(S_b_exp[s]!="V"&&S_b_exp[s]!="H")
                    s=rand()%S_b_exp.size();


                //cout <<S_b_exp[s]<<endl;
                //cout <<s<<endl;
                for(int it=s;it<S_b_exp.size();it++){
                    if(S_b_exp[it]!="V"&&S_b_exp[it]!="H")
                        break;
                    if(S_b_exp[it]=="V")
                        S_b_exp[it]="H";
                    else if(S_b_exp[it]=="H")
                        S_b_exp[it]="V";
                }

                for(int it=s-1;it>=0;it--){
                    if(S_b_exp[it]!="V"&&S_b_exp[it]!="H")
                        break;
                    if(S_b_exp[it]=="V")
                        S_b_exp[it]="H";
                    else if(S_b_exp[it]=="H")
                        S_b_exp[it]="V";
                }
               // print_exp(S_b_exp);

            }
            else if(r<100){
                //cout <<"==========op : M3=========="<<endl;
                //print_exp(S_b_exp);
                //int n=rand()%(S_b_exp.size()-1);
                //print_exp(S_b_exp);
                int rej=0;
                int n; 
                bool done=false;
                while(rej!=1000&&!done){
                    n=rand()%(S_b_exp.size()-1);
                    if((S_b_exp[n]=="H"||S_b_exp[n]=="V")&&(S_b_exp[n+1]!="H"&&S_b_exp[n+1]!="V"))
                        if(S_b_exp[n]!=S_b_exp[n+2])
                            done=true;
                    rej++;
                }
                //cout <<"rej"<<rej<<endl;
                if(done){
                    //cout <<S_b_exp[n]<< " "<<S_b_exp[n+1]<<endl;
                    swap(S_b_exp[n],S_b_exp[n+1]); 
                    //print_exp(S_b_exp);
                }
            }
            calc_position(S_b_list,S_b_exp);
            new_cost=get_bound(S_b_list).first*get_bound(S_b_list).second;
            //cout <<"cost"<<cost<<endl;
            //cout <<"newcost"<<new_cost<<endl;
                if(new_cost-best_cost<0){
                    cout<<"\n=====cost down=====\n";
                    cout<<best_cost<<"->"<<new_cost<<endl;
                    best_cost=new_cost;
                    best_exp=S_b_exp;
                }
            if((new_cost-cost)<=0){
                
                S_list=S_b_list;
                S_exp=S_b_exp;
                cost=new_cost;
            }
            else if((new_cost-cost)>0){
                double x = (double) rand() / (RAND_MAX + 1.0);
                if(x<=exp((0-(new_cost-cost))/T)){
                    cout <<"x="<<x<<endl;
                    cout <<"accept prob="<<pow(2.71828,(new_cost-cost)/(-T))<<endl;
                    cout<<"cost up "<<cost<<"->"<<new_cost<<endl;
                    S_list=S_b_list;
                    S_exp=S_b_exp;
                    cost=new_cost;
                }
            }
            else{
                reject++;
            }
            //cout <<i<<endl;
        }
        T=0.85*T;
        cout <<T<<endl;
    }
    cout <<"ANS:\n\n";
    
    print_exp(S_exp);

    polishexp=best_exp;

}


int main(int argc, char *argv[]){
    map<string,Macro>nodelist;
    ifstream infile(argv[1],std::ifstream::in);
    if(!infile.is_open()){
        cout <<"in file error\n";
        exit(0);
    }
    srand(time(NULL));
    string tok;
    while(infile>>tok){
        if(tok.find("#")!=-1)
            continue;
        Macro node;
        node.name=tok;
        infile>>tok;
        node.width=stoi(tok);
        infile>>tok;
        node.height=stoi(tok);
        nodelist[node.name]=node;
    }

    vector<string>init_polishexp;

    for(map<string,Macro>::iterator i=nodelist.begin();i!=nodelist.end();i++){
        init_polishexp.push_back(i->first);
        if(init_polishexp.size()==1){
            i++;
            init_polishexp.push_back(i->first);
        }
        int r=rand()%10;
        if(r<5)
            init_polishexp.push_back("H");
        else
            init_polishexp.push_back("V");

    }

    /*
    cout<<"==========init=========="<<endl;
    print_exp(init_polishexp);
    cout<<"==========init=========="<<endl;
    */


    simulated_annealing(nodelist,init_polishexp);
            /*init_polishexp.clear();
            //B4 B2 V B5 V B1 H B3 V
            init_polishexp.push_back("B4");
            init_polishexp.push_back("B2");
            init_polishexp.push_back("V");
            init_polishexp.push_back("B5");
            init_polishexp.push_back("V");
            init_polishexp.push_back("B1");
            init_polishexp.push_back("H");
            init_polishexp.push_back("B3");
            init_polishexp.push_back("V");*/



    calc_position(nodelist,init_polishexp);
    print_exp(init_polishexp);

    //cout <<init_polishexp.size()<<endl ;
    draw_image(nodelist,init_polishexp);

    /* for(auto i:nodelist){
        cout <<i.second.position_x<<",";
        cout <<i.second.position_y<<" ";
        cout <<i.second.width<<",";
        cout <<i.second.height<<endl;
    } */
    int width=get_bound(nodelist).first;
    int height=get_bound(nodelist).second;
    cout <<"width"<<width<<endl;
    cout <<"height"<<height<<endl;
    cout<<"cost = "<<width*height<<endl;

}