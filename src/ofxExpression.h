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

#define sadness_cpp11 0

#pragma mark for bright future
#if sadness_cpp11

template <typename ...>
struct BinaryOpHelper;

template <typename T, typename ... Ts>
struct BinaryOpHelper<T, Ts ...> {
    static bool IsBinaryOp(const string &command) {
        return T::eq(command) || BinaryOpHelper<Ts ...>::IsBinaryOp(command);
    }
    
    static Expr ConstructBinaryOp(const string &command, Expr arg1, Expr arg2) {
        if(T::eq(command)) return Expr(new T(arg1, arg2));
        else return BinaryOpHelper<Ts ...>::ConstructBinaryOp(command, arg1, arg2);
    }
};

template <>
struct BinaryOpHelper<> {
    static bool IsBinaryOp(const string &command) {
        return false;
    }
    
    static Expr ConstructBinaryOp(const string &command, Expr arg1, Expr arg2) {
        return Expr();
    }
};

using BOH = BinaryOpHelper<Add, Sub, Mul, Div, Pow>;

#endif

class ofxExpression {
    Expr expression;
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

    Expr parseError(const string &error) {
        ofLogError("ofxExpression") << error;
        return Expr();
    }
    
    static bool isEmpty(string str) {
        return str.length() == 0;
    }
    
    Expr parsePN(string expr) {
        ofLogNotice() << expr;
        vector<string> splitted = ofSplitString(expr, " ");
        ofRemove(splitted, &ofxExpression::isEmpty);
        queue<string> commands;
        for(const string &command : splitted) {
            if(command == "+")      commands.push(Add::commandName());
            else if(command == "-") commands.push(Sub::commandName());
            else if(command == "*") commands.push(Mul::commandName());
            else if(command == "/") commands.push(Div::commandName());
            else commands.push(command);
        }
        return expression = parsePN_impl(commands);
    }
    
    Expr parsePN_impl(queue<string> &commands) {
        if(commands.size() == 0) {
            return expression = parseError("command is finished before complete parsing");
        }
        string command = commands.front();
        commands.pop();
        if(isBinaryOp(command)) {
            Expr arg1 = parsePN_impl(commands);
            Expr arg2 = parsePN_impl(commands);
            expression = binaryOp(command, arg1, arg2);
        } else if(isUnaryOp(command)) {
            Expr arg = parsePN_impl(commands);
            expression = unaryOp(command, arg);
        } else if(isVariable(command)) {
            expression = variable(command);
        } else {
            expression = constant(command);
        }
        if(!expression) {
            return parseError(command + " : unknown command");
        }
        return expression;
    }

#define Eq(Op) Op::eq(command)
    
#define ConstructVar(Op) if(Eq(Op)) return Expr(new Op());
    inline bool isVariable(const string &command) const {
        return Eq(X)
            || Eq(Y)
            || Eq(Z)
        ;
    }
    Expr variable(const string &command) const {
        ConstructVar(X);
        ConstructVar(Y);
        ConstructVar(Z);
        return Expr();
    }
#undef ConstructVar
    
#define ConstructUnary(Op) if(Eq(Op)) return Expr(new Op(arg));
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
            || Eq(Floor)
            || Eq(Round)
            || Eq(Ceil)
        ;
    }
    Expr unaryOp(const string &command, Expr arg) {
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
            ConstructUnary(Floor);
            ConstructUnary(Ceil);
            ConstructUnary(Round);
        }
        return Expr();
    }
#undef ConstructUnary
    
#define ConstructBinary(Op) if(Eq(Op)) return Expr(new Op(arg1, arg2));
    inline bool isBinaryOp(const string &command) const {
        return Eq(Add)
            || Eq(Sub)
            || Eq(Mul)
            || Eq(Div)
            || Eq(Pow)
        ;
    }
    Expr binaryOp(const string &command, Expr arg1, Expr arg2) {
        if(arg1 && arg2) {
            ConstructBinary(Add);
            ConstructBinary(Sub);
            ConstructBinary(Mul);
            ConstructBinary(Div);
            ConstructBinary(Pow);
        }
        return Expr();
    }
#undef ConstructBinary
#undef Eq
    
    Expr constant(const string &command) {
        ofLogNotice("constant") << command;
        string dotTrashed = command;
        ofStringReplace(dotTrashed, ".", "");
        if(1 < command.length() - dotTrashed.length()) {
            return parseError(command + " : many . appeared.");
        }
        const char *s = dotTrashed.c_str();
        if(!isdigit(*s) && (*s != '-')) {
            return parseError(command + " : is not numeric string.");
        }
        s++;
        while(*s != '\0') {
            if(!isdigit(*s++)) return parseError(command + " : is not numeric string.");
        }
        
        return Expr(new Constant(ofToFloat(command)));
    }
};

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)

using ofx::Expression::ofxExpression;