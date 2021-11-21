Laboratorium 9                                              2002-04-16
==============
UWAGA!!!!
W przypadku problemów z poleceniam make, które powinno automatycznie kompilować źródła na podstawie ich zależności (tak mamy to zdefiniowane w pliku Makefile ale niestety nie zawsze działa) to proszę kompilację wykonać ręcznie podając zależności wraz z kopilowanym programam np: 

gcc semget.c err.c
Semafory w systemie Unix
------------------------

Literatura:

  * W. R. Stevens 
    "Programowanie zastosowan sieciowych w systemie UNIX", rozdz. 3.10

  * M. J. Bach
    "Budowa systemu operacyjnego UNIX", rozdz. 11.2.3

  * M. Rochkind
    "Programowanie w systemie UNIX dla zaawansowanych", rozdz. 7.6

  * man do poszczegolnych funkcji z sem.h 


1. Opis semaforow i dostepnych operacji.
----------------------------------------

Semafory w systemie Unix (Linux) stanowia istotne uogolnienie klasycznych
semaforow Dijkstry. Zestaw operacji na semaforach jest obszerny.
Operacje sa wykonywane na zestawach semaforow. Semafory nalezace do
danego zestawu sa ponumerowane kolejnymi liczbami naturalnymi,
poczynajac od zera. Mozliwe jest wykonywanie jednoczesnych operacji na
semaforach nalezacych do tego samego zestawu.

Podstawowe funkcje systemowe przeznaczone do obslugi semaforow:
  - semget - utworzenie zestawu semaforow
             (uzyskanie dostepu do istniejacego zestawu semaforow)
  - semop  - operacje na semaforach
  - semctl - operacje administracyjne na zestawie semaforow

Program w C korzystajacy z mechanizmu semaforow musi dolaczyc do
programu pliki naglowkowe:

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


1.1. Utworzenie (uzyskanie dostepu do) zestawu semaforow.
---------------------------------------------------------

Chcac rozpoczac prace z semaforami nalezy utworzyc zestaw semaforow lub
uzyskac dostep do juz istniejacego zestawu. Realizuje to funkcja
systemowa semget:

     int semget (key_t klucz, int lsem, int flagi)

Funkcja semget pobiera klucz, ktory powinien byc dluga liczba calkowita
i przekazuje identyfikator zestawu semaforow (albo -1 w przypadku bledu).

Klucz pelni role 'nazwy' zestawu semaforow. Procesy, ktore chca
korzystac z tego samego zestawu semaforow musza go utworzyc/otworzyc
podajac ten sam klucz.

Identyfikator zestawu semaforow jest obowiazkowym parametrem wszystkich
pozostalych funkcji systemowych dzialajacych na semaforach.
Identyfikator jest widoczny z zewnatrz (por. polecenie 'ipcs -s'). 

Parametr 'lsem' okresla liczbe semaforow w danym zestawie. Otwierajac
istniejacy zestaw semaforow mozna podac dowolna liczbe nie wieksza od
faktycznej liczby semaforow w danym zestawie (w szczegolnosci zero).

Parametr 'flagi' (por. opis funkcji msgget) sluzy do okreslenia:
  - praw dostepu do zestawu semaforow (analogicznie do praw dostepu do
       plikow),
  - sposobu otwarcia zestawu semaforow:
        IPC_CREAT - polecenie utworzenia zestawu semaforow
        IPC_CREAT | IPC_EXCL - utworzenie nieistniejacego zestawu 
                    (porazka jesli zestaw o danym kluczu juz istnieje)


*******************
*** Cwiczenie 1 ***
*******************

1. Skompiluj wszystkie przykladowe programy wywolujac program 'make'
   (nowy uniwersalny Makefile).

2. Wykonaj program semget:
     - utworz zestaw semaforow,
     - za pomoca 'ipcs -s' zobacz jakie zestawy sa juz w systemie,
     - sprobuj otworzyc jakis istniejacy zestaw podajac rozne
       parametry (lsem, flagi),
     - po zakonczeniu usun utworzony zestaw semaforow (ipcrm).

3. Zapoznaj sie z trescia programu 'semget.c'.

4. Przeczytaj 'man semget'.

****************************************************************************


1.2. Operacje na zestawie semaforow.
------------------------------------

Do wykonywania operacji na semaforach (podnoszenie, opuszczanie oraz
oczekiwanie na zerowa wartosc semafora) sluzy funcja systemowa 'semop'.

     int semop (int ident, struct sembuf *operacje, unsigned loper)

Znaczenie parametrow:
 * ident - identyfikator zestawu semaforow (przekazany przez 'semget')
 * loper - liczba operacji do jednoczesnego wykonania na semaforach
           nalezacych do danego zestawu,
           a rownoczesnie liczba elementow tablicy 'operacje',
           definiujacych operacje na poszczegolnych semaforach zestawu

