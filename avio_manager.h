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
    char jmbg[14]; // 13 cifara
    char ime[15];
    char prezime[15];
    char datum_rodjenja[12]; // reci cemo da je datum formata 20.05.2025. + 1 za '\0'
    char pol; // 'M' ili 'Z'
    unsigned int broj_pasosa; // nasao na internetu da ima 9 cifara, a max unsigned int moze deset (4294967295)

    struct put_st* next; // pokazivac na sledeceg putnika

} putnik;

typedef struct let_st
{
    unsigned int broj_leta; // reci cemo da je petocifreni broj (provera unosa)
    char oznaka_aviona[3]; // reci cemo da su oznake tipa A2, F7... + 1 za '\0'
    unsigned int ukupan_broj_mesta;

    char datum_poletanja[12]; // reci cemo da je datum formata 20.05.2025. + 1 za '\0'
    char vreme_poletanja[6]; // format -> 14:20 + 1 za '\0'
    char mesto_poletanja[30];
    char datum_sletanja[12]; // reci cemo da je datum formata 20.05.2025. + 1 za '\0'
    char vreme_sletanja[6]; // format -> 14:20 + 1 za '\0'
    char mesto_sletanja[30];

    float cena_karte;

    unsigned int putnici_na_letu[368]; // niz koji sadrzi brojeve pasosa putnika (mnogo je lakse tako za cuvanje podataka nego da to bude niz podataka tipa Putnik)

    bool otkazan;

    struct let_st* next; // pokazivac na sledeci let

} let;

// GLOBALNE PROMENLJIVE

extern let* prvi_let; // head pointer liste letova

extern putnik* prvi_putnik; // head pointer liste putnika

// ZAGLAVLJA FUNKCIJA

void ispisi_meni();
void lista_svih_letova();
void lista_letova_putnika(unsigned int trazeni_br_pasosa);
void lista_letova_do(const char* destinacija);
void lista_letova_od(const char* polazak);
void lista_letova_budzet(float budzet);
void info_o_letu(unsigned int broj_trazenog_leta);
void putnici_na_letu(unsigned int broj_trazenog_leta);

bool kreiraj_let();
bool novi_putnik();
bool izmeni_let(unsigned int broj_trazenog_leta);
bool izmeni_putnika(unsigned int trazeni_br_pasosa);
bool obrisi_putnika_sa_leta(unsigned int broj_trazenog_leta, unsigned int trazeni_br_pasosa);
bool otkazi_let(unsigned int broj_leta);

bool ucitaj_letove();
bool ucitaj_putnike();
bool sacuvaj_letove(let* prvi_let);
bool sacuvaj_putnike(putnik* prvi_putnik);
void oslobodi_letove(let* prvi_let);
void oslobodi_putnike(putnik* prvi_putnik);

bool validan_datum(const char* datum);
bool validno_vreme(const char* vreme);

#if defined(AVIOMANAGER_IMPLEMENTATION)

// return true -> doslo je do greske
// return false -> bez greske (isto kao return 0)

let* prvi_let = NULL;
putnik* prvi_putnik = NULL;

void ispisi_meni()
{
    printf("\n----Dobrodosli u AvioManager! Izaberite:\n");
    printf("\t1 - Kreiranje novog leta\n");
    printf("\t2 - Registracija novog putnika\n");
    printf("\t3 - Lista svih letova\n");
    printf("\t4 - Lista svih letova nekog putnika na osnovu broja pasosa\n"); // u zadatku pise jmbg, ali stranci ga nemaju pa je prakticnije preko broja pasosa
    printf("\t5 - Prikaz svih letova do nekog odredista\n");
    printf("\t6 - Prikaz svih letova iz nekog mesta\n");
    printf("\t7 - Listanje svih letova do odredjene cene\n");
    printf("\t8 - Prikaz informacija o nekom letu\n");
    printf("\t9 - Prikaz liste putnika na nekom letu\n");
    printf("\t10 - Izmena podataka o letu\n");
    printf("\t11 - Izmenu podataka o putniku na odredjenom letu\n");
    printf("\t12 - Brisanje putnika sa nekog leta\n");
    printf("\t13 - Otkazivanje leta\n");

    printf("\n\t0 - Izlaz iz programa i cuvanje podataka\n");
    printf("Vas izbor >> ");
}

