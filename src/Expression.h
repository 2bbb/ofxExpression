//
//  Expression.h
//
//  Created by ISHII 2bit on 2015/02/22.
//
//

#pragma once

#include "ofMain.h"

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

BEGIN_NAMESPACE(ofx)
BEGIN_NAMESPACE(Expression)

struct Expr;
typedef shared_ptr<Expr> Expr_;

struct Expr {
    Expr(Expr_ arg1 = Expr_(), Expr_ arg2 = Expr_(), Expr_ arg3 = Expr_())
    : arg1(arg1)
    , arg2(arg2)
    , arg3(arg3) {}
    
    virtual float eval(float x = 0.0f, float y = 0.0f, float z = 0.0f) const = 0;
protected:
    Expr_ arg1, arg2, arg3;
};

#define body(expr) virtual float eval(float x, float y, float z) const { return expr; }
#define op_check(op_expr)\
    static const string commandName() { return op_expr; }\
    static bool eq(const string &command) { return command == string(op_expr); }

struct Constant : Expr {
    float val;
    Constant(float val) : val(val) {}
    body(val);
};

#define var(name, expr, op_expr) struct name : Expr  {\
    body(expr);\
    op_check(op_expr);\
};

var(X, x, "x");
var(Y, y, "y");
var(Z, z, "z");

#undef var

#define ev(arg) arg->eval(x, y, z)
#define _1 ev(arg1)
#define _2 ev(arg2)
#define _3 ev(arg3)

#define bin_op(name, expr, op_expr)\
struct name : Expr {\
    name(Expr_ arg1, Expr_ arg2) : Expr(arg1, arg2) {};\
    body(expr);\
    op_check(op_expr);\
};

bin_op(Add, _1 + _2, "add");
bin_op(Sub, _1 - _2, "sub");
bin_op(Mul, _1 * _2, "mul");
bin_op(Div, _1 / _2, "div");
bin_op(Pow, powf(_1, _2), "pow");

#undef bin_op

#define un_op(name, expr, op_expr)\
struct name : Expr  {\
    name(Expr_ arg1) : Expr(arg1) {};\
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

#undef ev

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)
