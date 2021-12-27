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

void draw_image(map<string,Macro>nodelist,vector<string>polishexp,string filename){
    ofstream outfile(filename+".gp",std::ifstream::out);
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
    outfile<<"set output \""<<filename<<".png\"\n";
    outfile<<"replot\n";
    outfile<<"set terminal x11 persist size 1024,768\n";//linux=x11 macos=qt
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
    for(auto i:polishexp)
        cout<<i<<" ";
    cout <<endl;
}
void fprint_exp(vector<string>polishexp,string filename){
    ofstream outfile(filename+".polish",std::ifstream::out);
    for(auto i:polishexp)
        outfile<<i<<" ";
    outfile <<endl;
}
void fprint_position(map<string,Macro>nodelist,string filename){
    ofstream outfile(filename+".pl",std::ifstream::out);
    for(auto i:nodelist)
        outfile<<i.first<<" "<<i.second.position_x<<" "<<i.second.position_y<<endl;
    outfile <<endl;
}
void simulated_annealing(map<string,Macro>nodelist,vector<string>&polishexp){
    vector<string>*answer=&polishexp;
    
    calc_position(nodelist,polishexp);
    map<string,Macro>S_list=nodelist;
    vector<string>S_exp=polishexp;
    double reject=0;
    double MT=0;
    double T=3000;
    clock_t start=clock();
    long long int cost=get_bound(nodelist).first*get_bound(nodelist).second;
    long long int best_cost=cost;
    vector<string>best_exp=polishexp;

    while(T>1 || (reject/MT)>0.95){
        long long int new_cost;
        for(int i=0;i<7*nodelist.size();i++){
            clock_t cur=clock();
            if((cur-start)/CLOCKS_PER_SEC>7000){
                cout <<cur<<endl;
                cout <<start<<endl;
                cout<<"time up!"<<endl;
                polishexp=best_exp;
                return;
            }
            map<string,Macro>S_b_list=S_list;
            vector<string>S_b_exp=S_exp;
            int r=rand()%100;
            if(r<10){

                int s1=rand()%S_b_exp.size();
                while(S_b_exp[s1]=="V"||S_b_exp[s1]=="H")
                    s1=rand()%S_b_exp.size();

                int s2=rand()%S_b_exp.size();
                while((S_b_exp[s2]=="V"||S_b_exp[s2]=="H")||s1==s2)
                    s2=rand()%S_b_exp.size();
                swap(S_b_exp[s1],S_b_exp[s2]); 
            }
            else if (r<80){
                int s=rand()%S_b_exp.size();
                while(S_b_exp[s]!="V"&&S_b_exp[s]!="H")
                    s=rand()%S_b_exp.size();
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
            }
            else if(r<100){
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
                if(done){
                    swap(S_b_exp[n],S_b_exp[n+1]); 
                }
            }
            calc_position(S_b_list,S_b_exp);
            new_cost=get_bound(S_b_list).first*get_bound(S_b_list).second;
                if(new_cost-best_cost<0){
                    //cout<<"\n=====cost down=====\n";
                    //cout<<best_cost<<"->"<<new_cost<<endl;
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
                    //cout <<"x="<<x<<endl;
                    //cout <<"accept prob="<<pow(2.71828,(new_cost-cost)/(-T))<<endl;
                    //cout<<"cost up "<<cost<<"->"<<new_cost<<endl;
                    S_list=S_b_list;
                    S_exp=S_b_exp;
                    cost=new_cost;
                }
            }
            else{
                reject++;
            }
        }
        T=0.85*T;
        //cout <<T<<endl;
    }
    //cout <<"ANS:\n\n";
    
    //print_exp(S_exp);

    polishexp=best_exp;
}

int main(int argc, char *argv[]){
    map<string,Macro>nodelist;
    ifstream infile(argv[1],std::ifstream::in);
    if(!infile.is_open()){
        cout <<"in file error\n";
        exit(0);
    }
    string filename=argv[1];
    filename=filename.substr(filename.find("benchmark/")+1,filename.find(".nodes")-filename.find("benchmark/")-1);
    filename=filename.substr(filename.find("/")+1,filename.size());


    cout <<filename<<endl;
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
        int r=rand()%100;
        if(r<50)
            init_polishexp.push_back("H");
        else
            init_polishexp.push_back("V");

    }

    simulated_annealing(nodelist,init_polishexp);

    calc_position(nodelist,init_polishexp);
    fprint_exp(init_polishexp,filename);
    fprint_position(nodelist,filename);
    cout <<filename<<endl;
    draw_image(nodelist,init_polishexp,filename);

    long long int width=get_bound(nodelist).first;
    long long int height=get_bound(nodelist).second;
    cout <<"width"<<width<<endl;
    cout <<"height"<<height<<endl;
    cout<<"cost = "<<width*height<<endl;
}