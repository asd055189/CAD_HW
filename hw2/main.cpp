#include <bits/stdc++.h>
using namespace std;


class Node{
    public:
        Node(string n){
            name=n;
            priority=-1;
            op=0;
            state=-1;
        };
        ~Node(){};

        void setop(int n){
            op=n;
        };
        void setpre(Node*_in);
        void setnext(Node*_out);
        void outpre();
        void outnxt();
        vector<Node *>getparent(){return parent;};
        vector<Node *>getchild(){return child;};
        string getname(){return name;}
    private:
        int state;//-1:unready 0:ready 1:done 
        int priority;
        int op;//0:None 1:and 2:or 3:not
        string name;
        vector<Node *>parent;
        vector<Node *>child;
};
class ML_RCS{
    public:
        ML_RCS(map<string,Node*>graph,vector<int>A){
            limit_and=A[0];
            limit_or=A[1];
            limit_not=A[2];
            start=new Node("__start__");
            end=new Node("__end__");

            vector<Node*>start_node;
            vector<Node*>end_node;
            for(auto i:graph){
                if(i.second->getparent().size()==0)
                    start_node.push_back(i.second);
                if(i.second->getchild().size()==0)
                    end_node.push_back(i.second);
            }
            for (int i=0;i<start_node.size();i++)
                start->setnext(start_node[i]);
            for (int i=0;i<end_node.size();i++)
                end->setpre(end_node[i]);

            /*debug the start/end point
            start->outnxt();
            end->outpre();
            */

        };
        ~ML_RCS(){};
    private:
        int limit_and;
        int limit_or;
        int limit_not;
        Node *start;
        Node *end;
};
void Node::setpre(Node* _in){
    parent.push_back(_in);
}
void Node::setnext(Node* _out){
    child.push_back(_out);
}
void Node::outpre(){
    cout <<"predecessor: ";
    if(parent.size()==0){
        cout <<"- "<<endl;
        return;
    }
    for(auto i:parent){
        cout <<i->getname();
        if(i->name!=parent[parent.size()-1]->getname())
            cout <<", ";
    }
    cout <<"\b\b"<<endl;
}
void Node::outnxt(){
    cout <<"successor: ";
    if(child.size()==0){
        cout <<"- "<<endl;
        return;
    }
    for(auto i:child){
        cout <<i->getname();
        if(i->name!=child[child.size()-1]->getname())
            cout <<", ";
    }
    cout <<"\b\b"<<endl;
}

int main(int argc, char *argv[]){
    map<string,Node*>list;
    int mode=0;
    if(string(argv[1])=="-r"){
        mode=1;
        cout <<"Latency-constrained Scheduling\n";

    }
    else if(string(argv[1])=="-l"){
        mode=2;
        cout <<"Resource-constrained Scheduling\n";

    }
    else{
        cout <<"command flag error!\n";
        exit(0);
    }
	ifstream infile(argv[2],ifstream::in);
	//ofstream outfile("function.out",ifstream::out);
    //outfile << "Node function:"<<endl;
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
                    list[(*variables.rbegin())]->setop(2);
                }
                string table=token;
                infile>>token;
                string output=token;//unuse
                for (int i=0;i<table.size();i++){
                    if(table[i]=='1'){
                        exp+=" "+variables[i];
                        if(table[i+1]=='1')
                            list[(*variables.rbegin())]->setop(1);
                    }
                    else if(table[i]=='0'){
                        exp+=" "+variables[i]+"\'";
                        list[(*variables.rbegin())]->setop(3);
                    }
                }
            }
            //outfile <<*variables.rbegin()<<" ="<<exp<<endl;
        }
	}
    //outfile << "END"<<endl;
    infile.close();
    //outfile.close();
    if(mode==1){//MR_LCS
        
    }
    if(mode==2){//ML_RCS
        vector<int> a;
        a.push_back(stoi(string(argv[3])));
        a.push_back(stoi(string(argv[4])));
        a.push_back(stoi(string(argv[5])));
        ML_RCS run(list,a);
        int k=0;
    }

}