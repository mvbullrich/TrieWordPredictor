#include "utils.h"

int strLen(char* src) {

    int contador = 0;
    while(src[contador] != '\0'){
        contador++;
    }

    return contador;
}

char* strDup(char* src) {

    int largo = strLen(src);
    char* duplicado = (char*)malloc(sizeof(char)*(largo+1));

    for (int i=0; i<largo; i++){
        duplicado[i] = src[i];
    }
    duplicado[largo] = '\0';
    return duplicado;
}

// Keys Predict

struct keysPredict* keysPredictNew() {
    struct keysPredict* kt = (struct keysPredict*)malloc(sizeof(struct keysPredict));
    kt->first = 0;
    kt->totalKeys = 0;
    kt->totalWords = 0;
    return kt;
}

void keysPredictAddWord(struct keysPredict* kt, char* word) { //word es una palabra entera
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
				struct node* vr = addSortedNewNodeInLevel(&(curr), word[i]); // si da error strct node
				
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
	struct node* curr = kt->first;  // Comienza en la raí­z del trie
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

struct node* keysPredictFind(struct keysPredict* kt, char* word) {
    struct node* prev = NULL;
    struct node* find = kt->first;
    int largoPalabra = strLen(word);
    //printf("Largo de la palabra a buscar: %d\n", largoPalabra);
    for(int i=0; i<largoPalabra; i++){
        //printf("Buscando %c\n", word[i]);
        find = findNodeInLevel(&find, word[i]);
        if(find == NULL){
            return NULL;
        }
        //printf("Encontrado carácter '%c'\n", find->character);
        prev = find;
        find = find->down;
    }
    if (prev->end == 1){
        return prev;
    }
    return NULL;
}

    // Empezamos buscando la primera letra, y si no es igual a la primera letra de word, chau, hacemos next (si podemos)
    // dentro del ciclo buscamos en cada nivel la letra, si esta genial y si no esta, vemos si tiene next y sino end

    // struct node* current = kt->first;
    // struct node* prev = NULL;
    // int largoPalabra = strLen(word);
    // int i = 0;
    // while (i < largoPalabra){
    //     if(current == NULL){
    //         return 0;
    //     } else {
    //         if(current->character == word[i]){
    //             if(current->end == 1 && current->word == word){
    //                 return current; //aca deberiamos devolver el puntero con la palabra
    //             }
    //             current = current->down;
    //         } else{
    //             current = current->next;
    //         }
    //     }
    // }

    // (RECURSIÓN)
    //
    // f(n, lista)
    // if (n no hijos)
    //      // if (end = 1 && n = palabra) ->devuelvo nodo palabra
    //          else: return 0;
    // else
    //      if(existe n->down):
    //          f(n->down, lista)
    //      if (existe n->next)
    //          f(n->next, lista)
    //

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

int keysPredictCountWordAux(struct node* n) {
    if (n == NULL) return 0; // caso base

    int count = 0; // creo variable contador
    if (n->end == 1) {
        count++; // incremento el contador en 1
        //printf("Found word ending: %s\n", n->word);
    }

    // Sumar recursivamente desde los n- down y next
    count += keysPredictCountWordAux(n->down);
    count += keysPredictCountWordAux(n->next);

    return count;
}

char** keysPredictRun(struct keysPredict* kt, char* partialWord, int* wordsCount) {
    int largo = strLen(partialWord); // calcular largo del prefijo
    struct node* n = kt->first;
    struct node* prev = NULL;
    *wordsCount = 0; //quiero que el puntero apunte al valor 0

    // Aca queremos buscar el prefijo
    for (int i = 0; i < largo; i++) {
        n = findNodeInLevel(&n, partialWord[i]); // buscar la letra en el nivel

        if (n == NULL) { // No se encontró ninguna palabra para el prefijo
            *wordsCount = 0;
            return NULL;
        }

        // Si n no es NULL, mostramos el carácter encontrado
        prev = n;
        n = n->down; // Bajamos al siguiente nivel
    }

    *wordsCount = keysPredictCountWordAux(n); // saber la cantidad de palabras que hay
    char** listaPalabras = (char**) malloc((*wordsCount + 1) * sizeof(char*)); // creo la lista
    if (listaPalabras == NULL) { // Manejo de error por si hay algún problema al crear la lista
        *wordsCount = 0;
        return NULL;
    }

    int k = 0; //para contar
    if(prev != NULL && prev->end == 1) { // esto es por si el prefijo es una palabra
        listaPalabras[k] = strDup(prev->word);
        k++;
        (*wordsCount)++;
    }

    keysPredictRunRec(n, listaPalabras, &k); // llamamos a la función recursiva

    return listaPalabras;
}

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

void keysPredictDelete(struct keysPredict* kt) {

    // Borra la estructura keysPredict completa. Para esto debe borrar todos los nodos, incluyendo 
    // las palabras almacenadas en cada nodo de ser necesario. Adem´as debe borrar el nodo raíz.

}

void keysPredictPrint(struct keysPredict* kt) {
    printf("--- Predict --- Keys: %i Words: %i\n", kt->totalKeys, kt->totalWords);
    keysPredictPrintAux(kt->first, 0);
    printf("---\n");
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

// Auxiliar functions

struct node* findNodeInLevel(struct node** list, char character) {

    if(*list == NULL || (*list)->next == NULL) return NULL;

    struct node* p = *list;
    while(p != NULL){
        if(p->character == character){
            return p;
        }
        p = p->next;
    }

    return NULL;
}

struct node* addSortedNewNodeInLevel(struct node** list, char character) {

    struct node* nuevoNodo = (struct node*)malloc(sizeof(struct node));
    nuevoNodo->character = character;
    nuevoNodo->next = 0;
    nuevoNodo->end = 0;
    nuevoNodo->word = 0;
    nuevoNodo->down = 0;

    if (*list == NULL){ //si lista está vacía
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
        free(nuevoNodo); // El carácter ya existe, liberar el nuevo nodo
        return *list;
    }
    
    anterior->next = nuevoNodo;
    nuevoNodo->next = actual;

    return nuevoNodo;
}

void deleteArrayOfWords(char** words, int wordsCount) {

    // Recorrer 
}


