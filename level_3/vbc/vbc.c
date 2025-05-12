//this code is already given you just need to code the following functions found in vbc.c
//parse_factor - parse_term - parse_expr
//and you have to modify the following functions unexpected - accept - eval_tree - main
//read vbc.c and you will understand

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

node    *parse_term(char **s);
node    *parse_factor(char **s);
node    *parse_expr(char **s);


node    *parse_expr(char **s)
{
    node    *left;
    node    *right;
    node    tmp;

    left = parse_term(s);
    if (!left)
        return (NULL);
    while (**s == '+')
    {
        (*s)++;
        right = parse_term(s);
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

node    *parse_factor(char **s)
{
    node    tmp;
    node    *node;

    node = NULL;
    if (**s == '(')
    {
        (*s)++;
        node = parse_expr(s);
        if (!node || **s != ')')
        {
            destroy_tree(node);
            unexpected(**s);
            return (NULL);
        }
        (*s)++;
        return (node);
    }
    if (isdigit(**s))
    {
        
        tmp.type = VAL;
        tmp.val = **s - '0';
        node = new_node(tmp);
        (*s)++;
        return (node);
    }
    unexpected(**s);
    return (NULL);
}

node    *parse_term(char **s)
{
    node    *left;
    node    *right;
    node    tmp;

    left = parse_factor(s);
    if (!left)
        return (NULL);
    while (**s == '*')
    {
        (*s)++;
        right = parse_factor(s);
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
}

int main(int argc, char **argv)
{
    if (argc != 2)
        return (1);
    node *tree = parse_expr(&argv[1]);
    if (!tree)
        return (1);
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}
