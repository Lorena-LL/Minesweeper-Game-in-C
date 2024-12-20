#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minesweeper.h"



int main ()
{
    int nr_bombs = 3;
    char cols[3] = {'A', 'A', 'B'};
    int rows[3] = {1,4,3};
    int h = 6, w = 5;
    char t[h][w];
    memset(t, '0', sizeof(t));
    create(nr_bombs, cols, rows, h, w, t);
    char** s = init_state(h, w);
    if (s==NULL)
        printf("s e NULL.\n");
    player_view(h, w, t, s);    //functie care printeaza
    char* str = calloc(1001, sizeof(char));
    int val = 1;
    command* setofcomm;
    do{
        val = 1;
        printf("Enter commands:\n");
        gets(str);
        int nr_elem;
        setofcomm = extract_command(str, &nr_elem);
        int i, j;
        for (int q = 0; q < nr_elem; q++){
            i = setofcomm[q].rand - 1;
            if (setofcomm[q].coloana <= 'Z'){
                j = (int)setofcomm[q].coloana - 'A';
            }else {
                j = (int)setofcomm[q].coloana - 71;//vezi tabel ascii
            }
            if (setofcomm[q].tip == '.'){
                val = discover(i, j, h, w, t, s);
            }else {
                if(i>=0 && i<=h-1 && j>=0 && j<=w-1){
                    if(s[i][j] != '.'){
                        mark(i, j, h, w, s);
                    }else {
                        printf("\nYou can't mark something that has already been discoverd.\n\n");
                    }
                }else {
                    printf("You can't mark something outside the field.\n\n");
                }
            }
            if(val == 0){
                printf("\nYou can't discover again something that has already been discoverd.\n\n");
            }else if(val == -1){
                printf("\nGAME OVER. A mine was beneath %c%d. This is how the board looked like:\n\n", setofcomm[q].coloana, setofcomm[q].rand);
                print(h, w, t);
                exit(1);///Trebuie sa mai afisez si cum arata tabla cu toate bombele
            }else if(val == -2){
                printf("\nThe command was outside the field.\n\n");
            }
            if(check_win(h,w,t,s) == 0){
                player_view(h, w, t, s);
            }else{
                printf("\nYOU WON!!!!\nThis is how the board looked like:\n\n");
                print(h, w, t);
                exit(2);
            }

        }
    }while(val >= 0);
    free_mat2d(h, s);
    free(setofcomm);
    return 0;
}


//OBSERVATII:

//Coloanele se notează cu litere, de la A la Z și apoi de la a la z, de la stânga la dreapta,
//iar rândurile se numerotează de la 1, de sus în jos.
//Constrângeri: 0 < h < 100, 0 < c <= 52, h = număr rânduri, c = număr coloane


//Legenda matricii printate pentru jucator cu player_view
//nedescoperită - ? - dacă jucătorul nu știe ce se află acolo;
//descoperită - . - dacă jucătorul știe ce se află acolo (bombele nu pot fi descoperite);
//marcată - ! - dacă jucătorul crede că la poziția respectivă se află o bombă.


//Legenda input de la user (COMENZILE):
//Comenzile vor putea fi primite in felul urmator: un șir de caractere
// care reprezintă una sau mai multe comenzi, care pot fi sau nu separate prin spații.
//Constrângeri: lungimea șirului nu depășește 1000, șirul conține cel mult 100 comenzi.
// Șirul poate conține și comenzi greșite dar toate comenzile sunt de forma literă și număr, prefixate sau nu cu !.
// Cele care nu au ! reprezintă comenzi de descoperire.
//Exemplu de sir cu 6 comenzi: "A1  !B2!B2 A1A1 a103"


//Legenda pentru structura de date command:
//Definiți o structură care să reprezinte o comandă pentru joc cu următoarele componente, în această ordine:
//tipul comenzii care poate fi . (descoperire) sau ! (marcare/demarcare);
//coloana, un caracter care reprezintă coloana din matricea t;
//rândul, un întreg care reprezintă rândul din matricea t.

