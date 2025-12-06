#pragma once
char ** init_maze();

void generate_maze(char **maze, int x, int y);

void BFS_search(int y_Start, int x_Start, char **maze);

void random_side_position(char **maze);

void draw_map(char **maze);
