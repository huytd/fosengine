#ifndef GM_EX_OPERATOR_H
#define GM_EX_OPERATOR_H

void CallOperatorUnary( gmThread* pThread, gmVariable *operand, gmOperator Op );
void CallOperatorBinary( gmThread* pThread, gmVariable *operand, gmOperator Op );

void GM_CDECL gmOpGetInd(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpSetInd(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpAdd(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpSub(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpMul(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpDiv(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpRem(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpBitOr(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpBitXor(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpBitAnd(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpBitShiftLeft(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpBitShiftRight(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpBitInv(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpLessThan(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpGreaterThan(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpLessThanOrEqual(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpGreaterThanOrEqual(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpEqual(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpNotEqual(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpNeg(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpPos(gmThread* pThread, gmVariable *operand);
void GM_CDECL gmOpNot(gmThread* pThread, gmVariable *operand);

#endif