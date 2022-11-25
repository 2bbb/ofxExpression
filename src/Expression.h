//
//  Expression.h
//
//  Created by ISHII 2bit on 2015/02/22.
//
//

#pragma once

#include "ofUtils.h"

#include <memory>
#include <string>
#include <ctype.h>

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

BEGIN_NAMESPACE(ofx)
BEGIN_NAMESPACE(Expression)

struct Expr_;
typedef std::shared_ptr<Expr_> Expr;

struct Expr_ {
    Expr_(Expr arg1 = Expr(),
          Expr arg2 = Expr(),
          Expr arg3 = Expr(),
          Expr arg4 = Expr(),
          Expr arg5 = Expr(),
          Expr arg6 = Expr(),
          Expr arg7 = Expr(),
          Expr arg8 = Expr(),
          Expr arg9 = Expr(),
          Expr arg10 = Expr())
    : arg1{arg1}
    , arg2{arg2}
    , arg3{arg3}
    , arg4{arg4}
    , arg5{arg5}
    , arg6{arg6}
    , arg7{arg7}
    , arg8{arg8}
    , arg9{arg9}
    , arg10{arg10} {}
    
    virtual float eval(float x1 = 0.0f,
                       float x2 = 0.0f,
                       float x3 = 0.0f,
                       float x4 = 0.0f,
                       float x5 = 0.0f,
                       float x6 = 0.0f,
                       float x7 = 0.0f,
                       float x8 = 0.0f,
                       float x9 = 0.0f,
                       float x10 = 0.0f) const = 0;
protected:
    Expr arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10;
};

#define body(expr) virtual float eval(float x1 = 0.0f, float x2 = 0.0f, float x3 = 0.0f, float x4 = 0.0f, float x5 = 0.0f, float x6 = 0.0f, float x7 = 0.0f, float x8 = 0.0f, float x9 = 0.0f, float x10 = 0.0f) const { return expr; }
#define op_check(op_expr)\
    inline static const std::string commandName() { return op_expr; }\
    inline static bool eq(const std::string &command) { return command == std::string(op_expr); }

struct Constant : Expr_ {
    float val;
    Constant(float val) : val(val) {}
    body(val);
    static bool isConstant(std::string command) {
        std::string dotTrashed = command;
        ofStringReplace(dotTrashed, ".", "");
        if(1 < command.length() - dotTrashed.length()) {
            return false;
        }
        const char *s = dotTrashed.c_str();
        if(!isdigit(*s) && (*s != '-')) {
            return false;
        }
        s++;
        while(*s != '\0') {
            if(!isdigit(*s++)) return false;
        }
        return true;
    }
};

#define var(name, expr, op_expr) struct name : Expr_  {\
    body(expr);\
    op_check(op_expr);\
};

var( X,  x1,  "x");
var( Y,  x2,  "y");
var( Z,  x3,  "z");
var( W,  x4,  "w");
var( X1, x1,  "x1");
var( X2, x2,  "x2");
var( X3, x3,  "x3");
var( X4, x4,  "x4");
var( X5, x5,  "x5");
var( X6, x6,  "x6");
var( X7, x7,  "x7");
var( X8, x8,  "x8");
var( X9, x9,  "x9");
var(X10, x10, "x10");

#undef var

#define ev(arg) arg->eval(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10)
#define _1 ev(arg1)
#define _2 ev(arg2)
#define _3 ev(arg3)
#define _4 ev(arg4)
#define _5 ev(arg5)
#define _6 ev(arg6)
#define _7 ev(arg7)
#define _8 ev(arg8)
#define _9 ev(arg9)
#define _10 ev(arg10)

#define bin_op(name, expr, op_expr)\
struct name : Expr_ {\
    name(Expr arg1, Expr arg2) : Expr_(arg1, arg2) {};\
    body(expr);\
    op_check(op_expr);\
};

bin_op(Add, _1 + _2, "add");
bin_op(Sub, _1 - _2, "sub");
bin_op(Mul, _1 * _2, "mul");
bin_op(Div, _1 / _2, "div");
bin_op(Fmod, fmod(_1, _2), "fmod");
bin_op(Pow, powf(_1, _2), "pow");

#undef bin_op

#define un_op(name, expr, op_expr)\
struct name : Expr_  {\
    name(Expr arg1) : Expr_(arg1) {};\
    body(expr);\
    op_check(op_expr);\
};

un_op(Sin, sinf(_1), "sin");
un_op(Cos, cosf(_1), "cos");
un_op(Tan, tanf(_1), "tan");
un_op(Asin, asinf(_1), "asin");
un_op(Acos, acosf(_1), "acos");
un_op(Atan, atanf(_1), "atan");
un_op(Abs, fabsf(_1), "abs");
un_op(Log, logf(_1), "log");
un_op(Sqrt, sqrtf(_1), "sqrt");
un_op(Floor, floorf(_1), "floor");
un_op(Ceil, ceilf(_1), "ceil");
un_op(Round, roundf(_1), "round");

#undef un_op

#undef _1
#undef _2
#undef _3
#undef _4
#undef _5
#undef _6
#undef _7
#undef _8
#undef _9
#undef _10

#undef ev

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)
