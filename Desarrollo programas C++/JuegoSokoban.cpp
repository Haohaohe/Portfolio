#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <conio.h>
using namespace std;

const int MAX = 50;
const int MAXH = 10;
const int MAXE = 100;

typedef enum tCasilla{ Libre, Muro, DestinoL, DestinoC, DestinoJ, Jugador, Caja };

typedef enum tTecla{ Arriba, Abajo, Derecha, Izquierda, Salir, Nada, Deshacer };

typedef enum {
	Black = 00,
	Blue = 1,
	Green = 02,
	Cyan = 3,
	Red = 4,
	Magenta = 5,
	Brown = 6,
	LightGray = 7,
	DarkGray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightCyan = 11,
	LightRed = 12,
	LightMagenta = 13,
	Yellow = 14,
	White = 15
} tColor;

typedef tCasilla tTablero[MAX][MAX];

typedef struct{
	tTablero tablero;
	int nFilas;
	int nColumnas;
	int filaJ;
	int columnaJ;
	int numCajas;
	int cajasColocadas;
}tSokoban;

typedef tSokoban tTableros[MAXH];

typedef struct{
	tTableros newTablero;
	int cont;

}tHistoria;

typedef struct{
	tSokoban sokoban;
	int numMovimientos;
	string nFichero;
	int nivel;
	tHistoria historia;
}tJuego;

typedef struct{
	string nombre;
	int level;
	int numMov;
}tPartida;

typedef tPartida tExitos[MAXE];

typedef struct{
	string nombreJ;
	tExitos exitos;
	int numP_resueltas;
}tInfo;

void menu(tInfo &info, tJuego &juego);
void jugarPartida(tInfo &info, tJuego &juego);
void mostrarPartida(const tInfo &info);
bool cargarJuego(tJuego &juego);
bool cargarNivel(ifstream &fichero, tSokoban &sokoban, int nivel);
void dibujar(const tJuego &juego);
void inicializa(tJuego &juego);
tColor CeldaToColor(tCasilla num);
void dibujaCasilla(tCasilla casilla);
void colorFondo(int color);
tTecla leerTecla();
void hacerMovimiento(tJuego &juego, tTecla tecla);
bool deshacerMovimiento(tJuego &juego);
void guardarTablero(tJuego &juego);
bool bloqueado(const tJuego &juego);
void guardarPartida(const tInfo &info);
void ordenarPartidas(tInfo &info);

void colorFondo(int color) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(handle, 15 | (color << 4));
}

void dibujaCasilla(tCasilla casilla){
	switch (casilla){
	case Libre: { cout << "  "; } break;
	case Muro: { cout << "  "; } break;
	case  DestinoL: { cout << ".."; } break;
	case DestinoC: { cout << "[]"; } break;
	case DestinoJ: {	cout << "00"; } break;
	case Jugador: {	cout << "00"; } break;
	case Caja: {	cout << "()"; } break;
	}
}

tColor CeldaToColor(tCasilla num){

	tColor color;

	if (num == Libre){ color = Blue; }
	else if (num == DestinoL){ color = LightRed; }
	else if (num == DestinoC){ color = Red; }
	else if (num == DestinoJ){ color = Red; }
	else if (num == Jugador){ color = Brown; }
	else if (num == Caja){ color = LightGray; }
	else { color = Green; }
	return color;
}

void dibujar(const tJuego &juego){
	system("cls");
	cout << "Fichero: " << juego.nFichero;
	cout << "Nivel: " << juego.nivel << endl;
	for (int i = 0; i < juego.sokoban.nFilas; i++){
		for (int j = 0; j < juego.sokoban.nColumnas; j++){
			colorFondo(CeldaToColor(juego.sokoban.tablero[i][j]));
			dibujaCasilla(juego.sokoban.tablero[i][j]);
			colorFondo(Black); //Para ponerlo en negro ¿si?
		}
		cout << endl;
	}
	//Va haber doble salto¿?
	cout << endl << "Numero de movimientos: " << juego.numMovimientos << endl;
}

