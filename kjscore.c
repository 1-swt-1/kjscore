#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>//isalpha
#include "kjscore.h"

void wyczysc() {//czysczenie terminala unix
    system("clear");//dla windows: "cls"
}

void cz_buforu() {//czyszczenie buforu wprowadzania
    while (getchar() != '\n');
}

//funkcje do pobierania z klawiatury

int pobierz_int(int min, int max) {
    int liczba;
    while (1) {
       if (scanf("%d", &liczba) == 1) {
            if (liczba >= min && liczba <= max) {
                cz_buforu();
                return liczba;
            } else {
                printf("Błąd! Podaj liczbę z zakresu od %d do %d: ", min, max);
            }
       } else {
            printf("Błąd! To nie jest liczba.\nSpróbuj ponownie: ");
            cz_buforu();
       }
    }
}

void pobierz_tekst(char *cel, int max_len) {
    char bufor[101];
    while (1) {
        scanf("%s", bufor);

        int same_litery = 1;
        for (unsigned long i = 0; i < strlen(bufor); i++) {
            if(!isalpha(bufor[i]) && bufor[i] != '-') {//wykluczamy "-" dla podwojnych nazwisk
                same_litery = 0;
                break;
            }
        }

        if (same_litery && strlen(bufor) < max_len) {
            strcpy(cel, bufor);
            cz_buforu();
            return;
        } else {
            printf("\nBłąd Używaj tylko liter.\nSpróbuj ponownie: ");
            cz_buforu();
        }
    }
}


//pozostałe funkcje
Zawodnik *stworz(char *im, char *naz, int m, int g, int a, char *st) {
    Zawodnik *new = (Zawodnik *)malloc(sizeof(Zawodnik));//"(Zawodnik *)" symboliozuje strukture zawodnik
    if (new == NULL) exit(1);

    strcpy(new->imie, im);
    strcpy(new->nazwisko, naz);
    new->mecze = m;
    new->gole = g;
    new->asysty = a;
    strcpy(new->stan, st);
    new->next = NULL;//nastepny narazie ejst pusty
    return new;
}

void dodaj(Zawodnik **head) {
    char im[51], naz[51], st[51];
    int m, g, a;

    wyczysc();
    printf("===Dodawanie zawodnika===\n");
    printf("Podaj imie: ");
    pobierz_tekst(im, 51);
    printf("Podaj nazwisko: "); 
    pobierz_tekst(naz, 51);
    printf("Mecze: ");          
    m = pobierz_int(0, 100);
    printf("Gole: ");           
    g = pobierz_int(0, 100);
    printf("Asysty: ");         
    a = pobierz_int(0, 100);
    printf("Stan zdrowia (jedno slowo): ");   
    pobierz_tekst(st, 51);

    Zawodnik *new = stworz(im, naz, m, g, a, st); 
    new->next = *head;
    *head = new; 
}

void usun(Zawodnik **head) {
    wyczysc();
    if (*head == NULL) {//gdy nie ma pierwszego zawodnika
        printf("Lista pusta.\nNacisnij ENTER..."); 
        getchar(); 
        return;
    }

    char cel[51];
    printf("Nazwisko do usuniecia: "); 
    pobierz_tekst(cel, 51);

    Zawodnik *p = *head;
    Zawodnik *prev = NULL;
    if (p != NULL && strcmp(p->nazwisko, cel) == 0) {//strcmp zwraca 0 gdy wiadomosci są identyczne
        *head = p->next;
        free(p);
        printf("Usunieto.\nNacisnij ENTER aby kontynuować"); 
        getchar();//pauza żeby wymusic u uzytkownika wcisniecie enteru
        return;
    }

    while (p != NULL && strcmp(p->nazwisko, cel) != 0) {
        prev = p;
        p = p->next;//lączy dwoch zawodników ktorzy byli oddzieleni tym usuniętym
    }

    if (p == NULL) {//jesli p jest puste to nic nie znaleziono
        printf("Nie znaleziono.\n");
    } else {
        prev->next = p->next;//Przestań wskazywać na p, a zacznij wskazywać na to, co stoi za nim (omija zawodnika usunietego)
        free(p);
        printf("Usunieto.\n");
    }
    printf("Naciśnij ENTER aby kontynuować");
    getchar();
}