void lista_svih_letova()
{
    if (prvi_let == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Lista svih letova -----\n");

    let* tekuci = prvi_let;
    while (tekuci != NULL)
    {

        if (tekuci->oznaka_aviona != 0)
        {
            printf("Broj leta: %u", tekuci->broj_leta);
            printf("\n");

            printf("Oznaka aviona: %s\n", tekuci->oznaka_aviona);
            printf("Ukupan broj mesta: %u\n", tekuci->ukupan_broj_mesta);
            printf("Datum poletanja: %s\n", tekuci->datum_poletanja);
            printf("Vreme poletanja: %s\n", tekuci->vreme_poletanja);
            printf("Mesto poletanja: %s\n", tekuci->mesto_poletanja);
            printf("Datum sletanja: %s\n", tekuci->datum_sletanja);
            printf("Vreme sletanja: %s\n", tekuci->vreme_sletanja);
            printf("Mesto sletanja: %s\n", tekuci->mesto_sletanja);
            printf("Cena karte: %.2f\n", tekuci->cena_karte);
            printf("Otkazan: %s\n", tekuci->otkazan ? "Da" : "Ne");
            printf("----------------------------\n");

            tekuci = tekuci->next;
        }
    }
}

bool kreiraj_let()
{
    let* novi_let = (let*)malloc(sizeof(let));
    if (novi_let == NULL)
    {
        printf("Greska pri alokaciji memorije!\n");
        return true;
    }

    do
    {
        printf("Unesite broj leta (do 5 cifara, npr. 12345): ");
        scanf("%u", &novi_let->broj_leta);
    }
    while (novi_let->broj_leta > 99999 || novi_let->broj_leta < 1);

    do
    {
        printf("Unesite oznaku aviona (npr. A2): ");
        scanf("%2s", novi_let->oznaka_aviona);

        printf("Uneta oznaka: %s\n", novi_let->oznaka_aviona);

    }
    while (!(novi_let->oznaka_aviona[0] >= 'A' && novi_let->oznaka_aviona[0] <= 'Z' && novi_let->oznaka_aviona[1] >= '0' && novi_let->oznaka_aviona[1] <= '9'));

    do
    {
        printf("Unesite ukupan broj mesta: ");
        scanf("%u", &novi_let->ukupan_broj_mesta); // boeing 777 -> 368 mesta max
    }
    while (novi_let->ukupan_broj_mesta > 368 || novi_let->ukupan_broj_mesta == 0);

    do
    {
        printf("Unesite datum poletanja (npr. 20.05.2025.): ");
        scanf("%11s", novi_let->datum_poletanja);
    }
    while (!validan_datum(novi_let->datum_poletanja));

    do
    {
        printf("Unesite vreme poletanja (npr. 14:20): ");
        scanf("%11s", novi_let->vreme_poletanja);
    }
    while (!validno_vreme(novi_let->vreme_poletanja));

    getchar(); // da pocisti '\n'

    printf("Unesite mesto poletanja: ");
    scanf(" %[^\n]", novi_let->mesto_poletanja);

    do
    {
        printf("Unesite datum sletanja (npr. 20.05.2025.): ");
        scanf("%11s", novi_let->datum_sletanja);
    }
    while (!validan_datum(novi_let->datum_sletanja));

    do
    {
        printf("Unesite vreme sletanja (npr. 14:20): ");
        scanf("%11s", novi_let->vreme_sletanja);
    }
    while (!validno_vreme(novi_let->vreme_sletanja));

    getchar();

    printf("Unesite mesto sletanja: ");
    scanf(" %[^\n]", novi_let->mesto_sletanja);

    do
    {
        printf("Unesite cenu karte (u RSD): ");
        scanf("%f", &novi_let->cena_karte);
    }
    while (novi_let->cena_karte < 0);

    novi_let->otkazan = false;
    novi_let->next = NULL;

    unsigned int br;
    unsigned int index = 0; // lokalni brojac za trenutni broj putnika jer ga ne bi mogli ucitati sa tastature

    do
    {
        printf("Unesite brojeve pasosa putnika (za testiranje unesite 123456789, za izlaz 0): ");
        scanf("%u", &br);

        if (br == 0) break;

        if (br < 100000000 || br > 999999999)
        {
            printf("Broj pasosa ima 9 cifara. Pokusajte ponovo...\n");
        }
        else
        {
            if (index < novi_let->ukupan_broj_mesta)
            {
                novi_let->putnici_na_letu[index++] = br; // stavi ga na trenutni index, zatim uvecaj index (brojac) za jedan
            }
            else
            {
                printf("Dostignut maksimalan broj putnika!\n");
                break;
            }
        }
    }
    while (true);

    if (prvi_let == NULL)
    {
        prvi_let = novi_let;
    }
    else
    {
        let* tekuci = prvi_let;
        while (tekuci->next != NULL)
            tekuci = tekuci->next;

        tekuci->next = novi_let;
    }

    printf("Let je uspesno kreiran!\n");
    return false;
}

bool novi_putnik()
{
    putnik* novi_putnik = (putnik*)malloc(sizeof(putnik));
    if (!novi_putnik)
    {
        printf("Greska pri alokaciji memorije!\n");
        return true;
    }

    do
    {
        printf("Unesite jmbg (13 cifara): ");
        scanf("%13s", novi_putnik->jmbg);
    }
    while (strlen(novi_putnik->jmbg) != 13);

    do
    {
        printf("Unesite ime: ");
        scanf("%14s", novi_putnik->ime);
    }
    while (strlen(novi_putnik->ime) > 14 || strlen(novi_putnik->ime) < 2);

    do
    {
        printf("Unesite prezime: ");
        scanf("%14s", novi_putnik->prezime);
    }
    while (strlen(novi_putnik->prezime) > 14 || strlen(novi_putnik->prezime) < 2);

    do
    {
        printf("Unesite datum rodjenja (npr. 20.05.2000.): ");
        scanf("%11s", novi_putnik->datum_rodjenja);
    }
    while (!validan_datum(novi_putnik->datum_rodjenja));

    do
    {
        printf("Unesite pol (M ili Z): ");
        scanf(" %c", &novi_putnik->pol);
    }
    while (novi_putnik->pol != 'M' && novi_putnik->pol != 'Z');

    do
    {
        printf("Unesite broj pasosa (9 cifara): ");
        scanf("%9u", &novi_putnik->broj_pasosa);
    }
    while (novi_putnik->broj_pasosa < 100000000 || novi_putnik->broj_pasosa > 999999999);

    novi_putnik->next = NULL;

    if (prvi_putnik == NULL)
        prvi_putnik = novi_putnik;
    else
    {
        putnik* tekuci = prvi_putnik;
        while (tekuci->next != NULL)
            tekuci = tekuci->next;
        tekuci->next = novi_putnik;
    }

    printf("Putnik je uspesno kreiran!\n");
    return false;
}

bool validan_datum(const char* datum)
{
    if (datum == NULL || strlen(datum) != 11) // ako nema datuma nije validan, ako duzina nije onakva kakvu smo odredili isto
        return false;

    if (datum[2] != '.' || datum[5] != '.' || datum[10] != '.') // proveravamo da li string ima tacke na dobrim mestima
        return false;

    for (int i = 0; i < 11; i++)
    {
        if (i == 2 || i == 5 || i == 10) // preskacemo tacke
            continue;
        if (!isdigit(datum[i])) // a ako ostalo nisu brojevi, vracamo false
            return false;
    }

    // odvajanje dana, meseca i godine
    int dan = (datum[0] - '0') * 10 + (datum[1] - '0');
    int mesec = (datum[3] - '0') * 10 + (datum[4] - '0');
    int godina = (datum[6] - '0') * 1000 + (datum[7] - '0') * 100 + (datum[8] - '0') * 10 + (datum[9] - '0');

    struct tm t = {0};
    t.tm_mday = dan;
    t.tm_mon = mesec - 1; // meseci idu od 0 do 11
    t.tm_year = godina - 1900; // godine se racunaju od 1900

    // mktime automatski koriguje nevalidne datume
    if (mktime(&t) == -1)
        return false;

    // proverimo da li su se podaci promenili posle mktime
    if (t.tm_mday != dan || t.tm_mon != mesec - 1 || t.tm_year != godina - 1900)
        return false;

    return true;
}

bool validno_vreme(const char* vreme)
{
    if (vreme == NULL || strlen(vreme) != 5) // format mora biti tacno 5 karaktera
        return false;

    if (vreme[2] != ':') // ':' mora biti na trecem mestu
        return false;

    for (int i = 0; i < 5; i++)
    {
        if (i == 2) continue; // preskacemo ':'
        if (!isdigit(vreme[i])) // ostali moraju biti cifre
            return false;
    }

    // razdvajanje sati i minuta
    int sati = (vreme[0] - '0') * 10 + (vreme[1] - '0');
    int minuti = (vreme[3] - '0') * 10 + (vreme[4] - '0');


    if (sati < 0 || sati > 23 || minuti < 0 || minuti > 59) // provera da li su sati i minuti u validnom opsegu
        return false;

    return true;
}

bool sacuvaj_letove(let* prvi_let)
{
    FILE *f = fopen("letovi.dat", "wb");
    if (!f) // svejedno je, moze i if (f == NULL)
    {
        printf("Greska pri otvaranju fajla za pisanje.\n");
        return true;
    }

    let* tekuci = prvi_let;
    while (tekuci != NULL)
    {
        fwrite(tekuci, sizeof(let), 1, f);
        tekuci = tekuci->next;
    }

    fclose(f);
    return false;
}

bool sacuvaj_putnike(putnik* prvi_putnik)
{
    FILE *f = fopen("putnici.dat", "wb");
    if (!f)
    {
        printf("Greska pri otvaranju fajla za pisanje.\n");
        return true;
    }

    putnik* tekuci = prvi_putnik;
    while (tekuci != NULL)
    {
        fwrite(tekuci, sizeof(putnik), 1, f);
        tekuci = tekuci->next;
    }

    fclose(f);
    return false;
}

bool ucitaj_letove()
{
    FILE *f = fopen("letovi.dat", "rb");
    if (!f) return true;

    prvi_let = NULL;

    let temp;
    while (fread(&temp, sizeof(let), 1, f) == 1)
    {
        let* novi = (let*)malloc(sizeof(let));
        if (!novi)
        {
            printf("Greska pri alokaciji memorije.\n");
            fclose(f);
            return true;
        }

        *novi = temp;
        novi->next = NULL;

        if (prvi_let == NULL)
        {
            prvi_let = novi;
        }
        else
        {
            let* tekuci = prvi_let;
            while (tekuci->next != NULL)
                tekuci = tekuci->next;

            tekuci->next = novi;
        }
    }

    fclose(f);
    return false;
}

bool ucitaj_putnike()
{
    FILE *f = fopen("putnici.dat", "rb");
    if (!f) return true;

    prvi_putnik = NULL;

    putnik temp;
    while (fread(&temp, sizeof(putnik), 1, f) == 1)
    {
        putnik* novi = (putnik*)malloc(sizeof(putnik));
        if (!novi)
        {
            printf("Greska pri alokaciji memorije.\n");
            fclose(f);
            return true;
        }

        *novi = temp;
        novi->next = NULL;

        if (prvi_putnik == NULL)
        {
            prvi_putnik = novi;
        }
        else
        {
            putnik* tekuci = prvi_putnik;
            while (tekuci->next != NULL)
                tekuci = tekuci->next;

            tekuci->next = novi;
        }
    }

    fclose(f);
    return false;
}

void oslobodi_letove(let* prvi_let)
{
    let* temp;
    while (prvi_let != NULL)
    {
        temp = prvi_let;
        prvi_let = prvi_let->next;
        free(temp);
    }
}

void oslobodi_putnike(putnik* prvi_putnik)
{
    putnik* temp;
    while (prvi_putnik != NULL)
    {
        temp = prvi_putnik;
        prvi_putnik = prvi_putnik->next;
        free(temp);
    }
}

// -----------------------------------------------------------------------------

void lista_letova_putnika(unsigned int trazeni_br_pasosa)
{
    if (prvi_let == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Lista letova putnika %u -----\n", trazeni_br_pasosa);

    let* tekuci = prvi_let;
    while (tekuci != NULL)
    {
        int pronadjen = 0;

        for (int i = 0; i < 368; i++)
        {
            if (tekuci->putnici_na_letu[i] == trazeni_br_pasosa)
            {
                pronadjen = 1;
                break;
            }
        }

        if (pronadjen)
        {
            printf("Broj leta: %u\n", tekuci->broj_leta);
            printf("Oznaka aviona: %s\n", tekuci->oznaka_aviona);
            printf("Ukupan broj mesta: %u\n", tekuci->ukupan_broj_mesta);
            printf("Datum poletanja: %s\n", tekuci->datum_poletanja);
            printf("Vreme poletanja: %s\n", tekuci->vreme_poletanja);
            printf("Mesto poletanja: %s\n", tekuci->mesto_poletanja);
            printf("Datum sletanja: %s\n", tekuci->datum_sletanja);
            printf("Vreme sletanja: %s\n", tekuci->vreme_sletanja);
            printf("Mesto sletanja: %s\n", tekuci->mesto_sletanja);
            printf("Cena karte: %.2f\n", tekuci->cena_karte);
            printf("Otkazan: %s\n", tekuci->otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
        }

        tekuci = tekuci->next;
    }
}

void lista_letova_do(const char* destinacija)
{
    if (prvi_let == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Lista letova do %s -----\n", destinacija);

    let* tekuci = prvi_let;
    bool pronadjen = false;

    while (tekuci != NULL)
    {
        if (strcmp(tekuci->mesto_sletanja, destinacija) == 0) // ako su ova dva stringa podudarni...
        {
            pronadjen = true;

            printf("Broj leta: %u\n", tekuci->broj_leta);
            printf("Oznaka aviona: %s\n", tekuci->oznaka_aviona);
            printf("Ukupan broj mesta: %u\n", tekuci->ukupan_broj_mesta);
            printf("Datum poletanja: %s\n", tekuci->datum_poletanja);
            printf("Vreme poletanja: %s\n", tekuci->vreme_poletanja);
            printf("Mesto poletanja: %s\n", tekuci->mesto_poletanja);
            printf("Datum sletanja: %s\n", tekuci->datum_sletanja);
            printf("Vreme sletanja: %s\n", tekuci->vreme_sletanja);
            printf("Mesto sletanja: %s\n", tekuci->mesto_sletanja);
            printf("cena_karte karte: %.2f\n", tekuci->cena_karte);
            printf("Otkazan: %s\n", tekuci->otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
        }

        tekuci = tekuci->next;
    }

    if (pronadjen == false)
    {
        printf("Nema letova do te destinacije.\n");
    }
}

void lista_letova_od(const char* polazak)
{
    if (prvi_let == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Lista letova od %s -----\n", polazak);

    let* tekuci = prvi_let;
    bool pronadjen = false;

    while (tekuci != NULL)
    {
        if (strcmp(tekuci->mesto_poletanja, polazak) == 0)
        {
            pronadjen = true;

            printf("Broj leta: %u\n", tekuci->broj_leta);
            printf("Oznaka aviona: %s\n", tekuci->oznaka_aviona);
            printf("Ukupan broj mesta: %u\n", tekuci->ukupan_broj_mesta);
            printf("Datum poletanja: %s\n", tekuci->datum_poletanja);
            printf("Vreme poletanja: %s\n", tekuci->vreme_poletanja);
            printf("Mesto poletanja: %s\n", tekuci->mesto_poletanja);
            printf("Datum sletanja: %s\n", tekuci->datum_sletanja);
            printf("Vreme sletanja: %s\n", tekuci->vreme_sletanja);
            printf("Mesto sletanja: %s\n", tekuci->mesto_sletanja);
            printf("Cena karte: %.2f\n", tekuci->cena_karte);
            printf("Otkazan: %s\n", tekuci->otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
        }

        tekuci = tekuci->next;
    }

    if (pronadjen == false)
    {
        printf("Nema letova od tog mesta.\n");
    }
}

bool otkazi_let(unsigned int broj_leta)
{
    if (prvi_let == NULL)
    {
        printf("Nema letova za otkazivanje.\n");
        return true;
    }

    printf("\n-----------------------------\n");

    let* tekuci = prvi_let;
    while (tekuci != NULL)
    {

        if (tekuci->broj_leta == broj_leta)
        {
            tekuci->otkazan = true;
            printf("Broj leta: %u", tekuci->broj_leta);
            printf("\n");

            printf("Oznaka aviona: %s\n", tekuci->oznaka_aviona);
            printf("Ukupan broj mesta: %u\n", tekuci->ukupan_broj_mesta);
            printf("Datum poletanja: %s\n", tekuci->datum_poletanja);
            printf("Vreme poletanja: %s\n", tekuci->vreme_poletanja);
            printf("Mesto poletanja: %s\n", tekuci->mesto_poletanja);
            printf("Datum sletanja: %s\n", tekuci->datum_sletanja);
            printf("Vreme sletanja: %s\n", tekuci->vreme_sletanja);
            printf("Mesto sletanja: %s\n", tekuci->mesto_sletanja);
            printf("Cena karte: %.2f\n", tekuci->cena_karte);
            printf("Otkazan: %s\n", tekuci->otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
            return false;
        }

        tekuci = tekuci->next;
    }
    return true;

}

void lista_letova_budzet(float budzet)
{
    if (prvi_let == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Lista letova do %.2f RSD -----\n", budzet);

    let* tekuci = prvi_let;
    bool pronadjen = false;

    while (tekuci != NULL)
    {
        if (tekuci->cena_karte <= budzet)
        {
            pronadjen = true;
            printf("Broj leta: %u\n", tekuci->broj_leta);
            printf("Oznaka aviona: %s\n", tekuci->oznaka_aviona);
            printf("Ukupan broj mesta: %u\n", tekuci->ukupan_broj_mesta);
            printf("Datum poletanja: %s\n", tekuci->datum_poletanja);
            printf("Vreme poletanja: %s\n", tekuci->vreme_poletanja);
            printf("Mesto poletanja: %s\n", tekuci->mesto_poletanja);
            printf("Datum sletanja: %s\n", tekuci->datum_sletanja);
            printf("Vreme sletanja: %s\n", tekuci->vreme_sletanja);
            printf("Mesto sletanja: %s\n", tekuci->mesto_sletanja);
            printf("Cena karte: %.2f\n", tekuci->cena_karte);
            printf("Otkazan: %s\n", tekuci->otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
        }

        tekuci = tekuci->next;
    }

    if (pronadjen == false)
    {
        printf("Nema letova u okviru zadatog budzeta.\n");
    }
}

void info_o_letu(unsigned int broj_trazenog_leta)
{
    if (prvi_let == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Informacije o letu %u -----\n", broj_trazenog_leta);

    let* tekuci = prvi_let;
    while (tekuci != NULL)
    {

        if (tekuci->broj_leta == broj_trazenog_leta)
        {
            printf("Broj leta: %u", tekuci->broj_leta);
            printf("\n");

            printf("Oznaka aviona: %s\n", tekuci->oznaka_aviona);
            printf("Ukupan broj mesta: %u\n", tekuci->ukupan_broj_mesta);
            printf("Datum poletanja: %s\n", tekuci->datum_poletanja);
            printf("Vreme poletanja: %s\n", tekuci->vreme_poletanja);
            printf("Mesto poletanja: %s\n", tekuci->mesto_poletanja);
            printf("Datum sletanja: %s\n", tekuci->datum_sletanja);
            printf("Vreme sletanja: %s\n", tekuci->vreme_sletanja);
            printf("Mesto sletanja: %s\n", tekuci->mesto_sletanja);
            printf("Cena karte: %.2f\n", tekuci->cena_karte);
            printf("Otkazan: %s\n", tekuci->otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
            return;

        }
        tekuci = tekuci->next;
    }

}

void putnici_na_letu(unsigned int broj_trazenog_leta)
{
    if (prvi_let == NULL)
    {
        printf("Nema letova u sistemu.\n");
        return;
    }

    let* tekuci_let = prvi_let;
    while (tekuci_let != NULL && tekuci_let->broj_leta != broj_trazenog_leta)
    {
        tekuci_let = tekuci_let->next;
    }

    if (tekuci_let == NULL)
    {
        printf("Let sa brojem %u ne postoji.\n", broj_trazenog_leta);
        return;
    }

    printf("\n--- Putnici na letu broj %u ---\n", broj_trazenog_leta);

    bool ima_putnika = false;
    unsigned int put;

    for (unsigned int i = 0; i < tekuci_let->ukupan_broj_mesta; i++)
    {
        put = tekuci_let->putnici_na_letu[i];
        if (put == 0) // mesto je prazno
            continue;

        putnik* tekuci_putnik = prvi_putnik;
        while (tekuci_putnik != NULL && tekuci_putnik->broj_pasosa != put)
        {
            tekuci_putnik = tekuci_putnik->next;
        }

        if (tekuci_putnik != NULL)
        {
            ima_putnika = true;
            printf("Broj pasosa: %u\n", tekuci_putnik->broj_pasosa);
            printf("ime: %s\n", tekuci_putnik->ime);
            printf("prezime: %s\n", tekuci_putnik->prezime);
            printf("jmbg: %s\n", tekuci_putnik->jmbg);
            printf("Datum rodjenja: %s\n", tekuci_putnik->datum_rodjenja);
            printf("Pol: %c\n", tekuci_putnik->pol);
            printf("----------------------------\n");
        }
        else
        {
            printf("Putnik sa brojem pasosa %u nije pronadjen u sistemu.\n", put);
        }
    }

    if (ima_putnika == false)
    {
        printf("Nema putnika za navedeni broj leta.\n");
    }
}

bool izmeni_let(unsigned int broj_trazenog_leta)
{
    let* trazeni_let = prvi_let;

    while (trazeni_let != NULL && trazeni_let->broj_leta != broj_trazenog_leta) // trazenje leta
        trazeni_let = trazeni_let->next;

    if (trazeni_let == NULL)
    {
        printf("Let sa brojem %u nije pronadjen.\n", broj_trazenog_leta);
        return true;
    }

    printf("Izmena podataka za let broj %u\n", broj_trazenog_leta);

    char izbor;

    printf("Trenutna oznaka aviona: %s Zelite li da je izmenite? (d/n): ", trazeni_let->oznaka_aviona);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novu oznaku aviona (npr. A2): ");
            scanf("%2s", trazeni_let->oznaka_aviona);
        }
        while (!(trazeni_let->oznaka_aviona[0] >= 'A' && trazeni_let->oznaka_aviona[0] <= 'Z' &&
                 trazeni_let->oznaka_aviona[1] >= '0' && trazeni_let->oznaka_aviona[1] <= '9'));
    }

    printf("Trenutni ukupan broj mesta: %u Zelite li da ga izmenite? (d/n): ", trazeni_let->ukupan_broj_mesta);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novi broj mesta: ");
            scanf("%u", &trazeni_let->ukupan_broj_mesta);
        }
        while (trazeni_let->ukupan_broj_mesta == 0 || trazeni_let->ukupan_broj_mesta > 368);
    }

    printf("Trenutni datum poletanja: %s Zelite li da ga izmenite? (d/n): ", trazeni_let->datum_poletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novi datum poletanja: ");
            scanf("%11s", trazeni_let->datum_poletanja);
        }
        while (!validan_datum(trazeni_let->datum_poletanja));
    }

    printf("Trenutno vreme poletanja: %s Zelite li da ga izmenite? (d/n): ", trazeni_let->vreme_poletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novo vreme poletanja: ");
            scanf("%11s", trazeni_let->vreme_poletanja);
        }
        while (!validno_vreme(trazeni_let->vreme_poletanja));
    }

    getchar();

    printf("Trenutno mesto poletanja: %s Zelite li da ga izmenite? (d/n): ", trazeni_let->mesto_poletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        printf("Unesite novo mesto poletanja: ");
        scanf(" %[^\n]", trazeni_let->mesto_poletanja);
    }

    printf("Trenutni datum sletanja: %s Zelite li da ga izmenite? (d/n): ", trazeni_let->datum_sletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novi datum sletanja: ");
            scanf("%11s", trazeni_let->datum_sletanja);
        }
        while (!validan_datum(trazeni_let->datum_sletanja));
    }

    printf("Trenutno vreme sletanja: %s Zelite li da ga izmenite? (d/n): ", trazeni_let->vreme_sletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novo vreme sletanja: ");
            scanf("%11s", trazeni_let->vreme_sletanja);
        }
        while (!validno_vreme(trazeni_let->vreme_sletanja));
    }

    getchar();

    printf("Trenutno mesto sletanja: %s Zelite li da ga izmenite? (d/n): ", trazeni_let->mesto_sletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        printf("Unesite novo mesto sletanja: ");
        scanf(" %[^\n]", trazeni_let->mesto_sletanja);
    }

    printf("Trenutna cena_karte karte: %.2f Zelite li da je izmenite? (d/n): ", trazeni_let->cena_karte);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novu cenu karte (RSD): ");
            scanf("%f", &trazeni_let->cena_karte);
        }
        while (trazeni_let->cena_karte < 0);
    }

    printf("Let je uspesno izmenjen.\n");
    return false;
}

bool izmeni_putnika(unsigned int trazeni_br_pasosa)
{
    putnik* tekuci = prvi_putnik;

    while (tekuci != NULL && tekuci->broj_pasosa != trazeni_br_pasosa)
        tekuci = tekuci->next;

    if (tekuci == NULL)
    {
        printf("Putnik sa brojem pasosa %u nije pronadjen.\n", trazeni_br_pasosa);
        return true;
    }

    printf("Izmena podataka za putnika %u\n", tekuci->broj_pasosa);
    char izbor;

    printf("Trenutno ime: %s Zelite li da ga izmenite? (d/n): ", tekuci->ime);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        printf("Unesite novo ime: ");
        scanf(" %[^\n]", tekuci->ime);
    }

    printf("Trenutno prezime: %s Zelite li da ga izmenite? (d/n): ", tekuci->prezime);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        printf("Unesite novo prezime: ");
        scanf(" %[^\n]", tekuci->prezime);
    }

    printf("Trenutni pol: %c Zelite li da ga izmenite? (d/n): ", tekuci->pol);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        char pol;
        do
        {
            printf("Unesite novi pol (M ili Z): ");
            scanf(" %c", &pol);
        }
        while (pol != 'M' && pol != 'Z' && pol != 'm' && pol != 'z');
        tekuci->pol = toupper(pol); // cisto ako slucajno unese malo slovo
    }

    printf("Trenutni datum rodjenja: %s Zelite li da ga izmenite? (d/n): ", tekuci->datum_rodjenja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novi datum rodjenja (npr. 01.01.2000.): ");
            scanf("%11s", tekuci->datum_rodjenja);
        }
        while (!validan_datum(tekuci->datum_rodjenja));
    }

    printf("Trenutni jmbg: %s Zelite li da ga izmenite? (d/n): ", tekuci->jmbg);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novi jmbg (13 cifara): ");
            scanf("%13s", tekuci->jmbg);
        }
        while (strlen(tekuci->jmbg) != 13);
    }

    printf("Trenutni broj pasosa: %u Zelite li da ga izmenite? (d/n): ", tekuci->broj_pasosa);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novi broj pasosa (9 cifara): ");
            scanf("%u", &tekuci->broj_pasosa);
        }
        while (tekuci->broj_pasosa < 100000000 || tekuci->broj_pasosa > 999999999);
    }

    printf("Putnik je uspesno izmenjen.\n");
    return false;
}

