#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

enum directions {
    UP,
    RIGHT,
    DOWN,
    LEFT,
};
#define WALL 'M'
#define PATH ' '

int COLS = 61;
int ROWS = 21;

void shuffle_array(int directions[4], int size) {
    
    for (size_t i = size - 1; i > 0 ; i--)
    {
        int ran_i = rand() % (i+1);

        int temp = directions[i];
        directions[i] = directions[ran_i];
        directions[ran_i] = temp;
    }
    
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    printf("\033[2J\033[H"); 
    char **maze = init_maze(); // todo: add arguments

    generate_maze(maze, 0, 0);
    return 0;
}

void generate_maze(char **maze, int x, int y)
{
    maze[y][x] = ' ';

    int directions[4] = {UP, RIGHT, DOWN, LEFT};
    shuffle_array(directions, 4);

    for (int i = 0; i < 4; i++)
    {
        int pos_x = 0;
        int pos_y = 0;
        switch (directions[i])
        {
        case UP:
            pos_y = -2;
            break;
        case RIGHT:
            pos_x = 2;
            break;
        case DOWN:
            pos_y = 2;
            break;
        case LEFT:
            pos_x = -2;
            break;
        }

        int new_x = x + pos_x;
        int new_y = y + pos_y;

        if (new_x >= 0 && new_x < COLS && new_y >= 0 && new_y < ROWS && maze[new_y][new_x] == WALL)
        {
            maze[y + pos_y / 2][x + pos_x / 2] = PATH;
            draw_map(maze);
            generate_maze(maze, new_x, new_y);
        }
    }
}
void draw_map(char **maze) {
    printf("\033[H");
    for (int i = 0; i < COLS + 2; i++)
    {
        printf("#");
    }
    printf("\n");
    for (size_t i = 0; i < ROWS; i++)
    {
        write(STDOUT_FILENO, "#", 1);
        write(STDOUT_FILENO, maze[i], COLS);
        write(STDOUT_FILENO, "#\n", 2);
    }
    for (int i = 0; i < COLS + 2; i++)
    {
        write(STDOUT_FILENO, "#", 1);
    }
    printf("\n");
    usleep(100 * 1000);
}
char ** init_maze()
{
    char **maze = malloc(sizeof(char *) * ROWS);
    for (size_t i = 0; i < ROWS; i++)
    {
        maze[i] = malloc(sizeof(char) * COLS);
    }

    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t o = 0; o < COLS; o++)
        {
            maze[i][o] = WALL;
        }
    }
    return maze;
}
