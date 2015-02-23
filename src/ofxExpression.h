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

    static bool isEmpty(string str) {
        return str.length() == 0;
    }
    
    bool isNumber(const string &str) {
        
    }
    
    Expr_ parseError(const string &error) {
        ofLogError("ofxExpression") << error;
        return Expr_();
    }
    
    Expr_ parsePN(string expr) {
        vector<string> splitted = ofSplitString(expr, " ");
        ofRemove(splitted, &ofxExpression::isEmpty);
        queue<string> commands;
        for(const string &command : splitted) {
            commands.push(command);
        }
        return expression = parsePN(commands);
    }
    
    bool isVariable(const string &command) const {
        return command == "x" || command == "y" || command == "z";
    }
    
    Expr_ variable(const string &command) const {
        if(command == "x") {
            return Expr_(new X());
        }
        if(command == "y") {
            return Expr_(new Y());
        }
        if(command == "z") {
            return Expr_(new Z());
        }
    }
    
    bool isUnaryOp(const string &command) const {
        return command == "sin" || command == "cos" || command == "tan" || command == "asin" || command == "acos" || command == "atan" || command == "log";
    }
    
    Expr_ unaryOp(const string &op, Expr_ arg) {
        if(!static_cast<bool>(arg)) {
            return Expr_();
        }
        if(op == "sin") {
            return Expr_(new Sin(arg));
        }
        if(op == "cos") {
            return Expr_(new Cos(arg));
        }
        if(op == "tan") {
            return Expr_(new Tan(arg));
        }
        if(op == "asin") {
            return Expr_(new Asin(arg));
        }
        if(op == "acos") {
            return Expr_(new Acos(arg));
        }
        if(op == "atan") {
            return Expr_(new Atan(arg));
        }
        if(op == "log") {
            return Expr_(new Log(arg));
        }
        
        return Expr_();
    }
    
    bool isBinaryOp(const string &command) const {
        return command == "+" || command == "add"
            || command == "-" || command == "sub"
            || command == "*" || command == "mul"
            || command == "/" || command == "div"
            || command == "pow";
    }
    
    Expr_ binaryOp(const string &op, Expr_ arg1, Expr_ arg2) {
        if(!(static_cast<bool>(arg1) && static_cast<bool>(arg2))) {
            return Expr_();
        }
        if(op == "+" || op == "add") {
            return Expr_(new Add(arg1, arg2));
        }
        if(op == "-" || op == "sub") {
            return Expr_(new Sub(arg1, arg2));
        }
        if(op == "*" || op == "mul") {
            return Expr_(new Mul(arg1, arg2));
        }
        if(op == "/" || op == "div") {
            return Expr_(new Div(arg1, arg2));
        }
        if(op == "pow") {
            return Expr_(new Pow(arg1, arg2));
        }
        return Expr_();
    }
    
    Expr_ constant(const string &command) {
        string dotTrashed = command;
        ofStringReplace(dotTrashed, ".", "");
        if(1 < command.length() - dotTrashed.length()) {
            return parseError(command + " : many . appeared.");
        }
        const char *s = dotTrashed.c_str();
        while(*s != '\0') {
            if(!isdigit(*s++)) return parseError(command + " : is not numeric string.");
        }
        
        return Expr_(new Constant(ofToFloat(command)));
    }
    
    Expr_ parsePN(queue<string> &commands) {
        if(commands.size() == 0) {
            return expression = parseError("command is finished before complete parsing");
        }
        const string &command = commands.front();
        commands.pop();
        if(isBinaryOp(command)) {
            Expr_ arg1 = parsePN(commands);
            Expr_ arg2 = parsePN(commands);
            expression = binaryOp(command, arg1, arg2);
        } else if(isUnaryOp(command)) {
            Expr_ arg = parsePN(commands);
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
};

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)

using ofx::Expression::ofxExpression;