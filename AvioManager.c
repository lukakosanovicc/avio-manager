/*
 * © 2025 Luka Kosanovic
 * This project is licensed under the MIT License.
 * See the LICENSE file for details.
 */
// return true -> doslo je do greske
// return false -> bez greske (isto kao return 0)

#include "AvioManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

Let* prviLet = NULL;

Putnik* prviPutnik = NULL;

void IspisiMeni()
{
    printf("\n----Dobrodosli u AvioManager! Izaberite:\n");
    printf("\t1 - Kreiranje novog leta\n");
    printf("\t2 - Registracija novog putnika\n");
    printf("\t3 - Lista svih letova\n");
    printf("\t4 - Lista svih letova nekog putnika na osnovu broja pasosa\n"); // u zadatku pise JMBG, ali stranci ga nemaju pa je prakticnije preko broja pasosa
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

void ListaSvihLetova()
{
    if (prviLet == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Lista svih letova -----\n");

    Let* tekuci = prviLet;
    while (tekuci != NULL)
    {

        if (tekuci->OznakaAviona != 0)
        {
            printf("Broj leta: %u", tekuci->BrojLeta);
            printf("\n");

            printf("Oznaka aviona: %s\n", tekuci->OznakaAviona);
            printf("Ukupan broj mesta: %u\n", tekuci->UkupanBrojMesta);
            printf("Datum poletanja: %s\n", tekuci->DatumPoletanja);
            printf("Vreme poletanja: %s\n", tekuci->VremePoletanja);
            printf("Mesto poletanja: %s\n", tekuci->MestoPoletanja);
            printf("Datum sletanja: %s\n", tekuci->DatumSletanja);
            printf("Vreme sletanja: %s\n", tekuci->VremeSletanja);
            printf("Mesto sletanja: %s\n", tekuci->MestoSletanja);
            printf("Cena karte: %.2f\n", tekuci->Cena);
            printf("Otkazan: %s\n", tekuci->Otkazan ? "Da" : "Ne");
            printf("----------------------------\n");

            tekuci = tekuci->next;
        }
    }
}

bool KreirajLet()
{
    Let* noviLet = (Let*)malloc(sizeof(Let));
    if (noviLet == NULL)
    {
        printf("Greska pri alokaciji memorije!\n");
        return true;
    }

    do
    {
        printf("Unesite broj leta (do 5 cifara, npr. 12345): ");
        scanf("%u", &noviLet->BrojLeta);
    }
    while (noviLet->BrojLeta > 99999 || noviLet->BrojLeta < 1);

    do
    {
        printf("Unesite oznaku aviona (npr. A2): ");
        scanf("%2s", noviLet->OznakaAviona);

        // Debug: ispisi unetu oznaku
        printf("Uneta oznaka: %s\n", noviLet->OznakaAviona);

    }
    while (!(noviLet->OznakaAviona[0] >= 'A' && noviLet->OznakaAviona[0] <= 'Z' && noviLet->OznakaAviona[1] >= '0' && noviLet->OznakaAviona[1] <= '9'));

    do
    {
        printf("Unesite ukupan broj mesta: ");
        scanf("%u", &noviLet->UkupanBrojMesta); // boeing 777 -> 368 mesta max
    }
    while (noviLet->UkupanBrojMesta > 368 || noviLet->UkupanBrojMesta == 0);

    do
    {
        printf("Unesite datum poletanja (npr. 20.05.2025.): ");
        scanf("%11s", noviLet->DatumPoletanja);
    }
    while (!ValidanDatum(noviLet->DatumPoletanja));

    do
    {
        printf("Unesite vreme poletanja (npr. 14:20): ");
        scanf("%11s", noviLet->VremePoletanja);
    }
    while (!ValidnoVreme(noviLet->VremePoletanja));

    getchar(); // da pocisti '\n'

    printf("Unesite mesto poletanja: ");
    scanf(" %[^\n]", noviLet->MestoPoletanja);

    do
    {
        printf("Unesite datum sletanja (npr. 20.05.2025.): ");
        scanf("%11s", noviLet->DatumSletanja);
    }
    while (!ValidanDatum(noviLet->DatumSletanja));

    do
    {
        printf("Unesite vreme sletanja (npr. 14:20): ");
        scanf("%11s", noviLet->VremeSletanja);
    }
    while (!ValidnoVreme(noviLet->VremeSletanja));

    getchar();

    printf("Unesite mesto sletanja: ");
    scanf(" %[^\n]", noviLet->MestoSletanja);

    do
    {
        printf("Unesite cenu karte (u RSD): ");
        scanf("%f", &noviLet->Cena);
    }
    while (noviLet->Cena < 0);

    noviLet->Otkazan = false;
    noviLet->next = NULL;

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
            if (index < noviLet->UkupanBrojMesta)
            {
                noviLet->PutniciNaLetu[index++] = br; // stavi ga na trenutni index, zatim uvecaj index (brojac) za jedan
            }
            else
            {
                printf("Dostignut maksimalan broj putnika!\n");
                break;
            }
        }
    }
    while (true);

    if (prviLet == NULL)
    {
        prviLet = noviLet;
    }
    else
    {
        Let* tekuci = prviLet;
        while (tekuci->next != NULL)
            tekuci = tekuci->next;

        tekuci->next = noviLet;
    }

    printf("Let je uspesno kreiran!\n");
    return false;
}

