#include <bits/stdc++.h>
#include "Node.h"

#include "MRLCS.h"


using namespace std;

void travesal(Node *n);
int main(int argc, char *argv[]){
    map<string,Node*>list;
    cout <<"Latency-constrained Scheduling\n";

	ifstream infile(argv[1],ifstream::in);
	string token;
    if(!infile){
        cout<<"file error\n";
        exit(0);
    }
	while(infile >> token){
		if(token.find('#')!=-1){
			getline(infile,token);
            continue;
		}
        if(token==".model"){
            getline(infile,token);
            continue;
        }
        if(token==".inputs"){
            while(infile>>token){
                if(token==".outputs")
                    break;
                if(token=="\\")
                    continue;
                list[token]=new Node(token);
            }
        }
        if(token==".outputs"){
             while(infile>>token){
                if(token==".names")
                    break;
                if(token=="\\")
                    continue;
                list[token]=new Node(token);
            }
        }
        while(token==".names"){
            infile.get();

            vector<string> variables;
            bool continueflag=true;
            while(continueflag){
                getline(infile,token);
                if(token.find("\\")==-1)
                    continueflag=false;
                else
                    token=token.substr(0,token.find("\\")-1);

                while(token.size()!=0){
                    string x=token.substr(0,token.find(" "));
                    if(list.find(x)==list.end())
                        list[x]=new Node(x);
                    variables.push_back(x);
                    if(token.find(" ")==-1)
                        break;
                    token=token.substr(token.find(" ")+1,token.size()-1);
                }
            }
            //build graph
            for (int i=0;i<variables.size()-1;i++)
                list[variables[variables.size()-1]]->setpre(list[variables[i]]);
            for (int i=0;i<variables.size()-1;i++)
                list[variables[i]]->setnext(list[variables[variables.size()-1]]);
            //build graph
            string exp="";
            while(infile>>token){
                if(token.find(".names")!=-1 || token==".end")
                    break;
                if (exp!=""){
                    exp+=" +";
                    list[(*variables.rbegin())]->setop(1);
                }
                string table=token;
                infile>>token;
                string output=token;//unuse
                for (int i=0;i<table.size();i++){
                    if(table[i]=='1'){
                        exp+=" "+variables[i];
                        if(table[i+1]=='1')
                            list[(*variables.rbegin())]->setop(0);
                    }
                    else if(table[i]=='0'){
                        exp+=" "+variables[i]+"\'";
                        list[(*variables.rbegin())]->setop(2);
                    }
                }
            }
            //outfile <<*variables.rbegin()<<" ="<<exp<<endl;
        }
	}
    infile.close();

    Node *start;
    Node *end;
    start=new Node("__start__");
    end=new Node("__end__");
    vector<Node*>start_node;
    vector<Node*>end_node;
    for(auto i:list){
        if(i.second->getparent().size()==0)
            start_node.push_back(i.second);
        if(i.second->getchild().size()==0)
            end_node.push_back(i.second);
    }
    for (int i=0;i<start_node.size();i++){
        start_node[i]->setpre(start);
        start->setnext(start_node[i]);
    }
    for (int i=0;i<end_node.size();i++){
        end->setpre(end_node[i]);
        end_node[i]->setnext(end);
    }

    queue<Node *>q;
    q.push(end);
    while(!q.empty()){
        for(int i=0;i<q.front()->getparent().size();i++){
            int p=-1;
            for (auto j:q.front()->getparent()[i]->getchild()){
                if(j->getpriority()>p)
                    p=j->getpriority();
            }
            q.front()->getparent()[i]->setpriority(p+1);
            q.push(q.front()->getparent()[i]);
        }
        q.pop();
    }
    int L=stoi(string(argv[2]));
    MR_LCS mr_lcs(start,list,L);
    mr_lcs.checkfeasible();
    mr_lcs.run();


}