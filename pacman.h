#include <iostream>
#include <vector>
#include <ncurses.h>
#include <cstdlib>
//struktura zawierajaca pojedyncza czesc weza


struct pacmanPart
{
	int x,y;
	pacmanPart(int col,int row);
};

extern std::vector<pacmanPart> mapa;
extern std::vector<pacmanPart> pacMan;
extern std::vector<pacmanPart> karma;
extern std::vector<pacmanPart> ghost;
extern std::vector<pacmanPart> specialKarma;

class pacmanClass
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
