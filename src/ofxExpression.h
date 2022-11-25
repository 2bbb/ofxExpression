//
//  ofxExpression.h
//
//  Created by ISHII 2bit on 2015/02/22.
//
//

#pragma once

#include "Expression.h"
#include "Parser.h"

#include "ofUtils.h"

#include <string>
#include <queue>

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

BEGIN_NAMESPACE(ofx)
BEGIN_NAMESPACE(Expression)

class ofxExpression {
    Expr expression;
public:
    ofxExpression() {}
    ofxExpression(const std::string &expr, bool isPN = true) {
        if(isPN) {
            parsePN(expr);
        } else {
            parsePN(expr);
        }
    }
    
    float eval(float x1 = 0.0f,
               float x2 = 0.0f,
               float x3 = 0.0f,
               float x4 = 0.0f,
               float x5 = 0.0f,
               float x6 = 0.0f,
               float x7 = 0.0f,
               float x8 = 0.0f,
               float x9 = 0.0f,
               float x10 = 0.0f) const
    {
        if(expression) return expression->eval(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10);
        else {
            return 0.0f;
        }
    }
    
    inline float operator()(float x1 = 0.0f,
                            float x2 = 0.0f,
                            float x3 = 0.0f,
                            float x4 = 0.0f,
                            float x5 = 0.0f,
                            float x6 = 0.0f,
                            float x7 = 0.0f,
                            float x8 = 0.0f,
                            float x9 = 0.0f,
                            float x10 = 0.0f) const
    {
        return eval(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10);
    }
    
    bool isValid() const {
        return static_cast<bool>(expression);
    }

    Expr parseError(const std::string &error) {
        ofLogError("ofxExpression") << error;
        return Expr();
    }
    
    static bool isEmpty(const std::string &str) {
        return str.length() == 0;
    }
    
    bool parse(const std::string &expr) {
        Parser parser(expr);
        if(parser.isValidExpression()) {
            return parsePN(parser.polishNotationizedSource());
        } else {
            ofLogError("ofxExpression") << "evaluate invalid expression!!" << expr;
            return false;
        }
    }
    
    bool parsePN(const std::string &expr) {
        std::vector<std::string> splitted = ofSplitString(expr, " ");
        ofRemove(splitted, &ofxExpression::isEmpty);
        std::queue<std::string> commands;
        for(const std::string &command : splitted) {
            if(command == "+")      commands.push(Add::commandName());
            else if(command == "-") commands.push(Sub::commandName());
            else if(command == "*") commands.push(Mul::commandName());
            else if(command == "/") commands.push(Div::commandName());
            else commands.push(command);
        }
        expression = parsePN_impl(commands);
        if(!expression) {
            ofLogError("ofxExpression") << "evaluate invalid expression!!" << expr;
        }
        return expression != nullptr;
    }
    
    Expr parsePN_impl(std::queue<std::string> &commands) {
        if(commands.size() == 0) {
            return expression = parseError("command is finished before complete parsing");
        }
        std::string command = commands.front();
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
    inline bool isVariable(const std::string &command) const {
        return Eq(X)
            || Eq(Y)
            || Eq(Z)
            || Eq(W)
            || Eq(X1)
            || Eq(X2)
            || Eq(X3)
            || Eq(X4)
            || Eq(X5)
            || Eq(X6)
            || Eq(X7)
            || Eq(X8)
            || Eq(X9)
            || Eq(X10)
        ;
    }
    Expr variable(const std::string &command) const {
        ConstructVar(X);
        ConstructVar(Y);
        ConstructVar(Z);
        ConstructVar(W);
        ConstructVar(X1);
        ConstructVar(X2);
        ConstructVar(X3);
        ConstructVar(X4);
        ConstructVar(X5);
        ConstructVar(X6);
        ConstructVar(X7);
        ConstructVar(X8);
        ConstructVar(X9);
        ConstructVar(X10);
        return Expr();
    }
#undef ConstructVar
    
#define ConstructUnary(Op) if(Eq(Op)) return Expr(new Op(arg));
    inline bool isUnaryOp(const std::string &command) const {
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
    Expr unaryOp(const std::string &command, Expr arg) {
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
    inline bool isBinaryOp(const std::string &command) const {
        return Eq(Add)
            || Eq(Sub)
            || Eq(Mul)
            || Eq(Div)
            || Eq(Fmod)
            || Eq(Pow)
        ;
    }
    Expr binaryOp(const std::string &command, Expr arg1, Expr arg2) {
        if(arg1 && arg2) {
            ConstructBinary(Add);
            ConstructBinary(Sub);
            ConstructBinary(Mul);
            ConstructBinary(Div);
            ConstructBinary(Fmod);
            ConstructBinary(Pow);
        }
        return Expr();
    }
#undef ConstructBinary
#undef Eq
    
    Expr constant(const std::string &command) {
        if(!Constant::isConstant(command)) {
            return Expr();
        }
        
        return Expr(new Constant(ofToFloat(command)));
    }
};

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)

using ofx::Expression::ofxExpression;