void inicializa(tJuego &juego){
	for (int i = 0; i < MAX; i++){
		for (int j = 0; j < MAX; j++){
			juego.sokoban.tablero[i][j] = Libre;
		}
	}
	juego.numMovimientos = 0;
}

bool cargarNivel(ifstream &fichero, tSokoban &sokoban, int nivel){
	int nivelAux, i = 0, tam = 0;
	string aux;
	char aux1;
	sokoban.cajasColocadas = 0;
	sokoban.numCajas = 0;

	fichero >> aux;
	fichero >> nivelAux;
	fichero.get(aux1);

	while (nivel != nivelAux && !fichero.fail()){
		getline(fichero, aux);
		if ((aux == "")) {
			fichero >> aux;
			fichero >> nivelAux;
			fichero.get(aux1);
		}
	}

	if (nivel == nivelAux) {

		getline(fichero, aux);
		while (aux != "") {

			if (tam < aux.size()){
				tam = aux.size();
			}

			for (int j = 0; j < aux.size(); j++){
				if (aux[j] == '#'){
					sokoban.tablero[i][j] = Muro;
				}
				else if (aux[j] == ' '){
					sokoban.tablero[i][j] = Libre;
				}
				else if (aux[j] == '@'){
					sokoban.tablero[i][j] = Jugador;
					sokoban.columnaJ = j;
					sokoban.filaJ = i;
				}
				else if (aux[j] == '+'){
					sokoban.tablero[i][j] = DestinoJ;
					sokoban.columnaJ = j;
					sokoban.filaJ = i;
				}
				else if (aux[j] == '$'){
					sokoban.tablero[i][j] = Caja;
					sokoban.numCajas++;
				}
				else if (aux[j] == '.'){
					sokoban.tablero[i][j] = DestinoL;
				}
				else if (aux[j] == '*'){
					sokoban.tablero[i][j] = DestinoC;
					sokoban.cajasColocadas++;
					sokoban.numCajas++;
				}
			}
			i++;
			getline(fichero, aux);
		}
	}

	sokoban.nFilas = i;
	sokoban.nColumnas = tam;

	return nivel == nivelAux;
}

bool cargarJuego(tJuego &juego){
	string nombre;
	bool cargado = false;
	ifstream archivo;
	int nivel;

	cout << "Introduce el nombre del fichero: ";
	cin >> nombre;
	archivo.open(nombre);
	if (archivo.is_open()){
		cargado = true;

		cout << "Introduce el nivel: ";
		cin >> nivel;

		inicializa(juego);
		while (!cargarNivel(archivo, juego.sokoban, nivel)){
			cout << "No esta el nivel deseado, busca otro nivel.";
			archivo.close();
			system("cls");
			archivo.open(nombre);
			cout << "Introduce el nivel: ";
			cin >> nivel;
		}

		juego.nivel = nivel;
	}

	else { cout << "No se ha podido abrir el archivo." << endl; }

	return cargado;
}

tTecla leerTecla(){
	int dir;
	tTecla tecla;
	cin.sync();
	dir = _getch(); // dir: tipo int
	if (dir == 0xe0) {
		dir = _getch();
		if (dir == 72){ tecla = Arriba; }
		else if (dir == 80){ tecla = Abajo; }
		else if (dir == 77){ tecla = Derecha; }
		else if (dir == 75){ tecla = Izquierda; }
		
	}
	else if (dir == 27){ tecla = Salir; }
	else if (dir == 100 || dir == 68){ tecla = Deshacer; }
	else{ tecla = Nada; }

	return tecla;
}

