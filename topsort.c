#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Criacao linked list
typedef struct node {
    int val;
    struct node * next;
} linkedList;

//--------------------------------------------------------------
// Libera memoria dos ponteiros
int **liberarMemoria(int **vetor, int tamanho) {

    if (vetor == NULL) {
        return (NULL);
    }

    for (int i = 0; i < tamanho; i++) { 
        if (vetor[i]) { 
            free(vetor[i]); // libera linha da matriz
            vetor[i] = NULL;
        } 
    } 

    free(vetor);         //libera ponteiro
    vetor = NULL;

    return (NULL);   //retorna ponteiro null
}
//--------------------------------------------------------------


// Aloca espaco na memoria para matriz
int **alocarMatrix(int tamanho) {


    int **vetor;
    
    //Alocacao de um vetor
    vetor = (int**) malloc(tamanho * sizeof(int*));

    if (vetor == NULL) {
        printf ("Erro ao alocar memoria");
        return (NULL);
    }

    // aloca memoria para cada linha da matriz
    for (int i = 0; i < tamanho; i++) {
        vetor[i] = (int*) malloc(tamanho * sizeof(int));

        if (vetor[i] == NULL) {
            printf ("Memoria insuficiente");
            liberarMemoria(vetor, tamanho);
            return (NULL);
        }

        // inicializa matriz com 0s
            for (int j = 0; j < tamanho; j++)
                vetor[i][j] = 0;   
    }
    return (vetor); // retorna ponteiro para o vetor
}

//--------------------------------------------------------------


//Remove primeiro item da lista
int pop(linkedList ** head) {
    int retval = -1;
    linkedList * next_node = NULL;

    if (*head == NULL) {
        return -1;
    }

    next_node = (*head)->next;
    retval = (*head)->val;
    free(*head);
    *head = next_node;

    return retval;
}


//--------------------------------------------------------------

//Adiciona ao comeco da lista
void push(linkedList ** head, int val) {
    linkedList * new_node;
    new_node = (linkedList *) malloc(sizeof(linkedList));

    new_node->val = val;
    new_node->next = *head;
    *head = new_node;
}


//--------------------------------------------------------------

//Adiciona ao final da lista
void pushEnd(linkedList **head, int val) {
    linkedList * new_node;
    new_node = (linkedList *) malloc(sizeof(linkedList));
    new_node->val = val;
    new_node->next = NULL;


    //head NULL, a lista esta vazia
    if(*head == NULL)
         *head = new_node;
    //caso contrario acha o ultimo vertice
    else
    {
        linkedList *lastNode = *head;

        //endereço do ultimo vertice sera NULL
        while(lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }

        //adiciona ao final da linked list
        lastNode->next = new_node;
    }
}


//--------------------------------------------------------------


//Remove ultimo item da lista
int remove_last(linkedList **head) {

    linkedList *prev = NULL, *cur = NULL;
    int num = 0;
    
    //Lista nao existe ou esta vazia
    if(head == NULL || *head == NULL) 
        return 0;
  
    //Somente um vertice na lista
    if((*head)->next == NULL) {
        num = (*head)->val;
        free(*head);
        *head = NULL;
        return num;
    }
  
    prev = *head;
    cur = prev->next;
  
    while(cur->next) {
      prev = prev->next;
      cur = cur->next;
    }
    
    num = cur->val;

    prev->next = NULL;
    free(cur);

    return num;


}


//--------------------------------------------------------------


int main(void){

    int vertices, arestas;
    int **matriz;
    //int verticesSemAresta[vertices];

    //Linked list elementos em ordem
    linkedList *sorted = (linkedList *) malloc(sizeof(linkedList));
    sorted = NULL;

    //Linked list vertices sem aresta
    linkedList *verticesSemAresta = (linkedList *) malloc(sizeof(linkedList));
    verticesSemAresta = NULL;


    linkedList *head = NULL;
    head = (linkedList *) malloc(sizeof(linkedList));
    head->val = 0;



    printf("Digite a quantidade de vertices e arestas\n");
    scanf("%d %d",&vertices, &arestas);

    //Aloca um vetor com 0s
    matriz = alocarMatrix(vertices);

    int c; //Coleta resquicios da scanf
    while ((c = getchar()) != '\n' && c != EOF); 

    //Criacao de uma matriz de adjacencia
    for (int i = 0; i < vertices; i++){

        char input[arestas * 2];
        int num;

        printf("Digite as arestas do grafo %d\n", i + 1);
        fgets(input,arestas * 2,stdin);
        const char s[2] = " ";
        char *token;
        
        token = strtok(input, s);
        int j=0;    
        num = atoi(token);

        while( token != NULL ){
            j++;
            matriz[i][num - 1] = 1;
            token = strtok(NULL, s);
            num = atoi(token);
        }
        free(token);
    }
    
    //vertices sem arestas
    for (int i = 0; i < vertices; i++){
        int count = 0;
        for (int j = 0; j < vertices; j++){
            if (matriz[i][j] == 1){
                count = 1;
            }
        }
        if (count == 0){
            push(&verticesSemAresta,i + 1);
        }
    }

    //head aponta para os vertices sem arestas coletados no comeco
    head->next = verticesSemAresta;


    //Sort
    while (head != NULL){
        
        int temp = 0;
        temp = remove_last(&head);

        if (temp == 0)
            break;
          
        //Adiciona vertice a lista de sorted
        push(&sorted, temp);
        //Remove arestas conectadas ao vertice removido
        for (int i = 0; i < vertices; i++){

            int count = 0;

            //remove a linha da matriz com o vertice removido
            if (i == temp - 1){
                for (int z = 0; z < vertices; z++)
                {
                    matriz[i][z] = 1;
                }
                continue;
            }
            

            if (matriz[i][temp - 1] == 1){
                matriz[i][temp - 1] = 0;
            }

            for (int j = 0; j < vertices; j++){
                if (matriz[i][j] == 1){
                    count = 1;
                }
            }
            
            if (count == 0){
                pushEnd(&head,i + 1);
            }
        }
        //head = head->next;
        //printf("teste fim %d\n",head->val);
    }
    

    printf("Lista organizada \n");
    while (sorted != NULL){
        printf("%d ", sorted->val);
        sorted = sorted->next;
    }
   
/*
    printf("\nMatrix \n");
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++)
            printf("%d ", matriz[i][j]);
            printf("\n");
    }
 */   
    matriz = liberarMemoria(matriz,vertices);

}