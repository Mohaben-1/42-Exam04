#include <unistd.h>
#include <stdio.h>

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
        i++;
    return i;
}

int is_valid(char *str)
{
    int bal = 0; 
    int i = 0;
    while (str[i])
    {
        if (str[i] == '(')
            bal ++;
        else if (str[i] == ')')
        {
            bal --;
            if (bal < 0 )
                return 0;
        }
        i++;
    }
    return (bal == 0);
}

void rip(char *s, int i, int left, int right)
{
    if (left == 0 && right == 0)
    {
        if (is_valid(s))
            puts(s);
        return;
    }

    int j = i;
    while (s[j])
    {
        if (s[j] == '(' && left > 0)
        {
            s[j] = ' ';
            rip(s, j + 1, left - 1, right);
            s[j] = '(';
        } 
        else if (s[j] == ')' && right > 0)
        {
            s[j] = ' ';
            rip(s, j + 1, left, right - 1);
            s[j] = ')';
        }
        j++;
    }
}

int main (int ac , char *av[])
{
    if (ac != 2)
        return 1;
    int len = ft_strlen(av[1]);
    int left = 0 , right = 0 ;
    char str[len + 1];
    int i = 0;
    while (i < len)
    {
        str[i] = av[1][i];
        i++;
    }
    str[len] = '\0';

    i = 0;
    while (i < len)
    {
        if (str[i] == '(')
            left++;
        else if (str[i] == ')')
        {
            if (left > 0)
                left --;
            else 
                right++;
        }
        i++;
    }
    rip(str, 0, left, right);
}