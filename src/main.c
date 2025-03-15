#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void predictAndPrintAll(struct keysPredict* kt, char* partialWord ) {
    char** words;
    int wordsCount;
    words = keysPredictRun(kt, partialWord, &wordsCount);
    printf("Predicts: %s = ", partialWord);
    for(int i=0; i<wordsCount; i++) {
        printf("%s,", words[i]);
    }
    printf("\n");
    deleteArrayOfWords(words, wordsCount);
}

void findAndPrintAll(struct keysPredict* kt, char* word ) {
    struct node* result = keysPredictFind(kt, word);
    printf("Find: %s = %i\n", word, result==0);
}

int main() {
    struct keysPredict* kt = (struct keysPredict*)malloc(sizeof(struct keysPredict));
    if (kt == NULL) {
        printf("Error al asignar memoria para keysPredict.\n");
        return -1; // Retornar en caso de error de memoria
    }
    kt->first = NULL;
    kt->totalKeys = 0;
    kt->totalWords = 0;

    // Usar keysPredictAddWord para agregar palabras
    keysPredictAddWord(kt, "hola");
    keysPredictAddWord(kt, "hey");
    keysPredictAddWord(kt, "helium");
    keysPredictAddWord(kt, "hotel");
    keysPredictAddWord(kt, "habitacion");

    char* prefijo = "hey";
    int palabrasEncontradas = 0;

    char** resultados = keysPredictRun(kt, prefijo, &palabrasEncontradas);

    printf("Palabras que comienzan con '%s':\n", prefijo);
    for (int i = 0; i < palabrasEncontradas; i++) {
        printf("%s\n", resultados[i]);
        free(resultados[i]); // Liberar cada palabra
    }

    return 0;
}
