#include <math.h>
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
#define SQ(x) (x*x)

#define WALL 'M'
#define PATH ' '
#define START 'S'
#define END 'E'


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
// TODO: could use a clean up
void generate_random_start_end(char **maze) {
    int s_x;
    int s_y;
    while (1)
    {
        int vertical = rand() % 2;
        if (vertical)
        {
            int side = rand() % 2;
            int x = side * COLS-1;
            int y = rand() % ROWS;
            if (maze[y][x] != PATH)
            {
                continue;
            }
            maze[y][x] = 'S';
            s_x = x;
            s_y = y;
            break; 
        } else {
            int side = rand() % 2;
            int x = rand() % COLS;
            int y = side * ROWS-1;
            if (maze[y][x] != PATH)
            {
                continue;
            }
            maze[y][x] = 'S';
            s_x = x;
            s_y = y;
            break;
        }
    }
    while(1) {
        int e_x = rand() % COLS;
        int e_y = rand() % ROWS;

        if (maze[e_y][e_x] != PATH || sqrt(SQ(s_x - e_x) + SQ(s_y - s_x)) > (sqrt(SQ(ROWS) + SQ(COLS))/2))
        {
            maze[e_y][e_x] = 'E';
            return;
        }
        
    }
    
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));

    printf("\033[2J\033[H"); 
    char **maze = init_maze(); // todo: add arguments

    generate_maze(maze, 0, 0);
    generate_random_start_end(maze);
    draw_map(maze);
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
        for (size_t c = 0; c < COLS; c++)
        {
            switch (maze[i][c])
            {
            case WALL:
                write(STDOUT_FILENO, "\xE2\x96\x88", 3);
                break;
            case PATH:
                write(STDOUT_FILENO, "\xE2\x96\x91", 3);
                break;
            case START:
                write(STDOUT_FILENO, "\xE2\x98\x85", 3);
                break;
            case END:
                write(STDOUT_FILENO, "\xE2\x9D\xA4", 3);
                break;
            default:
                break;
            }
        }        
        write(STDOUT_FILENO, "#\n", 2);
    }
    for (int i = 0; i < COLS + 2; i++)
    {
        write(STDOUT_FILENO, "#", 1);
    }
    printf("\n");
    // usleep(100 * 1000);
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
