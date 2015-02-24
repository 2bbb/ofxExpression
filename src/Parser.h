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
    struct Expr_;
    typedef shared_ptr<Expr_> Expr;
    struct Expr_ {
        string val;
        vector<Expr> child;
    };
public:
    Parser(string source) : source(trimSpace(source)) {
        parse();
    }
    
    void debug() const {
        for(auto p : bracketPositions) {
            cout << p.first << ", " << p.second << endl;
        }
    }
private:
    bool parse() {
        if(!checkBracket()) {
            bracketPositions.clear();
            cout << "unbalanced bracket." << endl;
            return false;
        }
        
        return true;
    }
    
    bool checkBracket() {
        int nest = 0;
        vector<int> leftPositions;
        for(int i = 0; i < source.length(); i++) {
            if(source.substr(i, 1) == "(") {
                leftPositions.push_back(i);
            }
            if(source.substr(i, 1) == ")") {
                if(leftPositions.size() == 0) return false;
                int p = leftPositions.back();
                leftPositions.pop_back();
                bracketPositions.push_back(make_pair(p, i));
            }
        }
        return leftPositions.size() == 0;
    }
    
    string source;
    vector<pair<int, int> > bracketPositions;
    
    static string replace(string source, const string &search, const string &rep) {
        string::size_type pos(source.find(search));
        
        while(pos != string::npos) {
            source.replace(pos, search.length(), rep);
            pos = source.find(search, pos + rep.length() );
        }
        
        return source;
    }
    
    inline static string trimSpace(string source) {
        return replace(source, " ", "");
    }
};

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)
