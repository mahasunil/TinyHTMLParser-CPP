#include<iostream>
#include<vector>
#include<stack>

using namespace std;

class Node{
    private:
    string tagType;
    string textContent;
    vector<Node*>children;

    public:
    ~Node(){
        for(auto child:children){
            delete child;
        }
    }

    void addTextContent(const string &text){
        textContent += text;
    }
    void addTagType(const string &tag){
        tagType=tag;
    }
    void addChildren(Node *node){
        children.push_back(node);
    }
    void print(int depth=0){
        for(int i=0; i<depth; i++) cout<<" ";
        cout<<tagType;
        if(!textContent.empty()) cout<<": "<<textContent;
        cout<<endl;

        for(auto &child:children){
            child->print(depth+1);
        }
    }
};

class ParseHTML{
    private:
    string tagName;
    string texts;
    Node *root=nullptr;
    bool insideTag=false;
    stack<Node*>st;

    public:
    ~ParseHTML(){
        delete root;
    }

    ParseHTML(const string &html){
        for(size_t i=0; i<html.size(); i++){
            char ch=html[i];
            if(ch=='<'){
                if(!texts.empty() && !st.empty()){
                    st.top()->addTextContent(texts);
                }
                insideTag=true;
                tagName="";
                texts="";
            }else if(ch=='>'){
                insideTag=false;
                if(tagName[0]=='/'){
                    if(!st.empty()){
                        st.pop();
                    }
                }else{
                    Node *newNode=new Node();
                    newNode->addTagType(tagName);
                    if(st.empty()){
                        root=newNode;
                    }else{
                        st.top()->addChildren(newNode);
                    }
                    st.push(newNode);
                }
            }else{
                if(insideTag) tagName += ch;
                else texts += ch;
            }
        }
    }

    Node* getRoot() const {
        return root;
    }
};

int main(){
    string html="<html><body><h1>This is heading.</h1><p>This is paragraph.</p></body></html>";
    ParseHTML parse(html);
    Node *rootNode = parse.getRoot();
    rootNode->print();

    return 0;
}