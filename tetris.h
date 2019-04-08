#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
//struktura zawierajaca pojedyncza czesc weza

struct tetrisPart
{
	int x,y;
	tetrisPart(int col,int row);
};

extern std::vector<tetrisPart> mapa;
extern std::vector<tetrisPart> pacMan;
extern std::vector<tetrisPart> karma;
extern std::vector<tetrisPart> ghost;
extern std::vector<tetrisPart> specialKarma;

class tetrisClass
{
public:

	void createMap();
	bool collision(int);
	void movePac();
	void createPacMan();
	int getPacX();
	int getPacY();
	
	void createBadGhosts();
	void moveGhost();
	int ghostCollision(int,int);
};
