# generator_labiryntu
**konsolowy program w C++ pozwalający na generowanie labiryntu**

Pliki wchodzące w skład projektu:

- **GraphAsMatrix.cpp** (plik zawierający funkcję **main** wykorzystującą funkcjonalność wszystkich klas)

- **GraphAsMatrix.hpp** (zawiera klasę reprezentująca wykorzystywany w projekcie graf)
- **Iterator.hpp** (bazowa klasa iteratorów wykorzystywanych w grafie)
- **Vertex.hpp** (klasa wierzchołka wykorzystywana przez graf)
- **Edge.hpp** (klasa krawędzi wykorzystywana przez graf)

- **PathFiller.hpp** (klasa odpowiedzialna za wygenerowanie grafu, który może następnie zostać przełożony na labirynt)
- **Maze.hpp** (klasa implementująca generacje labiryntu w konsoli, na podstawie grafu)

Poniżej przedstawiam omówienie budowy każdej z wykorzystanej klasy (oprócz klas związanych bezpośrednio z implementacją grauf, ponieważ te zostały już omówione przy innej okazji).


###### Maze.hpp

Przewrotnie omówienie projektu rozpocznę od ostatniego jego elementu, czyli od klasy odpowiedzialnej za rysowanie labiryntu na podstawie dostarczonej klasy grafu. Klasa ta została napisana przeze mnie jako pierwsza, ponieważ dobrze zdradza zamysł idący za implementacją koncepcji rozpinania grafu.

Z racji na to, że zdecydowałem się na implementację konsolową, chciałem jakoś urozmaicić graficzną szatę projektu. Do tego celu wykorzystałem **ANSI escape sequences**: https://en.wikipedia.org/wiki/ANSI_escape_code#Colors, pozwalające mi za pomocą specjalnych znaczników zmienić kolory wyświetlanych w terminalu symbolów. Jeśli się nie mylę jest to **wyłącznie unix'owa składnia**. Uruchomienie więc projektu na systemie Windows, może dać więc bardzo nieporządane efekty (zamiast ładnego, kolorowego labiryntu wyświetli się nam na konsoli kilka tysięci niedziałających znaczników).

Rozpalonowanie labiryntu:

Tak naprawdę cała sekwencja wynikowa, czyli nasz labirynt reprezentowana jest w postaci długiego stringa, do jego tymczasowego przeptrzymywania w kodzie wykorzystam więc strumień **stringstream**, który pozwoli mi na łatwe dodawanie do niego elementów. Fakt, że wynik jest ciągłą sekwencją znaków utrudnia znacząco dodawanie do niego elementów grafu tworzących labirynt w porządanej kolejności. Trzeba wymyślić do tego celu zatem rozwiązanie dające nam elastyczność dodawania znaków w dowolnym miejscu ciągu.