void hacerMovimiento(tJuego &juego, tTecla tecla){
	int i, j;
	i = juego.sokoban.filaJ;
	j = juego.sokoban.columnaJ;


	if (tecla == Arriba){
		if (juego.sokoban.tablero[i - 1][j] == Libre || juego.sokoban.tablero[i - 1][j] == DestinoL){
			guardarTablero(juego);

			if (juego.sokoban.tablero[i][j] == DestinoJ){ juego.sokoban.tablero[i][j] = DestinoL; }
			else juego.sokoban.tablero[i][j] = Libre;

			if (juego.sokoban.tablero[i - 1][j] == DestinoL){ juego.sokoban.tablero[i - 1][j] = DestinoJ; }
			else juego.sokoban.tablero[i - 1][j] = Jugador;

			juego.sokoban.filaJ = i - 1;
			juego.sokoban.columnaJ = j;
			juego.numMovimientos++;
		}
		else if ((juego.sokoban.tablero[i - 1][j] == Caja || juego.sokoban.tablero[i - 1][j] == DestinoC) &&
			(juego.sokoban.tablero[i - 2][j] == Libre || juego.sokoban.tablero[i - 2][j] == DestinoL)){

			guardarTablero(juego);

			//Donde estaba
			if (juego.sokoban.tablero[i][j] == DestinoJ){ juego.sokoban.tablero[i][j] = DestinoL; }
			else juego.sokoban.tablero[i][j] = Libre;
			//Donde va
			if (juego.sokoban.tablero[i - 1][j] == DestinoL ||
				juego.sokoban.tablero[i - 1][j] == DestinoC) {
				if (juego.sokoban.tablero[i - 1][j] == DestinoC) juego.sokoban.cajasColocadas--;
				juego.sokoban.tablero[i - 1][j] = DestinoJ;
				
			}
			else juego.sokoban.tablero[i - 1][j] = Jugador;
			//Lo que mueve
			if (juego.sokoban.tablero[i - 2][j] == DestinoL) {
				juego.sokoban.tablero[i - 2][j] = DestinoC;
				juego.sokoban.cajasColocadas++;
			}
			else juego.sokoban.tablero[i - 2][j] = Caja;

			juego.sokoban.filaJ = i - 1;
			juego.sokoban.columnaJ = j;
			juego.numMovimientos++;
		}
	}
	else if (tecla == Abajo){
		if (juego.sokoban.tablero[i + 1][j] == Libre || juego.sokoban.tablero[i + 1][j] == DestinoL){
			guardarTablero(juego);

			if (juego.sokoban.tablero[i][j] == DestinoJ){ juego.sokoban.tablero[i][j] = DestinoL; }
			else juego.sokoban.tablero[i][j] = Libre;

			if (juego.sokoban.tablero[i + 1][j] == DestinoL){ juego.sokoban.tablero[i + 1][j] = DestinoJ; }
			else juego.sokoban.tablero[i + 1][j] = Jugador;

			juego.sokoban.filaJ = i + 1;
			juego.sokoban.columnaJ = j;
			juego.numMovimientos++;
		}
		else if ((juego.sokoban.tablero[i + 1][j] == Caja || juego.sokoban.tablero[i + 1][j] == DestinoC) &&
			(juego.sokoban.tablero[i + 2][j] == Libre || juego.sokoban.tablero[i + 2][j] == DestinoL)){
			guardarTablero(juego);

			//Donde estaba
			if (juego.sokoban.tablero[i][j] == DestinoJ){ juego.sokoban.tablero[i][j] = DestinoL; }
			else juego.sokoban.tablero[i][j] = Libre;
			//Donde va
			if (juego.sokoban.tablero[i + 1][j] == DestinoL ||
				juego.sokoban.tablero[i + 1][j] == DestinoC) {
				if (juego.sokoban.tablero[i + 1][j] == DestinoC) juego.sokoban.cajasColocadas--;
				juego.sokoban.tablero[i + 1][j] = DestinoJ;
			}
			else juego.sokoban.tablero[i + 1][j] = Jugador;
			//Lo que mueve
			if (juego.sokoban.tablero[i + 2][j] == DestinoL) {
				juego.sokoban.tablero[i + 2][j] = DestinoC;
				juego.sokoban.cajasColocadas++;
			}
			else juego.sokoban.tablero[i + 2][j] = Caja;

			juego.sokoban.filaJ = i + 1;
			juego.sokoban.columnaJ = j;
			juego.numMovimientos++;
		}
	}
	else if (tecla == Derecha){

		if (juego.sokoban.tablero[i][j + 1] == Libre || juego.sokoban.tablero[i][j + 1] == DestinoL){
			guardarTablero(juego);

			if (juego.sokoban.tablero[i][j] == DestinoJ){ juego.sokoban.tablero[i][j] = DestinoL; }
			else juego.sokoban.tablero[i][j] = Libre;

			if (juego.sokoban.tablero[i][j + 1] == DestinoL){ juego.sokoban.tablero[i][j + 1] = DestinoJ; }
			else juego.sokoban.tablero[i][j + 1] = Jugador;

			juego.sokoban.filaJ = i;
			juego.sokoban.columnaJ = j + 1;
			juego.numMovimientos++;
		}
		else if ((juego.sokoban.tablero[i][j + 1] == Caja || juego.sokoban.tablero[i][j + 1] == DestinoC) &&
			(juego.sokoban.tablero[i][j + 2] == Libre || juego.sokoban.tablero[i][j + 2] == DestinoL)){
			guardarTablero(juego);

			//Donde estaba
			if (juego.sokoban.tablero[i][j] == DestinoJ){ juego.sokoban.tablero[i][j] = DestinoL; }
			else juego.sokoban.tablero[i][j] = Libre;
			//Donde va
			if (juego.sokoban.tablero[i][j + 1] == DestinoL ||
				juego.sokoban.tablero[i][j + 1] == DestinoC) {
				if (juego.sokoban.tablero[i][j + 1] == DestinoC) juego.sokoban.cajasColocadas--;
				juego.sokoban.tablero[i][j + 1] = DestinoJ;
			}
			else juego.sokoban.tablero[i][j + 1] = Jugador;
			//Lo que mueve
			if (juego.sokoban.tablero[i][j + 2] == DestinoL) {
				juego.sokoban.tablero[i][j + 2] = DestinoC;
				juego.sokoban.cajasColocadas++;
			}
			else juego.sokoban.tablero[i][j + 2] = Caja;

			juego.sokoban.filaJ = i;
			juego.sokoban.columnaJ = j + 1;
			juego.numMovimientos++;
		}
	}
	else if (tecla == Izquierda){

		if (juego.sokoban.tablero[i][j - 1] == Libre || juego.sokoban.tablero[i][j - 1] == DestinoL){
			guardarTablero(juego);


			if (juego.sokoban.tablero[i][j] == DestinoJ){ juego.sokoban.tablero[i][j] = DestinoL; }
			else juego.sokoban.tablero[i][j] = Libre;

			if (juego.sokoban.tablero[i][j - 1] == DestinoL){ juego.sokoban.tablero[i][j - 1] = DestinoJ; }
			else juego.sokoban.tablero[i][j - 1] = Jugador;

			juego.sokoban.filaJ = i;
			juego.sokoban.columnaJ = j - 1;
			juego.numMovimientos++;
		}
		else if ((juego.sokoban.tablero[i][j - 1] == Caja || juego.sokoban.tablero[i][j - 1] == DestinoC) &&
			(juego.sokoban.tablero[i][j - 2] == Libre || juego.sokoban.tablero[i][j - 2] == DestinoL)){
			guardarTablero(juego);


			//Donde estaba
			if (juego.sokoban.tablero[i][j] == DestinoJ){ juego.sokoban.tablero[i][j] = DestinoL; }
			else juego.sokoban.tablero[i][j] = Libre;
			//Donde va
			if (juego.sokoban.tablero[i][j - 1] == DestinoL ||
				juego.sokoban.tablero[i][j - 1] == DestinoC) {
				if (juego.sokoban.tablero[i][j - 1] == DestinoC) juego.sokoban.cajasColocadas--;
				juego.sokoban.tablero[i][j - 1] = DestinoJ;
			}
			else juego.sokoban.tablero[i][j - 1] = Jugador;
			//Lo que mueve
			if (juego.sokoban.tablero[i][j - 2] == DestinoL) {
				juego.sokoban.tablero[i][j - 2] = DestinoC;
				juego.sokoban.cajasColocadas++;
			}
			else juego.sokoban.tablero[i][j - 2] = Caja;

			juego.sokoban.filaJ = i;
			juego.sokoban.columnaJ = j - 1;
			juego.numMovimientos++;
		}
	}

	else if (tecla == Deshacer){
		if (deshacerMovimiento(juego)){
			juego.numMovimientos--;
			juego.historia.cont--;
			for (int i = 0; i < juego.sokoban.nFilas; i++){
				for (int j = 0; j < juego.sokoban.nColumnas; j++){
					juego.sokoban.tablero[i][j] = juego.historia.newTablero[juego.historia.cont].tablero[i][j];
				}
			}
			juego.sokoban = juego.historia.newTablero[juego.historia.cont];
		}
		else cout << "No se puede desahacer el movimiento" << endl;
	}
}

