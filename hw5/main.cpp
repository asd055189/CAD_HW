#include "bits/stdc++.h"

using namespace std;

struct Macro{
    string name;
    int width;
    int height;
    int position_x=0;
    int position_y=0;
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
        outfile<<"set object "<<to_string(_ID++)<<" rect from "<<i.second.position_x<<","<<i.second.position_y<<" to "<<i.second.position_x+i.second.width<<","<<i.second.position_y+i.second.height<<" lw 1\n";
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
        cout <<i<<endl;
        
        if(i!="V" && i!="H"){
            ss.push(&nodelist[i]);
            cout <<"push"<<endl;
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

                tmp->position_x=macro1->position_x;
                tmp->position_y=macro1->position_y;
                tmp->width=macro1->width+macro2->width;
                tmp->height=max(macro1->height,macro2->height);
            }
            if(i=="H"){
                macro2->position_y=macro1->position_y+macro1->height;
                macro2->position_x=macro1->position_x;

                tmp->position_x=macro1->position_x;
                tmp->position_y=macro1->position_y;

                tmp->width=max(macro1->width,macro2->width);
                tmp->height=macro1->height+macro2->height;

            }
            ss.push(tmp);

            cout <<macro1->position_x<<",";
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
            cout <<tmp->height<<" "<<tmp->name<<endl;
        }
    }

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
    srand( 1234 );

    vector<string>init_polishexp;
    for(map<string,Macro>::iterator i=nodelist.begin();i!=nodelist.end();i++){
        init_polishexp.push_back(i->first);
        if(init_polishexp.size()==1){
            i++;
            init_polishexp.push_back(i->first);
        }
        int r=rand()%2;
        if(r==0)
            init_polishexp.push_back("H");
        else
            init_polishexp.push_back("V");

    }

    calc_position(nodelist,init_polishexp);
    draw_image(nodelist,init_polishexp);

    for(auto i:nodelist){
        cout <<i.second.position_x<<",";
        cout <<i.second.position_y<<" ";
        cout <<i.second.width<<",";
        cout <<i.second.height<<endl;
    }
    cout <<get_bound(nodelist).first<<endl;
    cout <<get_bound(nodelist).second<<endl;

}