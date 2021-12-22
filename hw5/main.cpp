#include "bits/stdc++.h"

using namespace std;

struct Macro{
    string name;
    int width;
    int height;
    int position_x=0;
    int position_y=0;
    vector<string> list;
};

pair<int, int> get_bound(map<string,Macro>nodelist){
    int width,height;
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
    int height=0,width=0;
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
                    nodelist[j].position_x=macro2->position_x;
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
                    nodelist[j].position_y=macro2->position_y;
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

            /* cout <<macro1->position_x<<",";
            cout <<macro1->position_y<<" ";
            cout <<macro1->width<<",";
            cout <<macro1->height<<" "<<macro1->name<<endl;
            cout <<macro2->position_x<<",";
            cout <<macro2->position_y<<" ";
            cout <<macro2->width<<",";
            cout <<macro2->height<<" "<<macro2->name<<endl;
            cout <<tmp->position_x<<",";
            cout <<tmp->position_y<<" ";
            cout <<tmp->width<<",";
            cout <<tmp->height<<" "<<tmp->name<<endl; */
        }
    }

}
void print_exp(vector<string>polishexp){
    cout<<"======print_exp======\n";
    for(auto i:polishexp){
        cout<<i<<" ";
    }
    cout<<"\n======print_exp======\n";

    cout <<endl;
    
}
void simulated_annealing(map<string,Macro>nodelist,vector<string>&polishexp){
    vector<string>*answer=&polishexp;
    
    srand( 1234 );
    calc_position(nodelist,polishexp);
    map<string,Macro>S_list=nodelist;
    vector<string>S_exp=polishexp;

    double T=1000;
    while(T>10){
        int cost=get_bound(nodelist).first*get_bound(nodelist).second;
        int new_cost;
        for(int i=0;i<5*nodelist.size();i++){
                print_exp(S_exp);

            map<string,Macro>S_b_list=S_list;
            vector<string>S_b_exp=S_exp;
            //M1 (Operand Swap): Swap two adjacent operands.
            //M2 (Chain Invert): Complement some chain (V = H, H = V).
            //M3 (Operator/Operand Swap): Swap two adjacent operand

            int r=rand()%3;
            if(r==0){
                cout <<"==========op : M1=========="<<endl;
                int s1=rand()%nodelist.size();
                int s2=rand()%nodelist.size();
                while(S_exp[s1]=="V" || S_exp[s1]=="H"){
                    s1++;
                    if(s1==nodelist.size()-1)
                        s1=0;
                }
                while(S_exp[s2]=="V" || S_exp[s2]=="H"){
                    s2++;
                    if(s2==nodelist.size()-1)
                        s2=0;
                }
                swap(S_b_exp[s1],S_b_exp[s2]); 
                print_exp(S_b_exp);

            }
            else if (r==1){
                cout <<"==========op : M2=========="<<endl;
                /* int s=rand()%nodelist.size();
                int e=s;
                if(s==0){
                    while(S_exp[e]=="V" || S_exp[e]=="H"){
                        e++;
                    }
                }
                else{
                    while((S_exp[e]=="V" || S_exp[e]=="H") &&s!=0){
                        s--;
                    }
                    while(S_exp[e]=="V" || S_exp[e]=="H"){
                        e++;
                    }
                }
                for(int it=s;it<=e;it++){
                    if(S_exp[it]=="V"){
                        S_b_exp[it]="H";
                    }
                    else if(S_exp[it]=="H"){
                        S_b_exp[it]="V";
                    }
                } */
            }
            else if(r==2){
                cout <<"==========op : M3=========="<<endl;

                /* int n=rand()%(nodelist.size()-1);
                swap(S_b_exp[n],S_b_exp[n+1]); */
            }
            calc_position(S_b_list,S_b_exp);
            new_cost=get_bound(S_b_list).first*get_bound(S_b_list).second;
            if((new_cost-cost)<=0){
                S_list=S_b_list;
                S_exp=S_b_exp;
                cost=new_cost;
            }
            else if((new_cost-cost)>0){
                double x = (double) rand() / (RAND_MAX + 1.0);
                if(x>=pow(2.71828,(new_cost-cost)/(-T))){
                    S_list=S_b_list;
                    S_exp=S_b_exp;
                    cost=new_cost;
                }
            }
            cout <<i<<endl;
        }
        T=0.85*T;
        cout <<T<<endl;
    }
    cout <<"ANS:\n\n";
    print_exp(S_exp);

    polishexp=S_exp;
    /*
    while not yet “frozen” do
        for 1 <= i<=  P do
            Pick a random neighbor S' of S;
            delta=  cost(S') - cost(S);
            if delta <= 0 
                S = S'
            if delta > 0 
                S = S' with probability ;
            T = rT; 
    return S;


     M1 (Operand Swap): Swap two adjacent operands.
    M2 (Chain Invert): Complement some chain (V = H, H = V).
    M3 (Operator/Operand Swap): Swap two adjacent operand
    and operator */
}


int main(int argc, char *argv[]){
    map<string,Macro>nodelist;
    ifstream infile(argv[1],std::ifstream::in);
    if(!infile.is_open()){
        cout <<"in file error\n";
        exit(0);
    }

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
            init_polishexp.push_back("H");
    }


    //simulated_annealing(nodelist,init_polishexp);

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
    cout <<get_bound(nodelist).first<<endl;
    cout <<get_bound(nodelist).second<<endl;

}