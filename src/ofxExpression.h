//
//  ofxExpression.h
//
//  Created by ISHII 2bit on 2015/02/22.
//
//

#pragma once

#include "Expression.h"
#include "Parser.h"

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

BEGIN_NAMESPACE(ofx)
BEGIN_NAMESPACE(Expression)

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
    
    bool parse(string expr) {
        Parser parser(expr);
        if(parser.isValidExpression()) {
            return parsePN(parser.polishNotationizedSource());
        } else {
            ofLogError("ofxExpression") << "evaluate invalid expression!!" << expr;
            return false;
        }
    }
    
    bool parsePN(string expr) {
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
        expression = parsePN_impl(commands);
        if(!expression) {
            ofLogError("ofxExpression") << "evaluate invalid expression!!" << expr;
        }
        return expression;
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
        if(!Constant::isConstant(command)) {
            return Expr();
        }
        
        return Expr(new Constant(ofToFloat(command)));
    }
};

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)

using ofx::Expression::ofxExpression;