bool NoviPutnik()
{
    Putnik* noviPutnik = (Putnik*)malloc(sizeof(Putnik));
    if (!noviPutnik)
    {
        printf("Greska pri alokaciji memorije!\n");
        return true;
    }

    do
    {
        printf("Unesite JMBG (13 cifara): ");
        scanf("%13s", noviPutnik->JMBG);
    }
    while (strlen(noviPutnik->JMBG) != 13);

    do
    {
        printf("Unesite ime: ");
        scanf("%14s", noviPutnik->Ime);
    }
    while (strlen(noviPutnik->Ime) > 14 || strlen(noviPutnik->Ime) < 2);

    do
    {
        printf("Unesite prezime: ");
        scanf("%14s", noviPutnik->Prezime);
    }
    while (strlen(noviPutnik->Prezime) > 14 || strlen(noviPutnik->Prezime) < 2);

    do
    {
        printf("Unesite datum rodjenja (npr. 20.05.2000.): ");
        scanf("%11s", noviPutnik->DatumRodjenja);
    }
    while (!ValidanDatum(noviPutnik->DatumRodjenja));

    do
    {
        printf("Unesite pol (M ili Z): ");
        scanf(" %c", &noviPutnik->Pol);
    }
    while (noviPutnik->Pol != 'M' && noviPutnik->Pol != 'Z');

    do
    {
        printf("Unesite broj pasosa (9 cifara): ");
        scanf("%9u", &noviPutnik->BrojPasosa);
    }
    while (noviPutnik->BrojPasosa < 100000000 || noviPutnik->BrojPasosa > 999999999);

    noviPutnik->next = NULL;

    if (prviPutnik == NULL)
        prviPutnik = noviPutnik;
    else
    {
        Putnik* tekuci = prviPutnik;
        while (tekuci->next != NULL)
            tekuci = tekuci->next;
        tekuci->next = noviPutnik;
    }

    printf("Putnik je uspesno kreiran!\n");
    return false;
}

bool ValidanDatum(const char* datum)
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

bool ValidnoVreme(const char* vreme)
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

bool SacuvajLetove(Let* prviLet)
{
    FILE *f = fopen("Letovi.dat", "wb");
    if (!f) // svejedno je, moze i if (f == NULL)
    {
        printf("Greska pri otvaranju fajla za pisanje.\n");
        return true;
    }

    Let* tekuci = prviLet;
    while (tekuci != NULL)
    {
        fwrite(tekuci, sizeof(Let), 1, f);
        tekuci = tekuci->next;
    }

    fclose(f);
    return false;
}

bool SacuvajPutnike(Putnik* prviPutnik)
{
    FILE *f = fopen("Putnici.dat", "wb");
    if (!f)
    {
        printf("Greska pri otvaranju fajla za pisanje.\n");
        return true;
    }

    Putnik* tekuci = prviPutnik;
    while (tekuci != NULL)
    {
        fwrite(tekuci, sizeof(Putnik), 1, f);
        tekuci = tekuci->next;
    }

    fclose(f);
    return false;
}

