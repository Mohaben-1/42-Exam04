#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

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

node *parse_factor(char **s); // ++
node *parse_expr(char **s); // ++
node *parse_term(char **s); // ++

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
        printf("Unexpexted end of input\n");
    exit(1); // ++
}

int accept(char **s, char c)
{
    if (**s == c) // ++ 
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
    return (0); // +
}

node *parse_factor(char **s)
{
    if (isdigit(**s))
    {
        node n = {VAL, **s - '0', NULL, NULL};
        (*s)++;
        return (new_node(n));
    }
    if (accept(s, '('))
    {
        node *n = parse_expr(s);
        if (!expect(s, ')'))
        {
            destroy_tree(n);
            return (NULL);
        }
        return (n);
    }
    unexpected(**s);
    return (NULL);
}

node *parse_expr(char **s)
{
    node *left = parse_term(s);
    if (!left)
        return (NULL);

    while (accept(s, '+'))
    {
        node *right = parse_term(s);
        if (!right)
        {
            destroy_tree(left);
            return (NULL);
        }
        
        
        
        node n = {ADD, 0, left, right};
        left = new_node(n);
        if (!left)
        {
            destroy_tree(right);
            return (NULL);
        }
    }
    return (left);
}


node *parse_term(char **s)
{
    node *left = parse_factor(s);
    if (!left)
        return (NULL);
    while (accept(s, '*'))
    {
        node *right = parse_factor(s);
        if (!right)
        {
            destroy_tree(left);
            return (NULL);
        }


        node n = {MULTI, 0, left, right};
        left = new_node(n);
        if (!left)
        {
            destroy_tree(right);
            return (NULL);
        }
    }
    return (left);
}

int main(int ac, char **av)
{
    if (ac != 2)
        return (1);
    char *s = av[1]; // +
    node *tree = parse_expr(&s); // +
    if (!tree || *s)
    { // +
        if (*s)
            unexpected(*s);
        else
            unexpected(0);
        return (1);
    } // +
    printf("%d\n", eval_tree(tree));
    destroy_tree(tree);
}