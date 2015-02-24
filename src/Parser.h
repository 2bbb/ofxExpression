//
//  Parser.h
//
//  Created by ISHII 2bit on 2015/02/22.
//
//

#pragma once

#include "Expression.h"
#include <cctype>

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

BEGIN_NAMESPACE(ofx)
BEGIN_NAMESPACE(Expression)

class Parser {
    struct Node_;
    typedef shared_ptr<Node_> Node;
    struct Node_ {
        Node_()
        : inner("")
        , name("")
        , val("") {}
        
        Node_(const string &inner, const string &name)
        : inner(inner)
        , name(name)
        , val("") {}
        string inner;
        string name;
        string val;
        
        vector<Node> children;
        
        string print() const {
            stringstream ss("");
            if(val != "") ss << val;
            else ss << inner;
            for(int i = 0; i < children.size(); i++) {
                ss << " " << children[i]->print();
            }
            return ss.str();
        }
        
        void debug(string level = "") const {
            ofLogNotice("debug") << level << name << " / " << inner << " / " << val;
            for(int i = 0; i < children.size(); i++) {
                children[i]->debug(level + "  ");
            }
        }
    };
    
public:
    Parser(string source)
    : source(trimSpace(source))
    , id(0)
    {
        parse();
    }
    
    inline string polishNotationizedSource() const {
        return master->print();
    }
    
    void debug() const {
        master->debug();
    }
private:
    bool parse() {
        ofLogNotice("source") << source;
        
        master = parseImpl(source);
        
        return true;
    }
    
    Node parseImpl(string source) {
        master = parseBrackets(source);
        ofLogNotice("after parse brackets") << master->print();
        master->debug("    ");
        
        master = simplify(master);
        ofLogNotice("simplify") << master->print();
        master->debug("    ");
        
        master = splitByOps(master);
        ofLogNotice("after split by ops") << master->print();
        master->debug("    ");
        
        master = simplify(master);
        ofLogNotice("simplify") << master->print();
        master->debug("    ");
        
        master = parseTerm(master);
        ofLogNotice("after parse term") << master->print();
        master->debug("    ");
        
        master = simplify(master);
        ofLogNotice("simplify") << master->print();
        master->debug("    ");
    }
    
    Node simplify(Node node) {
        for(int i = 0; i < node->children.size(); i++) {
            node->children[i] = simplify(node->children[i]);
        }
        if(node->children.size() == 1 && node->children[0]->name == node->inner) {
            return node->children[0];
        } else {
            return node;
        }
    }
    
    Node parseBrackets(string &inner) {
        int nest = 0,
            left = -1,
            right = -1;
        for(int i = 0; i < inner.length(); i++) {
            if(inner.substr(i, 1) == "(") {
                if(left == -1) left = i;
                nest++;
            }
            if(inner.substr(i, 1) == ")") {
                nest--;
                if(nest == 0) {
                    right = i + 1;
                    break;
                }
            }
        }
        
        if(nest < 0) {
            ofLogError() << "unblanced brackets";
            return Node();
        }
        
        if(0 <= left && 0 <= right) {
            string fragments = inner.substr(left, right - left);
            string name = getID();
            replace(inner, fragments, name, 1);
            fragments = fragments.substr(1, fragments.length() - 2);
            Node t = Node(new Node_(fragments, name));
            
            Node child;
            do {
                child = parseBrackets(t->inner);
                if(child) {
                    if(!child) {
                        return Node();
                    }
                    t->children.push_back(child);
                }
            } while(child);
            
            return t;
        } else {
            return Node();
        }
    }

    Node splitByOps(Node node) {
        node = splitByOp(node, "*");
        node = splitByOp(node, "/");
        node = splitByOp(node, "+");
        return node;
    }
    
    Node splitByOp(Node node, string op) {
        if(!node) return node;
        
        size_t pos = node->inner.find_last_of(op);
        if(pos == string::npos) {
            return node;
        }
        vector<string> splitted;
        splitted.push_back(node->inner.substr(0, pos));
        splitted.push_back(node->inner.substr(pos + 1));
        
        vector<Node> children = node->children;
        node->children.clear();
        
        for(int i = 0; i < splitted.size(); i++) {
            bool isInChildren = false;
            Node newChild;
            for(int j = 0; j < children.size(); j++) {
                Node child = children[j];
                if(splitted[i] == child->name) {
                    isInChildren = true;
                    newChild = child;
                    break;
                } else if(splitted[i].find(child->name) != string::npos) {
                    isInChildren = true;
                    newChild = Node(new Node_(splitted[i], ""));
                    splitByOps(child);
                    newChild->children.push_back(child);
                    break;
                }
            }
            if(!isInChildren) {
                newChild = Node(new Node_(splitted[i], ""));
            }
            newChild = splitByOps(newChild);
            node->children.push_back(newChild);
        }
        
        node->val = op;
        node->inner = "";
        return node;
    }
    
    Node parseTerm(Node node) {
        if(!node) return node;
        
        if(Constant::isConstant(node->inner) || X::eq(node->inner) || Y::eq(node->inner) || Z::eq(node->inner)) {
            node->val = node->inner;
            node->inner = "";
        }
        
        for(int i = 0; i < node->children.size(); i++) {
            if(node->children[i]->name != "" && node->inner.find(node->children[i]->name) != string::npos) {
                node->val = node->inner;
                node->inner = "";
                replace(node->val, node->children[i]->name, "");
            }
        }
        
        for(int i = 0; i < node->children.size(); i++) {
            node->children[i] = parseTerm(node->children[i]);
        }
        return node;
    }
    
    Node master;
    string source;
    vector<pair<int, int> > bracketPositions;
    
    static string replace(string &str, const string &search, const string &rep, int n = 0) {
        string::size_type pos(str.find(search));
        int c = 0;
        
        while(pos != string::npos) {
            str.replace(pos, search.length(), rep);
            pos = str.find(search, pos + rep.length() );
            c++;
            if(n == c) return str;
        }
        
        return str;
    }
    
    inline static string trimSpace(string source) {
        return replace(source, " ", "");
    }
    
    vector<string> split(const string &str, const string &delim){
        vector<string> res;
        size_t current = 0, found, delimlen = delim.size();
        while((found = str.find(delim, current)) != string::npos){
            res.push_back(string(str, current, found - current));
            current = found + delimlen;
        }
        res.push_back(string(str, current, str.size() - current));
        return res;
    }
    
    int id;
    
    string getID() {
        stringstream ss;
        ss << "[_" << id << "]";
        id++;
        if(1000 < id) ofExit();
        return ss.str();
    }
};

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)
