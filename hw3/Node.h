#ifndef NODE_H
#define NODE_H
#include <bits/stdc++.h>
using namespace std;

class Node{
    public:
        Node(string n){
            name=n;
            ALAP=-1;
            ASAP=-2;
            op=-1;
            state=-1;
        };
        ~Node(){};
        void setALAP(int n){ALAP=n;};
        void setASAP(int n){ASAP=n;};
        void setstate(int n){state=n;};
        void setop(int n){op=n;};
        void setpre(Node*_in);
        void setnext(Node*_out);
        void outpre();
        void outnxt();
        vector<Node *>getparent(){return parent;};
        vector<Node *>getchild(){return child;};
        string getname(){return name;}
        int getstate(){return state;};
        int getop(){return op;};
        int getALAP(){return ALAP;};
        int getASAP(){return ASAP;};


    private:
        int state;//-1:unready 0:ready 1:done 
        int ALAP;
        int ASAP;
        int op;//-1:None 0:and 1:or 2:not
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
        cout <<" "<<i->getALAP();
        if(i->name!=child[child.size()-1]->getname())
            cout <<", ";
    }
    cout <<"\b\b"<<endl;
}
#endif 
