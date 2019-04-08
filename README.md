# Systemy Operacyjne - Projekt 3
# Opis Projektu
Zostając przy konwencji gier stworzymy projekt gry Pac-Man. Po planszy poruszać się będzie
tytułowy Pac-Man, którego ścigać będą złe duszki. Nasza postać ma jedno życie. Na mapie
pojawią się również obiekty, które pozwolą (przez pewną ilość ruchów) na zjedzenie
przeciwnika przy ewentualnej kolizji. Pac-Man zwycięży, gdy zje wszystkie białe kulki. Po kolizji
z duchem traci życie.

# Zasoby
Wektor – Pac-Man

Wektor – złe duszki

Wektor – białe kulki

Wektor – kulki mocy

# Wątki
Wątek główny rysujący planszę

Wątek poruszający Pac-Manem

Wątek poruszający duchami

# Kompilacja
g++ -std=c++11 tetris.cpp main.cpp -lncurses -pthread

# Uruchomienie - terminal otwarty na pełne okno
./a.out
