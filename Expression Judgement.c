#include "Expression BiTree.c"

int main() {
    //主函数，程序的主体
    BiTree T;
    QList Q;
    char *Exp;
    int F, L;

    while (1) { //无限循环
        Q = GetExpression();
        Exp = Transform(Q);
        F = CheckExpression1(Exp);
        if (F != 1) ErrorPrint(Exp, F);
        else { //表达式合法
            printf("The expression is legal!\n");
            L = strlen(Exp);
            F = CheckExpression2(Exp, 0, L - 1);
            if (F != 1) ErrorPrint(Exp, F);
            else { //表达式可算
                printf("The exprssion is calculable!\n");
                T = CreateBiTree(Exp, 0, L - 1);
                DestroyBiTree(T);
            } //if
        } //if
    } //while
}