bool deshacerMovimiento(tJuego &juego){
	return (juego.numMovimientos > 0 && juego.historia.cont > 0);
}

void guardarTablero(tJuego &juego){

	if (juego.historia.cont >= MAXH){
		for (int v = 0; v < MAXH - 1; v++){
			for (int i = 0; i < juego.sokoban.nFilas; i++){
				for (int j = 0; j < juego.sokoban.nColumnas; j++){
					juego.historia.newTablero[v].tablero[i][j] = juego.historia.newTablero[v + 1].tablero[i][j];
				}
			}
			juego.historia.newTablero[v] = juego.historia.newTablero[v + 1];
		}
		juego.historia.cont = 9;
	}


	for (int i = 0; i < juego.sokoban.nFilas; i++){
		for (int j = 0; j < juego.sokoban.nColumnas; j++){
			juego.historia.newTablero[juego.historia.cont].tablero[i][j] = juego.sokoban.tablero[i][j];
		}
	}
	juego.historia.newTablero[juego.historia.cont] = juego.sokoban;
	juego.historia.cont++;

}

bool bloqueado(const tJuego &juego){
	bool bloqueado = false;
	int cajasComprobar = juego.sokoban.numCajas - juego.sokoban.cajasColocadas;
	int i = 0, j;

	while( i < juego.sokoban.nFilas && cajasComprobar != 0 && !bloqueado){
		j = 0;
		while (j < juego.sokoban.nColumnas && cajasComprobar != 0 && !bloqueado){
			if (juego.sokoban.tablero[i][j] == Caja){
				//Compruebo que esta en una esquina
				if (juego.sokoban.tablero[i][j - 1] == Muro && (juego.sokoban.tablero[i + 1][j] == Muro
					|| juego.sokoban.tablero[i - 1][j] == Muro)){
					bloqueado = true;
				}
				else if (juego.sokoban.tablero[i][j + 1] == Muro && (juego.sokoban.tablero[i + 1][j] == Muro
					|| juego.sokoban.tablero[i - 1][j] == Muro)){
					bloqueado = true;
					}
				//Si no estuviera en una esquina comprobamos la siguiente caja
				else cajasComprobar--;

				}
			else j++;
		}
		i++;
	}
	return bloqueado;
}

