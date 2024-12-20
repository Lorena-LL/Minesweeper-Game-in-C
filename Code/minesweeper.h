#ifndef MINESWEEPER_H_INCLUDED
#define MINESWEEPER_H_INCLUDED

typedef struct command{
    char tip;
    char coloana;
    int rand;
}command;
command* extract_command(char* str, int* d);
void print(int h, int w, char t[h][w]);
void create(int nr_bombs, char cols[], int rows[], int h, int w, char t[h][w]);
void free_mat2d(int n, char** mat);
char** init_state(int h, int w);
int discover(int i, int j, int h, int w, char t[h][w], char** s);
void mark(int i, int j, int h, int w, char** s);
void player_view(int h, int w, char t[h][w], char** s);
int check_win(int h, int w, char t[h][w], char** s);
unsigned long long powfunc(int b, int e);
unsigned long long Hash(char** s, int h, int w);
void run_tests();

#endif // MINESWEEPER_H_INCLUDED
