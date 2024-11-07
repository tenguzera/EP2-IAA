#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct atualizacao {
    int numeroAtt;
    int duracao;
} atualizacao;

typedef struct operacao {
    char tipo;
    int instante;
    int numero;
    int duracao;
} operacao;

/*********************************************************************/
/**   ACH2002 - Introdução à Análise de Algoritmos                  **/
/**   EACH-USP - Segundo Semestre de 2023                           **/
/**   Prof.Dra. Ariane Machado Lima                                 **/
/**                                                                 **/
/**   Exercício de Programação 2: Algoritmo Guloso, Ordenação, Heap **/
/**                                                                 **/
/**   <João Ricardo Paiva Camargo>            <14747274>            **/
/**                                                                 **/
/*********************************************************************/

void insertionSort(atualizacao arr[], int size){
    int i, j;
    atualizacao key;
    for (i = 1; i < size; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j].duracao > key.duracao) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void cenarioUm(int periodo, char* arquivo){

    FILE* file = fopen (arquivo, "r");
    int numUpdates;
    fscanf (file, "%d", &numUpdates);

    atualizacao updates[numUpdates];
    int i = 0;
    while(!feof (file)){
        fscanf(file, "%i", &updates[i].duracao);
        updates[i].numeroAtt = i + 1;
        i++;
    }
    fclose (file);

    insertionSort(updates, numUpdates);

    int tempo = 0;
    int j = 0;
    while(tempo < periodo){
        tempo = tempo + updates[j].duracao;
        if(tempo <= periodo) j++;
    }

    FILE* out = fopen("saida1.txt","w");
    fprintf(out,"%i", j);
    for(int k = 0; k < j; k++) fprintf(out, " %i", updates[k].numeroAtt);
    fprintf(out,"\n");
}

int buscaSequencial(atualizacao arr[], int size, int numero) {
    for (int i = 0; i < size; i++) {
        if (arr[i].numeroAtt == numero) {
            return i;
        }
    }
}

void swap(atualizacao *a, atualizacao *b) {
    atualizacao temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(atualizacao arr[], int index) {
    while (index > 0 && arr[index].duracao < arr[(index - 1) / 2].duracao) {
        swap(&arr[index], &arr[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void heapify(atualizacao arr[], int size, int i){
    int menor = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < size && arr[l].duracao < arr[menor].duracao)
      menor = l;
    if (r < size && arr[r].duracao < arr[menor].duracao)
      menor = r;
    if (menor != i)
    {
      swap(&arr[i], &arr[menor]);
      heapify(arr, size, menor);
    }
}

void insert(atualizacao arr[], int* size, atualizacao newUpd){
  if (*size == 0) {
    arr[0] = newUpd;
    (*size)++;
  }
  else {
    arr[*size] = newUpd;
    (*size)++;
    for (int i = *size / 2 - 1; i >= 0; i--) {
      heapify(arr, *size, i);
    }
  }
}

atualizacao extrairHeapMin(atualizacao arr[], int* size){
    atualizacao min = arr[0];
    arr[0] = arr[*size - 1];
    (*size)--;
    heapify(arr, *size, 0);
    return min;
}

void diminuirChaveHeap(atualizacao arr[], int size, int ch, int numero){
    int i = buscaSequencial(arr, size, numero);
    arr[i].duracao = ch;
    heapifyUp(arr, i);
}

void construirHeapMin(atualizacao arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}

void cenarioDois(int periodo, char* arquivo){

    FILE* file = fopen (arquivo, "r");
    int numUpdates;
    fscanf (file, "%d", &numUpdates);

    atualizacao updates[numUpdates];
    int i = 0;
    while(i < numUpdates){
        fscanf(file, "%i", &updates[i].duracao);
        updates[i].numeroAtt = i + 1;
        i++;
    }

    operacao op[100];
    char linhanova;
    int l = 0;
    while(!feof (file)){
        linhanova = fgetc(file);
        op[l].tipo = fgetc(file);
        fscanf (file, "%i", &op[l].instante);
        fscanf (file, "%i", &op[l].numero);
        fscanf (file, "%i", &op[l].duracao);
        l++;
    }

    fclose (file);

    construirHeapMin(updates, numUpdates);

    int tempo = 0;
    int j = 0;
    int k = 0;
    int numAttRealizadas[100];
    while(tempo < periodo){
        if(op[k].instante <= tempo){
            if(op[k].tipo == 'c') diminuirChaveHeap(updates, numUpdates, op[k].duracao, op[k].numero);
            if(op[k].tipo == 'i'){
                atualizacao novo;
                novo.duracao = op[k].duracao;
                novo.numeroAtt = op[k].numero;
                insert(updates, &numUpdates, novo);
            }
           if(k < i) k++;
           continue;
        }
        atualizacao realizada = extrairHeapMin(updates, &numUpdates);
        tempo = tempo + realizada.duracao;
        numAttRealizadas[j] = realizada.numeroAtt;
        if(tempo <= periodo) j++;
    }

    FILE* out = fopen("saida2.txt","w");
    fprintf(out,"%i", j);
    for(int p = 0; p < j; p++) fprintf(out, " %i", numAttRealizadas[p]);
    fprintf(out,"\n");

}

int main(int argc, char* argv[]){

    if (argc != 4){
        printf("Este programa exige três argumentos: a duração do período de atualizações, o número do cenário executado e o nome do arquivo de entrada.\n");
        exit(1);
    }

    if(strcmp(argv[2], "1")==0) cenarioUm(atoi(argv[1]), argv[3]);
    if(strcmp(argv[2], "2")==0) cenarioDois(atoi(argv[1]), argv[3]); 
}