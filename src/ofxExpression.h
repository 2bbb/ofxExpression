//
//  ofxExpression.h
//
//  Created by ISHII 2bit on 2015/02/22.
//
//

#pragma once

#include "Expression.h"

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

BEGIN_NAMESPACE(ofx)
BEGIN_NAMESPACE(Expression)

class ofxExpression {
    Expr_ expression;
public:
    ofxExpression() {}
    ofxExpression(string expr, bool isPN = true) {
        if(isPN) {
            parsePN(expr);
        } else {
            parsePN(expr);
        }
    }
    
    float eval(float x = 0.0f, float y = 0.0f, float z = 0.0f) const {
        if(expression) return expression->eval(x, y, z);
        else {
            ofLogError("ofxExpression") << "evaluate invalid expression!!";
            return 0.0f;
        }
    }
    
    inline float operator()(float x = 0.0f, float y = 0.0f, float z = 0.0f) const {
        return eval(x, y, z);
    }
    
    bool isValid() const {
        return static_cast<bool>(expression);
    }

    Expr_ parseError(const string &error) {
        ofLogError("ofxExpression") << error;
        return Expr_();
    }
    
    static bool isEmpty(string str) {
        return str.length() == 0;
    }
    
    Expr_ parsePN(string expr) {
        vector<string> splitted = ofSplitString(expr, " ");
        ofRemove(splitted, &ofxExpression::isEmpty);
        queue<string> commands;
        for(const string &command : splitted) {
            commands.push(command);
        }
        return expression = parsePN_impl(commands);
    }
    
    Expr_ parsePN_impl(queue<string> &commands) {
        if(commands.size() == 0) {
            return expression = parseError("command is finished before complete parsing");
        }
        const string &command = commands.front();
        commands.pop();
        if(isBinaryOp(command)) {
            Expr_ arg1 = parsePN_impl(commands);
            Expr_ arg2 = parsePN_impl(commands);
            expression = binaryOp(command, arg1, arg2);
        } else if(isUnaryOp(command)) {
            Expr_ arg = parsePN_impl(commands);
            expression = unaryOp(command, arg);
        } else if(isVariable(command)) {
            expression = variable(command);
        } else {
            expression = constant(command);
        }
        if(!expression) {
            ofLogError("ofxExpression") << (command + " : unknown command");
        }
        return expression;
    }

#define Eq(Op) Op::eq(command)
    
#define ConstructVar(Op) if(Eq(Op)) return Expr_(new Op());
    inline bool isVariable(const string &command) const {
        return Eq(X)
            || Eq(Y)
            || Eq(Z)
        ;
    }
    Expr_ variable(const string &command) const {
        ConstructVar(X);
        ConstructVar(Y);
        ConstructVar(Z);
        return Expr_();
    }
#undef ConstructVar
    
#define ConstructUnary(Op) if(Eq(Op)) return Expr_(new Op(arg));
    inline bool isUnaryOp(const string &command) const {
        return Eq(Sin)
            || Eq(Cos)
            || Eq(Tan)
            || Eq(Asin)
            || Eq(Acos)
            || Eq(Atan)
            || Eq(Abs)
            || Eq(Log)
            || Eq(Sqrt)
        ;
    }
    Expr_ unaryOp(const string &command, Expr_ arg) {
        if(arg) {
            ConstructUnary(Sin);
            ConstructUnary(Cos);
            ConstructUnary(Tan);
            ConstructUnary(Asin);
            ConstructUnary(Acos);
            ConstructUnary(Atan);
            ConstructUnary(Abs);
            ConstructUnary(Log);
            ConstructUnary(Sqrt);
        }
        return Expr_();
    }
#undef ConstructUnary
    
#define ConstructBinary(Op) if(Eq(Op)) return Expr_(new Op(arg1, arg2));
    inline bool isBinaryOp(const string &command) const {
        return Eq(Add)
            || Eq(Sub)
            || Eq(Mul)
            || Eq(Div)
            || Eq(Pow)
        ;
    }
    Expr_ binaryOp(const string &command, Expr_ arg1, Expr_ arg2) {
        if(arg1 && arg2) {
            ConstructBinary(Add);
            ConstructBinary(Sub);
            ConstructBinary(Mul);
            ConstructBinary(Div);
            ConstructBinary(Pow);
        }
        return Expr_();
    }
#undef ConstructBinary
#undef Eq
    
    Expr_ constant(const string &command) {
        string dotTrashed = command;
        ofStringReplace(dotTrashed, ".", "");
        if(1 < command.length() - dotTrashed.length()) {
            return parseError(command + " : many . appeared.");
        }
        const char *s = dotTrashed.c_str();
        if(!isdigit(*s) && *s != '-') {
            return parseError(command + " : is not numeric string.");
        }
        s++;
        while(*s != '\0') {
            if(!isdigit(*s++)) return parseError(command + " : is not numeric string.");
        }
        
        return Expr_(new Constant(ofToFloat(command)));
    }
};

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)

using ofx::Expression::ofxExpression;