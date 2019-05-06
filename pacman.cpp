#include "pacman.h"
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>

std::mutex mx;
std::mutex mxd;
std::condition_variable cv;
std::vector<pacmanPart> mapa;
std::vector<pacmanPart> pacMan;
std::vector<pacmanPart> karma;
std::vector<pacmanPart> ghost;
std::vector<pacmanPart> specialKarma;

bool gra=true;
int niesmiertelny=0;

pacmanPart::pacmanPart(int col,int row) //konstruktor
{
	x=col;
	y=row;	
}

void pacmanClass::createPacMan()
{
	mxd.lock();
	pacMan.push_back(pacmanPart(24,75)); // na poczatku pacman to X
	move(24,75);
	addch('X');
	refresh();
	mxd.unlock();
	while(gra)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::unique_lock<std::mutex> lock(mx);
		if(gra)
			pacmanClass::movePac();
		cv.notify_all();
	}
}

int pacmanClass::getPacX()
{
	return pacMan[0].x;
}

int pacmanClass::getPacY()
{
	return pacMan[0].y;
}

void pacmanClass::movePac()
{
	int kierunek = getch()%4;	
	if(niesmiertelny>0) 
		niesmiertelny--;
	
	if(collision(kierunek))
	{
		if(kierunek==2)
		{
			move(pacMan[0].x+1,pacMan[0].y);
			printw("P");
			move(pacMan[0].x,pacMan[0].y);
			printw(" ");
			pacMan[0].x=pacMan[0].x+1;
			//zjedzenie karmy
			for(int roz=0;roz<karma.size();roz++)
				if(karma[roz].x == pacMan[0].x && karma[roz].y == pacMan[0].y)
					karma.erase(karma.begin()+roz);
			
			for(int roz=0;roz<specialKarma.size();roz++)
				if(specialKarma[roz].x == pacMan[0].x && specialKarma[roz].y == pacMan[0].y)
				{
					specialKarma.erase(karma.begin()+roz);
					niesmiertelny=20;
				}		

			//kolizja z duchem
			if(niesmiertelny==0)
			{
				for(int i=0;i<6;i++)
					if(pacMan[0].x == ghost[i].x && pacMan[0].y == ghost[i].y)
					{
						move(24,75);
						printw("KONIEC GRY - WYGRALY DUCHY :)");
						gra=false;
					}
			}
			refresh();
		}
		else if(kierunek==3)
		{
			move(pacMan[0].x-1,pacMan[0].y);
			printw("P");
			move(pacMan[0].x,pacMan[0].y);
			printw(" ");
			pacMan[0].x=pacMan[0].x-1;
			for(int roz=0;roz<karma.size();roz++)
				if(karma[roz].x == pacMan[0].x && karma[roz].y == pacMan[0].y)
					karma.erase(karma.begin()+roz);
			refresh();
		}
		else if(kierunek==1)
		{
			move(pacMan[0].x,pacMan[0].y+1);
			printw("P");
			move(pacMan[0].x,pacMan[0].y);
			printw(" ");
			pacMan[0].y=pacMan[0].y+1;
			for(int roz=0;roz<karma.size();roz++)
				if(karma[roz].x == pacMan[0].x && karma[roz].y == pacMan[0].y)
					karma.erase(karma.begin()+roz);
			refresh();
		}
		else if(kierunek==0)
		{
			move(pacMan[0].x,pacMan[0].y-1);
			printw("P");
			move(pacMan[0].x,pacMan[0].y);
			printw(" ");
			pacMan[0].y=pacMan[0].y-1;
			for(int roz=0;roz<karma.size();roz++)
				if(karma[roz].x == pacMan[0].x && karma[roz].y == pacMan[0].y)
					karma.erase(karma.begin()+roz);
			refresh();
		}
	}
}

bool pacmanClass::collision(int k)
{
	int x,y;
	x=pacMan[0].x;
	y=pacMan[0].y;
	
	if(k==2)
		x++;
	else if(k==3)
		x--;
	else if(k==1)
		y++;
	else if(k==0)
		y--;

	for(int i=0;i<mapa.size();i++)
		if((x==mapa[i].x && y==mapa[i].y) || x<1 || x>49 || y<1 || y>149)
			return false;
	return true;
}

void pacmanClass::createBadGhosts()
{
	mxd.lock();
	ghost.push_back(pacmanPart(3,3));
	move(3,3);
	addch('B');

	ghost.push_back(pacmanPart(3,70));
	move(3,70);
	addch('B');

	ghost.push_back(pacmanPart(23,25));
	move(23,25);
	addch('B');

	ghost.push_back(pacmanPart(44,3));
	move(44,3);
	addch('B');

	ghost.push_back(pacmanPart(44,70));
	move(44,70);
	addch('B');

	ghost.push_back(pacmanPart(30,90));
	move(30,90);
	addch('B');

	refresh();
	mxd.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	while(gra)
	{
		std::unique_lock<std::mutex> lock(mx);
		cv.wait(lock);
		if(gra)
			pacmanClass::moveGhost();
		std::this_thread::sleep_for(std::chrono::milliseconds(80));
	}
}

