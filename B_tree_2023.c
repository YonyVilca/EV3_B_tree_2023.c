#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct bt_Nodo *b_Tree;

b_Tree btree_Crear(void);

//////// liberar espacio */
void btree_Liberar(b_Tree t);

//////// funcion que devuelve un valor distinto de cero si la llave está presente en el árbol */
int btree_Buscar(b_Tree t, int key);

//////// insertar un nuevo elemento en el árbol */
/////// TAREA
void btree_Insertar(b_Tree t, int key);

//////// imprimir todas las claves del árbol en orden */
//////  TAREA
void btree_Print(b_Tree t);

typedef struct bt_Nodo *b_Tree;

#define MAX_LLAVES (3)

struct bt_Nodo {
    int esHoja;
    int numLLaves;
    int LLaves[MAX_LLAVES];
    struct bt_Nodo *hijos[MAX_LLAVES+1];
};

b_Tree
btree_Crear(void)
{
    b_Tree b;

    b = malloc(sizeof(*b));
    assert(b);

    b->esHoja = 1;
    b->numLLaves = 0;

    return b;
}

void btree_Liberar(b_Tree b)
{
    if (!b->esHoja) {
        for (int i = 0; i <= b->numLLaves; i++) {
            btree_Liberar(b->hijos[i]);
        }
    }

    free(b);
}

static int btree_BuscarLLave(int n, const int *a, int llave)
{
    int inf = -1;
    int sup = n;
    int med;

    while (inf < sup - 1) {
        med = (inf + sup) / 2;
        if (a[med] == llave) {
            return med;
        } else if (a[med] < llave) {
            inf = med;
        } else {
            sup = med;
        }
    }

    return sup;
}

int btree_Buscar(b_Tree b, int llave)
{
    if (b->numLLaves == 0) {
        return 0;
    }

    int pos = btree_BuscarLLave(b->numLLaves, b->LLaves, llave);

    if (pos < b->numLLaves && b->LLaves[pos] == llave) {
        return 1;
    } else {
        return (!b->esHoja && btree_Buscar(b->hijos[pos], llave));
    }
}
//////// insertar un nuevo elemento en el árbol */
/////// TAREA
//////// imprimir todas las claves del árbol en orden */
//////  TAREA

void btree_Insertar(b_Tree t, int key)
{
    // Si el árbol está vacío, se crea un nuevo nodo hoja y se inserta la clave
    if (t->numLLaves == 0) {
        t->LLaves[0] = key;
        t->numLLaves = 1;
        return;
    }

    // Si el nodo actual es una hoja, se inserta la clave en su posición adecuada
    if (t->esHoja) {
        int pos = btree_BuscarLLave(t->numLLaves, t->LLaves, key);

        // Desplazar las claves mayores hacia la derecha para hacer espacio para la nueva clave
        for (int i = t->numLLaves; i > pos; i--) {
            t->LLaves[i] = t->LLaves[i - 1];
        }

        // Insertar la nueva clave en la posición adecuada
        t->LLaves[pos] = key;
        t->numLLaves++;
    } else {
        // Si el nodo actual no es una hoja, se encuentra el hijo correspondiente
        int pos = btree_BuscarLLave(t->numLLaves, t->LLaves, key);

        // Si el hijo está lleno, se realiza una división para mantener el tamaño máximo de claves
        if (t->hijos[pos]->numLLaves == MAX_LLAVES) {
            b_Tree nuevoHijo = malloc(sizeof(*nuevoHijo));
            assert(nuevoHijo);

            // Se divide el hijo lleno en dos nuevos hijos
            int mediana = t->hijos[pos]->LLaves[MAX_LLAVES / 2];
            nuevoHijo->esHoja = t->hijos[pos]->esHoja;

            // Se mueven las claves y los hijos a los nuevos nodos
            for (int i = MAX_LLAVES / 2 + 1; i < MAX_LLAVES; i++) {
                nuevoHijo->LLaves[i - (MAX_LLAVES / 2 + 1)] = t->hijos[pos]->LLaves[i];
                nuevoHijo->hijos[i - (MAX_LLAVES / 2 + 1)] = t->hijos[pos]->hijos[i];
            }
            nuevoHijo->hijos[MAX_LLAVES - (MAX_LLAVES / 2)] = t->hijos[pos]->hijos[MAX_LLAVES];

            t->hijos[pos]->numLLaves = MAX_LLAVES / 2;
            nuevoHijo->numLLaves = MAX_LLAVES - (MAX_LLAVES / 2);

            // Se desplazan los hijos hacia la derecha para hacer espacio para el nuevo hijo
            for (int i = t->numLLaves; i > pos; i--) {
                t->hijos[i + 1] = t->hijos[i];
            }

            // Se inserta el nuevo hijo en la posición adecuada
            t->hijos[pos + 1] = nuevoHijo;

            // Se desplazan las claves hacia la derecha para hacer espacio para la mediana
            for (int i = t->numLLaves; i > pos; i--) {
                t->LLaves[i] = t->LLaves[i - 1];
            }

            // Se inserta la mediana en la posición adecuada
            t->LLaves[pos] = mediana;
            t->numLLaves++;

            // Se decide en qué hijo se debe insertar la clave
            if (key > mediana) {
                pos++;
            }
        }

        // Se realiza la inserción recursiva en el hijo correspondiente
        btree_Insertar(t->hijos[pos], key);
    }
}

void btree_Print(b_Tree t)
{
    if (t) {
        int i;
        for (i = 0; i < t->numLLaves; i++) {
            if (!t->esHoja) {
                btree_Print(t->hijos[i]);
            }
            printf("%d ", t->LLaves[i]);
        }

        if (!t->esHoja) {
            btree_Print(t->hijos[i]);
        }
    }
}

int main()
{
    b_Tree btree = btree_Crear();

 // Insertar valores en el árbol
    btree_Insertar(btree, 5);
    btree_Insertar(btree, 2);
    btree_Insertar(btree, 7);

    // Imprimir las claves del árbol en orden
    btree_Print(btree);

    btree_Liberar(btree);
    return 0;
}
