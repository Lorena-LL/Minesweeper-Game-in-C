#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "minesweeper.h"

int is_letter(char x){
    if(x>='A'&& x <= 'Z' || x>= 'a' && x<='z') return 1;
    else return 0;
}

int is_less100(char* x){
    int number = 0;
    for(int i = 0; x[i] >= '0' && x[i] <='9' && x[i] != '\0'; i++){
        number = number*10 + x[i] - '0';
    }
    if(number<100) return number;
    return -2;
}

command* extract_command(char* str, int* i){
    command* setofcomm = calloc(100, sizeof(command));
    if(setofcomm == NULL){
        printf("Not enough memory for the matrix that contains the commands.\n");
        return NULL;
    }
    char* strcomm = strtok(str, " ");
    *i =0;
    while (strcomm != NULL){
        for(int j = 0; strcomm[j] != '\0'; j++){
            if(is_letter(strcomm[j])== 1){
                if (j>=1 && strcomm[j-1] == '!' && is_less100(strcomm + j+2) != -2){
                    setofcomm[*i].tip = '!';
                    setofcomm[*i].coloana = strcomm[j];
                    setofcomm[*i].rand = is_less100(strcomm + j +1);
                    (*i)++;
                }else if (is_less100(strcomm + j + 1) != -2){
                    setofcomm[*i].tip = '.';
                    setofcomm[*i].coloana = strcomm[j];
                    setofcomm[*i].rand = is_less100(strcomm + j+1);
                    (*i)++;
                }
            }
        }
        strcomm = strtok(NULL, " ");
    }
    return setofcomm;
}

void print(int h, int w, char t[h][w]){
    char column = 'A';
    int row = 1;
    printf("   ");
    for (int i = 0; i < w; i++){
        printf("%c ",column);
        if (column != 'Z'){ //vezi tabel ascii
            column = (int)column + 1;
        }else {
            column = (int)column + 7;
        }
    }
    printf("\n");
    column = 'A';
    for (int i = 0; i < h; i++){
        printf ("%-3d", row);
        for (int j = 0; j < w; j++){
            printf ("%c ", t[i][j]);
        }
        printf("%2d\n", row);
        row ++;
    }
    printf("   ");
    for (int i = 0; i < w; i++){
        printf("%c ",column);
        if (column != 'Z'){ //vezi tabel ascii
            column = (int)column + 1;
        }else {
            column = (int)column + 7;
        }
    }
    printf("\n\n");
}

void create(int nr_bombs, char cols[], int rows[], int h, int w, char t[h][w]){
    char c;
    int r;
    int xc, xr;
    for (int i = 0; i < nr_bombs; i++){
        c = cols[i];
        r = rows[i];
        if (c <= 'Z'){
            xc = (int)c - 65;
        }else {
            xc = (int)c - 71;
        }
        xr = r - 1;
        t[xr][xc] = 'X';
    }
    //in continuare i si j determina pozita careia ii determin cate bombe vecine are
    //iar k si l determina vecinii
    for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            int vecini = 0;
            if (t[i][j] != 'X'){
                for (int k = i-1; k <= i+1; k++){
                    if (k>=0 && k<h){ //evit situatia in care indexul e -1 sau h
                        for (int l = j-1; l <= j+1; l++){
                            if (l>=0 && l<w){ //evit situatia in care indexul e -1 sau w
                                if (k == i && l == j){
                                    continue;
                                }else {
                                    if(t[k][l] == 'X'){
                                        vecini++;
                                    }
                                }

                            }
                        }
                    }
                }
            }else {
                continue;
            }
            if (vecini){
                t[i][j] = vecini+48; //vezi tabel ascii
            }else {
                t[i][j] = '.';
            }
        }
    }
}

void free_mat2d(int n, char** mat){
    for (int i = 0; i<n; i++){
        free(mat[i]);
    }
    free(mat);
}

char** init_state(int h, int w){
    char** s = (char**)malloc(h*sizeof(char*));
    if (s == NULL){
        printf("Not enough memory for the state array s.\n");
        return NULL;
    }
    for (int i = 0; i<h; i++){
        s[i] = (char*)malloc(w*sizeof(char));
        if (s[i] == NULL){
            printf("Not enough memory for the state array s.\n");
            free_mat2d(i, s);
            return NULL;
        }
    }
    for (int i = 0; i<h; i++){
        for (int j = 0; j<w; j++){
            s[i][j] = '?';
        }
    }
    return s;
}

int discover(int i, int j, int h, int w, char t[h][w], char** s){
    if (i>=0 && i<h && j>=0 && j<w){
        if (t[i][j] == 'X'){
            return -1;
        }else if (s[i][j] == '.'){
            return 0;
        }else if (t[i][j]>='1' && t[i][j]<='8'){
            s[i][j] = '.';
            return 1;
        } else if (t[i][j] == '.'){
            s[i][j] = '.';
            discover(i-1, j-1, h, w, t, s);
            discover(i-1, j, h, w, t, s);
            discover(i-1, j+1, h, w, t, s);
            discover(i, j-1, h, w, t, s);
            discover(i, j+1, h, w, t, s);
            discover(i+1, j-1, h, w, t, s);
            discover(i+1, j, h, w, t, s);
            discover(i+1, j+1, h, w, t, s);
            return 2;
        }
    }else {
        return -2;
    }
}

