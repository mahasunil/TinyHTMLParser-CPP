#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
#include <map>

using namespace std;

class Node
{
private:
    string tagType;
    string textContent;
    map<string, string, greater<string>> attributes;
    vector<Node *> children;

public:
    ~Node()
    {
        for (auto child : children)
        {
            delete child;
        }
    }

    void addTextContent(const string &text)
    {
        textContent += text;
    }
    void addTagType(const string &tag)
    {
        tagType = tag;
    }
    void addAttribute(const string &key, const string &val)
    {
        attributes[key] = val;
    }
    void addChildren(Node *node)
    {
        children.push_back(node);
    }

    void print(int depth = 0)
    {
        for (int i = 0; i < depth; i++)
            cout << " ";
        cout << tagType;

        if (!attributes.empty())
        {
            cout << ":";
            cout << "[";
            bool first = true;
            for (auto &attr : attributes)
            {
                if (!first)
                    cout << ", ";
                cout << attr.first << "=\"" << attr.second << "\"";
                first = false;
            }
            cout << "]";
        }

        if (!textContent.empty())
            cout << ": " << textContent;
        cout << endl;

        for (auto &child : children)
        {
            child->print(depth + 1);
        }
    }
};

class ParseHTML
{
private:
    string tagName;
    string texts;
    Node *root = nullptr;
    bool insideTag = false;
    stack<Node *> st;

public:
    ~ParseHTML()
    {
        delete root;
    }

    ParseHTML(const string &html)
    {
        for (size_t i = 0; i < html.size(); i++)
        {
            char ch = html[i];
            if (ch == '<')
            {
                if (!texts.empty() && !st.empty())
                {
                    st.top()->addTextContent(texts);
                }
                insideTag = true;
                tagName = "";
                texts = "";
            }
            else if (ch == '>')
            {
                insideTag = false;
                if (tagName[0] == '/')
                {
                    if (!st.empty())
                    {
                        st.pop();
                    }
                }
                else
                {
                    stringstream ss(tagName);
                    string tagText;
                    ss >> tagText;

                    Node *newNode = new Node();
                    newNode->addTagType(tagText);

                    while (ss >> ws && ss.peek() != EOF)
                    {
                        string key, value;
                        while (ss.peek() != EOF && ss.peek() != '=' && !isspace(ss.peek()))
                        {
                            key.push_back(ss.get());
                        }
                        ss >> ws;
                        if (ss.peek() == '=')
                        {
                            ss.get();
                            ss >> ws;
                            if (ss.peek() == '"' || ss.peek() == '\'')
                            {
                                char quote = ss.get();

                                while (ss.peek() != quote && ss.peek() != EOF)
                                {
                                    value.push_back(ss.get());
                                }
                                if (ss.peek() == quote)
                                {
                                    ss.get();
                                }
                            }
                            else
                            {
                                while (ss.peek() != EOF && !isspace(ss.peek()))
                                {
                                    value.push_back(ss.get());
                                }
                            }
                        }
                        if (!key.empty())
                        {
                            newNode->addAttribute(key, value);
                        }
                    }

                    if (st.empty())
                    {
                        root = newNode;
                    }
                    else
                    {
                        st.top()->addChildren(newNode);
                    }
                    st.push(newNode);
                }
            }
            else
            {
                if (insideTag)
                    tagName += ch;
                else
                    texts += ch;
            }
        }
    }

    Node *getRoot() const
    {
        return root;
    }
};

int main()
{
    string html = R"(<html><body><h1 id="mainHeader" class="title">This is heading.</h1><p class="para">This is paragraph.</p></body></html>)";
    ParseHTML parse(html);
    Node *rootNode = parse.getRoot();
    rootNode->print();

    return 0;
}