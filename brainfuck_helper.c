#include "brainfuck_helper.h"
#include <stdio.h>



// refaire sans realloc a chaque fois 
// faire un talbeau de taille 100 et rajouter 100 si on dépasse
char* get_input_prog(char* input_filename)
{
    FILE *fichier;
    char *tableau = malloc(1*sizeof(char));
    
    fichier = fopen(input_filename,"r");
    if (fichier==NULL)
    {
        perror("erreur lors de l'ouverture du fichier");
        return NULL;
    }
    char car;
    int taille_tab = 0;
    car = fgetc(fichier);
    if (car != EOF)
    {
        tableau[taille_tab] = (char)car;
    }
    while((car = fgetc(fichier))!= EOF)
    {   
        taille_tab++;
        tableau = realloc(tableau,taille_tab*sizeof(char));
        tableau[taille_tab]=(char)car;
    }
    return tableau;
}

void free_input_prog(char* input_prog){
    free(input_prog);
}

void* build_loops(char* input_prog){
    int* pile = (int*)malloc(strlen(input_prog) * sizeof(int)); // On alloue la pile 
    if (pile == NULL) {
        fprintf(stderr, "Erreur : impossible d'allouer de la mémoire pour la pile des boucles.\n");
        exit(EXIT_FAILURE);
    }
    *pile = -1; // On initialise la pile
    return pile;
}

void free_loops(void* loops){
    free(loops);
}

void execute_instruction(char** ipp, uint8_t** dpp, int* loops){
    char* inst = *ipp; // le tableau d'instructions
    char* vals = *dpp; // le tableau de valeurs;
    while(*inst != '\0'){
        switch(*inst){
            case '>':
                vals++; // on décale a droite le pointeur
                break;
            case '<':
                vals--; // on decale a gauche le pointeur
                break;
            case '+':
                (*vals)++; // on incrémente la valeur pointée
                break;
            case '-':
                (*vals)--; // on décrémente la valeur pointée
                break;
            case '.':
                putchar(*vals); // affiche le caractère qui correspond au code ascii de la valeur pointée
                break;
            case ',':
                *vals = getchar();
                break;
            case '[':
                if (*vals == 0) {
                    int depth = 1;
                    int pc = inst;     // on fait une copie du pointeur d'instruction
                    while (depth != 0) { // on cherche la pos du ']' correspondant dans les instructions
                        pc++;
                        if (inst[pc] == '[') {
                            depth++;
                        } else if (inst[pc] == ']') {
                            depth--;
                        }
                    }
                    // on va l'instruction pc
                    inst = pc;
                } else {
                    // on empile l'addresse de l'instruction '['
                    loops++;
                    *loops = inst;
                }
                break;
                break;
            case ']':
                if (**dpp != 0){
                    int cpt = 1;
                    while (cpt != 0){
                        (*ipp)--;
                        if (**ipp == ']'){
                            cpt++;
                        }
                        if (**ipp == '['){
                            cpt--;
                        }
                    }
                }
                break;
        }
        (*ipp)++;

        ipp++; // on prend l'instruction suivante
    }
}