void mark(int i, int j, int h, int w, char** s){//verific daca i si j sunt in matrice(nu sunt, ies din functie)
    if(s[i][j] == '?'){
        s[i][j] = '!';
    }else if(s[i][j] == '!'){
        s[i][j] = '?';
    }
}

void player_view(int h, int w, char t[h][w], char** s){
    char column = 'A';
    int row = 1;
    printf("   ");
    for (int i = 0; i < w; i++){
        printf("%c ",column);
        if (column != 'Z'){ //vezi tabel ascii
            column = (int)column + 1;
        }else {
            column = (int)column + 7;
        }
    }
    printf("\n");
    column = 'A';
    for (int i = 0; i < h; i++){
        printf ("%-3d", row);
        for (int j = 0; j < w; j++){
            if(s[i][j] == '.'){
                printf ("%c ", t[i][j]);
            } else {
                printf ("%c ", s[i][j]);
            }
        }
        printf("%2d\n", row);
        row ++;
    }
    printf("   ");
    for (int i = 0; i < w; i++){
        printf("%c ",column);
        if (column != 'Z'){ //vezi tabel ascii
            column = (int)column + 1;
        }else {
            column = (int)column + 7;
        }
    }
    printf("\n\n");
}

int check_win(int h, int w, char t[h][w], char** s){
    for (int i = 0; i<h; i++){
        for (int j = 0; j<w; j++){
            if(s[i][j] == '?' && t[i][j] != 'X'){
                return 0;
            }
        }
    }
    return 1;
}

unsigned long long powfunc(int b, int e){
    unsigned long long res = 1;
    for (int i = 1; i <= e; i++){
        res *= (unsigned long long)b;
    }
    return res;
}

unsigned long long Hash(char** s, int h, int w){
    int p = 5;
    unsigned long long res = 0;
    for(int i = 0; i<= h-1; i++){
        for(int j = 0; j<= w-1; j++){
            unsigned long long temp = ((unsigned long long)s[i][j]) *  powfunc(p, i*w+j);
            res += temp;
        }
    }
    return res;
}

void run_tests(){
    freopen("minesweeper.out", "w", stdout);
    FILE* pin = fopen("minesweeper.in", "r");
    if(pin == NULL){
        printf("eroare la deschiderea minesweeper.in\n");
        exit(5);
    }
    int T;
    int h, w, n;
    char* str = malloc(1001*sizeof(char));
    if (str == NULL){
        printf("eroare la alocare str\n");
        exit(5);
    }
    fscanf(pin, "%d", &T);
    int size;
    for (int x = 0; x < T; x++){
        fscanf(pin, "%d%d%d", &h, &w, &n);
        char cols[n];
        int rows[n];
        fscanf(pin, "%s", cols);
        for(int y = 0; y < n; y++){
            fscanf(pin, "%d", &rows[y]);
        }
        fscanf(pin, "\n");
        fgets(str, 1001, pin);
        size = strlen(str);
        if(str[size-1] == '\n'){
            str[size-1] = '\0';
            size --;
        }
        char t[h][w];
        memset(t, 0, sizeof(t));
        create(n, cols, rows, h, w, t);
        char** s = init_state(h, w);
        if (s==NULL){
            printf("s e NULL.\n");
            exit(6);
        }
        int nr_elem;
        command* setofcomm = extract_command(str, &nr_elem);
        int i, j, val = 3, gamestate = 0;
        for (int q = 0; q < nr_elem; q++){
            i = setofcomm[q].rand - 1;
            if (setofcomm[q].coloana <= 'Z'){
                j = (int)setofcomm[q].coloana - 'A';
            }else {
                j = (int)setofcomm[q].coloana - 71;//vezi tabel ascii
            }
            if(i>=0 && i<=h-1 && j>=0 && j<=w-1){
                if (setofcomm[q].tip == '.'){
                    val = discover(i, j, h, w, t, s);
                    if(val == -1){
                        gamestate = -1;
                        break;
                    }
                }else {
                    if(s[i][j] != '.'){
                        mark(i, j, h, w, s);
                    }
                }
            }
        }
        if(gamestate != -1 && val != 3){ //val ramane 3 cand nu se da nicio comanda de descoperire => jocul continua => gamestate ramane 0 (valoare cu care l-am initializat) si ni mai verificam cu check_win
            gamestate = check_win(h, w, t, s);
        }
        unsigned long long val_h = Hash(s, h, w);
        printf("%d %llu\n", gamestate, val_h);
        free_mat2d(h, s);
        free(setofcomm);
    }
    fclose(pin);
}