Wszystkie operacje okreslone w tablicy 'operacje' beda wykonane na
podanych semaforach jako jedna niepodzielna operacja.

Struktura 'sembuf' (zdefiniowana w pliku 'sys/sem.h') jest nastepujaca:

struct sembuf {
  short int sem_num;		/* semaphore number */
  short int sem_op;		/* semaphore operation */
  short int sem_flg;		/* operation flag */
};

Opis jednej operacji obejmuje:
  * numer semafora w zestawie ('sem_num')
  * rodzaj operacji ('sem_op'), bedacy liczba calkowita, majacy
    nastepujace znaczenie:
     - wartosc dodatnia: zwiekszenie wartosci semafora (ewentualne
       uwolnienie czekajacych procesow),
     - wartosc ujemna: zadanie zmniejszenia wartosci semafora,
     - zero: sprawdzenie i ewentualne oczekiwanie az wartoscia semafora
       bedzie zero;
  * opcje ('sem_flg'), na przyklad:
     - IPC_NOWAIT - operacja nieblokujaca, tzn. jesli operacja nie moze
          byc wykonana natychmiast, to 'semop' natychmiast sie konczy
          (z wartoscia -1), a zmiennej 'errno' zostaje nadana wartosc EAGAIN,
     - SEM_UNDO - jesli operacja zmienia wartosc semafora, zmiana ta zostanie
                  zniwelowana w momencie zakonczenia wykonywania procesu

Wartoscia funkcji 'semop' jest zero, jesli jej wykonanie zakonczylo sie
sukcesem, a -1 w przypadku bledu.


*******************
*** Cwiczenie 2 ***
*******************

1. Utworz zestaw semaforow (program 'semget').
2. Korzystajac z programu 'semop' wykonaj rozne operacje na zestawie
   semaforow (pojedyncze, zlozone; blokujace, nieblokujace itp.).
3. Zapoznaj sie z trescia programu 'semop.c'.
4. Przeczytaj 'man semop'.
5. Usun wszystkie utworzone semafory.

****************************************************************************


1.3. Operacje administracyjne na zestawie semaforow.
----------------------------------------------------

Do wykonywania dodatkowych operacji na zestawie semaforow (pojedynczych
semaforach zestawu) sluzy funkcja systemowa 'semctl'.

    int semctl (int ident, int semnum, int polecenie, union semun arg));

Znaczenie parametrow:
 * ident  - identyfikator zestawu semaforow (przekazany przez 'semget')
 * semnum - numer semafora, na ktorym ma byc wykonana operacja
 * arg    - parametry polecenia lub wartosci przekazywane przez 'semctl'
            (unia 'semun' jest zdefuniowana w pliku 'semun.h')
 * polecenie -
        GETVAL   - pobranie wartosc semafora
        SETVAL   - nadanie wartosci semaforowi (inicjacja, ale nie tylko) 
        GETPID   - przekazanie identyfikatora procesu, ktory jako
                   ostatni wykonal 'semop' na semaforze 'semnum'
        GETNCNT  - przekazanie liczby procesow czekajacych na
                   zwiekszenie wartosci semafora 'semnum'
        GETZCNT  - przekazanie liczby procesow czekajacych na
                   uzyskanie przez semafor 'semnum' zera
        GETALL   - pobranie wartosci wszystkich semaforow w zestawie
        SETALL   - nadanie wartosci wszystkim semaforom w zestawie
        IPC_STAT - pobranie informacji o zestawie semaforow
                   (m.in. identyfikator procesu, ktory ostatni wykonal
                   operacje na wskazanym semaforze, prawa dostepu,
                   liczba semaforow, czas ostatniej operacji/zmiany)
        IPC_SET  - zmiana niektorych atrybutow zestawu semaforow
                   (identyfikator wlasciciela/grupy, prawa dostepu)
        IPC_RMID - usuniecie zestawu semaforow
                   (obudzenie wszystkich czekajacych procesow - zmienna
                   'errno' ustawiana na EIDRM).

Wartoscia funkcji jest przekazywana wartosc (polecenia GETVAL, GETPID,
GETNCNT, GETZCNT) lub -1 w przypadku bledu, a wowczas zmienna 'errno'
okresla rodzaj bledu. Podstawowe kody bledow:
   EACCES - proces nie ma prawa wykonac podanej operacji
   EINVAL - niepoprawna wartosc (rodzaj polecenia, identyfikator zestawu).


*******************
*** Cwiczenie 3 ***
*******************

1. Utworz zestaw semaforow (program 'semget').
2. Korzystajac z programu 'semctl' nadaj poczatkowe wartosci semaforom w
   zestawie.
