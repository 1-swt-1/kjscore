#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>//isalpha
#include "kjscore.h"

int main(int l_arg, char *lista_arg[]) { // l_arg -> ile argumentow zostało wprowadzonych przy uruchamianiu
                                        // lista_arg[0] -> nazwa programu
                                        // lista_arg[1] -> nazwa pliku bazy (txt)

    if (l_arg < 2) {
        printf("Błąd! Nie podano nazwy pliku bazy danych.\n");
        printf("Prawidłowe uruchomienie programu: %s nazwa_pliku.txt\n", lista_arg[0]);
        return 1; //zwracamy błąd
    }

    char *plik_bazy = lista_arg[1];

    Zawodnik *kadra = NULL;//wskaźnik na strukture | NULL poniewaz po uruchomieniu programu lista na poczatku jest pusta
    int opcja;

    wczytaj(&kadra, plik_bazy);//wczytanie kadry z pliku txt

    while(1) {
        wyczysc();

        //interfejs menu
        printf("|=======KJSCORE=======|\n");
        printf("|=====Real Madryt=====|\n");
        printf("|⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣖⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀|\n");
        printf("|⠀⠀⠀⠀⠀⣠⣤⣴⡾⠛⠛⠿⣦⣤⣄⡀⠀⠀⠀⠀⠀|\n");
        printf("|⠀⠀⠀⢠⣾⠋⢹⡟⠀⠀⠀⠀⢹⣯⠙⢿⡄⠀⠀⠀⠀|\n");
        printf("|⠀⠀⠀⠸⣧⣄⣨⣿⣶⣶⣶⣶⣾⣇⣀⣼⡇⠀⠀⠀⠀|\n");
        printf("|⠀⠀⠀⠀⠈⣛⣭⣵⡶⠶⠶⢶⣦⣭⣛⠉⠀⠀⠀⠀⠀|\n");
        printf("|⠀⠀⢀⣴⡿⠋⠉⠀⠀⠀⠀⠀⠀⠈⠙⠿⣦⡀⠀⠀⠀|\n");
        printf("|⠀⣰⡿⠋⣠⡾⣷⣄⣀⣠⣀⣀⣀⣴⢷⣤⠈⢻⣦⠀⠀|\n");
        printf("|⣰⡟⠀⣼⠟⢀⣼⠿⣿⣍⢉⣿⠿⣷⡄⠹⣷⡀⢻⣧⠀|\n");
        printf("|⣿⠃⣸⡟⢀⣿⠃⠀⠈⠻⠟⠁⠀⠈⠀⠀⢹⣧⠀⣿⡄|\n");
        printf("|⣿⠀⢿⡇⠸⣿⠀⠀⢰⣶⣶⡆⠀⠀⠀⠀⢸⣿⠀⣿⡇|\n");
        printf("|⢿⡆⠸⣷⡀⢻⣦⡀⢸⡷⠶⠆⠀⣠⡀⠀⣼⡏⢠⣿⠀|\n");
        printf("|⠘⣿⡄⠹⠗⠀⠙⠿⣿⣧⣤⣶⠿⠋⠀⠸⠟⢀⣾⠇⠀|\n");
        printf("|⠀⠈⢿⣦⡀⠀⠀⠀⠈⠁⠀⠀⠀⠀⠀⢀⣴⡿⠃⠀⠀|\n");
        printf("|⠀⠀⠀⠙⠻⢶⣤⣄⣀⣀⣀⣀⣀⣤⣶⠿⠋⠀⠀⠀⠀|\n");
        printf("|⠀⠀⠀⠀⠀⠀⠈⠉⠛⠛⠛⠛⠉⠉⠀⠀⠀⠀⠀⠀⠀|\n");
        printf("|                     |\n");
        printf("|=======OPCJE:========|\n");
        printf("|1. Wyswietl kadre    |\n");
        printf("|2. Dodaj zawodnika   |\n");
        printf("|3. Usun zawodnika    |\n");
        printf("|4. Wyszukaj(edycja)  |\n");
        printf("|5. Zapisz zmiany     |\n");
        printf("|0. Wyjdz             |\n");
        printf("|=====================|\n");
        printf("Wybieram: ");

        opcja = pobierz_int(0,5);

        switch (opcja) {
            case 1: wyswietl(kadra); break;
            case 2: dodaj(&kadra); break;
            case 3: usun(&kadra); break;
            case 4: wyszukaj(kadra); break;
            case 5: zapisz(kadra, plik_bazy); break;
            case 0: zwolnij(kadra); return 0;
            default: break;
        }
    }

    return 0;

}