bool obrisi_putnika_sa_leta(unsigned int broj_trazenog_leta, unsigned int trazeni_br_pasosa)
{
    let* tekuci_let = prvi_let;

    while (tekuci_let != NULL && tekuci_let->broj_leta != broj_trazenog_leta)
        tekuci_let = tekuci_let->next;

    if (tekuci_let == NULL)
    {
        printf("Let sa brojem %u nije pronadjen.\n", broj_trazenog_leta);
        return true;
    }


    int index_trazenog_putnika = -1;
    for (unsigned int i = 0; i < tekuci_let->ukupan_broj_mesta; i++) // pronadji pasos u nizu putnika
    {
        if (tekuci_let->putnici_na_letu[i] == trazeni_br_pasosa)
        {
            index_trazenog_putnika = i;
            break;
        }
    }

    if (index_trazenog_putnika == -1)
    {
        printf("Putnik sa pasosem %u nije pronadjen na letu %u.\n", trazeni_br_pasosa, broj_trazenog_leta);
        return true;
    }


    for (unsigned int i = index_trazenog_putnika; i < tekuci_let->ukupan_broj_mesta - 1; i++)
    {
        tekuci_let->putnici_na_letu[i] = tekuci_let->putnici_na_letu[i + 1]; // pomeri ostale ulevo
    }

    tekuci_let->putnici_na_letu[tekuci_let->ukupan_broj_mesta - 1] = 0; // ocisti zadnji slot (ostaje duplikat od pomeranja)

    printf("Putnik sa pasosem %u je uklonjen sa leta %u.\n", trazeni_br_pasosa, broj_trazenog_leta);
    return false;
}

#endif // defined
#endif // AVIOMANAGER_H_INCLUDED
