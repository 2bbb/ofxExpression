//
//  StringUtilities.h
//
//  Created by ISHII 2bit on 2015/02/24.
//
//

#pragma once

#include "ofMain.h"

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

BEGIN_NAMESPACE(ofx)
BEGIN_NAMESPACE(Expression)

static string replace(string source, const string &search, const string &rep, int n = 0) {
    string::size_type pos(source.find(search));
    int c = 0;
    
    while(pos != string::npos) {
        source.replace(pos, search.length(), rep);
        pos = source.find(search, pos + rep.length() );
        if(n == ++c) break;
    }
    
    return source;
}

static vector<string> split(const string &str, const string &delim, int n = 0){
    vector<string> res;
    size_t current = 0, found, delimlen = delim.size();
    int c = 0;
    while((found = str.find(delim, current)) != string::npos){
        res.push_back(string(str, current, found - current));
        current = found + delimlen;
        if(n == ++c) break;
    }
    res.push_back(string(str, current, str.size() - current));
    return res;
}

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)