void zapisz(Zawodnik *head, char *nazwa_pliku) {
    FILE *f = fopen(nazwa_pliku, "w");
    if (f == NULL) {
        printf("Bląd otwarcia pliku do zapisu!\n");
        printf("Naciśnij ENTER aby kontynuować"); 
        getchar();
        return;
    }

    Zawodnik *p = head;//wskaźnik do chodzenia po liście od początku
    while (p != NULL) {
        fprintf(f, "imię:%s nazwisko:%s mecze:%d gole:%d asysty:%d stan_zdrowia:%s\n", 
            p->imie, p->nazwisko, p->mecze, p->gole, p->asysty, p->stan);
        p = p->next;
    }
    fclose(f);
    wyczysc();
    printf("Zapisano zmiany do pliku: %s\n", nazwa_pliku);
    printf("Naciśnij ENTER aby kontynuować"); 
    getchar(); 
}

void wczytaj(Zawodnik **head, char *nazwa_pliku) {
    FILE *f = fopen(nazwa_pliku, "r");//otwarcie pliku w trybie odczytu (read)
    if (f == NULL) return;//brak pliku to nie błąd, po prostu pusta lista na start

    char im[51], naz[51], st[51];
    int m, g, a;
    while (fscanf(f, "imię:%s nazwisko:%s mecze:%d gole:%d asysty:%d stan_zdrowia:%s\n", im, naz, &m, &g, &a, st) == 6) {
        Zawodnik *new = stworz(im, naz, m, g, a, st);//tworzenie zawodnika w pamięci na podstawie linii z pliku
        new->next = *head;//doklejanie go do budowanej listy
        *head = new;//aktualizacja początku listy
    }
    fclose(f);
}

//funkcja sortująca bąbelkowo po liczbie goli (malejąco)
void sortuj_gole(Zawodnik *head) {
    if (head == NULL) return; //jeśli lista jest pusta, nie ma czego sortować
    int zamiana; //flaga informująca czy w danej turze coś przestawiliśmy
    Zawodnik *p; //wskaźnik do przeglądania listy
    Zawodnik *last = NULL; //wskaźnik ograniczający zakres sortowania

    do { //pętla sortowania bąbelkowego
        zamiana = 0; //na początku tury zakładamy że wszystko jest w porządku
        p = head; //zaczynamy od początku listy
        while (p->next != last) { //idziemy do przodu aż do miejsca już posortowanego
            if (p->gole < p->next->gole) { //jeśli obecny ma mniej goli niż następny (zła kolejność)
                char t_txt[51]; //bufor na tekst do zamiany miejscami
                int t_num; //bufor na liczby do zamiany miejscami
                strcpy(t_txt, p->imie); strcpy(p->imie, p->next->imie); strcpy(p->next->imie, t_txt); //zamiana imion
                strcpy(t_txt, p->nazwisko); strcpy(p->nazwisko, p->next->nazwisko); strcpy(p->next->nazwisko, t_txt); //zamiana nazwisk
                strcpy(t_txt, p->stan); strcpy(p->stan, p->next->stan); strcpy(p->next->stan, t_txt); //zamiana stanu zdrowia
                t_num = p->mecze; p->mecze = p->next->mecze; p->next->mecze = t_num; //zamiana meczów
                t_num = p->gole; p->gole = p->next->gole; p->next->gole = t_num; //zamiana goli
                t_num = p->asysty; p->asysty = p->next->asysty; p->next->asysty = t_num; //zamiana asyst
                zamiana = 1; //odnotowanie że dokonaliśmy poprawki w tej turze
            }
            p = p->next;//przejście do następnej pary zawodników
        }

        last = p; //ostatni sprawdzony element jest już na swoim miejscu
    } while (zamiana); //powtarzaj póki w liście dojdzie do jakichkolwiek zmian
}

