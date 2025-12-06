#pragma once
char ** init_maze();

void generate_maze(char **maze, int x, int y);

void random_side_position(char **maze);

void draw_map(char **maze);