void pacmanClass::moveGhost()
{
	int kierunek,wybor;
	if(niesmiertelny==0)
	{
		for(int duch=0;duch<6;duch++)
		{
			kierunek= ( std::rand() %4 );
			wybor=ghostCollision(kierunek,duch);
			if(wybor==2)
			{
				if(kierunek==0)
				{
					move(ghost[duch].x+1,ghost[duch].y);
					printw("B");
					int rozmiar=karma.size();
					int rozmiar2=specialKarma.size();
					int xx=ghost[duch].x;
					int yy=ghost[duch].y;
					bool gwiazda=true;
					for(int roz=0;roz<rozmiar;roz++)
						if(karma[roz].x==xx && karma[roz].y==yy)
						{
							move(ghost[duch].x,ghost[duch].y);
							printw(".");
							gwiazda=false;
						}
					for(int roz=0;roz<rozmiar2;roz++)
						if(specialKarma[roz].x==xx && specialKarma[roz].y==yy)
						{
							move(ghost[duch].x,ghost[duch].y);
							printw("*");
							gwiazda=false;
						}

					if(gwiazda)
					{
						move(ghost[duch].x,ghost[duch].y);
						printw(" ");
					}
					ghost[duch].x=ghost[duch].x+1;
					refresh();
				}
				else if(kierunek==1)
				{
					move(ghost[duch].x-1,ghost[duch].y);
					printw("B");
					int rozmiar=karma.size();
					int rozmiar2=specialKarma.size();
					int xx=ghost[duch].x;
					int yy=ghost[duch].y;
					bool gwiazda=true;
					for(int roz=0;roz<rozmiar;roz++)
						if(karma[roz].x==xx && karma[roz].y==yy)
						{
							move(ghost[duch].x,ghost[duch].y);
							printw(".");
							gwiazda=false;
						}
					for(int roz=0;roz<rozmiar2;roz++)
						if(specialKarma[roz].x==xx && specialKarma[roz].y==yy)
						{
							move(ghost[duch].x,ghost[duch].y);
							printw("*");
							gwiazda=false;
						}
					if(gwiazda)
					{
						move(ghost[duch].x,ghost[duch].y);
						printw(" ");
					}
					ghost[duch].x=ghost[duch].x-1;
					refresh();

				}
				else if(kierunek==2)
				{
					move(ghost[duch].x,ghost[duch].y+1);
					printw("B");
					int rozmiar=karma.size();
					int rozmiar2=specialKarma.size();
					int xx=ghost[duch].x;
					int yy=ghost[duch].y;
					bool gwiazda=true;
					for(int roz=0;roz<rozmiar;roz++)
						if(karma[roz].x==xx && karma[roz].y==yy)
						{
							move(ghost[duch].x,ghost[duch].y);
							printw(".");
							gwiazda=false;
						}
					for(int roz=0;roz<rozmiar2;roz++)
						if(specialKarma[roz].x==xx && specialKarma[roz].y==yy)
						{
							move(ghost[duch].x,ghost[duch].y);
							printw("*");
							gwiazda=false;
						}
					if(gwiazda)
					{
						move(ghost[duch].x,ghost[duch].y);
						printw(" ");
					}
					ghost[duch].y=ghost[duch].y+1;
					refresh();

				}
				else if(kierunek==3)
				{
					move(ghost[duch].x,ghost[duch].y-1);
					printw("B");
					int rozmiar=karma.size();
					int rozmiar2=specialKarma.size();
					int xx=ghost[duch].x;
					int yy=ghost[duch].y;
					bool gwiazda=true;
					for(int roz=0;roz<rozmiar;roz++)
					{
						if(karma[roz].x==xx && karma[roz].y==yy)
						{
							move(ghost[duch].x,ghost[duch].y);
							printw(".");
							gwiazda=false;
						}
					}
					for(int roz=0;roz<rozmiar2;roz++)
						if(specialKarma[roz].x==xx && specialKarma[roz].y==yy)
						{
							move(ghost[duch].x,ghost[duch].y);
							printw("*");
							gwiazda=false;
						}
					if(gwiazda)
					{
						move(ghost[duch].x,ghost[duch].y);
						printw(" ");
					}
					ghost[duch].y=ghost[duch].y-1;
					refresh();
				}
			} 
			else if(wybor==1)
			{
				
				for(int i=1;i<50;i++)
					for(int j=1;j<150;j++)
					{
						move(i,j);
						addch(' ');
					}
				move(24,75);
				printw("KONIEC GRY - WYGRALY DUCHY :)");
				gra=false;
				duch=6;
			}
		}
	}
}