//funkcja sortująca alfabetycznie po nazwisku
void sortuj_nazwisko(Zawodnik *head) {
    if (head == NULL) return; //jeśli brak zawodników, wyjdź
    int zamiana; //flaga kontrolna bąbelkowania
    Zawodnik *p; //wskaźnik pomocniczy
    Zawodnik *last = NULL; //bariera końca sortowania

    do { //pętla główna
        zamiana = 0; //reset flagi zamiany
        p = head; //start od początku
        while (p->next != last) { //przeglądanie par elementów
            if (strcmp(p->nazwisko, p->next->nazwisko) > 0) { //jeśli nazwisko jest "większe" (dalej w alfabecie) niż następne
                char t_txt[51]; //bufor tekstowy
                int t_num; //bufor liczbowy
                strcpy(t_txt, p->imie); strcpy(p->imie, p->next->imie); strcpy(p->next->imie, t_txt); //zamiana imion
                strcpy(t_txt, p->nazwisko); strcpy(p->nazwisko, p->next->nazwisko); strcpy(p->next->nazwisko, t_txt); //zamiana nazwisk
                strcpy(t_txt, p->stan); strcpy(p->stan, p->next->stan); strcpy(p->next->stan, t_txt); //zamiana stanu
                t_num = p->mecze; p->mecze = p->next->mecze; p->next->mecze = t_num; //zamiana meczów
                t_num = p->gole; p->gole = p->next->gole; p->next->gole = t_num; //zamiana goli
                t_num = p->asysty; p->asysty = p->next->asysty; p->next->asysty = t_num; //zamiana asyst
                zamiana = 1; //potwierdzenie dokonania zmiany
            }

            p = p->next; //kolejna para
        }
        last = p; //zawężenie obszaru sortowania
    } while (zamiana); //pętla kręci się dopóki lista nie będzie idealnie poukładana
}

void wyswietl(Zawodnik *head) { //funkcja drukująca całą kadrę na ekranie
    int wyb = -1; //zmienna na wybór opcji sortowania (cos innego niż 0,1,2)
    while (wyb != 0) {
        wyczysc();
        printf("|=================Kadra zespolu==================|\n");
        printf("|Imie||Nazwisko||Mecze||Gole||Asysty||StanZdrowia|\n");
        printf("|================================================|\n");
        
        Zawodnik *p = head;
        while (p != NULL) {
            printf("| %s %s M:%d G:%d A:%d %s\n", 
                p->imie, p->nazwisko, p->mecze, p->gole, p->asysty, p->stan);
            p = p->next; //skok do następnej osoby
        }

        printf("|================================================|\n");
        printf("|1. Sortuj po golach strzelonych                 |\n");
        printf("|2. Sortuj alfabetycznie (po nazwisku)           |\n");
        printf("|0. Wyjscie                                      |\n");
        printf("|================================================|\n");
        printf("Wybieram: ");
        
        wyb = pobierz_int(0, 2);

        if (wyb == 1) { //jeśli wybrano sortowanie po golach
            sortuj_gole(head);
        } else if (wyb == 2) {
            sortuj_nazwisko(head);
        }
    }
}

//funkcja czyszcząca pamięć RAM przed zamknięciem programu
void zwolnij(Zawodnik *head) { 
    while (head != NULL) { //pętla działa dopóki w liście są elementy
        Zawodnik *tmp = head; //zapamiętanie adresu obecnego zawodnika
        head = head->next; //przesunięcie głowy listy na następną osobę
        free(tmp);
    }
}

