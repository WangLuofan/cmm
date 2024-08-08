#ifndef __CMM_DEF_H__
#define __CMM_DEF_H__

typedef enum ArithOper{
    ArithOper_Add,
    ArithOper_Sub,
    ArithOper_Mul,
    ArithOper_Div,
    ArithOper_Mod,
    ArithOper_Inc,
    ArithOper_Dec
}ArithOper;

typedef enum CompOper {
    CompOper_LessThan,
    CompOper_LessOrEqual,
    CompOper_Equal,
    CompOper_NotEqual,
    CompOper_GreaterThan,
    CompOper_GreaterOrEqual
}CompOper;

typedef enum LogicalOper {
    LogicalOper_And,
    LogicalOper_Or,
    LogicalOper_Not
}LogicalOper;

typedef enum BitOper {
    BitOper_And,
    BitOper_Or,
    BitOper_Not,
    BitOper_Xor,
    BitOper_LShift,
    BitOper_RShift
}BitOper;

typedef enum AssignOper {
    AssignOper_Assign,
    AssignOper_AddAssign,
    AssignOper_SubAssign,
    AssignOper_MulAssign,
    AssignOper_DivAssign,
    AssignOper_ModAssign,
    AssignOper_AndAssign,
    AssignOper_OrAssign,
    AssignOper_XorAssign,
    AssignOper_LShiftAssign,
    AssignOper_RShiftAssign
}AssignOper;

typedef enum TypeKind {
    TypeKind_Void,
    TypeKind_Char,
    TypeKind_Short,
    TypeKind_Int,
    TypeKind_Long,
    TypeKind_LongLong,
    TypeKind_Float,
    TypeKind_Double,
    TypeKind_LongDouble
}TypeKind;

#endif