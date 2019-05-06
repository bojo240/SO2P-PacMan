#include "pacman.h"
#include <thread>
#include <vector>
using namespace std;

int main()
{	
	initscr(); 	//stworzenie okna
	int maxwidth,maxheight; // wymiary okna konsoli
	nodelay(stdscr,true); // getch nie czeka na nacisniecie przycisku
	keypad(stdscr,true); //obsluga klawiatury w programie a nie shellu
	noecho();// musi byc
	getmaxyx(stdscr,maxheight,maxwidth);// ustawienie wartosci -- nie trzeba przekazywac przez referencje
	srand(time(0)); //pseudolosowosc

	refresh(); //aktualizacja obrazu
	
	//niezbedne deklaracje
	pacmanClass pacMan,ghosts,mapa;
	std::vector<std::thread> threads; //wektor watkow

	threads.push_back(std::thread (&pacmanClass::createMap,&mapa)); //utworzenie i dodatnie watkow do wektora
	threads.push_back(std::thread (&pacmanClass::createPacMan,&pacMan));
	threads.push_back(std::thread (&pacmanClass::createBadGhosts,&ghosts));

	for(auto& thread : threads)
		thread.join();
	
	nodelay(stdscr,false);// po skonczeniu pracy programu, odblokowana mozliwosc pisania aby moc zakonczyc dzialanie programu
	getch();
	endwin();// resetuje terminal do domyslnego stanu
	return 0;
}
