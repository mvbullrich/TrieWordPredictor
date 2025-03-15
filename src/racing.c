#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char character;
    struct node* next;
    int end;
    char* word;
    struct node* down;
};

struct keysPredict {
    struct node* first;
    int totalKeys;
    int totalWords;
};

void deleteArrayOfWords(char** words, int wordsCount) {
	int i=0;
    for(i=0;i<wordsCount;i++){
		free(words[i]);
	}
	free(words);
}


struct keysPredict* keysPredictNew() {
    struct keysPredict* kt = (struct keysPredict*)malloc(sizeof(struct keysPredict));
    kt->first = 0;
    kt->totalKeys = 0;
    kt->totalWords = 0;
    return kt;
}

// Function to duplicate strings (simulating strDup)
int strLen(char* src) {
    int contador = 0;
    while(src[contador] != '\0'){
        contador++;
    }
    return contador;
}

char* strDup(char* src) {
    int largo = strLen(src);
    char* duplicado = (char*)malloc(sizeof(char) * (largo + 1));
    for (int i = 0; i < largo; i++){
        duplicado[i] = src[i];
    }
    duplicado[largo] = '\0';
    return duplicado;
}

struct node* findNodeInLevel(struct node** list, char character) {
		// Verificar si la lista es NULL
		// Recorrer la lista de nodos
	struct node* p = *list;
	while (p != NULL) {
		if (p->character == character) {
			return p;  // Retornar el nodo si coincide el carácter
		}
		p = p->next;  // Ir al siguiente nodo en la lista
	}
		
	return NULL;  // Si no se encuentra el carácter, retornar NULL
}

struct node* addSortedNewNodeInLevel(struct node** list, char character) {
    struct node* nuevoNodo = (struct node*)malloc(sizeof(struct node));
    nuevoNodo->character = character;
    nuevoNodo->next = 0;
    nuevoNodo->end = 0;
    nuevoNodo->word = 0;
    nuevoNodo->down = 0;

    if (*list == NULL){ //si lista estÃ¡ vacÃ­a
        *list = nuevoNodo;
        return *list;
    }
    if ((*list)->character > character){ //si tenemos que meter el nuevo nodo al principio
        nuevoNodo->next = *list;
        *list = nuevoNodo;
        return *list;
    }

    struct node* actual = *list;
    struct node* anterior = NULL;

    while(actual != NULL && actual->character < character){
        anterior = actual;
        actual = actual->next;
    }

    if (actual != NULL && actual->character == character) {
        free(nuevoNodo); // El carÃ¡cter ya existe, liberar el nuevo nodo
        return *list;
    }
    
    anterior->next = nuevoNodo;
    nuevoNodo->next = actual;

    return *list;
}

void keysPredictAddWord(struct keysPredict* kt, char* word) { 
	struct node* curr= kt->first;
	struct node* prev = NULL;
	int largoPalabra = strLen(word);
	int contador =0;
	int i = 0;
	
	if (largoPalabra==0){
		return;
	}

	while(i<largoPalabra){
		if (curr==0){
			curr = addSortedNewNodeInLevel(&(curr), word[i]); //apunta a la primera letra de la lista
			if (prev==0){
				kt->first=curr;  //conectar nodo con la lista
			} else{
				prev->down=curr; //conecta dos nodos
			}
			kt->totalKeys++;


			
		}else{
			struct node* res=findNodeInLevel(&(curr),word[i]);
			if(res==NULL){
				struct node* vr = addSortedNewNodeInLevel(&(curr), word[i]); 
				
				if(prev!=0){
				prev->down= curr;
				}else{
					kt-> first = vr;
				}
				curr= findNodeInLevel(&(curr),word[i]);
				kt->totalKeys++;
			}else{
				curr=res;
				contador++;
			
			}
		}
		prev=curr;
		curr=curr->down;
		i++;
	}
	if (prev->end == 1 && contador == largoPalabra){
		return;
	}
	else{
		prev->word=strDup(word);
		kt->totalWords++;
		prev-> end = 1;
}
}

