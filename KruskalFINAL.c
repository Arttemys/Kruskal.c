#include<stdio.h>
#include<stdlib.h>

#define VERTICES 6  //VERTICES SI O SI >=2
///Casos automaticos para VERTICES e [4,6]
#define ASCII 65 ///NO TOCAR 65=A

///ESTRUCTURAS

typedef struct ARISTA
{
    int u;
    int v;
    int costo;
}arista;

//Cola de prioridad
typedef struct RAMA
{
    arista a;
    struct RAMA *sig;
}rama;

typedef struct ENCABEZADO
{
    int cuenta;
    int primer_elemento;
}encabezado;

typedef struct NOMBRE
{
int nombre_conjunto;
int siguiente_elemento;
}nombre;

typedef struct CONJUNTO_CE
{
   nombre nombres[VERTICES];
   encabezado encabezamientos_conjunto[VERTICES];
}conjunto_CE;

//PROTOTIPOS PARA KRUSKAL
void inicial(int,int,conjunto_CE *);
void combina(int,int,conjunto_CE *);
int encuentra(int,conjunto_CE *);
void kruskal(rama*,rama *);

//PROTOTIPOS COLA DE PRIORIDAD
void inserta(int,int,int,rama **);//agrega elemento en la pila
arista sacar_min(rama**);
void lista (rama *);//Muestra cola de prioridad

///PROTOTIPOS CREADOS POR NUESTRO GRUPO
void VaciaCola(rama**);//Limpia memoria heap de cola
void validar(int,int,int **);//Valida costo >=0
void mostrar(conjunto_CE);//Muestra CE

int main()
{   int **M,i,j,OP;
    rama *arbol=NULL;
    //calloc inicializa array en 0
    ///CREO ARRAY BIDIMENCIONAL EN MEMORIA HEAP
    M=(int**)calloc(VERTICES,sizeof(int*));
    for(i=0;i<VERTICES;i++)
    {
       M[i]=(int*)calloc(VERTICES,sizeof(int));
    }

    if(VERTICES>=4 &&VERTICES<=6) /// En estos casos dejamos en manos del USUARIO elegir si quiere carga MANUAL o AUTOMATICA
    {
        printf("--------MENU--------\n0_Carga Manual.\n1_Carga automatica.\nIngrese 0 o 1: ");scanf("%d",&OP);
        while(OP<0||OP>1){printf("ERROR elija opcion correcta (0,1)\n");scanf("%d",&OP);}
        printf("--------------------\n\n");
        switch(OP)
        {
            case 0:
                { ///CARGA MANUAL
                    for(i=0;i<VERTICES;i++)
                    {
                        for(j=i+1;j<VERTICES;j++)
                        {
                            printf("M[%d][%d]\n",i,j);
                            printf("Ingrese costo>=0 de lado entre vertices %c y %c \n",ASCII+i,ASCII+j);
                            scanf("%d",&M[i][j]);
                            validar(i,j,M);
                        }

                    }
                }break;
            case 1:
                {   ///CARGA AUTOMATICA

                    switch(VERTICES)
                    {
                    case 4: //Creado por nosotros
                            {
                              M[0][2]=5;M[0][3]=8;M[1][2]=3;M[1][3]=3;M[2][3]=1;
                            }break;
                    case 5:  //Creado por nosotros
                            {
                            M[0][1]=0;M[0][2]=5;M[0][3]=8;M[0][4]=6;
                                      M[1][2]=3;M[1][3]=0;M[1][4]=7;
                                                M[2][3]=1;M[2][4]=0;
                                                          M[3][4]=1;
                            }break;
                    case 6: //Caso que figura en PDF de kruskal
                        {
                         M[0][1]=6;M[0][2]=1;M[0][3]=5;M[1][2]=5;M[1][4]=3;M[2][3]=5;M[2][4]=6; M[2][5]=4;M[3][5]=2;M[4][5]=6;
                        }break;
                    }


                }
        }
    }
    else
    {
        ///CARGA MANUAL SI SON MENOS DE 4 O MAS DE 6 VERTICES
        for(i=0;i<VERTICES;i++)
        {
            for(j=i+1;j<VERTICES;j++)
            {
                printf("M[%d][%d]\n",i,j);
                printf("Ingrese costo>=0 de lado entre vertices %c y %c \n",ASCII+i,ASCII+j);
                scanf("%d",&M[i][j]);
                validar(i,j,M);
            }

        }
    }



///Carga de la cola de prioridad
    for( i=0;i<VERTICES;i++)
    {
       for( j, j=i+1;j<VERTICES;j++)
       {
           if(M[i][j]!=0)
           {
               inserta(i,j,M[i][j],&arbol);
           }
       }
    }
    ///Libero memoria del array
    for(i=0;i<VERTICES;i++) free(M[i]);
    free(M);


    rama *Sol=NULL; /// Cola Solucion
    ///Muestro cola y desarrollo kruskal
    printf("\n-------------------------\n");
    printf("Lista de Costos:\n");
    lista(arbol);

   kruskal(arbol,Sol); // Llamado a Kruskal y desarrollo del algoritmo

   printf("Vacio cola de prioridad\n");
   VaciaCola(&arbol);
    printf("\nFin del programa. Adios\nSaludos Atte Damian Acurso, Alejandra Arrieta y Nardelli Luciano\n");

    return 0;
}