int pacmanClass::ghostCollision(int k,int i)
{
	int x,y;
	int zwroconaWartosc;
	x=ghost[i].x;
	y=ghost[i].y;
	if(k==0)
		x++;
	else if(k==1)
		x--;
	else if(k==2)
		y++;
	else if(k==3)
		y--;
	
	int mapax,mapay;
	int pacX,pacY;
	pacX=getPacX();
	pacY=getPacY();

	for(int nr=0;nr<mapa.size();nr++)
	{
		mapax = mapa[nr].x;
		mapay = mapa[nr].y;

		if(x<1 || x>49 || y<1 || y>149)
		{
			zwroconaWartosc=0;
			return zwroconaWartosc;
		} 
		else if(x==mapax && y==mapay)
		{
			zwroconaWartosc=0;
			return zwroconaWartosc;
		} 
		else if (x==pacX && y==pacY)
		{
			zwroconaWartosc=1;
			return zwroconaWartosc;
		}
	}

	zwroconaWartosc=2;
	return zwroconaWartosc;
}

void pacmanClass::createMap()
{
	int q = 11;		//ilosc otwartych wierszy
	int e = 31;		//ilosc otwartych kolumn
	int mheight = (q*4) + 1;
	int mwidth = (e*4) + 1;
	int wholes = 20;	//szansa na wystapienie dziury w murze 1/x

	mxd.lock();

	for(int i=2;i<mheight;i++) //zerowanie ekranu
		for(int j=2;j<mwidth;j++)
		{
			karma.push_back(pacmanPart(i,j));
			move(i,j);
			addch('.');
		}

	for(int i=1;i<=mheight;i++) // rysowanie ram pion pelnych
	{
		mapa.push_back(pacmanPart(i,1));
		move(i,1);
		addch('@');

		mapa.push_back(pacmanPart(i,mwidth));
		move(i,mwidth);
		addch('@');
	}

	for(int i = 2;i<mwidth;i++) // rysowanie ram poz bez rogow
	{
		mapa.push_back(pacmanPart(1,i));
		move(1,i);
		addch('@');

		mapa.push_back(pacmanPart(mheight,i));
		move(mheight,i);
		addch('@');
	}

	int temp_min = (mwidth-5) / 8;
	if(mwidth > mheight) temp_min = (mheight-5) /8;

	for(int i = 0; i < temp_min; i++) // sciany pion pelne z możliwym przejsciem
	{
		for(int j = ((i*4)+5); j<(mheight-(i*4)-3); j++)
		{
			if(rand()%wholes == 0 && j<(mheight-(i*4)-7)) j+=2;
			else{
				mapa.push_back(pacmanPart(j,((i*4) + 5)));
				move(j,((i*4) + 5));
				addch('@');
			}
		}
		for(int j = ((i*4)+5); j<(mheight-(i*4)-3); j++)
		{
			if(rand()%wholes == 0 && j<(mheight-(i*4)-7)) j+=2;
			else{
				mapa.push_back(pacmanPart(j,mwidth-((i*4) + 4)));
				move(j,mwidth-((i*4) + 4));
				addch('@');
			}
		}
	}

	for(int i = 0; i < temp_min; i++) // sciany poz bez rogow z mozliwym przejsciem
	{
		for(int j = ((i*4)+6); j < (mwidth-(i*4)-4); j++)
		{
			if(rand()%wholes == 0 && j<(mwidth-(i*4)-8)) j+=2;
			else{
				mapa.push_back(pacmanPart(((i*4)+5),j));
				move(((i*4)+5),j);
				addch('@');
			}
		}
		for(int j = ((i*4)+6); j < (mwidth-(i*4)-4); j++)
		{
			if(rand()%wholes == 0 && j<(mwidth-(i*4)-8)) j+=2;
			else{
				mapa.push_back(pacmanPart(mheight-((i*4)+4),j));
				move(mheight-((i*4)+4),j);
				addch('@');
			}
		}
	}

	specialKarma.push_back(pacmanPart(31,10));
	move(31,10);
	addch('*');

	specialKarma.push_back(pacmanPart(27,20));
	move(27,20);
	addch('*');
		
	specialKarma.push_back(pacmanPart(23,40));
	move(23,40);
	addch('*');
	
	specialKarma.push_back(pacmanPart(34,60));
	move(34,60);
	addch('*');
	
	refresh();
	mxd.unlock();

}