void keysPredictRemoveWord(struct keysPredict* kt, char* word) {
	struct node* curr = kt->first;  // Comienza en la raÃ­z del trie
    struct node* prev = NULL;
    int largoPalabra = strLen(word);  // Asume que strLen devuelve el tamaÃ±o de la cadena
    int i = 0;
	
	if (largoPalabra==0){
		return;
	}
    // Recorre la estructura buscando el nodo que corresponde a la Ãºltima letra de la palabra
    while (i <= largoPalabra && curr != NULL) {
        struct node* res = findNodeInLevel(&(curr), word[i]);  // Busca si existe un nodo con la letra actual

        if (res == NULL) {
            // Si no se encuentra un nodo correspondiente, la palabra no existe en el trie
            return;
        }

        // Avanza al siguiente nivel de nodos
        prev = res;
        curr = res->down;
        i++;
    }
    // Si encontramos el nodo que corresponde a la Ãºltima letra de la palabra
    if (prev != NULL && prev->end == 1) {
        prev->end = 0;  // Marca que este nodo ya no es el final de una palabra
        free(prev->word);  // Libera la memoria de la palabra almacenada
        prev->word = NULL;  // Elimina la referencia a la palabra
        kt->totalWords--;  // Decrementa el contador de palabras
    }
}


int keysPredictCountWordAux(struct node* n) {
    if (n == NULL) return 0; // caso base

    int count = 0; // creo variable contador
    if (n->end == 1) {
        count++; // incremento el contador en 1
        //printf("Found word ending: %c\n", n->character); // depuración: muestra las palabras contadas
    } else {
        //printf("Skipping node: %c\n", n->character); // depuración: muestra nodos no contados
    }

    // Sumar recursivamente desde los nodos down y next
    count += keysPredictCountWordAux(n->down);
    count += keysPredictCountWordAux(n->next);

    return count;
}
	

void keysPredictRunRec(struct node* n, char** lista, int* k) {
    if (n == NULL) return; // caso base

    if (n->end == 1) { // Si es el final de una palabra
        lista[*k] = strDup(n->word); // guardo la palabra en la lista
        //printf("Adding word to list: %s at index %d\n", n->word, *k);
        (*k)++; // incremento el contador
    }

    // llamadas recursivas
    keysPredictRunRec(n->down, lista, k);
    keysPredictRunRec(n->next, lista, k);
}

char** keysPredictRun(struct keysPredict* kt, char* partialWord, int* wordsCount) {
    int largo = strLen(partialWord); // calcular largo del prefijo
    struct node* n = kt->first;
    struct node* prev = NULL;
    *wordsCount = 0; //quiero que el puntero apunte al valor 0

    // Aca queremos buscar el prefijo
    for (int i = 0; i < largo; i++) {
        //printf("Buscando carácter '%c' en el nivel\n", partialWord[i]);
        n = findNodeInLevel(&n, partialWord[i]); // buscar la letra en el nivel

        if (n == NULL) { // No se encontró ninguna palabra para el prefijo
            //printf("No se encontró ninguna palabra para el prefijo: %s\n", partialWord);
            *wordsCount = 0;
            return NULL;
        }

        // Si n no es NULL, mostramos el carácter encontrado
        //printf("Encontrado carácter '%c'\n", n->character);
        prev = n;
        n = n->down; // Bajamos al siguiente nivel
    }

    *wordsCount = keysPredictCountWordAux(n); // saber la cantidad de palabras que hay
    //printf("WordsCount: %d\n", *wordsCount);
    int k = 0; //para contar
    if(prev != NULL && prev->end == 1) { // esto es por si el prefijo es una palabra
        //listaPalabras[k] = strDup(prev->word);
        //k++;
        (*wordsCount)++;
        //printf("WordsCount: %d\n", *wordsCount);
    }
    char** listaPalabras = (char**) malloc((*wordsCount) * sizeof(char*)); // creo la lista
    if (listaPalabras == NULL) { // Manejo de error por si hay algún problema al crear la lista
        *wordsCount = 0;
        return NULL;
    }
    if(prev != NULL && prev->end == 1) { // esto es por si el prefijo es una palabra
        listaPalabras[k] = strDup(prev->word);
        k++;
        //(*wordsCount)++;
        //printf("WordsCount: %d\n", *wordsCount);
    }

    keysPredictRunRec(n, listaPalabras, &k); // llamamos a la función recursiva

    return listaPalabras;
}