void validar(int i,int j, int **M)//Valido costos >=0
{
    while(M[i][j]<0)
    {
        printf("ERROR ingrego num negativo. Ingrese costo>=0 de lado entre vertices %c y %c \n",ASCII+i,ASCII+j);
        scanf("%d",&M[i][j]);
    }
}

void VaciaCola(rama** A) //ASCII=65=A
{
    ///Libero memoria de cola
    while(A!=NULL)
    {
         //printf("Elimino de la cola la arista formada por los vertices %c y %c de coste %d\n",(*A)->a.u+ASCII,(*A)->a.v +ASCII,(*A)->a.costo);

        if((*A)->sig!=NULL)
        {
            (*A)=(*A)->sig;
        }
        else A=NULL;


    }
    if(A==NULL) printf("Cola  vacia\n");
    else("Problemas todavia no se elimino la cola de la memoria\n");

}

void inserta(int i,int j,int c,rama **A)//Menor a mayor  //ASCII=65=A
{

 ///Se dejaron diferentes printf para vericar en cual caso entra
    if ((*A) == NULL){  //Creo cola si esta NULL
        printf("Se ingreso el costo %d entre los vertices %c y %c\n", c,ASCII+i, ASCII+j);
        (*A) = (rama*) malloc (sizeof(rama));
        (*A)->a.u = i; /// Fila
        (*A)->a.v = j; /// Columna
        (*A)->a.costo = c;
        (*A)->sig = NULL;
    } else {
        if ((*A)->a.costo > c){//Caso Inicio agrega izquierda del nodo actual
            printf("Se inserto el siguiente costo %d de los vertices %c y %c\n", c, ASCII+i, ASCII+j);
            rama *NuevoDato = (rama*) malloc (sizeof(rama));
            /// se crea Nuevo Dato
            NuevoDato->a.costo = c;
            NuevoDato->a.u = i;
            NuevoDato->a.v = j;

            NuevoDato->sig = (*A);
            (*A) = NuevoDato;
        } else if ((*A)->a.costo <= c && (*A)->sig != NULL && (*A)->sig->a.costo > c)//Agrega entre 2 nodos
            {
            printf("Se Ingreso el siguiente costo %d de los vertices %c y %c\n", c, ASCII+i, ASCII+j);
            rama *NuevoDato = (rama*) malloc (sizeof(rama));
            /// se cre Nuevo Dato
            NuevoDato->a.costo = c;
            NuevoDato->a.u = i;
            NuevoDato->a.v = j;

            NuevoDato->sig = (*A)->sig;
            (*A)->sig = NuevoDato;
            }
            else {
            inserta(i,j,c,&((*A)->sig)); // Recursion
        }
    }

}

arista sacar_min(rama **A)//Retorno informacion del nodo min
{
    if(A!=NULL)
    {
        arista LD;
        LD=(*A)->a;
        (*A)=(*A)->sig;
        return (LD);
    }
}

void lista(rama *A)//Muestro cola //ASCII=65=A
{

   if(A!=NULL)
    {
        printf("Costo %d en la arista %c ,%c\n",A->a.costo, A->a.u+ASCII, A->a.v+ASCII);
        lista(A->sig);
    }
    else printf("NULL\n");

}

int encuentra(int F, conjunto_CE *C)//Retorna nombre del conjunto
{
    return (C->nombres[F].nombre_conjunto);
}


void inicial(int A,int x, conjunto_CE *L)//Inicializo el Arreglo C
{
    conjunto_CE *C=L;
    C->nombres[x].nombre_conjunto=A;
    C->nombres[x].siguiente_elemento=0;
    C->encabezamientos_conjunto[A].cuenta=1;
    C->encabezamientos_conjunto[A].primer_elemento=x;

    *L=*C;

}