bool UcitajLetove()
{
    FILE *f = fopen("Letovi.dat", "rb");
    if (!f) return true;

    prviLet = NULL;

    Let temp;
    while (fread(&temp, sizeof(Let), 1, f) == 1)
    {
        Let* novi = (Let*)malloc(sizeof(Let));
        if (!novi)
        {
            printf("Greska pri alokaciji memorije.\n");
            fclose(f);
            return true;
        }

        *novi = temp;
        novi->next = NULL;

        if (prviLet == NULL)
        {
            prviLet = novi;
        }
        else
        {
            Let* tekuci = prviLet;
            while (tekuci->next != NULL)
                tekuci = tekuci->next;

            tekuci->next = novi;
        }
    }

    fclose(f);
    return false;
}

bool UcitajPutnike()
{
    FILE *f = fopen("Putnici.dat", "rb");
    if (!f) return true;

    prviPutnik = NULL;

    Putnik temp;
    while (fread(&temp, sizeof(Putnik), 1, f) == 1)
    {
        Putnik* novi = (Putnik*)malloc(sizeof(Putnik));
        if (!novi)
        {
            printf("Greska pri alokaciji memorije.\n");
            fclose(f);
            return true;
        }

        *novi = temp;
        novi->next = NULL;

        if (prviPutnik == NULL)
        {
            prviPutnik = novi;
        }
        else
        {
            Putnik* tekuci = prviPutnik;
            while (tekuci->next != NULL)
                tekuci = tekuci->next;

            tekuci->next = novi;
        }
    }

    fclose(f);
    return false;
}

void OslobodiLetove(Let* prviLet)
{
    Let* temp;
    while (prviLet != NULL) {
        temp = prviLet;
        prviLet = prviLet->next;
        free(temp);
    }
}

void OslobodiPutnike(Putnik* prviPutnik)
{
    Putnik* temp;
    while (prviPutnik != NULL) {
        temp = prviPutnik;
        prviPutnik = prviPutnik->next;
        free(temp);
    }
}

// -----------------------------------------------------------------------------

