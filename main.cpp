#include <bits/stdc++.h>

using namespace std;

#define PRINCIPIANTE 0
#define INTERMEDIO 1
#define AVANZADO 2
#define LADOMAX 25
#define MAXMINAS 99
#define TAMAMOV 526 /* Tamaño del movimiento, no puedo usar la 'ñ' (25 * 25 - 99) */

int LADO; /* Tamaño del lado */
int MINAS; /* Numero de minas en el tablero */

/* Funcion para ver si una celda dada es valida (fila, columna) */
bool esValida(int fila, int columna)
{
    /* Retorna verdadero si el numero esta en el rango deseado */
    return (fila >= 0) && (fila < LADO) && (columna >= 0)
           && (columna < LADO);
}

class Tablero {

public:
    char** tablero;
    Tablero()
    {
        tablero = new char*[LADOMAX + 1];
        for (int i = 0; i <= LADOMAX; i++) {
            tablero[i] = new char[LADOMAX + 1];
            for (int j = 0; j <= LADOMAX; j++) {
                tablero[i][j] = '-';
            }
        }
    }

    	/* Funcion para ver si una celda tiene una mina */
    bool esMina(int fila, int columna)
    {
        if (tablero[fila][columna] == '*')
            return (true);
        else
            return (false);
    }

    /* Funcion para obtener el movimiento del usuario */
    void moverse(int* x, int* y)
    {
	/* Tomar la entrada del movimiento */
        printf("Ingresar tu movimiento, (fila, columna) -> ");
        scanf("%d %d", x, y);
        return;
    }

    /* Funcion para imprimir el estado actual del tablero */
    void imprimirTablero()
    {
        int i, j;
        printf("    ");

        for (i = 0; i < LADO; i++)
            printf("%d ", i);

        printf("\n\n");

        for (i = 0; i < LADO; i++) {
            printf("%d   ", i);

            for (j = 0; j < LADO; j++)
                printf("%c ", tablero[i][j]);
            printf("\n");
        }
        return;
    }

	/* Funcion para contar el numero de minas en celdas adyacentes */
    int contarMinasAdyacentes(int fila, int columna, int minas[][2])
    {
        int i;
        int contar = 0;

        /*
         Contar las minas en los 8 espacios adyacentes 

            Celda = Celda actual (fila, columna)
            N = Norte        (fila-1, columna)
            S =  South        (fila+1, columna)
            E =  Este         (fila, columna+1)
            O =  Oeste           (fila, columna-1)
            N.E =  Noreste   (fila-1, columna+1)
            N.O =  Noroeste   (fila-1, columna-1)
            S.E =  Sureste  (fila+1, columna+1)
            S.W = Suroeste   (fila+1, columna-1)
        */

        int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
        int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

        for (int d = 0; d < 8; d++) {
            int nuevaFila = fila + dx[d];
            int nuevaColumna = columna + dy[d];

            if (esValida(nuevaFila, nuevaColumna) == true) {
                if (esMina(nuevaFila, nuevaColumna) == true)
                    contar++;
            }
        }
        return (contar);
    }

   /* Funcion para poner las minas de manera aleatoria */
    void ponerMinas(int minas[][2])
    {
        bool marca[LADOMAX * LADOMAX];

        memset(marca, false, sizeof(marca));

	/* Continuar hasta que todas las minas hayan sido puestas */
        for (int i = 0; i < MINAS;) {
            int random = rand() % (LADO * LADO);
            int x = random / LADO;
            int y = random % LADO;

	    /* Añadir mina si no hay mina puesta en esta posicion */
            if (marca[random] == false) {
		/* Fila de indices de mina*/
                minas[i][0] = x;
                /* Column de indices de minas */
                minas[i][1] = y;

		/* Poner la mina */
                tablero[minas[i][0]][minas[i][1]] = '*';
                marca[random] = true;
                i++;
            }
        }
        return;
    }

