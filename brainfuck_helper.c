#include "brainfuck_helper.h"
#include <stdio.h>
#include <string.h>



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
    int taille_tab = 1;
    car = fgetc(fichier);
    if (car != EOF)
    {
        tableau[taille_tab-1] = (char)car;
    }
    while((car = fgetc(fichier))!= EOF)
    {   
        if (car != '\n'){
            taille_tab++;
            tableau = realloc(tableau,taille_tab*sizeof(char));
            tableau[taille_tab-1]=(char)car;
        }
       
    }
    fclose(fichier);
    return tableau;
}

void free_input_prog(char* input_prog){
    free(input_prog);
}

void* build_loops(char* input_prog) {
    int num_brackets = 0;

    // on compte le nombre de crochets ouvrants pour savoir la taille de la pile
    for (int i = 0; input_prog[i] != '\0' ; i++) {
        if (input_prog[i] == '[') {
            num_brackets++;
        }
    }
    // on alloue la pile 
    char** loop_stack = (char**)malloc(num_brackets * sizeof(char*));

    return loop_stack;
}

void free_loops(void* loops){
    free(loops);
}

void execute_instruction(char** ipp, uint8_t** dpp, void* loops){
    int stack_top = -1; // le sommet de la pile
    char** loop_stack = (char**)loops; // la pile des boucles
    // ** : valeur, * : addresse
    while(**ipp != '\0')
    {
        switch(**ipp)
        {
            case '>':
                (*dpp)++; // on décale a droite le pointeur
                break;
            case '<':
                (*dpp)--; // on decale a gauche le pointeur
                break;
            case '+':
                (*(*dpp))++; // on incrémente la valeur pointée
                break;
            case '-':
                (*(*dpp))--; // on décrémente la valeur pointée
                break;
            case '.':
                // afficher la valeur de **dpp 
                putchar(**dpp); // affiche le caractère qui correspond au code ascii de la valeur pointée
                break;
            case ',':
                (**dpp) = getchar();
                break;
            case '[':
                if ((**dpp) == 0) 
                {
                    int loop_count = 1;
                    while (loop_count != 0) 
                    {
                        (*ipp)++;
                        if (**ipp == '[') {
                            loop_count++;
                        } 
                        else if (**ipp == ']') 
                        {
                            loop_count--;
                        }
                    }
                }
                else
                {   
                    stack_top++;
                    loop_stack[stack_top] = *ipp;

                }

                break;
            case ']':
                if ((*(*dpp)) != 0)
                {   
                    if (stack_top >= 0) {
                        // Retourner à l'index du crochet ouvrant '[' correspondant
                        (*ipp) = loop_stack[stack_top];
                        (*ipp)--; // pour compenser l'incrémentation de la boucle while
                        // On dépile
                        stack_top--;
                    } else {
                        // Gérer le cas où la pile est vide
                        printf("La pile est vide, impossible de dépiler.\n");
                        // Faire quelque chose en conséquence, comme sortir de la fonction ou afficher un message d'erreur
                    }

                } 
                else {
                    // on depile le [ de la pile correspondant (le dernier element de la pile)
                    loop_stack[stack_top] = 0;
                    stack_top--;
                }
                break;
            default:
                printf("caractere non reconnu \n");
                exit(EXIT_FAILURE);
                break;
        }
        (*ipp)++; // instruction suivante
    }
}