void combina(int A,int B, conjunto_CE *L)  //Combino Aristas
{
    int i;
    conjunto_CE *C=L; /// Hacemos pasaje por referencia sin utilizar punteros

    if(C->encabezamientos_conjunto[A].cuenta>C->encabezamientos_conjunto[B].cuenta) // Ingresa si la Cuenta es mayor en A que en B
    { ///A es el conjunto mas grande , combina B dentro de A
        /*encuentra el final de B , cambiando los nombres
            de los conjuntos
            por A conforme se avanza*/
        i=C->encabezamientos_conjunto[B].primer_elemento;


         while(C->nombres[i].siguiente_elemento!=0)
       {
           C->nombres[i].nombre_conjunto=A;
           i=C->nombres[i].siguiente_elemento;
           /*agrega a la lista A al final de la B y llama A
                al resultado*/
                ///ahora i es el ´ındice del ´ultimo elemento de B
       }
       C->nombres[i].nombre_conjunto=A;
       C->nombres[i].siguiente_elemento=C->encabezamientos_conjunto[A].primer_elemento;
       C->encabezamientos_conjunto[A].primer_elemento=C->encabezamientos_conjunto[B].primer_elemento;
       C->encabezamientos_conjunto[A].cuenta=C->encabezamientos_conjunto[A].cuenta+C->encabezamientos_conjunto[B].cuenta;
    }
    else ///B es el conjunto mas grande , combina A dentro de B
        /*Similar al codigo de arriba pero cambiando A por B y B por A donde corresponda*/
    {
        i=C->encabezamientos_conjunto[A].primer_elemento;
        while(C->nombres[i].siguiente_elemento!=0)
       {
           C->nombres[i].nombre_conjunto=B;
           i=C->nombres[i].siguiente_elemento;
       }

       C->nombres[i].nombre_conjunto=B;
       C->nombres[i].siguiente_elemento=C->encabezamientos_conjunto[B].primer_elemento;
       C->encabezamientos_conjunto[B].primer_elemento=C->encabezamientos_conjunto[A].primer_elemento;
       C->encabezamientos_conjunto[B].cuenta=C->encabezamientos_conjunto[B].cuenta+C->encabezamientos_conjunto[A].cuenta;
    }
    *L=*C;
}

void mostrar(conjunto_CE componentes)///Muestra tabla de arreglos segun componentes, se utiliza para verificar
{
    int V;
    printf("\nNombre               Encabezamiento\n---------------     ------------------\n\nNom_Con  Sig_El      Cuen    Prim_Ele\n");
    for(V=0;V<VERTICES;V++)
        printf("  %d         %d         %d          %d\n",(componentes).nombres[V].nombre_conjunto,componentes.nombres[V].siguiente_elemento,componentes.encabezamientos_conjunto[V].cuenta,componentes.encabezamientos_conjunto[V].primer_elemento);

        printf("\n");
}

void kruskal(rama *A, rama * S)//Desarrollo del algoritmo de kruskal
{

int comp_n; /// cantidad actual de componentes
int comp_u; /// nombre de los componentes
int comp_v; /// nombre de los componentes
int V; /// Vertices
int Krustal=0; /// Acumulador de Costes minimos
conjunto_CE componentes; /// Estructura Componentes (compuesta por 4 arreglos)
arista  a; /// Contiene al Componente Min de la cola de prioridad
    comp_n=VERTICES;
    printf("\nAlgoritmo de kruskal\n");
    for(V=0;V<comp_n;V++)
    {
       inicial(V,V,&componentes); //Inicializacion
    }
    printf("La tabla incialmente tiene los siguiente valores:\n");
    mostrar(componentes);//Muestra La tabla de arreglos al inicio y al final


    printf("\nComienzo de algoritmo\n"); //  Arranca el Algoritmo en si
    while (comp_n>1)
    {a=sacar_min(&A);
        comp_u=encuentra(a.u,&componentes);
        comp_v=encuentra(a.v,&componentes);

        if(comp_u!=comp_v) //Comparacion de componentes para ver si se forma ciclo, si son distintos utiliza COMBINA
        {
            combina(comp_u,comp_v,&componentes);
            comp_n--;
            printf("Agrego elemento en el arbol de solucion.");
            inserta(a.u,a.v,a.costo,&S);
            Krustal+=a.costo;
            //mostrar(componentes);  //Si se quiere ver la tabla paso a paso
        }
    }
    printf("La tabla al finalizar kruskal es la siguiente:\n");
    mostrar(componentes);
    printf("El arbol de kruskal es:\n"); /// Arbol Solucion
    lista(S);
    printf("\nEl costo minimo  es: %d\n\n",Krustal); /// Costo minimo

    printf("Vacio cola de solucion\n");
    VaciaCola(&S);
}