void ListaLetovaPutnika(unsigned int trazeni_brPasosa)
{
    if (prviLet == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Lista letova putnika %u -----\n", trazeni_brPasosa);

    Let* tekuci = prviLet;
    while (tekuci != NULL)
    {
        int pronadjen = 0;

        for (int i = 0; i < 368; i++)
        {
            if (tekuci->PutniciNaLetu[i] == trazeni_brPasosa)
            {
                pronadjen = 1;
                break;
            }
        }

        if (pronadjen)
        {
            printf("Broj leta: %u\n", tekuci->BrojLeta);
            printf("Oznaka aviona: %s\n", tekuci->OznakaAviona);
            printf("Ukupan broj mesta: %u\n", tekuci->UkupanBrojMesta);
            printf("Datum poletanja: %s\n", tekuci->DatumPoletanja);
            printf("Vreme poletanja: %s\n", tekuci->VremePoletanja);
            printf("Mesto poletanja: %s\n", tekuci->MestoPoletanja);
            printf("Datum sletanja: %s\n", tekuci->DatumSletanja);
            printf("Vreme sletanja: %s\n", tekuci->VremeSletanja);
            printf("Mesto sletanja: %s\n", tekuci->MestoSletanja);
            printf("Cena karte: %.2f\n", tekuci->Cena);
            printf("Otkazan: %s\n", tekuci->Otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
        }

        tekuci = tekuci->next;
    }
}

void ListaLetovaDo(const char* destinacija)
{
    if (prviLet == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Lista letova do %s -----\n", destinacija);

    Let* tekuci = prviLet;
    bool pronadjen = false;

    while (tekuci != NULL)
    {
        if (strcmp(tekuci->MestoSletanja, destinacija) == 0) // ako su ova dva stringa podudarni...
        {
            pronadjen = true;

            printf("Broj leta: %u\n", tekuci->BrojLeta);
            printf("Oznaka aviona: %s\n", tekuci->OznakaAviona);
            printf("Ukupan broj mesta: %u\n", tekuci->UkupanBrojMesta);
            printf("Datum poletanja: %s\n", tekuci->DatumPoletanja);
            printf("Vreme poletanja: %s\n", tekuci->VremePoletanja);
            printf("Mesto poletanja: %s\n", tekuci->MestoPoletanja);
            printf("Datum sletanja: %s\n", tekuci->DatumSletanja);
            printf("Vreme sletanja: %s\n", tekuci->VremeSletanja);
            printf("Mesto sletanja: %s\n", tekuci->MestoSletanja);
            printf("Cena karte: %.2f\n", tekuci->Cena);
            printf("Otkazan: %s\n", tekuci->Otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
        }

        tekuci = tekuci->next;
    }

    if (pronadjen == false)
    {
        printf("Nema letova do te destinacije.\n");
    }
}

void ListaLetovaOd(const char* polazak)
{
    if (prviLet == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Lista letova od %s -----\n", polazak);

    Let* tekuci = prviLet;
    bool pronadjen = false;

    while (tekuci != NULL)
    {
        if (strcmp(tekuci->MestoPoletanja, polazak) == 0)
        {
            pronadjen = true;

            printf("Broj leta: %u\n", tekuci->BrojLeta);
            printf("Oznaka aviona: %s\n", tekuci->OznakaAviona);
            printf("Ukupan broj mesta: %u\n", tekuci->UkupanBrojMesta);
            printf("Datum poletanja: %s\n", tekuci->DatumPoletanja);
            printf("Vreme poletanja: %s\n", tekuci->VremePoletanja);
            printf("Mesto poletanja: %s\n", tekuci->MestoPoletanja);
            printf("Datum sletanja: %s\n", tekuci->DatumSletanja);
            printf("Vreme sletanja: %s\n", tekuci->VremeSletanja);
            printf("Mesto sletanja: %s\n", tekuci->MestoSletanja);
            printf("Cena karte: %.2f\n", tekuci->Cena);
            printf("Otkazan: %s\n", tekuci->Otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
        }

        tekuci = tekuci->next;
    }

    if (pronadjen == false)
    {
        printf("Nema letova od tog mesta.\n");
    }
}

bool OtkaziLet(unsigned int BrojLeta)
{
    if (prviLet == NULL)
    {
        printf("Nema letova za otkazivanje.\n");
        return true;
    }

    printf("\n-----------------------------\n");

    Let* tekuci = prviLet;
    while (tekuci != NULL)
    {

        if (tekuci->BrojLeta == BrojLeta)
        {
            tekuci->Otkazan = true;
            printf("Broj leta: %u", tekuci->BrojLeta);
            printf("\n");

            printf("Oznaka aviona: %s\n", tekuci->OznakaAviona);
            printf("Ukupan broj mesta: %u\n", tekuci->UkupanBrojMesta);
            printf("Datum poletanja: %s\n", tekuci->DatumPoletanja);
            printf("Vreme poletanja: %s\n", tekuci->VremePoletanja);
            printf("Mesto poletanja: %s\n", tekuci->MestoPoletanja);
            printf("Datum sletanja: %s\n", tekuci->DatumSletanja);
            printf("Vreme sletanja: %s\n", tekuci->VremeSletanja);
            printf("Mesto sletanja: %s\n", tekuci->MestoSletanja);
            printf("Cena karte: %.2f\n", tekuci->Cena);
            printf("Otkazan: %s\n", tekuci->Otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
            return false;
        }

        tekuci = tekuci->next;
    }
    return true;

}

void ListaLetovaBudzet(float budzet)
{
    if (prviLet == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Lista letova do %.2f RSD -----\n", budzet);

    Let* tekuci = prviLet;
    bool pronadjen = false;

    while (tekuci != NULL)
    {
        if (tekuci->Cena <= budzet)
        {
            pronadjen = true;
            printf("Broj leta: %u\n", tekuci->BrojLeta);
            printf("Oznaka aviona: %s\n", tekuci->OznakaAviona);
            printf("Ukupan broj mesta: %u\n", tekuci->UkupanBrojMesta);
            printf("Datum poletanja: %s\n", tekuci->DatumPoletanja);
            printf("Vreme poletanja: %s\n", tekuci->VremePoletanja);
            printf("Mesto poletanja: %s\n", tekuci->MestoPoletanja);
            printf("Datum sletanja: %s\n", tekuci->DatumSletanja);
            printf("Vreme sletanja: %s\n", tekuci->VremeSletanja);
            printf("Mesto sletanja: %s\n", tekuci->MestoSletanja);
            printf("Cena karte: %.2f\n", tekuci->Cena);
            printf("Otkazan: %s\n", tekuci->Otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
        }

        tekuci = tekuci->next;
    }

    if (pronadjen == false)
    {
        printf("Nema letova u okviru zadatog budzeta.\n");
    }
}

void InfoOLetu(unsigned int BrojTrazenogLeta)
{
    if (prviLet == NULL)
    {
        printf("Nema letova za prikaz.\n");
        return;
    }

    printf("\n----- Informacije o letu %u -----\n", BrojTrazenogLeta);

    Let* tekuci = prviLet;
    while (tekuci != NULL)
    {

        if (tekuci->BrojLeta == BrojTrazenogLeta)
        {
            printf("Broj leta: %u", tekuci->BrojLeta);
            printf("\n");

            printf("Oznaka aviona: %s\n", tekuci->OznakaAviona);
            printf("Ukupan broj mesta: %u\n", tekuci->UkupanBrojMesta);
            printf("Datum poletanja: %s\n", tekuci->DatumPoletanja);
            printf("Vreme poletanja: %s\n", tekuci->VremePoletanja);
            printf("Mesto poletanja: %s\n", tekuci->MestoPoletanja);
            printf("Datum sletanja: %s\n", tekuci->DatumSletanja);
            printf("Vreme sletanja: %s\n", tekuci->VremeSletanja);
            printf("Mesto sletanja: %s\n", tekuci->MestoSletanja);
            printf("Cena karte: %.2f\n", tekuci->Cena);
            printf("Otkazan: %s\n", tekuci->Otkazan ? "Da" : "Ne");
            printf("----------------------------\n");
            return;

        }
        tekuci = tekuci->next;
    }

}

void PutniciNaLetu(unsigned int BrojTrazenogLeta)
{
    if (prviLet == NULL)
    {
        printf("Nema letova u sistemu.\n");
        return;
    }

    Let* tekuciLet = prviLet;
    while (tekuciLet != NULL && tekuciLet->BrojLeta != BrojTrazenogLeta)
    {
        tekuciLet = tekuciLet->next;
    }

    if (tekuciLet == NULL)
    {
        printf("Let sa brojem %u ne postoji.\n", BrojTrazenogLeta);
        return;
    }

    printf("\n--- Putnici na letu broj %u ---\n", BrojTrazenogLeta);

    bool imaPutnika = false;
    unsigned int put;

    for (unsigned int i = 0; i < tekuciLet->UkupanBrojMesta; i++)
    {
        put = tekuciLet->PutniciNaLetu[i];
        if (put == 0) // mesto je prazno
            continue;

        Putnik* tekuciPutnik = prviPutnik;
        while (tekuciPutnik != NULL && tekuciPutnik->BrojPasosa != put)
        {
            tekuciPutnik = tekuciPutnik->next;
        }

        if (tekuciPutnik != NULL)
        {
            imaPutnika = true;
            printf("Broj pasosa: %u\n", tekuciPutnik->BrojPasosa);
            printf("Ime: %s\n", tekuciPutnik->Ime);
            printf("Prezime: %s\n", tekuciPutnik->Prezime);
            printf("JMBG: %s\n", tekuciPutnik->JMBG);
            printf("Datum rodjenja: %s\n", tekuciPutnik->DatumRodjenja);
            printf("Pol: %c\n", tekuciPutnik->Pol);
            printf("----------------------------\n");
        }
        else
        {
            printf("Putnik sa brojem pasosa %u nije pronadjen u sistemu.\n", put);
        }
    }

    if (imaPutnika == false)
    {
        printf("Nema putnika za navedeni broj leta.\n");
    }
}

bool IzmeniLet(unsigned int BrojTrazenogLeta)
{
    Let* trazeniLet = prviLet;

    while (trazeniLet != NULL && trazeniLet->BrojLeta != BrojTrazenogLeta) // trazenje leta
        trazeniLet = trazeniLet->next;

    if (trazeniLet == NULL)
    {
        printf("Let sa brojem %u nije pronadjen.\n", BrojTrazenogLeta);
        return true;
    }

    printf("Izmena podataka za let broj %u\n", BrojTrazenogLeta);

    char izbor;

    printf("Trenutna oznaka aviona: %s Zelite li da je izmenite? (d/n): ", trazeniLet->OznakaAviona);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do {
            printf("Unesite novu oznaku aviona (npr. A2): ");
            scanf("%2s", trazeniLet->OznakaAviona);
        } while (!(trazeniLet->OznakaAviona[0] >= 'A' && trazeniLet->OznakaAviona[0] <= 'Z' &&
                   trazeniLet->OznakaAviona[1] >= '0' && trazeniLet->OznakaAviona[1] <= '9'));
    }

    printf("Trenutni ukupan broj mesta: %u Zelite li da ga izmenite? (d/n): ", trazeniLet->UkupanBrojMesta);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do {
            printf("Unesite novi broj mesta: ");
            scanf("%u", &trazeniLet->UkupanBrojMesta);
        } while (trazeniLet->UkupanBrojMesta == 0 || trazeniLet->UkupanBrojMesta > 368);
    }

    printf("Trenutni datum poletanja: %s Zelite li da ga izmenite? (d/n): ", trazeniLet->DatumPoletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do {
            printf("Unesite novi datum poletanja: ");
            scanf("%11s", trazeniLet->DatumPoletanja);
        } while (!ValidanDatum(trazeniLet->DatumPoletanja));
    }

    printf("Trenutno vreme poletanja: %s Zelite li da ga izmenite? (d/n): ", trazeniLet->VremePoletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do {
            printf("Unesite novo vreme poletanja: ");
            scanf("%11s", trazeniLet->VremePoletanja);
        } while (!ValidnoVreme(trazeniLet->VremePoletanja));
    }

    getchar();

    printf("Trenutno mesto poletanja: %s Zelite li da ga izmenite? (d/n): ", trazeniLet->MestoPoletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        printf("Unesite novo mesto poletanja: ");
        scanf(" %[^\n]", trazeniLet->MestoPoletanja);
    }

    printf("Trenutni datum sletanja: %s Zelite li da ga izmenite? (d/n): ", trazeniLet->DatumSletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do {
            printf("Unesite novi datum sletanja: ");
            scanf("%11s", trazeniLet->DatumSletanja);
        } while (!ValidanDatum(trazeniLet->DatumSletanja));
    }

    printf("Trenutno vreme sletanja: %s Zelite li da ga izmenite? (d/n): ", trazeniLet->VremeSletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do {
            printf("Unesite novo vreme sletanja: ");
            scanf("%11s", trazeniLet->VremeSletanja);
        } while (!ValidnoVreme(trazeniLet->VremeSletanja));
    }

    getchar();

    printf("Trenutno mesto sletanja: %s Zelite li da ga izmenite? (d/n): ", trazeniLet->MestoSletanja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        printf("Unesite novo mesto sletanja: ");
        scanf(" %[^\n]", trazeniLet->MestoSletanja);
    }

    printf("Trenutna cena karte: %.2f Zelite li da je izmenite? (d/n): ", trazeniLet->Cena);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do {
            printf("Unesite novu cenu karte (RSD): ");
            scanf("%f", &trazeniLet->Cena);
        } while (trazeniLet->Cena < 0);
    }

    printf("Let je uspesno izmenjen.\n");
    return false;
}

bool IzmeniPutnika(unsigned int trazeni_brPasosa)
{
    Putnik* tekuci = prviPutnik;

    while (tekuci != NULL && tekuci->BrojPasosa != trazeni_brPasosa)
        tekuci = tekuci->next;

    if (tekuci == NULL)
    {
        printf("Putnik sa brojem pasosa %u nije pronadjen.\n", trazeni_brPasosa);
        return true;
    }

    printf("Izmena podataka za putnika %u\n", tekuci->BrojPasosa);
    char izbor;

    printf("Trenutno ime: %s Zelite li da ga izmenite? (d/n): ", tekuci->Ime);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        printf("Unesite novo ime: ");
        scanf(" %[^\n]", tekuci->Ime);
    }

    printf("Trenutno prezime: %s Zelite li da ga izmenite? (d/n): ", tekuci->Prezime);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        printf("Unesite novo prezime: ");
        scanf(" %[^\n]", tekuci->Prezime);
    }

    printf("Trenutni pol: %c Zelite li da ga izmenite? (d/n): ", tekuci->Pol);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        char pol;
        do {
            printf("Unesite novi pol (M ili Z): ");
            scanf(" %c", &pol);
        } while (pol != 'M' && pol != 'Z' && pol != 'm' && pol != 'z');
        tekuci->Pol = toupper(pol); // cisto ako slucajno unese malo slovo
    }

    printf("Trenutni datum rodjenja: %s Zelite li da ga izmenite? (d/n): ", tekuci->DatumRodjenja);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do {
            printf("Unesite novi datum rodjenja (npr. 01.01.2000.): ");
            scanf("%11s", tekuci->DatumRodjenja);
        } while (!ValidanDatum(tekuci->DatumRodjenja));
    }

    printf("Trenutni JMBG: %s Zelite li da ga izmenite? (d/n): ", tekuci->JMBG);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novi JMBG (13 cifara): ");
            scanf("%13s", tekuci->JMBG);
        } while (strlen(tekuci->JMBG) != 13);
    }

    printf("Trenutni broj pasosa: %u Zelite li da ga izmenite? (d/n): ", tekuci->BrojPasosa);
    scanf(" %c", &izbor);
    if (izbor == 'd' || izbor == 'D')
    {
        do
        {
            printf("Unesite novi broj pasosa (9 cifara): ");
            scanf("%u", &tekuci->BrojPasosa);
        } while (tekuci->BrojPasosa < 100000000 || tekuci->BrojPasosa > 999999999);
    }

    printf("Putnik je uspesno izmenjen.\n");
    return false;
}