void borranodos(struct node*n){
	if(n==NULL) return;
		borranodos(n->next);
		borranodos(n->down);
	if (n->word!=NULL){
		free(n->word);
	}
	free(n);
}


void keysPredictDelete(struct keysPredict* kt) {
	if (kt == NULL) return;
	borranodos(kt->first);
	free(kt);
}



// Auxiliar functions

void keysPredictListAllRec(struct node* n, char** lista, int* contador){
    if (n == NULL) return; // caso base
    if (n->end == 1){
        lista[*contador] = strDup(n->word); // guardo la palabra en la lista
        (*contador)++; // incremento el contador en 1
    }

    // llamadas recursivas
    keysPredictListAllRec(n->down, lista, contador);
    keysPredictListAllRec(n->next, lista, contador);
}

char** keysPredictListAll(struct keysPredict* kt, int* wordsCount) {
    if(kt==NULL || kt->first == NULL){ // Si kt está vacío
        *wordsCount = 0;
        return NULL;
    }
    *wordsCount = 0;
    struct node* nodo = kt->first;
    *wordsCount = keysPredictCountWordAux(nodo); // contar la cantidad de palabras que hay
    char** palabras = (char**) malloc((*wordsCount) * sizeof(char*)); // creo la lista
    if (palabras == NULL) {
        // Manejo de errores si hay algún error al crear la lista
        *wordsCount = 0; // Asegúrate de que el conteo de palabras sea cero si falla
        return NULL;
    }
    int cantidad = 0; // para contar y usar de index
    keysPredictListAllRec(nodo, palabras, &cantidad); // llamado a funcion recursiva
    return palabras;
}

struct node* keysPredictFind(struct keysPredict* kt, char* word) {
    struct node* prev = NULL;
    struct node* find = kt->first;
    int largoPalabra = strLen(word);
    for(int i=0; i<largoPalabra; i++){
        find = findNodeInLevel(&find, word[i]);
        if(find == NULL){
            return NULL;
        }
        prev = find;
        find = find->down;
    }
    if (prev->end == 1){
        return prev;
    }
    return NULL;
}

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
    printf("Find: %s = %i\n", word, result!=0);
}

void keysPredictPrintAux(struct node* n, int level) {
    if(!n) return;
    struct node* current = n;
    while(current) {
        for(int i=0; i<level;i++) printf(" |   ");
        if(current->end) {
            printf("[%c]\n",current->character);
        } else  {
            printf(" %c \n",current->character);
        }
        keysPredictPrintAux(current->down, level+1);
        current = current->next;
    }
}

void keysPredictPrint(struct keysPredict* kt) {
    printf("--- Predict --- Keys: %i Words: %i\n", kt->totalKeys, kt->totalWords);
    keysPredictPrintAux(kt->first, 0);
    printf("---\n");
}

int main() {
  
    struct keysPredict* kt = keysPredictNew();
    
    // keysPredict - crear un diccionario
    keysPredictAddWord(kt, "alfajor");
    keysPredictAddWord(kt, "canoa");
    keysPredictAddWord(kt, "rinoceronte");
    keysPredictAddWord(kt, "casa");
    keysPredictAddWord(kt, "rino");
    
    keysPredictPrint(kt);

    keysPredictRemoveWord(kt, "canoa");
    keysPredictAddWord(kt, "pato");

    // keysPredict - predecir palabras
    predictAndPrintAll(kt,"c");
    predictAndPrintAll(kt,"ca");
    predictAndPrintAll(kt,"casa");
    predictAndPrintAll(kt,"casas");

    keysPredictPrint(kt);

    char letras[] = "abcdefghijklmnopqrstuvwxyz";
    char p[3]; // Cadena para almacenar las combinaciones de dos letras
    p[2] = '\0'; // Terminador de cadena

    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            p[0] = letras[i];
            p[1] = letras[j];
            predictAndPrintAll(kt, p); // Llama a la función con la combinación actual
        }
    }


    return 0;
}
