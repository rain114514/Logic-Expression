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
    return ch >= 'A' && ch <= 'Z' || ch == '0' || ch == '1' || ch == '!' || ch == '&' || ch == '|' || ch == '>' || ch == '=' || ch == '(' || ch == ')';
} //IsLegal

int CheckExpression1(char Exp[]) {
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
} //CheckExpression1

int IsMatched(char *Exp, int S, int E) {
    //检验字符串Exp[S..E]的两端的括号是否匹配
    int Layer = 0;
    int i;

    for (i = S; i < E; i++) { //除末端外全部遍历
        if (Exp[i] == '(') Layer++; //左括号
        else if (Exp[i] == ')') { //右括号
            Layer--;
            if (Layer == 0) return 0; //提前匹配成功
        } //if
    } //for

    return 1;
} //IsMatched

int IsOperator(char ch) {
    //判断一个符号是否是运算符
    return ch == '!' || ch == '&' || ch == '|' || ch == '>' || ch == '=';
} //IsOperator

int Compare(char ch1, char ch2) {
    //返回运算符ch1优先于于ch2的正确与否
    char Ope[5] = {'!', '&', '|', '>', '='};
    int pos1, pos2;
    int i;

    if (IsOperator(ch1) && IsOperator(ch2)) { //都是运算符
        for (i = 0; i < 5; i++) { //遍历
            if (Ope[i] == ch1) pos1 = i; //运算符1
            if (Ope[i] == ch2) pos2 = i; //运算符2
        } //for

        return pos1 < pos2;
    } //if
} //Compare

int FindLowest(char *Exp, int S, int E) {
    //找到表达式Exp[S..E]中最后被运算的运算符
    int **Ope;
    int L = strlen(Exp), Layer = 0, LO = 0, pos = 0;
    int i;

    Ope = (int**)malloc(L * sizeof(int*));
    for (i = 0; i < L; i++) Ope[i] = (int*)malloc(sizeof(int) * 2); //申请空间
    for (i = 0; i < L; i++) { //遍历
        if (IsOperator(Exp[i]) && Layer == 0) { //符合条件
            Ope[LO][0] = Exp[i];
            Ope[LO++][1] = i;
        } else if (Exp[i] == '(') Layer++; //左括号
        else if (Exp[i] == ')') Layer--; //右括号
    } //for
    for (i = 0; i < LO; i++) { //遍历
        if (Compare(Ope[pos][0], Ope[i][0]) || Ope[pos][0] == Ope[i][0] && Ope[pos][1] < Ope[i][1] ) pos = i; //优先级更低或更靠后
    } //for

    return Ope[pos][1];
} //FindLowest

int IsVariable(char ch) {
    //返回字符ch是否为逻辑变量
    return ch >= 'A' && ch <= 'Z';
} //IsVariable

int IsConstant(char ch) {
    //返回字符ch是否为逻辑常量
    return ch == '0' || ch == '1';
} //IsConstant

int CheckOperator(char *Exp, int pos, int S, int E) {
    //判断下标pos处的运算符有没有被正确使用
    if (pos < S || pos > E) return 0; //不在界内
    if (!IsOperator(Exp[pos])) return 0; //不是运算符
    if (Exp[pos] == '!') { //非运算符
        if (pos == E) return 0; //右边缘
        if (IsOperator(Exp[pos + 1])) return 0; //右边不是运算数
    } else if (Exp[pos] == '&') { //合取运算符
        if (pos == S || pos == E) return 0; //左右边缘
        if (IsOperator(Exp[pos - 1]) || IsOperator(Exp[pos + 1])) return 0; //左边或右边不是运算数
    } else if (Exp[pos] == '|') { //析取运算符
        if (pos == S || pos == E) return 0;
        if (IsOperator(Exp[pos - 1]) || IsOperator(Exp[pos + 1])) return 0;
    } else if (Exp[pos] == '>') { //蕴涵运算符
        if (pos == S || pos == E) return 0;
        if (IsOperator(Exp[pos - 1]) || IsOperator(Exp[pos + 1])) return 0;
    } else if (Exp[pos] == '=') { //等价运算符
        if (pos == S || pos == E) return 0;
        if (IsOperator(Exp[pos - 1]) || IsOperator(Exp[pos + 1])) return 0;
    } //if

    return 1;
} //CheckOperator

