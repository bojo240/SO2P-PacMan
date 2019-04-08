//Random kom
#include "tetris.h"
#include <thread>
#include <vector>
using namespace std;
// DFDASFDASFDF
int main()
{	
	//stworzenie okna
	initscr();
	int delay,maxwidth,maxheight;
	nodelay(stdscr,true); // getch nie czeka na nacisniecie przycisku
	keypad(stdscr,true);
	noecho();
	curs_set(0);
	getmaxyx(stdscr,maxheight,maxwidth);
	
	delay=110000;
	bool get=false;
	srand(time(0));
	
	//linie brzegowe

	for(int i=0; i<maxheight-1;i++)
	{
		move(i,0);
		addch('X');
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

	refresh();
		
	tetrisClass pacMan,ghosts,mapa;
	std::vector<std::thread> threads;

	threads.push_back(std::thread (&tetrisClass::createMap,&mapa));
	threads.push_back(std::thread (&tetrisClass::createPacMan,&pacMan));
	threads.push_back(std::thread (&tetrisClass::createBadGhosts,&ghosts));

	for(auto& thread : threads)
		thread.join();
	
	nodelay(stdscr,false);
	getch();
	endwin();
	return 0;
}
