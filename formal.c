#include <stdio.h>
#include <stdlib.h>

#define MAX_BUFFER 64

typedef struct{
    int dni;
    char nombre[MAX_BUFFER];
    char apellido[MAX_BUFFER];
    char cargo[MAX_BUFFER];
    int prioridad;
}DatosPersona;

typedef struct{
    DatosPersona datos;
    struct Nodo *ant, *sig;
}Nodo;

typedef Nodo* Lista;

Nodo* leer_nodo(FILE *f){
    DatosPersona buffer;
    Nodo *nuevo = NULL;
    if( fscanf(f, "%d, %[^,], %[^,], %[^,], %d", &buffer.dni, buffer.nombre, buffer.apellido, buffer.cargo, &buffer.prioridad) == 5 ){
        nuevo = malloc(sizeof(*nuevo));
        if( nuevo ){
            nuevo->datos = buffer;
            nuevo->ant = nuevo->sig = NULL;
        }
    }
    return nuevo;
}

Lista push_front(Lista l, Nodo *n){
    n->sig = l;
    if( l ) l->ant = n;
    return n;
}

Lista leer_lista(FILE *f){
    Lista l = NULL;
    Nodo *nodo = NULL;
    while( nodo = leer_nodo(f) )
        l = push_front(l, nodo);
    return l;
}

void imprimir_lista(Lista l){
    for(Nodo *n = l; n; n = n->sig)
        printf("%d, %s, %s, %s, %d\n", n->datos.dni, n->datos.nombre, n->datos.apellido, n->datos.cargo, n->datos.prioridad);
}

void borrar_lista(Lista l){
    if( l ){
        borrar_lista(l->sig);
        free(l);
    }
}

int main(){
    FILE *f = fopen("test.txt", "r");

    if( f == 0 ){
        puts("Error opening file.");
        return -1;
    }
AAA
    Lista l = leer_lista(f);
    imprimir_lista(l);

    borrar_lista(l);
    fclose(f);
    return 0;
}
