//
//  Parser.h
//
//  Created by ISHII 2bit on 2015/02/22.
//
//

#pragma once

#include "Expression.h"
#include "StringUtilities.h"

#include "ofLog.h"
#include "ofAppRunner.h"

#include <sstream>
#include <vector>
#include <map>

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

BEGIN_NAMESPACE(ofx)
BEGIN_NAMESPACE(Expression)

class Parser {
    struct Node_;
    typedef std::shared_ptr<Node_> Node;
    struct Node_ {
        Node_(const std::string &val, const std::string &name)
        : val{val}
        , name{name}
        {}
        std::string val;
        std::string name;
        std::vector<Node> children;
        std::string print() const {
            std::stringstream ss{""};
            ss << val;
            for(int i = 0; i < children.size(); i++) {
                ss << " " << children[i]->print();
            }
            return ss.str();
        }
    };
    
    Node createNode(const std::string &val, const std::string &name = "") {
        return Node(new Node_(val, name));
    }
public:
    Parser(const std::string &source)
    : source{source}
    , index{0}
    , tag{"ofxExpression::Parser"}
    {
        treatment();
        parse();
    }
    
    bool parse() {
        master = parseImpl(createNode(treatment()));
        return isValidExpression();
    }
    
    bool isValidExpression() const {
        return static_cast<bool>(master);
    }
    
    std::string polishNotationizedSource() const {
        if(master) {
            return master->print();
        } else {
            ofLogError(tag) << "illegal expression: \"" << rawSource() << "\"";
            return "";
        }
    }
    
    std::string rawSource() const {
        return source;
    }
    
private:
    std::string tag;
    std::string treatment() const {
        std::string source = replace(this->source, " ", "");
        source = replace(source, "\t", "");
        source = replace(source, "\n", "");
        source = replace(source, "--", "+");
        source = replace(source, "++", "+");
        source = replace(source, "-+", "-");
        source = replace(source, "+-", "-");
        return source;
    }
    
    Node parseImpl(Node node) {
        std::map<std::string, std::string> fragments;
        int nest{0}, left;
        std::string s;
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
                        std::string fragment = node->val.substr(left, i - left + 1);
                        std::string name = getID();
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
        
        std::string op;
        if((op = searchOps(node->val)) != "") {
            size_t pos = node->val.find_last_of(op);
            std::string left = node->val.substr(0, pos);
            std::string right = node->val.substr(pos + 1);
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
                std::string fragment = pair->second.substr(1, pair->second.length() - 2);
                if(false && fragment.find(",") != std::string::npos) {
                    std::vector<std::string> splitted = split(fragment, ",");
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
    
    std::string searchOps(std::string fragment) const {
        static const char * ops[] = {",", "+", "-", "*", "/", "%", "^"};
        for(int i = 0; i < 7; i++) {
            if(fragment.find_last_of(ops[i]) != std::string::npos) {
                return ops[i];
            }
        }
        return "";
    }
    
    std::string source;
    Node master;
    
    int index;
    
    std::string getID() {
        std::stringstream ss{""};
        ss << "[#" << index << "]";
        index++;
        if(500 < index) {
            ofLogError(tag) << "???";
            ofExit(-1);
        }
        return ss.str();
    }
};

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)
