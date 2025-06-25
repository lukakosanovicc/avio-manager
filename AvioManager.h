/*
 * © 2025 Luka Kosanovic
 * This project is licensed under the MIT License.
 * See the LICENSE file for details.
 */
#ifndef AVIOMANAGER_H_INCLUDED
#define AVIOMANAGER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

// STRUKTURE

typedef struct put_st
{
    char JMBG[14]; // 13 cifara
    char Ime[15];
    char Prezime[15];
    char DatumRodjenja[12]; // reci cemo da je datum formata 20.05.2025. + 1 za '\0'
    char Pol; // 'M' ili 'Z'
    unsigned int BrojPasosa; // nasao na internetu da ima 9 cifara, a max unsigned int moze deset (4294967295)

    struct put_st* next; // pokazivac na sledeceg putnika

} Putnik;

typedef struct let_st
{
    unsigned int BrojLeta; // reci cemo da je petocifreni broj (provera unosa)
    char OznakaAviona[3]; // reci cemo da su oznake tipa A2, F7... + 1 za '\0'
    unsigned int UkupanBrojMesta;

    char DatumPoletanja[12]; // reci cemo da je datum formata 20.05.2025. + 1 za '\0'
    char VremePoletanja[6]; // format -> 14:20 + 1 za '\0'
    char MestoPoletanja[30];
    char DatumSletanja[12]; // reci cemo da je datum formata 20.05.2025. + 1 za '\0'
    char VremeSletanja[6]; // format -> 14:20 + 1 za '\0'
    char MestoSletanja[30];

    float Cena;

    unsigned int PutniciNaLetu[368]; // niz koji sadrzi brojeve pasosa putnika (mnogo je lakse tako za cuvanje podataka nego da to bude niz podataka tipa Putnik)

    bool Otkazan;

    struct let_st* next; // pokazivac na sledeci let

} Let;

// GLOBALNE PROMENLJIVE

extern Let* prviLet; // head pointer liste letova

extern Putnik* prviPutnik; // head pointer liste putnika

// ZAGLAVLJA FUNKCIJA

void IspisiMeni();
void ListaSvihLetova();
void ListaLetovaPutnika(unsigned int trazeni_brPasosa);
void ListaLetovaDo(const char* destinacija);
void ListaLetovaOd(const char* polazak);
void ListaLetovaBudzet(float budzet);
void InfoOLetu(unsigned int BrojTrazenogLeta);
void PutniciNaLetu(unsigned int BrojTrazenogLeta);

bool KreirajLet();
bool NoviPutnik();
bool IzmeniLet(unsigned int BrojTrazenogLeta);
bool IzmeniPutnika(unsigned int trazeni_brPasosa);
bool ObrisiPutnikaSaLeta(unsigned int BrojTrazenogLeta, unsigned int trazeni_brPasosa);
bool OtkaziLet(unsigned int BrojLeta);

bool UcitajLetove();
bool UcitajPutnike();
bool SacuvajLetove(Let* prviLet);
bool SacuvajPutnike(Putnik* prviPutnik);
void OslobodiLetove(Let* prviLet);
void OslobodiPutnike(Putnik* prviPutnik);

bool ValidanDatum(const char* datum);
bool ValidnoVreme(const char* vreme);

#endif // AVIOMANAGER_H_INCLUDED
