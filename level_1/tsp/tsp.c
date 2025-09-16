#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h> //+++

// compute the distance between two points
float    distance(float a[2], float b[2])
{
    return sqrtf((b[0] - a[0]) * (b[0] - a[0]) + (b[1] - a[1]) * (b[1] - a[1]));
}

ssize_t size;               //+++
bool    visited[11];      //+++
float   (*array)[2];       //+++
float   bst = 999999999;  //+++

bool all_visited()          // +++
{
    int i = 0;
    while (i < size)
    {
        if (!visited[i])
            return false;
        i++;
    }
    return true;
}

void tsp(int cur, float dis) // +++
{
    int i = 0;

    if (all_visited())
    {
        dis += distance(array[cur], array[0]);
        if (dis < bst)
            bst = dis;
    }
    while (i < size)
    {
        if (!visited[i])
        {
            dis += distance(array[cur], array[i]);
            visited[i] = true;
            tsp(i, dis);
            dis -= distance(array[cur], array[i]);
            visited[i] = false;
        }
        i++;
    }
}


ssize_t    file_size(FILE *file)
{
    char    *buffer = NULL;
    size_t    n = 0;
    ssize_t ret;

    errno = 0;

    for (ret = 0; getline(&buffer, &n, file) != -1; ret++);

    free(buffer);
    if (errno || fseek(file, 0, SEEK_SET))
        return -1;
    return ret;
}

int        retrieve_file(float (*array)[2], FILE *file)
{
    int tmp;
    for (size_t i = 0; (tmp = fscanf(file, "%f, %f\n", array[i] + 0, array[i] + 1)) != EOF; i++)
        if (tmp != 2)
        {
            errno = EINVAL;
            return -1;
        }
    if (ferror(file))
        return -1;
    return 0;
}

int    main(int ac, char **av)
{
    char *filename = "stdin";
    FILE *file = stdin;
    if (ac > 1)
    {
        filename = av[1];
        file = fopen(filename, "r");
    }
    if (!file)
    {
        fprintf(stderr, "Error opening %s: %m\n", filename);
        return 1;
    }
    size = file_size(file); // +++++
    if (size == -1)
    {
        fprintf(stderr, "Error reading %s: %m\n", filename);
        fclose(file);
        return 1;
    }
    array = calloc(size, sizeof (float [2])); // ++++
    if (!array)
    {
        fprintf(stderr, "Error: %m\n");
        fclose(file);
        return 1;
    }
    if (retrieve_file(array, file) == -1)
    {
        fprintf(stderr, "Error reading %s: %m\n", av[1]);
        fclose(file);
        free(array);
        return 1;
    }
    if (ac > 1)
        fclose(file);

    //++++++
    for (int i = 0; i < 11; i++)
        visited[i] = 0;
    tsp(0,0);
    printf("%.2f", bst);
    //++++++

    free(array);
}