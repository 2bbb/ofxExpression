//
//  Parser.h
//
//  Created by ISHII 2bit on 2015/02/22.
//
//

#pragma once

#include "Expression.h"
#include "StringUtilities.h"

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

BEGIN_NAMESPACE(ofx)
BEGIN_NAMESPACE(Expression)

class Parser {
    struct Node_;
    typedef shared_ptr<Node_> Node;
    struct Node_ {
        Node_(const string &val, const string &name)
        : val(val)
        , name(name) {}
        string val;
        string name;
        vector<Node> children;
        string print() const {
            stringstream ss;
            ss << val;
            for(int i = 0; i < children.size(); i++) {
                ss << " " << children[i]->print();
            }
            return ss.str();
        }
    };
    
    Node createNode(const string &val, const string &name = "") {
        return Node(new Node_(val, name));
    }
public:
    Parser(const string &source)
    : source(source)
    , index(0)
    , tag("ofxExpression::Parser") {
        treatment();
        parse();
    }
    
    bool parse() {
        master = parseImpl(createNode(treatment()));
        return master;
    }
    
    bool isValidExpression() const {
        return master;
    }
    
    string polishNotationizedSource() const {
        if(master) {
            return master->print();
        } else {
            ofLogError(tag) << "illegal expression";
            return "";
        }
    }
    
    string rawSource() const {
        return source;
    }
    
private:
    string tag;
    string treatment() const {
        string source = replace(this->source, " ", "");
        source = replace(source, "\t", "");
        source = replace(source, "\n", "");
        source = replace(source, "--", "+");
        source = replace(source, "++", "+");
        source = replace(source, "-+", "-");
        source = replace(source, "+-", "-");
        return source;
    }
    
    Node parseImpl(Node node) {
        map<string, string> fragments;
        int nest = 0, left;
        string s;
        for(int i = 0; i < node->val.length(); i++) {
            switch(node->val[i]) {
                case '(':
                    if(nest == 0) left = i;
                    nest++;
                    break;
                case ')':
                    nest--;
                    if(nest < 0) {
                        ofLogError(tag) << "unbalanced brackets.";
                        return Node();
                    } else if(nest == 0) {
                        string fragment = node->val.substr(left, i - left + 1);
                        string name = getID();
                        node->val = replace(node->val, fragment, name, 1);
//                        fragment = fragment.substr(1, fragment.length() - 2);
                        fragments[name] = fragment;
                        i = -1;
                    }
                    break;
                default:
                    break;
            }
        }
        
        string op;
        if((op = searchOps(node->val)) != "") {
            size_t pos = node->val.find_last_of(op);
            string left = node->val.substr(0, pos);
            string right = node->val.substr(pos + 1);
            for(auto it = fragments.begin(); it != fragments.end(); it++) {
                left = replace(left, it->first, it->second);
                right = replace(right, it->first, it->second);
            }
            Node leftNode = parseImpl(createNode(left));
            if(!leftNode) {
                ofLogError(tag) << "invalid left : " << left;
                return Node();
            }
            Node rightNode = parseImpl(createNode(right));
            if(!rightNode) {
                ofLogError(tag) << "invalid right : " << right;
                return Node();
            }
            node->children.push_back(leftNode);
            node->children.push_back(rightNode);
            if(op != ",") node->val = op;
            else node->val = "";
        } else {
            if(fragments.size() == 1) {
                auto pair = fragments.begin();
                node->val = replace(node->val, pair->first, "");
                string fragment = pair->second.substr(1, pair->second.length() - 2);
                if(false && fragment.find(",") != string::npos) {
                    vector<string> splitted = split(fragment, ",");
                    for(int i = 0; i < splitted.size(); i++) {
                        Node child = parseImpl(createNode(splitted[i]));
                        if(!child) {
                            ofLogError(tag) << "invalid : " << fragment << " / " << splitted[i];
                            return Node();
                        }
                        node->children.push_back(child);
                    }
                } else {
                    Node child = parseImpl(createNode(fragment));
                    if(!child) {
                        ofLogError(tag) << "invalid :" << fragment;
                        return Node();
                    }
                    node->children.push_back(child);
                }
            }
        }
        
        return node;
    }
    
    string searchOps(string fragment) const {
        static const char * ops[] = {",", "+", "-", "*", "/"};
        for(int i = 0; i < 5; i++) {
            if(fragment.find_last_of(ops[i]) != string::npos) {
                return ops[i];
            }
        }
        return "";
    }
    
    string source;
    Node master;
    
    int index;
    
    string getID() {
        stringstream ss;
        ss << "[#" << index << "]";
        index++;
        if(500 < index) {
            ofLogError(tag) << "fuck";
            ofExit(-1);
        }
        return ss.str();
    }
};

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)