    /* Funcion para reemplazar la mina (row, col) y
     ponerla en un espacio vacante */
    void reemplazarMina(int fila, int columna)
    {
        for (int i = 0; i < LADO; i++) {
            for (int j = 0; j < LADO; j++) {
                /* Encontrar la primera ubicacion en el tablero 
                * que no tiene una mina 
		* ponerla ahi. */
                if (tablero[i][j] != '*') {
                    tablero[i][j] = '*';
                    tablero[fila][columna] = '-';
                    return;
                }
            }
        }
        return;
    }
};
class Juego {
public:
     /* Funcion recursiva para jugar buscaminas */
    bool jugarBuscaMinas(Tablero& miTablero,
                             Tablero& tableroReal,
                             int minas[][2], int fila,
                             int columna, int* movimientosRestantes)
    {
	 /* Caso base de recursion */
        if (miTablero.tablero[fila][columna] != '-')
            return (false);

        int i, j;

	/* Se abrio una mina 
	 * el usuario va a perder. 
	 */
        if (tableroReal.tablero[fila][columna] == '*') {
            miTablero.tablero[fila][columna] = '*';
            for (i = 0; i < MINAS; i++)
                miTablero.tablero[minas[i][0]][minas[i][1]]
                    = '*';

            miTablero.imprimirTablero();
            printf("\nPerdiste!\n");
            return (true);
        }
        else {
		/* Calcular el numero de minas adyacentes y ponerlas en la pantalla */
            int contar = tableroReal.contarMinasAdyacentes(
                fila, columna, minas);
            (*movimientosRestantes)--;

            miTablero.tablero[fila][columna] = contar + '0';

            if (!contar) {
                /*
                Recursion para todas las minas adyacentes 

                Celda =  celda actual (fila, columna)
                N =  Norte        (fila-1, columna)
                S = Sur      (fila+1, columna)
                E = Este         (fila, columna+1)
                O =  Oeste            (fila, columna-1)
                N.E = Noreste  (fila-1, columna+1)
                N.O = Noroeste   (fila-1, columna-1)
                S.E = Sureste (fila+1, columna+1)
                S.O =  Suroeste   (fila+1, columna-1)
                */

                int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
                int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

                for (int d = 0; d < 8; d++) {
                    int nuevaFila = fila + dx[d];
                    int nuevaColumna = columna + dy[d];

                    if (esValida(nuevaFila, nuevaColumna) == true) {
                        if (tableroReal.esMina(nuevaFila, nuevaColumna)
                            == false)
                            jugarBuscaMinas(
                                miTablero, tableroReal, minas,
                                nuevaFila, nuevaColumna, movimientosRestantes);
                    }
                }
            }
            return (false);
        }
    }

	/* Funcion para engañar revelando las posiciones de las minas */
    void mentirBuscaminas(Tablero& tableroReal)
    {
        printf("Las ubicaciones de las minas son-\n");
        tableroReal.imprimirTablero();
        return;
    }

    /* Funcion para jugar buscaminas */
    void buscaMinas(Tablero& tableroReal, Tablero& miTablero)
    {
	/* Inicialmente el juego no esta terminado */
        bool juegoAcabado = false;

        int movimientosRestantes = LADO * LADO - MINAS, x, y;
        int minas[MAXMINAS][2]; /* Guarda las coordenadas (x, y) de todas las minas */


	/* reemplazar las minas aleatoriamente */
        tableroReal.ponerMinas(minas);

        int indiceDeMovimientoActual = 0;
        while (juegoAcabado == false) {
            printf("Estado actual del tablero : \n");
            miTablero.imprimirTablero();
            miTablero.moverse(&x, &y);

            /* esto es para garantizar que el primer movimiento este libre de minas */
            if (indiceDeMovimientoActual == 0) {
                // Si el primer movimiento de es una mina 
                // luego se quita la mina de esa 
                // ubicacion 
                if (tableroReal.esMina(x, y) == true)
                    tableroReal.reemplazarMina(x, y);
            }

            indiceDeMovimientoActual++;

            juegoAcabado = jugarBuscaMinas(
                miTablero, tableroReal, minas, x, y,
                &movimientosRestantes);

            if ((juegoAcabado == false) && (movimientosRestantes == 0)) {
                printf("\nGanaste !\n");
                juegoAcabado = true;
            }
        }
        return;
    }
};

void escogerNivelDeDificultad()
{
    /*
    --> PRINCIPIANTE = Celdas de 9 * 9 y 10 Minas
    --> INTERMEDIO = Celdas de 16 * 16 y 40 Minas
    --> AVANZADO = Celdas de 24 * 24 y 99 Minas
    */

    int nivel;

    printf("Ingresar el nivel de dificultad\n");
    printf("Presionar 0 para PRINCIPIANTE (celdas de 9 * 9 y 10 "
           "Minas)\n");
    printf("Presionar 1 para INTERMEDIO (celdas de 16 * 16 y 40 "
           "Minas)\n");
    printf("Presionar 2 para AVANZADO (celdas de 24 * 24 y 99 "
           "Minas)\n");

    scanf("%d", &nivel);

    if (nivel == PRINCIPIANTE) {
        LADO = 9;
        MINAS = 10;
    }

    if (nivel == INTERMEDIO) {
        LADO = 16;
        MINAS = 40;
    }

    if (nivel == AVANZADO) {
        LADO = 24;
        MINAS = 99;
    }
}

int main()
{
    /* Escoger un nivel entre
--> PRINCIPIANTE = celdas de 9 * 9 y 10 Minas
--> INTERMEDIO = celdas de 16 * 16 y  40 Minas
--> AVANZADO = celdas de 24 * 24 y 99 Minas
*/

    Tablero miTablero, tableroReal;

    Juego* juego = new Juego();
    escogerNivelDeDificultad(); 

    juego->buscaMinas(miTablero, tableroReal);
    return 0;
}
