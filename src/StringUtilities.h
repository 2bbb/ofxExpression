//
//  StringUtilities.h
//
//  Created by ISHII 2bit on 2015/02/24.
//
//

#pragma once

#include <string>
#include <vector>

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

BEGIN_NAMESPACE(ofx)
BEGIN_NAMESPACE(Expression)

static std::string replace(std::string source,
                           const std::string &search,
                           const std::string &rep,
                           int n = 0)
{
    std::string::size_type pos(source.find(search));
    int c = 0;
    
    while(pos != std::string::npos) {
        source.replace(pos, search.length(), rep);
        pos = source.find(search, pos + rep.length() );
        if(n == ++c) break;
    }
    
    return source;
}

static std::vector<std::string> split(const std::string &str,
                                      const std::string &delim,
                                      int n = 0)
{
    std::vector<std::string> res;
    size_t current = 0, found, delimlen = delim.size();
    int c = 0;
    while((found = str.find(delim, current)) != std::string::npos){
        res.push_back(std::string(str, current, found - current));
        current = found + delimlen;
        if(n == ++c) break;
    }
    res.push_back(std::string(str, current, str.size() - current));
    return res;
}

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)
