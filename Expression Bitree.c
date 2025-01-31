/*
本文将用于实现根据逻辑表达式建立逻辑二叉表达式树，内容包括：
1. 逻辑二叉表达式树的定义代码
2. 获取逻辑表达式输入的实现代码
3. 逻辑表达式的合法性检验的实现代码
4. 合法性检验返回参数的处理的实现代码
5. 根据合法逻辑表达式建立逻辑二叉表达式树的实现代码
*/

#include "Queue & Stack.c"

typedef struct Binode {
    char data;
    struct Binode *left;
    struct Binode *right;
} Binode, *Bitree; //有左右子树的二叉树节点

QList GetExpression() {
    //获取输入的表达式，存储到链队列模拟的字符串中
    QList Q = InitQueue();
    char ch = getchar();

    while (ch != '\n') {
        EnQueue(Q, ch);
        ch = getchar();
    } //while

    return Q;
} //GetExpression

int IsLegal(char ch) {
    //判断字符ch是否合法
    return ch >= 'A' && ch <= 'Z' || ch == '0' || ch == '1' || ch == '!' || ch == '&' || ch == '|' || ch == '-' || ch == '<' || ch == '>' || ch == '(' || ch == ')';
} //IsVariable

int CheckExpression(QList Q) {
    //检验表达式Q是否合法
    QNode p = Q->front->next;
    int Layer = 0;

    if (QueueEmpty(Q)) return 0; //表达式为空
    while (p != Q->rear) {
        if (!IsLegal(p->data)) return -1; //表达式有非法字符
        if (p->data == '(') Layer++;
        else if (p->data == ')') {
            Layer--;
            if (Layer < 0) return -3; //右括号匹配失败
        } else if (p->data == '-') {
            if (p->next != Q->rear && p->next->data == '>') p = p->next;
            else return -1; //未构成运算符
        } else if (p->data == '<') {
            if (p->next != Q->rear && p->next->next != Q->rear && p->next->data == '-' && p->next->next->data == '>') p = p->next->next;
            else return -1; //未构成运算符
        } //if
        p = p->next;
    } //while
    if (Layer > 0) return -2;

    return 1;
} //CheckExpression

void ErrorPrint(QList Q, int E) {
    //根据错误类型显示错误
    QNode p = Q->front->next;
    int pos = 1, Layer = 0, Lpos;

    switch (E) {
        case 0: { //输入为空串
            printf("Error: Empty Input!\n");
            break;
        } case -1: { //输入包含非法字符
            while (p != Q->rear) {
                if (!IsLegal(p->data)) break;
                if (p->data == '-' && (p->next == Q->rear || p->next->data != '>')) break;
                if (p->data == '<' && (p->next == Q->rear || p->next->next == Q->rear || p->next->data != '-' || p->next->next->data != '>')) break;
                p = p->next;
                pos++;
            } //while
            printf("Error: Illgeal character!\n");
            printf("The first illegal character is %c, located at %d!\n", p->data, pos);
            break;
        } case -2: { //输入有左括号匹配失败
            while (p != Q->rear) {
                if (p->data == '(') {
                    Layer++;
                    if (Layer == 1) Lpos = pos;
                } else if (p->data == ')') Layer--;
                p = p->next;
                pos++;
            } //while
            printf("Error: Unmatched left bracket!\n");
            printf("The first unmatched left bracket locates at %d!\n", Lpos);
            break;
        } case -3: { //输入有右括号匹配失败
            while (p != Q->rear) {
                if (p->data == '(') Layer++;
                else if (p->data == ')') {
                    Layer--;
                    if (Layer < 0) break;
                } //if
                p = p->next;
                pos++;
            } //while
            printf("Error: Unmatched right bracket!\n");
            printf("The first unmatched right bracket loactes at %d!\n", pos);
            break;
        } default: break;
    } //switch
    DestroyQueue(Q); //销毁非法表达式
} //ErrorPrint