bool ganado(tInfo &info, tJuego &juego){
	bool ganado = (juego.sokoban.cajasColocadas == juego.sokoban.numCajas);
	bool existe = false;
	if (ganado){
		for (int i = 0; i < info.numP_resueltas; i++){
			if (juego.nFichero == info.exitos[i].nombre && juego.nivel == info.exitos[i].level && juego.numMovimientos < info.exitos[i].numMov){
				info.exitos[i].numMov = juego.numMovimientos;
				existe = true;
			}
		}

		if (!existe){
			info.exitos[info.numP_resueltas].nombre = juego.nFichero;
			info.exitos[info.numP_resueltas].level = juego.nivel;
			info.exitos[info.numP_resueltas].numMov = juego.numMovimientos;
			info.numP_resueltas++;
		}
	}

	return ganado;
}

void jugarPartida(tInfo &info, tJuego &juego){
	
	if (cargarJuego(juego)){
		dibujar(juego);
		tTecla tecla = leerTecla();
		while (tecla != Salir){
			hacerMovimiento(juego, tecla);
			dibujar(juego);
			if (bloqueado(juego)){
				cout << "Te has quedado bloqueado." << endl;
			}
			else if (ganado(info, juego)){
				cout << "Felicidades!!!" << endl;
			}
			tecla = leerTecla();
		}
	}

	else cout << "No se ha podido cargar el fichero exitosamente o no existe el nivel. ";
	cout << "Hasta otra!";
}