//funkcja do szukania konkretnych danych w kadrze
void wyszukaj(Zawodnik *head) { 
    wyczysc();
    printf("|===Wyszukiwanie Zawodnika===|\n");
    printf("|1. Po nazwisku(Edycja stanu)|\n");
    printf("|2. Po golach strzelonych    |\n");
    printf("|0. Wyjdz                    |\n");
    printf("|============================|\n");
    printf("Wybieram: ");

    int opcja = pobierz_int(0, 2);
    if (opcja == 0) return; //jeśli wybrano 0, wróć do menu głównego

    //szukanie po nazwisku
    if (opcja == 1) { //jeśli szukamy po nazwisku
        wyczysc();
        char cel[51]; //zmienna na szukane słowo
        printf("Podaj nazwisko: ");
        pobierz_tekst(cel, 51); //pobranie nazwiska do zmiennej cel

        Zawodnik *p = head; //start szukania od początku listy
        int znaleziono = 0; //flaga czy w ogóle kogoś trafiliśmy

        while (p != NULL) { //przeglądanie całej listy
            if (strcmp(p->nazwisko, cel) == 0) { //jeśli nazwisko z listy zgadza się z wpisanym
                znaleziono = 1; //zaznaczenie że mamy trafienie
                wyczysc();
                printf("|==Znaleziono=============================|\n");
                printf("|Imię: %s\n", p->imie); //wyświetlenie imienia
                printf("|Nazwisko: %s\n", p->nazwisko); //wyświetlenie nazwiska
                printf("|Mecze: %d\n", p->mecze); //wyświetlenie meczów
                printf("|Gole: %d\n", p->gole); //wyświetlenie goli
                printf("|Asysty: %d\n", p->asysty); //wyświetlenie asyst
                printf("|Stan Zdrowia: %s\n", p->stan); //wyświetlenie stanu zdrowia
                printf("|=========================================|\n");
                printf("|==OPCJE:=================================|\n");
                printf("|1. Edytuj stan zdrowia                   |\n");
                printf("|0. Wyjdź                                 |\n");
                printf("|=========================================|\n");
                printf("|Wybieram: ");
                
                int sub = pobierz_int(0, 1);
                
                if (sub == 1) { //jeśli chcemy edytować stan zdrowia
                    wyczysc();
                    printf("Podaj nowy stan zdrowia: ");
                    pobierz_tekst(p->stan, 51); //nadpisanie starego stanu nowym tekstem
                    wyczysc();
                    printf("Zaktualizowano stan zdrowia!\n");
                    printf("Nacisnij ENTER aby wrocic...");
                    getchar();
                }
                return;//wyjście z funkcji szukania po znalezieniu i obsłużeniu osoby
            }
            p = p->next;//przejście do kolejnej pozycji z listy
        }
        
        if (!znaleziono) { //jeśli pętla się skończyła a "znaleziono" nadal wynosi 0
            printf("\nNie znaleziono zawodnika o nazwisku: %s\n", cel);
            printf("Nacisnij ENTER...");
            getchar(); //pauza
        }
    }
    
    //szukanie po golach
    else if (opcja == 2) { //jeśli szukamy wszystkich z konkretną liczbą bramek
        int szukane_gole; //zmienna na liczbę goli do filtra
        wyczysc();
        printf("Podaj liczbe goli: ");
        szukane_gole = pobierz_int(0, 100);

        Zawodnik *p = head; //start od początku listy
        int znaleziono = 0; //flaga trafień
        wyczysc();
        printf("=== Zawodnicy z liczbą goli: %d ===\n", szukane_gole);
        while (p != NULL) { //sprawdzanie każdego zawodnika po kolei

            if (p->gole == szukane_gole) { //jeśli gole zawodnika są równe szukanym
                printf("Imię: %s  Nazwisko: %s  Mecze: %d  Gole: %d  Asysty: %d  Stan: %s\n", p->imie, p->nazwisko, p->mecze, p->gole, p->asysty, p->stan); //wypisanie danych osoby
                znaleziono = 1; //odnotowanie trafienia
            }

            p = p->next; //skok do następnego zawodnika
        }

        if (!znaleziono) { //jeśli nikt w całej kadrze nie ma tylu goli
            printf("Brak zawodników spełniających kryterium.\n");
        }
        
        printf("\nNaciśnij ENTER aby kontynuować...");
        getchar();
    } //koniec sekcji szukania po golach
}