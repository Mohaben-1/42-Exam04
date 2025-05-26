#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
    enum {
        ADD,
        MULTI,
        VAL
    }   type;
    int val;
    struct node *l;
    struct node *r;
}   node;



node    *new_node(node n)
{
    node *ret = calloc(1, sizeof(n));
    if (!ret)
        return (NULL);
    *ret = n;
    return (ret);
}

void    destroy_tree(node *n)
{
    if (!n)
        return ;
    if (n->type != VAL)
    {
        destroy_tree(n->l);
        destroy_tree(n->r);
    }
    free(n);
}




void    unexpected(char c)
{
    if (c)
        printf("Unexpected token '%c'\n", c);
    else
        printf("Unexpected end of input\n"); //+++++++++++++++
}

int accept(char **s, char c)
{
    if (**s == c)   //+++++++++++++++
    {
        (*s)++;
        return (1);
    }
    return (0);
}

int expect(char **s, char c)
{
    if (accept(s, c))
        return (1);
    unexpected(**s);
    return (0);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int      check_balance(char *s);
node    *parse_number_or_group(char **s);
node    *parse_addition(char **s);
node    *parse_multiplication(char **s);


node    *parse_number_or_group(char **s)
{
    node    *res;
    node    tmp;

    res = NULL;
    if (**s == '(')
    {
        (*s)++;
        res = parse_addition(s);
        if (!res || **s != ')')
        {
            destroy_tree(res);
            unexpected(**s);
            return (NULL);
        }
        (*s)++;
        return (res);
    }
    if (isdigit(**s))
    {
        tmp.type = VAL;
        tmp.val = **s - '0';
        res = new_node(tmp);
        (*s)++;
        return (res);
    }
    unexpected(**s);
    return (NULL);
}

node    *parse_addition(char **s)
{
    node    *left;
    node    *right;
    node    tmp;

    left = parse_multiplication(s);
    if (!left)
        return (NULL);
    while (**s == '+')
    {
        (*s)++;
        right = parse_multiplication(s);
        if (!right)
        {
            destroy_tree(left);
            return (NULL);
        }
        tmp.type = ADD;
        tmp.l = left;
        tmp.r = right;
        left = new_node(tmp);
    }
    return (left);
}


node    *parse_multiplication(char **s)
{
    node    *left;
    node    *right;
    node    tmp;

    left = parse_number_or_group(s);
    if (!left)
        return (NULL);
    while (**s == '*')
    {
        (*s)++;
        right = parse_number_or_group(s);
        if (!right)
        {
            destroy_tree(left);
            return (NULL);
        }
        tmp.type = MULTI;
        tmp.l = left;
        tmp.r = right;
        left = new_node(tmp);
    }
    return (left);
}


int check_balance(char *s)
{
    int balance;
    int i;

    balance = 0;
    i = 0;
    while (s[i])
    {
        if (s[i] == '(')
            balance++;
        else if (s[i] == ')')
        {
            balance--;
            if (balance < 0)
                return (-1);
        }
        i++;
    }
    return (balance);
}

int eval_tree(node *tree)
{
    switch (tree->type)
    {
        case ADD:
            return (eval_tree(tree->l) + eval_tree(tree->r));
        case MULTI:
            return (eval_tree(tree->l) * eval_tree(tree->r));
        case VAL:
            return (tree->val);
    }
    return (0);
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    if (check_balance(argv[1]) == -1)
        return(printf("Unexpected token ')'"), 1);
    node *tree = parse_addition(&argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