void mostrarPartida(const tInfo &info){
	cout << "  Tableros derritados por " << info.nombreJ << endl;
	cout << left << setw(15) << "Fichero" << left << setw(10) << "Nivel" << left << setw(11) << "movimientos" << endl;

	for (int i = 0; i < info.numP_resueltas; i++){
		cout << setw(20) << info.exitos[i].nombre << setw(10) << info.exitos[i].level 
			<< setw(11) << info.exitos[i].numMov << endl;
	}
	system("pause");
}

void menu(tInfo &info, tJuego &juego){
	int op;

	do{

		do{
			system("cls");
			cout << "1. Jugar partida. " << endl << "2. Mostrar partidas ganadas. " << endl
				<< "0. Terminar. " << endl << "Introduce una opcion: ";
			cin >> op;

		} while (op < 0 || op > 2);

		
		if (op == 1){
			jugarPartida(info, juego);
		}
		else if (op == 2){
			ordenarPartidas(info);
			mostrarPartida(info);
		}

		
	} while (op != 0);

}

void guardarPartida(const tInfo &info){
	ofstream fichero;
	fichero.open(info.nombreJ);
	for (int i = 0; i < info.numP_resueltas; i++){
		fichero << info.exitos[i].nombre << info.exitos[i].level << info.exitos[i].numMov << endl;
	}
	fichero << "XXX";
}

void ordenarPartidas(tInfo &info){
	int i = 1;
	tPartida aux;
	while (i < info.numP_resueltas){
		if (info.exitos[i].nombre <= info.exitos[i - 1].nombre){
			for (int j = i; j > 0; j--){
				if (info.exitos[j].nombre < info.exitos[j - 1].nombre){
					aux = info.exitos[j];
					info.exitos[j] = info.exitos[j - 1];
					info.exitos[j - 1] = aux;
				}
				else if (info.exitos[j].nombre == info.exitos[j - 1].nombre && info.exitos[j].level < info.exitos[j - 1].level){
					aux = info.exitos[j];
					info.exitos[j] = info.exitos[j - 1];
					info.exitos[j - 1] = aux;
				}
			}
		}
			i++;
	}

}


int main(){
	tInfo info;
	ifstream fichero;
	tJuego juego;
	string centinela;

	info.numP_resueltas = 0;
	juego.historia.cont = 0;
	juego.sokoban.cajasColocadas = 0;

	cout << "Introduce tu nombre: ";
	cin >> info.nombreJ;
	cout << "Bienvenido " << info.nombreJ << endl;
	system("pause");

	//Para no seguir poniedno .txt en todas partes.
	info.nombreJ += ".txt";
	fichero.open(info.nombreJ);
	if (fichero.is_open()){

		fichero >> centinela;
		while (centinela != "XXX"){
			info.exitos[info.numP_resueltas].nombre = centinela;
			fichero >> info.exitos[info.numP_resueltas].level;
			fichero >> info.exitos[info.numP_resueltas].numMov;
			info.numP_resueltas++;
			fichero >> centinela;
		}
	}
	else{
		cout << "No se encontro la informacion del jugador. Se creara nueva." << endl;
		system("pause");
	}
	system("cls");

	menu(info, juego);

	ordenarPartidas(info);
	guardarPartida(info);

	
	
	return 0;
}