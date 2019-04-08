#include "tetris.h"
#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>

std::mutex mx;
std::mutex mxd;
std::condition_variable cv;
std::vector<tetrisPart> mapa;
std::vector<tetrisPart> pacMan;
std::vector<tetrisPart> karma;
std::vector<tetrisPart> ghost;
std::vector<tetrisPart> specialKarma;

bool gra=true;
int niesmiertelny=0;

tetrisPart::tetrisPart(int col,int row)
{
	x=col;
	y=row;	
}

void tetrisClass::createPacMan()
{
	mxd.lock();
	pacMan.push_back(tetrisPart(24,75));
	move(24,75);
	addch('X');
	refresh();
	mxd.unlock();
	while(gra)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		std::unique_lock<std::mutex> lock(mx);
		if(gra)
			tetrisClass::movePac();
		cv.notify_all();
	}
}

int tetrisClass::getPacX()
{
	return pacMan[0].x;
}

int tetrisClass::getPacY()
{
	return pacMan[0].y;
}

void tetrisClass::movePac()
{
	int kierunek;
	kierunek= ( std::rand() %4 ) ;
	if(niesmiertelny>0) 
	{
		niesmiertelny--;
	}
	if(collision(kierunek))
	{
		if(kierunek==0)
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
		else if(kierunek==1)
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
		else if(kierunek==2)
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
		else if(kierunek==3)
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

bool tetrisClass::collision(int k)
{
	int x,y;
	x=pacMan[0].x;
	y=pacMan[0].y;
	
	if(k==0)
		x++;
	else if(k==1)
		x--;
	else if(k==2)
		y++;
	else if(k==3)
		y--;

	for(int i=0;i<mapa.size();i++)
		if((x==mapa[i].x && y==mapa[i].y) || x<1 || x>49 || y<1 || y>149)
			return false;
	return true;
}

void tetrisClass::createBadGhosts()
{
	mxd.lock();
	
	ghost.push_back(tetrisPart(3,3));
	move(3,3);
	addch('B');

	ghost.push_back(tetrisPart(3,70));
	move(3,70);
	addch('B');

	ghost.push_back(tetrisPart(3,135));
	move(3,135);
	addch('B');

	ghost.push_back(tetrisPart(45,3));
	move(45,3);
	addch('B');

	ghost.push_back(tetrisPart(45,70));
	move(45,70);
	addch('B');


	ghost.push_back(tetrisPart(45,135));
	move(45,135);
	addch('B');

	refresh();
	mxd.unlock();
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	while(gra)
	{
		std::unique_lock<std::mutex> lock(mx);
		cv.wait(lock);
		if(gra)
			tetrisClass::moveGhost();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void tetrisClass::moveGhost()
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

int tetrisClass::ghostCollision(int k,int i)
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

void tetrisClass::createMap()
{
	

	//maksymalne height - 49
	//minimalne height - 1
	
	//maksymalne width - 150
	//minimalne width - 1
	mxd.lock();

	for(int i=1;i<50;i++)
		for(int j=1;j<150;j++)
		{
			karma.push_back(tetrisPart(i,j));
			move(i,j);
			addch('.');
		}


	//move(height,width)

	//pion
	for(int i=1;i<15;i++)
	{
		mapa.push_back(tetrisPart(i,15));
		move(i,15);
		addch('@');
	}
	
	for(int i=20;i<30;i++)
	{
		mapa.push_back(tetrisPart(i,15));
		move(i,15);
		addch('@');
	}

	for(int i=35;i<45;i++)
	{
		mapa.push_back(tetrisPart(i,15));
		move(i,15);
		addch('@');
	}

	for(int i=5;i<10;i++)
	{
		mapa.push_back(tetrisPart(i,30));
		move(i,30);
		addch('@');
	}
	
	for(int i=15;i<20;i++)
	{
		mapa.push_back(tetrisPart(i,30));
		move(i,30);
		addch('@');
	}

	for(int i=30;i<40;i++)
	{
		mapa.push_back(tetrisPart(i,30));
		move(i,30);
		addch('@');
	}

	for(int i=1;i<26;i++)
	{
		mapa.push_back(tetrisPart(i,45));
		move(i,45);
		addch('@');
	}

	for(int i=30;i<=49;i++)
	{
		mapa.push_back(tetrisPart(i,45));
		move(i,45);
		addch('@');
	}

	for(int i=5;i<10;i++)
	{
		mapa.push_back(tetrisPart(i,60));
		move(i,60);
		addch('@');
	}

	for(int i=20;i<35;i++)
	{
		mapa.push_back(tetrisPart(i,60));
		move(i,60);
		addch('@');
	}

	for(int i=45;i<=49;i++)
	{
		mapa.push_back(tetrisPart(i,60));
		move(i,60);
		addch('@');
	}

	for(int i=15;i<20;i++)
	{
		mapa.push_back(tetrisPart(i,75));
		move(i,75);
		addch('@');
	}

	for(int i=30;i<45;i++)
	{
		mapa.push_back(tetrisPart(i,75));
		move(i,75);
		addch('@');
	}

	for(int i=10;i<15;i++)
	{
		mapa.push_back(tetrisPart(i,90));
		move(i,90);
		addch('@');
	}

	for(int i=25;i<30;i++)
	{
		mapa.push_back(tetrisPart(i,90));
		move(i,90);
		addch('@');
	}

	for(int i=40;i<50;i++)
	{
		mapa.push_back(tetrisPart(i,90));
		move(i,90);
		addch('@');
	}

	for(int i=1;i<5;i++)
	{
		mapa.push_back(tetrisPart(i,105));
		move(i,105);
		addch('@');
	}

	for(int i=15;i<26;i++)
	{
		mapa.push_back(tetrisPart(i,105));
		move(i,105);
		addch('@');
	}

	for(int i=30;i<36;i++)
	{
		mapa.push_back(tetrisPart(i,105));
		move(i,105);
		addch('@');
	}

	for(int i=20;i<35;i++)
	{
		mapa.push_back(tetrisPart(i,120));
		move(i,120);
		addch('@');
	}

	for(int i=5;i<30;i++)
	{
		mapa.push_back(tetrisPart(i,135));
		move(i,135);
		addch('@');
	}
	


	
	//poziom
	for(int i=1;i<15;i++)
	{
		mapa.push_back(tetrisPart(35,i));
		move(35,i);
		addch('@');
	}

	for(int i=15;i<30;i++)
	{
		mapa.push_back(tetrisPart(45,i));
		move(45,i);
		addch('@');
	}

	for(int i=15;i<30;i++)
	{
		mapa.push_back(tetrisPart(30,i));
		move(30,i);
		addch('@');
	}

	for(int i=15;i<45;i++)
	{
		mapa.push_back(tetrisPart(25,i));
		move(25,i);
		addch('@');
	}

	for(int i=15;i<30;i++)
	{
		mapa.push_back(tetrisPart(15,i));
		move(15,i);
		addch('@');
	}

	for(int i=30;i<45;i++)
	{
		mapa.push_back(tetrisPart(10,i));
		move(10,i);
		addch('@');
	}

	for(int i=45;i<75;i++)
	{
		mapa.push_back(tetrisPart(15,i));
		move(15,i);
		addch('@');
	}

	for(int i=90;i<105;i++)
	{
		mapa.push_back(tetrisPart(15,i));
		move(15,i);
		addch('@');
	}

	for(int i=120;i<135;i++)
	{
		mapa.push_back(tetrisPart(15,i));
		move(15,i);
		addch('@');
	}

	for(int i=60;i<135;i++)
	{
		mapa.push_back(tetrisPart(10,i));
		move(10,i);
		addch('@');
	}

	for(int i=60;i<90;i++)
	{
		mapa.push_back(tetrisPart(5,i));
		move(5,i);
		addch('@');
	}

	for(int i=105;i<120;i++)
	{
		mapa.push_back(tetrisPart(5,i));
		move(5,i);
		addch('@');
	}

	for(int i=75;i<90;i++)
	{
		mapa.push_back(tetrisPart(20,i));
		move(20,i);
		addch('@');
	}

	for(int i=60;i<105;i++)
	{
		mapa.push_back(tetrisPart(25,i));
		move(25,i);
		addch('@');
	}

	for(int i=45;i<60;i++)
	{
		mapa.push_back(tetrisPart(30,i));
		move(30,i);
		addch('@');
	}

	for(int i=75;i<105;i++)
	{
		mapa.push_back(tetrisPart(35,i));
		move(35,i);
		addch('@');
	}

	for(int i=120;i<150;i++)
	{
		mapa.push_back(tetrisPart(35,i));
		move(35,i);
		addch('@');
	}

	for(int i=45;i<75;i++)
	{
		mapa.push_back(tetrisPart(40,i));
		move(40,i);
		addch('@');
	}

	for(int i=90;i<135;i++)
	{
		mapa.push_back(tetrisPart(40,i));
		move(40,i);
		addch('@');
	}

	for(int i=105;i<135;i++)
	{
		mapa.push_back(tetrisPart(45,i));
		move(45,i);
		addch('@');
	}

	specialKarma.push_back(tetrisPart(3,10));
	move(3,10);
	addch('*');

	specialKarma.push_back(tetrisPart(13,20));
	move(13,20);
	addch('*');
		
	specialKarma.push_back(tetrisPart(23,40));
	move(23,40);
	addch('*');
	
	specialKarma.push_back(tetrisPart(19,61));
	move(19,61);
	addch('*');
	
	refresh();
	mxd.unlock();
}