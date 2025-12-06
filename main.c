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

typedef struct point {
    int y;
    int x;
} POINT;

typedef struct q_item
{
    int y;
    int x;
    struct q_item *next;
} Q_ITEM;

typedef struct queue 
{
    Q_ITEM* first;
    Q_ITEM* last;
    int size;
}QU;

#define SQ(x) (x*x)

#define WALL 'M'
#define PATH ' '
#define VISI '.'
#define START 'S'
#define END 'E'

// Text colors and reset
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
// Macros for color changing
#define CHANGE_C(color) do { \
    write(STDOUT_FILENO, color, strlen(color)); \
} while (0)
#define RESET_C do { \
    write(STDOUT_FILENO, RESET, strlen(RESET)); \
} while (0)

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
QU* q_init() {
    QU* q = malloc(sizeof(QU));
    q->first = q->last = NULL;
    q->size = 0;
    return q;
}
void q_free(QU* q) {
    Q_ITEM* t = NULL;
    for (Q_ITEM* i = q->first; i != NULL; i = t)
    {
        t = i->next;
        free(i);
    }
    free(q);
    return;
    
}
void q_add(QU* q, int y, int x) {
    q->size++;
    Q_ITEM *temp = q->last;
    Q_ITEM *new = malloc(sizeof(Q_ITEM));
    new->y = y;
    new->x = x;
    if (q->last != NULL)
        q->last->next = new;
    
    q->last = new;
    if (q->size == 1)
    {
        q->first = new;
    }
}
Q_ITEM *q_take(QU* q) {
    if (q->size == 0)
        return NULL;
    Q_ITEM* ret = q->first;
    q->first = q->first->next;
    q->size--;
    return ret;    
}
// TODO: could use a clean up
void generate_random_start_end(char **maze, int *s_x, int *s_y) {
    while (1)
    {
        int x;
        int y;
        int vertical = rand() % 2;
        if (vertical)
        {
            int side = rand() % 2;
            x = side * COLS-1;
            y = rand() % ROWS;
            if (maze[y][x] != PATH)
            {
                continue;
            }
            maze[y][x] = 'S';
        } else {
            int side = rand() % 2;
            x = rand() % COLS;
            y = side * ROWS-1;
            if (maze[y][x] != PATH)
            {
                continue;
            }
            maze[y][x] = 'S';
        }
        
        *s_x = x;
        *s_y = y;
        break; 

    }
    while(1) {
        int e_x = rand() % COLS;
        int e_y = rand() % ROWS;

        if (maze[e_y][e_x] != PATH || sqrt(SQ(*s_x - e_x) + SQ(*s_y - e_y)) > (sqrt(SQ(ROWS) + SQ(COLS))/2))
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
    int x_Start;
    int y_Start;
    generate_random_start_end(maze, &x_Start, &y_Start);
    draw_map(maze);

    BFS_search(y_Start, x_Start, maze);

    return 0;
}

void BFS_search(int y_Start, int x_Start, char **maze)
{
    // Breath first algorithm
    int directions[4] = {UP, RIGHT, DOWN, LEFT};
    QU *Q = q_init();
    q_add(Q, y_Start, x_Start);
    while (Q->size)
    {
        Q_ITEM *point = q_take(Q);
        int pos_x = point->x;
        int pos_y = point->y;

        draw_map(maze);
        maze[pos_y][pos_x] = VISI;

        for (size_t i = 0; i < 4; i++)
        {
            int change_x = 0;
            int change_y = 0;
            switch (directions[i])
            {
            case UP:
                change_y = -1;
                break;
            case RIGHT:
                change_x = 1;
                break;
            case DOWN:
                change_y = 1;
                break;
            case LEFT:
                change_x = -1;
                break;
            }
            int new_y = pos_y + change_y;
            int new_x  = pos_x + change_x;
            if (new_x >= 0 && new_x < COLS && new_y >= 0 && new_y < ROWS)
            {
                if ((maze[new_y][new_x] == PATH))
                {
                    q_add(Q, new_y, new_x);
                } else if (maze[new_y][new_x] == END)
                {
                    draw_map(maze);
                    q_free(Q);
                    return;
                }
            }
        }
    }
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
            case VISI:
                write(STDOUT_FILENO, "\xE2\x96\x92", 3);
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