3. Wykonaj operacje na semaforach tego zestawu ('semop').
4. Sprawdz stan semaforow ('semctl'), ewentualnie zmien pewne wartosci.
5. Zapoznaj sie z trescia programu 'semop.c'.
6. Przeczytaj 'man semop'.
7. Usun wszystkie utworzone semafory.

****************************************************************************


*****************************************************
*** Cwiczenie 4 - poglebienie wiedzy o semaforach ***
*****************************************************

1. Sprawdz (o ile jeszcze tego nie wiesz) jakie wartosci poczatkowe sa
   nadawane semaforom.
2. Rozpoznaj strategie obslugi operacji na semaforach.
    * najlepiej podzielic sie na male grupki i wspolnie przeprowadzic
      pewne testy
    * kolejnosc wykonywania klasycznych operacji P, V na jednym
      semaforze - czy jest to kolejka prosta, czy moze nastapic
      zaglodzenie procesu o nadmiernych wymaganiach?
    * kolejnosc wykonywania jednoczesnych operacji na kilku semaforach
3. Nie zapomnij o usunieciu wszystkich utworzonych semaforow.

****************************************************************************


2. Implementacja klasycznych operacji na semaforach.

Zapoznaj sie z implementacja klasycznych operacji P i V, umieszczona w
plikach 'simple_sem.h', 'simple_sem.c'.



3. Wykaz wszystkich plikow uzywanych w tym scenariuszu.


Makefile
        Uniwersalny makefile.

err.c, err.h
	Obsluga bledow.

semun.h 
        Definicja unii semun (jesli system nie dostarcza tej definicji).
 
semget.c
	Program tworzy zestawy semaforow.

semop.c
        Program wykonuje zwykle operacje na semaforach, a wiec przechodzenie
	pod semaforem, podnoszenie semafora, etc.

semctl.c
	Program wykonuje operacje "administracyjne" na semaforach.

simple_sem.h, simple_sem.c
	Biblioteka oferujaca klasyczne operacje P i V na semaforach.


4. Uwaga. 
   Do najnowszych wersji systemu Linux wprowadzono zmiany 
   dotyczace unii semun. W poprzednich wersjach unia ta byla definiowana
   w pliku sys/sem.h. W nowszych wersjach te unie trzeba zdefiniowac samemu. 
   W niektorych wersjach (na przyklad na komputerach w labie) wywolujac 
   funkcje systemowa semctl mozna teraz pominac trzeci
   argument, jesli jest on niepotrzebny. Powoduje to jednak, ze 
   program nie da sie skompilowac w starszych systemach, dlatego 
   unikamy tego w programach przykladowych.



               ZADANIE PROGRAMISTYCZNE NR 9
               ----------------------------
  
  ***********************************************************************
  * Linia produkcyjna jest zamknieta petla o wielkosci M.               *
  * Przy tej linii pracuje N robotnikow. Kazdy z nich realizuje i-ta    *
  * faze produkcji, przy czym pierwszy proces wklada element na tasme,  *
  * a ostatni zdejmuje, czyniac miejsce pierwszemu. Kazda faza moze     *
  * rozpoczac sie dopiero po zakonczeniu poprzedniej.                   *
  *                                                                     *
  * Napisz program tworzacy linie produkcyjna wielkosci M, obslugiwana  *
  * przez N procesow, ktore powinny wykonac C cykli obrobki i zakonczyc *
  * dzialanie (M, N i C - argumenty programu).                          *
  * Przetwarzanymi obiektami sa liczby calkowite. Dla uproszczenia      *
  * przyjmujemy, ze program od razu pobiera od uzytkownika C liczb.     *
  * Pierwsza faza produkcji to wstawienie obiektu na tasme, czyli       *
  * pobranie kolejnej liczby. Kazda nastepna faza produkcji polega na   *
  * pobraniu liczby calkowitej k z tasmy i przekazaniu liczby k/2,      *
  * jesli liczba byla parzysta, a wpp. liczby 3k+1, chyba, ze liczba    *
  * byla, jedynka, wowczas przekazywana jest jedynka. Liczba            *
  * przekazywana jest na to samo miejsce na tasmie, z ktorego zostala   *
  * pobrana. Ostatnia faza, czyli zdjecie elementu z tasmy, to          *
  * wypisanie wyniku procesowi glownemu. Po zakonczeniu pracy proces    * 
  * glowny wypisuje wszystkie wyniki.                                   *
  *                                                                     *
  * Synchronizacja procesow robotnikow oraz przekazywanie wszelkich     *
  * informacji miedzy nimi powinno sie odbywac wylacznie za pomoca      *
  * mechanizmu semaforow. Nie wolno uzywac zadnych innych mechanizmow.  *
  ***********************************************************************