bool ObrisiPutnikaSaLeta(unsigned int BrojTrazenogLeta, unsigned int trazeni_brPasosa)
{
    Let* tekuciLet = prviLet;

    while (tekuciLet != NULL && tekuciLet->BrojLeta != BrojTrazenogLeta)
        tekuciLet = tekuciLet->next;

    if (tekuciLet == NULL)
    {
        printf("Let sa brojem %u nije pronadjen.\n", BrojTrazenogLeta);
        return true;
    }


    int indexTrazenogPutnika = -1;
    for (unsigned int i = 0; i < tekuciLet->UkupanBrojMesta; i++) // pronadji pasos u nizu putnika
    {
        if (tekuciLet->PutniciNaLetu[i] == trazeni_brPasosa)
        {
            indexTrazenogPutnika = i;
            break;
        }
    }

    if (indexTrazenogPutnika == -1)
    {
        printf("Putnik sa pasosem %u nije pronadjen na letu %u.\n", trazeni_brPasosa, BrojTrazenogLeta);
        return true;
    }


    for (unsigned int i = indexTrazenogPutnika; i < tekuciLet->UkupanBrojMesta - 1; i++)
    {
        tekuciLet->PutniciNaLetu[i] = tekuciLet->PutniciNaLetu[i + 1]; // pomeri ostale ulevo
    }

    tekuciLet->PutniciNaLetu[tekuciLet->UkupanBrojMesta - 1] = 0; // ocisti zadnji slot (ostaje duplikat od pomeranja)

    printf("Putnik sa pasosem %u je uklonjen sa leta %u.\n", trazeni_brPasosa, BrojTrazenogLeta);
    return false;
}
