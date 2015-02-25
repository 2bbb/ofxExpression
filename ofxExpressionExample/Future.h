//
//  Future.h
//
//  Created by ISHII 2bit on 2015/02/25.
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

END_NAMESPACE(Expression)
END_NAMESPACE(ofx)
