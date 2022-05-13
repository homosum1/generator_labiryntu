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

- **makefile**

Poniżej przedstawiam omówienie budowy każdej z wykorzystanej klasy (oprócz klas związanych bezpośrednio z implementacją grauf, ponieważ te zostały już omówione przy innej okazji).


## Maze.hpp

Przewrotnie omówienie projektu rozpocznę od ostatniego jego elementu, czyli od klasy odpowiedzialnej za rysowanie labiryntu na podstawie dostarczonej klasy grafu. Klasa ta została napisana przeze mnie jako pierwsza, ponieważ dobrze zdradza zamysł idący za implementacją koncepcji rozpinania grafu.

Z racji na to, że zdecydowałem się na implementację konsolową, chciałem jakoś urozmaicić graficzną szatę projektu. Do tego celu wykorzystałem **ANSI escape sequences**: https://en.wikipedia.org/wiki/ANSI_escape_code#Colors, pozwalające mi za pomocą specjalnych znaczników zmienić kolory wyświetlanych w terminalu symbolów. Jeśli się nie mylę jest to **wyłącznie unix'owa składnia**. Uruchomienie więc projektu na systemie Windows, może dać więc bardzo nieporządane efekty (zamiast ładnego, kolorowego labiryntu wyświetli się nam na konsoli kilka tysięci niedziałających znaczników).

Rozpalonowanie labiryntu:

Tak naprawdę cała sekwencja wynikowa, czyli nasz labirynt reprezentowana jest w postaci długiego stringa, do jego tymczasowego przeptrzymywania w kodzie wykorzystam więc strumień **stringstream**, który pozwoli mi na łatwe dodawanie do niego elementów. Fakt, że wynik jest ciągłą sekwencją znaków utrudnia znacząco dodawanie do niego elementów grafu tworzących labirynt w porządanej kolejności. Trzeba wymyślić do tego celu zatem rozwiązanie dające nam elastyczność dodawania znaków w dowolnym miejscu ciągu.

Do dobrego zobrazowania tego co chcemy uzyskać przytoczę poglądowe zjęcie:


