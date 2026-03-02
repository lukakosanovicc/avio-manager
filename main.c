/*
 * © 2025 Luka Kosanovic
 * This project is licensed under the MIT License.
 * See the LICENSE file for details.
 */
#define AVIOMANAGER_IMPLEMENTATION
#include "avio_manager.h"

#include <stdio.h>

int main()
{
    printf("\n\t\t\t\t\t\tLuka Kosanovic 2025\n\n");

    if (ucitaj_letove("letovi.dat") == false) printf("Letovi su uspesno ucitani.\n");
    else printf("Nema prethodno sacuvanih letova ili je doslo do greske.\n");

    if (ucitaj_putnike("putnici.dat") == false) printf("Putnici su uspesno ucitani.\n");
    else printf("Nema prethodno sacuvanih putnika ili je doslo do greske.\n");

    printf("\nUkoliko zelite da izbrisete putnike i letove, izbrisite putnici.dat i letovi.dat iz istog foldera kao i ova aplikacija");

    printf("\n\nSAVET: Ukoliko pokrecete ovaj program po prvi put/prazni su fajlovi za ucitavanje, prvo napravite putnika pa onda let\n\n");

    unsigned int izbor;
    do
    {
        ispisi_meni();

        scanf("%u", &izbor);

        switch(izbor)
        {
        case 1:
            kreiraj_let();
            break;
        case 2:
            novi_putnik();
            break;
        case 3:
            lista_svih_letova();
            break;
        case 4:
        {
            unsigned int trazeni_br_pasosa = 0;
            do
            {
                printf("Unesite broj pasosa (9 cifara): ");
                scanf("%u", &trazeni_br_pasosa);
            }
            while (trazeni_br_pasosa < 100000000 || trazeni_br_pasosa > 999999999);
            lista_letova_putnika(trazeni_br_pasosa);
            break;
        }
        case 5:
        {
            char mesto_sletanja[30];
            printf("\nUnesite do koje destinacije: ");
            scanf(" %[^\n]", mesto_sletanja);
            lista_letova_do(mesto_sletanja);
            break;
        }
        case 6:
        {
            char mesto_poletanja[30];
            printf("\nUnesite od kog mesta: ");
            scanf(" %[^\n]", mesto_poletanja);
            lista_letova_od(mesto_poletanja);
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
            lista_letova_budzet(budzet);
            break;
        }
        case 8:
        {
            unsigned int broj_trazenog_leta;
            printf("\nUnesite broj leta: ");
            do
            {
                printf("Unesite broj leta (5 cifara, npr. 12345): ");
                scanf("%u", &broj_trazenog_leta);
            } while (broj_trazenog_leta > 99999 || broj_trazenog_leta < 1);
            info_o_letu(broj_trazenog_leta);
            break;
        }
        case 9:
        {
            unsigned int broj_trazenog_leta;
            printf("\nUnesite broj leta: ");
            do
            {
                printf("Unesite broj leta (5 cifara, npr. 12345): ");
                scanf("%u", &broj_trazenog_leta);
            } while (broj_trazenog_leta > 99999 || broj_trazenog_leta < 1);
            putnici_na_letu(broj_trazenog_leta);
            break;
        }
        case 10:
        {
            unsigned int broj_trazenog_leta;
            printf("\nUnesite broj leta: ");
            do
            {
                printf("Unesite broj leta (5 cifara, npr. 12345): ");
                scanf("%u", &broj_trazenog_leta);
            } while (broj_trazenog_leta > 99999 || broj_trazenog_leta < 1);
            izmeni_let(broj_trazenog_leta);
            break;
        }
        case 11:
        {
            unsigned int trazeni_br_pasosa = 0;
            do
            {
                printf("Unesite broj pasosa (9 cifara): ");
                scanf("%u", &trazeni_br_pasosa);
            }
            while (trazeni_br_pasosa < 100000000 || trazeni_br_pasosa > 999999999);
            izmeni_putnika(trazeni_br_pasosa);
            break;
        }
        case 12:
        {
            unsigned int trazeni_br_pasosa = 0;
            do
            {
                printf("Unesite broj pasosa (9 cifara): ");
                scanf("%u", &trazeni_br_pasosa);
            }
            while (trazeni_br_pasosa < 100000000 || trazeni_br_pasosa > 999999999);

            fflush(stdin);

            unsigned int broj_trazenog_leta;
            printf("\nUnesite broj leta: ");
            do
            {
                printf("Unesite broj leta (5 cifara, npr. 12345): ");
                scanf("%u", &broj_trazenog_leta);
            } while (broj_trazenog_leta > 99999 || broj_trazenog_leta < 1);

            obrisi_putnika_sa_leta(broj_trazenog_leta, trazeni_br_pasosa);
            break;
        }
        case 13:
        {
            unsigned int broj_leta;
            do
            {
                printf("Unesite broj leta (do 5 cifara, npr. 12345): ");
                scanf("%u", &broj_leta);
            }
            while (broj_leta > 99999 || broj_leta < 1);

            otkazi_let(broj_leta);
            break;
        }


        case 0:
            if (sacuvaj_letove(prvi_let) == false) printf("Letovi su uspesno sacuvani.\n");
            else printf("Doslo je do greske prilikom cuvanja letova...\n");

            if (sacuvaj_putnike(prvi_putnik) == false) printf("Putnici su uspesno sacuvani.\n");
            else printf("Doslo je do greske prilikom cuvanja putnika...\n");
            break;
        default:
            printf("Nije uneta ispravna vrednost!\n");
            break;
        }

    } while (izbor != 0);

    oslobodi_letove(prvi_let);
    oslobodi_putnike(prvi_putnik);
    printf("Dovidjenja!\n");

    return 0;
}
