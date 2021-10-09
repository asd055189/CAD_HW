#include <bits/stdc++.h>
using namespace std;

class Node{
    public:
        Node(string n){name=n;};
        ~Node(){};
        void setpre(Node*_in);
        void setnext(Node*_out);
        void outpre();
        void outnxt();
        string getname(){return name;}
    private:
        string name;
        vector<Node *>parent;
        vector<Node *>child;
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
	ifstream infile(argv[1],ifstream::in);
	ofstream outfile("function.out",ifstream::out);
    outfile << "Node function:"<<endl;
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
                if (exp!="")
                    exp+=" +";
                string table=token;
                infile>>token;
                string output=token;//unuse
                for (int i=0;i<table.size();i++){
                    if(table[i]=='1')
                        exp+=" "+variables[i];
                    else if(table[i]=='0')
                        exp+=" "+variables[i]+"\'";
                }      
            }
            outfile <<*variables.rbegin()<<" ="<<exp<<endl;
        }
	}
    outfile << "END"<<endl;
    infile.close();
    outfile.close();

    string input;
    while(true){
        cout <<"Please input a node: ";
        cin>>input;
        if(input=="0")
            break;
        if(list.find(input)==list.end()){
            cout<<"node x does not exist"<<endl;
            continue;
        }
        list[input]->outpre();
        list[input]->outnxt();
    }

}