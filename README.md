# Systemy Operacyjne - Projekt 
# Opis Projektu
Po planszy porusza się tytułowy Pac-Man, którego ścigać będą złe duszki. Nasza postać ma jedno życie. 
Na mapie pojawiają się również obiekty, które pozwolą (przez pewną ilość ruchów) na zjedzenie
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
g++ -std=c++11 pacman.cpp main.cpp -lncurses -pthread

# Uruchomienie w terminalu otwartym na pełne okno
./a.out
