/*
 * © 2025 Luka Kosanovic
 * This project is licensed under the MIT License.
 * See the LICENSE file for details.
 */
#include "AvioManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

int main()
{
    printf("\n\t\t\t\t\t\tLuka Kosanovic 2025\n\n");

    if (UcitajLetove("Letovi.dat") == false) printf("Letovi su uspesno ucitani.\n");
    else printf("Nema prethodno sacuvanih letova ili je doslo do greske.\n");

    if (UcitajPutnike("Putnici.dat") == false) printf("Putnici su uspesno ucitani.\n");
    else printf("Nema prethodno sacuvanih putnika ili je doslo do greske.\n");

    printf("\nUkoliko zelite da izbrisete putnike i letove, izbrisite Putnici.dat i Letovi.dat iz istog foldera kao i ova aplikacija");

    printf("\n\nSAVET: Ukoliko pokrecete ovaj program po prvi put/prazni su fajlovi za ucitavanje, prvo napravite putnika pa onda let\n\n");

    unsigned int izbor;
    do
    {
        IspisiMeni();

        scanf("%u", &izbor);

        switch(izbor)
        {
        case 1:
            KreirajLet();
            break;
        case 2:
            NoviPutnik();
            break;
        case 3:
            ListaSvihLetova();
            break;
        case 4:
        {
            unsigned int trazeni_brPasosa = 0;
            do
            {
                printf("Unesite broj pasosa (9 cifara): ");
                scanf("%u", &trazeni_brPasosa);
            }
            while (trazeni_brPasosa < 100000000 || trazeni_brPasosa > 999999999);
            ListaLetovaPutnika(trazeni_brPasosa);
            break;
        }
        case 5:
        {
            char MestoSletanja[30];
            printf("\nUnesite do koje destinacije: ");
            scanf(" %[^\n]", MestoSletanja);
            ListaLetovaDo(MestoSletanja);
            break;
        }
        case 6:
        {
            char MestoPoletanja[30];
            printf("\nUnesite od kog mesta: ");
            scanf(" %[^\n]", MestoPoletanja);
            ListaLetovaOd(MestoPoletanja);
            break;
        }
        case 7:
        {
            float budzet;
            printf("\nUnesite Vas budzet: ");
            do
            {
                scanf("%f", &budzet);
            } while (budzet < 0);
            ListaLetovaBudzet(budzet);
            break;
        }
        case 8:
        {
            unsigned int BrojTrazenogLeta;
            printf("\nUnesite broj leta: ");
            do
            {
                printf("Unesite broj leta (5 cifara, npr. 12345): ");
                scanf("%u", &BrojTrazenogLeta);
            } while (BrojTrazenogLeta > 99999 || BrojTrazenogLeta < 1);
            InfoOLetu(BrojTrazenogLeta);
            break;
        }
        case 9:
        {
            unsigned int BrojTrazenogLeta;
            printf("\nUnesite broj leta: ");
            do
            {
                printf("Unesite broj leta (5 cifara, npr. 12345): ");
                scanf("%u", &BrojTrazenogLeta);
            } while (BrojTrazenogLeta > 99999 || BrojTrazenogLeta < 1);
            PutniciNaLetu(BrojTrazenogLeta);
            break;
        }
        case 10:
        {
            unsigned int BrojTrazenogLeta;
            printf("\nUnesite broj leta: ");
            do
            {
                printf("Unesite broj leta (5 cifara, npr. 12345): ");
                scanf("%u", &BrojTrazenogLeta);
            } while (BrojTrazenogLeta > 99999 || BrojTrazenogLeta < 1);
            IzmeniLet(BrojTrazenogLeta);
            break;
        }
        case 11:
        {
            unsigned int trazeni_brPasosa = 0;
            do
            {
                printf("Unesite broj pasosa (9 cifara): ");
                scanf("%u", &trazeni_brPasosa);
            }
            while (trazeni_brPasosa < 100000000 || trazeni_brPasosa > 999999999);
            IzmeniPutnika(trazeni_brPasosa);
            break;
        }
        case 12:
        {
            unsigned int trazeni_brPasosa = 0;
            do
            {
                printf("Unesite broj pasosa (9 cifara): ");
                scanf("%u", &trazeni_brPasosa);
            }
            while (trazeni_brPasosa < 100000000 || trazeni_brPasosa > 999999999);

            fflush(stdin);

            unsigned int BrojTrazenogLeta;
            printf("\nUnesite broj leta: ");
            do
            {
                printf("Unesite broj leta (5 cifara, npr. 12345): ");
                scanf("%u", &BrojTrazenogLeta);
            } while (BrojTrazenogLeta > 99999 || BrojTrazenogLeta < 1);

            ObrisiPutnikaSaLeta(BrojTrazenogLeta, trazeni_brPasosa);
            break;
        }
        case 13:
        {
            unsigned int brojLeta;
            do
            {
                printf("Unesite broj leta (do 5 cifara, npr. 12345): ");
                scanf("%u", &brojLeta);
            }
            while (brojLeta > 99999 || brojLeta < 1);

            OtkaziLet(brojLeta);
            break;
        }


        case 0:
            if (SacuvajLetove(prviLet) == false) printf("Letovi su uspesno sacuvani.\n");
            else printf("Doslo je do greske prilikom cuvanja letova...\n");

            if (SacuvajPutnike(prviPutnik) == false) printf("Putnici su uspesno sacuvani.\n");
            else printf("Doslo je do greske prilikom cuvanja putnika...\n");
            break;
        default:
            printf("Nije uneta ispravna vrednost!\n");
            break;
        }

    } while (izbor != 0);

    OslobodiLetove(prviLet);
    OslobodiPutnike(prviPutnik);
    printf("Dovidjenja!\n");

    return 0;
}
