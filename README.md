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

Do dobrego zobrazowania tego co chcemy uzyskać przytoczę poglądowe zjęcie:


![Zrzut ekranu 2022-05-13 o 09 48 17](https://user-images.githubusercontent.com/36136484/168236473-74c3497f-7413-420c-bbf9-daf7302d7747.png)


Wykorzystanemu grafowi odpowiadać będzie dynamicznie alokowana dwuwymiarowa tablicy, której indeksy poszczególnych pól odpowiadają numerom wierzchołków grafu. Każde z takich pól, musi mieć w jakiś sposób określoną "rozdzielczość". Przyjąłem, że każde pole reprezentowane jest przez siatkę znaków o rozmiarach 4x4, która reprezentowana jest przez specjalną strukturę **block**. Jest to prosta struktura składająca się z czterech strumieni stringstream. Można się zastanawiać po co tak komplikować problem i używać tej struktury, zamiast prostego stringstreama lub stringa reprezentującego pojedynczy blok siatki?

Zastanówmy się jak działa nasz algorytm. Przechodząc po kolejnych krawędziach grafu będziemy na bierząco stwierdzać na którym połączeniu siatki powinna znaleźć się ściana, a gdzie może powinno znaleźć się przejście (istnienie krawędzi pomiędzy dwoma wierzchołkami będzie tutaj reprezentować przejście pomiędzy dwoma segmentami siatki). Jak już wspomniałem proces analizy jest dynamiczny i rysując dany blok nie wiemy przykładowo czy fakt istnienia lewej ściany (pola: 00, 01, 02, 03) pozwala nam automatycznie na określenie dalszych pól (np: pól pierwszego rzędzu 10, 20, 30, 40), przecież musimy zdecydować czy od góry blok ten nie jest połączony ścianą!

Sturktura **block** wypełniana jest więc "ściana po ścianie", co równoważne jest z równoczesną edycją wielu wierszy lub wielu kolumn w tym samym czasie. Proces ten mając do dyspozycji tylko jednego stringa, byłby bardzo czasochłonny i problematyczny do implementacji.

przykładowe bloki labiryntu:


![Zrzut ekranu 2022-05-13 o 10 00 32](https://user-images.githubusercontent.com/36136484/168238508-e77dc71a-2da1-4183-acaf-343fdcedfd41.png) &nbsp;&nbsp;![Zrzut ekranu 2022-05-13 o 10 00 50](https://user-images.githubusercontent.com/36136484/168238544-e24cdade-e762-4837-a7d5-8bcd65e4d1eb.png)

*czerwony kolor reprezentuje ściany, natomiast czerwony posadzkę po której możemy się poruszać.

Po przejściu wszystkich krawędzi grafu pozostaje nam sklejenie ze sobą wszystkich bloków. Bloki sklejam w ramach jednej pętli for() przenosząc kolejne wiersze bloków pochodzących z tego samego wiersza struktury do pojedynczego sturmienia.