![Zrzut ekranu 2022-05-13 o 09 48 17](https://user-images.githubusercontent.com/36136484/168236473-74c3497f-7413-420c-bbf9-daf7302d7747.png)


Wykorzystanemu grafowi odpowiadać będzie dynamicznie alokowana dwuwymiarowa tablicy, której indeksy poszczególnych pól odpowiadają numerom wierzchołków grafu. Każde z takich pól, musi mieć w jakiś sposób określoną "rozdzielczość". Przyjąłem, że każde pole reprezentowane jest przez siatkę znaków o rozmiarach 3x3, która reprezentowana jest przez specjalną strukturę **block**. Jest to prosta struktura składająca się z trzech strumieni stringstream. Można się zastanawiać po co tak komplikować problem i używać tej struktury, zamiast prostego stringstreama lub stringa reprezentującego pojedynczy blok siatki?

Zastanówmy się jak działa nasz algorytm. Przechodząc po kolejnych krawędziach grafu będziemy na bierząco stwierdzać na którym połączeniu siatki powinna znaleźć się ściana, a gdzie może powinno znaleźć się przejście (istnienie krawędzi pomiędzy dwoma wierzchołkami będzie tutaj reprezentować przejście pomiędzy dwoma segmentami siatki). Jak już wspomniałem proces analizy jest dynamiczny i rysując dany blok nie wiemy przykładowo czy fakt istnienia lewej ściany (pola: 00, 01, 02) pozwala nam automatycznie na określenie dalszych pól (np: pól trzeciego rzędzu 10, 20, 30), przecież musimy zdecydować czy od dołu blok ten nie jest połączony ścianą! W tej reprezentacji blok może utworzyć ścianę z lewej strony lub z dołu.

Sturktura **block** wypełniana jest więc "ściana po ścianie", co równoważne jest z równoczesną edycją wielu wierszy lub wielu kolumn w tym samym czasie. Proces ten mając do dyspozycji tylko jednego stringa, byłby bardzo czasochłonny i problematyczny do implementacji.

przykładowe bloki labiryntu:

![Zrzut ekranu 2022-05-13 o 10 29 40](https://user-images.githubusercontent.com/36136484/168243617-a25a3126-210d-4dee-8791-8b7e0fecc66c.png) &nbsp;![Zrzut ekranu 2022-05-13 o 10 30 03](https://user-images.githubusercontent.com/36136484/168243689-35c0e77d-481a-420f-862c-b066cec4364f.png)

*czerwony kolor reprezentuje ściany, natomiast czerwony posadzkę po której możemy się poruszać.

Po przejściu wszystkich krawędzi grafu pozostaje nam sklejenie ze sobą wszystkich bloków. Bloki sklejam w ramach jednej pętli for() przenosząc kolejne wiersze bloków pochodzących z tego samego wiersza struktury do pojedynczego sturmienia.


## PathFiller.hpp

Dobrze, wiemy już jak narysować labirynt na podstawie grafu, jak jednak utworzyć w grafie krawędzie tak, aby odpowiadiały one ścieżce wyglądem odpowiadającej ścieżkę labiryntu? Rozwiązaniem tutaj okaże się lekko zmodyfikowana wersja **algorytmu Kruskala**. Algorytm ten odpowiada za wyznaczenie minimalnego drzewa rozpinającego graf. Rezultatem takiego roziwązanania powinien graf, przypominający ten pokazany poniżej:

![Zrzut ekranu 2022-05-13 o 10 16 24](https://user-images.githubusercontent.com/36136484/168241256-a957a8f3-0519-445d-a65e-b8eba7fc0b8c.png)

Jest tutaj jeden problem. Algorytm Kruskala, zakłada, że dostarczymy mu graf w którym istnieją już krawędzie, ale też krawędzie też będą miały odpowiadające im wagi. Utworzony przez nas graf nie posiada natomiast, ani krawędzi, ani tym bardziej wag które mogłyby im odpowiadać.

Utworzę więc krawędzie, jednak nie wszystkie! takie działanie byłoby bardzo nie wydajne, ponieważ większość krawędzi jaka mogłaby istnieć nie przełoży się na wygląd naszego labiryntu. Rozplanujmy więc, jakie krawędzie należałoby przeznaczyć do analizy. Tak naprawdę w ramach danego wierzchołka interesuje nas wyłącznie utworzenie krawędzi z sąsiadującymi wierzchołkami (jak sąsiadów nie uznajemy wierzchołków znajdujących się na skos od naszego wierzchołka bazowego). Narzucamy jeszcze kilka ograniczeń na wierzchołki skarnej i w rezultacie otrzymujemy porządany schemat:

![Zrzut ekranu 2022-05-13 o 10 31 03](https://user-images.githubusercontent.com/36136484/168244525-f9a4d057-5e3a-44e2-bde9-b25149318846.png)

możemy więc podzielić wierzchołki na 3 kategorie:
- skrajne z lewej strony
- wewnętrzne
- skrajne z prawej strony

Metoda **fillPath()** odpowiada za wypełnienie grafu krawędziami w taki właśnie sposób. Następną metodą jest metoda **randomizeWeights()**, której sama nazwa wskazuje, że będzie ona odpowiedizalna za nadanie krawędziom losowych wag. Korzystam w tym celu z iteratora przechodzącego po krawędziach grafu.

Ostatnią w tej klasie i zarazem najbardziej złożoną metodą jest metoda **createSecondGraph()**, która stanowi implementację mojej wersji algorytmu Kruskala. Na początek towrzę tablicę przechowującą parę **std::pair** w której skład wchodzi wskaźnik do danej krawędzi oraz waga, która została jej przypisana. Następnie tworzę kontener **std::set** nie przyjmujący duplikatów obiektów. Wykorzystuję go do odseparowania duplikatów danych krawędzi (korzystam z grafu nieskierowanego, zatem zarówno krawędź 1->2 jak i 2->1 istnieje w grafie i reprezentuje to samo połączenie w moim układzie labiryntu). Po utworzeniu tabeli sortuję ją na podstawie wag, warto zaznaczyć, że operuję tutaj na wskaźnikach co skutecznie minimalizuje koszt obliczeń. Do sorotowania wykorzystyałem zaimplementowany przeze mnie algorytm **insertion sort** sortujący wskaźniki do krawędzi na podstawie przypisanych im wag.

Teraz pozostaje mi stowrzenie nowego grafu, na podstawie specjalnie wyselekcjonowanych krawędzi. Jak to zrobić? Na początku do każdego wierzchołka przypisuję obiekt **std::set** i umieszczam w nim wierzchołek do którego został on przyłączony, następnie wybieram kolejne krawędzie z posortowanej listy i jeżeli wierzchołki między którymi rozpostarta jest krawędź należą do odzdzielnych kontenerów **std::set** to oznacza to, że dodanie tej krawędzi nie pozowli na utworzenie się cyklu w naszym grafie (bram cyklów w grafie gwarantuje nam ładny spójny wygląd labirytnu). W takiej sytuacji możemy śmiało dodać tą krawędź do nowego grafu, a następnie połączyć sety do których należały wierzchołki składające się na krawędź.


## Maze.hpp

Poniżej zamieszczam kilka przykładów wygenerowanych labiryntów

x: 3 y: 4

![Zrzut ekranu 2022-05-13 o 10 52 56](https://user-images.githubusercontent.com/36136484/168247989-f1da6318-7f6e-40c0-91b0-955d78c888a6.png)

x: 10 y: 10

![Zrzut ekranu 2022-05-13 o 10 53 11](https://user-images.githubusercontent.com/36136484/168248042-a746e811-fd4f-4714-b673-6656f714acd4.png)

x: 15 y: 10

![Zrzut ekranu 2022-05-13 o 10 53 29](https://user-images.githubusercontent.com/36136484/168248090-25d9892e-114e-41ac-824b-86320513cec7.png)

x: 20 y: 20

![Zrzut ekranu 2022-05-13 o 10 54 45](https://user-images.githubusercontent.com/36136484/168248373-d21f2407-829a-4872-a457-3c66aa1d3402.png)

## Kompilacja i uruchomienie projektu

Plik projektu znajduje się w folderze **projekt**. Po pobraniu folderu należy przejść do jego lokalizacji i wykonać polecenie **make** wykonujące za nas kompilację wyspecyfikowaną w pliku **makefile**. Następnie program uruchamiamy poleceniem **./GraphAsMatrix.x** zostaniemy następnie poproszenie przez program o podanie wymiarów labiryntu jaki chcemy wygenerować.

![Zrzut ekranu 2022-05-13 o 10 56 45](https://user-images.githubusercontent.com/36136484/168248766-d06e83c6-9372-4809-a823-ae41d786b644.png)


## Złożoność obliczeniowa 

**n** - liczba wierzchołków grafu

złożoność metody rysującej graf **O(n)**

złożoność metod generujących graf:

- utworzenie krawędzi **O(n)**
- nadanie losowych priorytetów krawędziom **O(n)** (dokładnie **O(an)** - mimo zagdnieżdżonej pętli, wewnętrzna pętla wykona się maksymalnie 4 razy a = około 4)
- utworzenie drugiego grafu **O(n^2)** - zaimplementowanie przeze mnie sortowanie w psotaci **insertion sort** posiada złożoność **O(n^2)**, która ogranicza od góry złożonośc samej części algorytmu odpowiedzialnej za sprawdzanie przynależności do setów oraz ich łączenie. Przy zaimplementowaniu lepszego algorytmu sortowania złożoność czasowa mogłaby być tutaj znacząco ograniczona.