int CheckExpression2(char *Exp, int S, int E) {
    //对字符串Exp[S..E]进行可算性检验
    int pos, F1, F2;

    if (S > E) return -5; //没字符，不可算
    if (S == E) { //只有一个字符
        if (Exp[S] >= 'A' && Exp[S] <= 'Z' || Exp[S] == '0' || Exp[S] == '1') return 1; //可算字符
        else return -5; //不可算字符
    } //if
    while (S < E && Exp[S] == '(' && Exp[E] == ')') { //超过两个字符且有括号
        if (IsMatched(Exp, S, E)) { //匹配的括号
            S++;
            E--;
        } //if
    } //while
    pos = FindLowest(Exp, S, E);
    if (pos == -1) return -5; //没有这样的运算符
    if (!CheckOperator(Exp, pos, S, E)) return -5; //运算符未被正确使用
    F1 = CheckExpression2(Exp, S, pos - 1);
    F2 = CheckExpression2(Exp, pos + 1, E); //检验两个子式的可算性
    if (F1 && F2) return 1;
    else return -5;
} //CheckExpression2

void ErrorPrint(char Exp[], int E) {
    //根据错误类型E输出Exp中的错误
    int Layer = 0, Lpos;
    int i;

    switch (E) { //判断
        case 0: { //该表达式为空
            printf("Error: Empty expression!\n");
            break;
        } case -1: { //该表达式包含非法字符
            printf("Error: Illegal character!\n");
            for (i = 0; Exp[i] != '\0'; i++) { //寻找第一个非法字符
                if (!IsLegal(Exp[i])) break; //非法字符
            } //for
            printf("The first illegal character is %c, located at %d!\n", Exp[i], i + 1);
            break;
        } case -2: { //该表达式存在左括号匹配失败
            printf("Error: Unmatched left bracket!\n");
            for (i = 0; Exp[i] != '\0'; i++) { //遍历
                if (Exp[i] == '(') { //左括号
                    if (Layer == 0) Lpos = i + 1; //最外层左括号
                    Layer++;
                } else if (Exp[i] == ')') Layer--; //右括号
            } //for
            printf("The first unmatched left bracket locates at %d\n", Lpos);
            break;
        } case -3: { //该表达式存在右括号匹配失败
            printf("Error: Unmatched right bracket!\n");
            for (i = 0; Exp[i] != '\0'; i++) { //寻找第一个匹配失败的右括号
                if (Exp[i] == '(') Layer++; //左括号
                else if (Exp[i] == ')') { //右括号
                    if (Layer == 0) break; //右括号匹配失败
                    Layer--;
                } //if
            } //for
            printf("The first unmatched right bracket locates at %d\n", i + 1);
            break;
        } case -4: { //该表达式存在空括号
            printf("Error: Empty brackets!\n");
            for (i = 0; Exp[i + 1] != '\0'; i++) { //寻找第一个空括号
                if (Exp[i] == '(' && Exp[i + 1] == ')') break; //空括号
            } //for
            printf("The first empty bracket locates at %d\n", i + 1);
            break;
        } case -5: { //该表达式不可算
            printf("Error: Incalculable expression!\n");
            break;
        } //case
    } //switch
} //ErrorPrint

BiTree CreateBiTree(char Exp[], int S, int E) {
    //根据表达式Exp[S..E]建立逻辑二叉表达式树
    BiTree T;
    int pos;

    if (S > E) return NULL; //表达式为空
    if (S == E) { //表达式仅有一个逻辑变量或逻辑常量
        T = (BiTree)malloc(sizeof(BiNode));
        T->data = Exp[S];
        T->left = NULL;
        T->right = NULL;

        return T;
    } //if
    while (S <= E && Exp[S] == '(' && Exp[E] == ')') { //表达式被括号包裹
        if (IsMatched(Exp, S, E)) { //两端括号是匹配的
            S++;
            E--;
        } //if
    } //while
    pos = FindLowest(Exp, S, E);
    T = (BiTree)malloc(sizeof(BiNode));
    T->data = Exp[pos];
    T->left = CreateBiTree(Exp, S, pos - 1);
    T->right = CreateBiTree(Exp, pos + 1, E);

    return T;
} //CreateBiTree