#include "Queue & Stack.c"
#include <string.h>

typedef struct BiNode { //定义二叉树节点
    char data;
    struct BiNode *left;
    struct BiNode *right;
} BiNode, *BiTree;

QList GetExpression() {
    //返回存储有输入表达式的链队列
    QList Q = InitQueue();
    char ch = getchar();

    while (ch != '\n') { //逐个获取输入字符
        EnQueue(Q, ch);
        ch = getchar();
    } //while

    return Q;
} //GetExpreesion

char *Transform(QList Q) {
    //对链队列Q存储的表达式进行处理
    QList NQ = InitQueue();
    QNode p = Q->front->next;
    char *Exp, ch;
    int len = 0;
    int i;

    while (p != Q->rear) {
        if (p->next != Q->rear && p->data == '-' && p->next->data == '>') { //蕴涵运算符
            EnQueue(NQ, '>');
            p = p->next;
        } else if (p->next != Q->rear && p->next->next != Q->rear && p->data == '<' && p->next->data == '-' && p->next->next->data == '>') { //等价运算符
            EnQueue(NQ, '=');
            p = p->next->next;
        } else EnQueue(NQ, p->data); //其他符号
        p = p->next;
        len++;
    } //while
    DestroyQueue(Q); //用完了销毁
    Exp = (char*)malloc(sizeof(char) * (len + 1));
    for (i = 0; i <= len; i++) { //将NQ存储的表达式转移到Exp中
        ch = DeQueue(NQ);
        Exp[i] = ch;
    } //for
    Exp[len] = '\0';
    DestroyQueue(NQ);

    return Exp;
} //Transform

int IsLegal(char ch) {
    //返回字符ch是否合法
    return ch >= 'A' && ch <= 'Z' || ch == '!' || ch == '&' || ch == '|' || ch == '>' || ch == '=' || ch == '(' || ch == ')';
} //IsLegal

int CheckString1(char Exp[]) {
    //对表达式进行合法性检验
    int Layer = 0;
    int i;

    if (Exp[0] == '\0') return 0; //空表达式
    for (i = 0; Exp[i] != '\0'; i++) { //检验是否包含非法字符
        if (!IsLegal(Exp[i])) return -1; //包含非法字符
    } //for
    for (i = 0; Exp[i] != '\0'; i++) { //检验是否存在左右括号匹配失败
        if (Exp[i] == '(') Layer++; //左括号
        else if (Exp[i] == ')') { //右括号
            if (Layer == 0) return -3; //右括号匹配失败
            Layer--;
        } //if
    } //for
    if (Layer > 0) return -2; //左括号匹配失败
    for (i = 0; Exp[i + 1] != '\0'; i++) { //检验是否存在空括号
        if (Exp[i] == '(' && Exp[i + 1] == ')') return -4;
    } //for

    return 1;
} //CheckString1