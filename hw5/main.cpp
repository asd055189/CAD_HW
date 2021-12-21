#include "bits/stdc++.h"

using namespace std;

struct Macro{
    string name;
    int width;
    int height;
    int position_x=0;
    int position_y=0;
};

void draw_image(map<string,Macro>nodelist,vector<string>polishexp ){
    ofstream outfile("result.gp",std::ifstream::out);
    if(!outfile.is_open()){
        cout <<"out file error\n";
        exit(0);
    }
    outfile<<"reset\n";
    outfile<<"set title \"result\"\n";
    outfile<<"set xlabel \"X\"\n";
    outfile<<"set ylabel \"Y\"\n";
    outfile<<"set xtics "<<"10.0"<<"\n";//tick = max(width,height)/10
    outfile<<"set ytics "<<"10.0"<<"\n";//tick = max(width,height)/10
    outfile<<"set object "<<"1"<<" rect from "<<"1"<<","<<"1"<<" to "<<"2"<<","<<"2"<<" lw 1\n";
    outfile<<"set object "<<"2"<<" rect from "<<"1"<<","<<"3"<<" to "<<"2"<<","<<"4"<<" lw 1\n";
    outfile<<"set label "<<"\"H\""<<" at 1.5,3.5 center\n";
    outfile<<"plot [0:"<<"500"<<"][0:"<<"500"<<"] 0\n";//max(width,height)
    outfile<<"set terminal png size 1024,768\n";
    outfile<<"set output \"result.png\"\n";
    outfile<<"replot\n";
    outfile<<"set terminal x11 persist size 1024,768\n";
    outfile<<"replot\n";
}

void calc_position(map<string,Macro>&nodelist,vector<string>polishexp){
    stack<Macro>ss;
    int _ID=0;

    for(auto i:polishexp){
        cout <<i<<endl;
        
        if(i!="V" && i!="H"){
            ss.push(nodelist[i]);
            cout <<"push"<<endl;
        }else{
            Macro tmp,macro1,macro2;
            macro2.name=ss.top().name;
            macro2.position_x=ss.top().position_x;
            macro2.position_y=ss.top().position_y;
            macro2.width=ss.top().width;
            macro2.height=ss.top().height;
            ss.pop();

            macro1.name=ss.top().name;
            macro1.position_x=ss.top().position_x;
            macro1.position_y=ss.top().position_y;
            macro1.width=ss.top().width;
            macro1.height=ss.top().height;
            ss.pop();

            tmp.name="bind"+to_string(_ID++);
            if(i=="V"){
                tmp.position_x=macro1.position_x;
                tmp.position_y=macro1.position_y;
                tmp.width=max(macro1.width,macro2.width);
                tmp.height=macro1.height+macro2.height;
            }
            if(i=="H"){
                tmp.position_x=macro1.position_x;
                tmp.position_y=macro1.position_y;
                tmp.width=macro1.width+macro2.width;
                tmp.height=max(macro1.height,macro2.height);
            }
            ss.push(tmp);
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


    vector<string>init_polishexp;
    for(map<string,Macro>::iterator i=nodelist.begin();i!=nodelist.end();i++){
        init_polishexp.push_back(i->first);
        if(init_polishexp.size()==1){
            i++;
            init_polishexp.push_back(i->first);
        }

        init_polishexp.push_back("V");
    }
    calc_position(nodelist,init_polishexp);
    draw_image(nodelist,init_polishexp);

    for(auto i:nodelist){
        cout <<i.second.position_x<<endl;
    }

}