#include "tetris.h"
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
	
	//linie brzegowe
	for(int i=0; i<maxheight-1;i++)
	{
		move(i,0);//przesuwa na wybrane pole
		addch('X');//i drukuje litere
	}
	for(int i=0; i<maxwidth-1;i++)
	{
		move(0,i);
		addch('X');
	}
	for(int i=0; i<maxwidth; i++)
	{
		move(maxheight-1,i);
		addch('X');
	}
	for(int i=0;i<maxheight;i++)
	{
		move(i,maxwidth-1);
		addch('X');
	}

	refresh(); //aktualizacja obrazu
	
	//niezbedne deklaracje
	tetrisClass pacMan,ghosts,mapa;
	std::vector<std::thread> threads;

	threads.push_back(std::thread (&tetrisClass::createMap,&mapa));
	threads.push_back(std::thread (&tetrisClass::createPacMan,&pacMan));
	threads.push_back(std::thread (&tetrisClass::createBadGhosts,&ghosts));

	for(auto& thread : threads)
		thread.join();
	
	nodelay(stdscr,false);// po skonczeniu pracy programu, odblokowana mozliwosc pisania aby moc zakonczyc dzialanie programu
	getch();
	endwin();// resetuje terminal do domyslnego stanu
	return 0;
}
