#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definition des types

// Structure de données qui represente les caractéristiques du chapitre
typedef struct Chapitre
{
    int id;                           // Id (un entier positif qui représente le rang du chapitre).
    char titre[100];                  // Titre du chapitre
    int nombres_pages;                // Nombre de pages de chapitre (donné par l’auteur)
    char contenu[1000];               // Cِontenu du chapitre
    char Titre_Chapitre_Suivant[100]; // Titre du chapitre suivant
} Chapitre;

// Structure de données qui represente un livre composé de N chapitre
typedef struct Element_Livre *Livre;

typedef struct Element_Livre
{
    Chapitre Info;
    Livre Suivant;
} Element_Livre;

// Des actions parametrées pour rendre le code plus lisible et plus facile à manipuler

// Création d'un chapitre
Chapitre Creer_Chapitre()
{
    Chapitre chap;
    printf("Donner le titre du chapite\n");
    fflush(stdin);
    scanf("%[^\n]s", &chap.titre);
    printf("Donner le nombre de pages du chapitre\n");
    scanf("%d", &chap.nombres_pages);
    printf("Donner le contenu du chapitre\n");
    fflush(stdin);
    scanf("%[^\n]s", &chap.contenu);
    return chap;
}

// Fonction qui incremente les id apres l'insertion d'un chapitre si nécessaire
void inc_id(Livre Q)
{
    // cas de base : l'id du chapitre est égal a 1 donc on a parcouru toute le liste
    if (Q->Info.id != 1)
    {
        // Tant qu'on est pas arrivé a la fin en incremente
        Q->Info.id++;

        // appel recursive
        inc_id(Q->Suivant);
    }
}

// Fonction qui decremente les id apres la suppression d'un chapitre si nécessaire
void dec_id(Livre Q)
{
    // cas de base : l'id du chapitre est égal a 1 donc on a parcouru toute le liste
    if (Q->Info.id != 1)
    {
        // Tant qu'on est pas arrivé a la fin en decremonte
        Q->Info.id--;

        // appel recursive
        dec_id(Q->Suivant);
    }
}

// Afficher un chapitre dans le livre
void Afficher_Chapitre(Chapitre chap)
{
    // gerer la mise en forme de l'affichage pour un affichage clair
    printf("\tChapitre %d: %s \n", chap.id, chap.titre);
    printf("\tNombre de pages: %d\n", chap.nombres_pages);
    printf("\tContenu: %s\n", chap.contenu);
    printf("\tChapitre Suivant:%s\n\n", chap.Titre_Chapitre_Suivant);
    printf("\t*******\n");
}

// L'auteur peut à tout moment

// Ajouter un chapitre (au début, au milieu ou à la fin).
void Ajouter_Chapitre(Livre *livre, int pos)
{
    Livre P;

    // cas 1: 1er chapitre d'un nouveau livre OU insertion au debut
    if (pos == 1)
    {
        // Creer le chapitre
        Chapitre chap = Creer_Chapitre();

        chap.id = 1;

        // creation de l'element de la liste
        P = malloc(sizeof(Element_Livre));
        // cas 1.1: Liste vide
        if (*livre == NULL)
        {

            P->Suivant = P;
            // remplire le champ titre du chapitre suivant (dans ce cas par le titre du chapitre lui meme car il n'y a qu'un seul chapitre)

            strcpy(chap.Titre_Chapitre_Suivant, chap.titre);
            P->Info = chap;

            // affecter l'element a la tete
            (*livre) = P;
        }

        // cas 1.2:Insertion au debut
        else
        {
            // incrementer tout les IDs de la liste avant l'insertion au debut
            inc_id((*livre)->Suivant);

            // incrementer la tete (La fonction n'incremente pas le tete)
            (*livre)->Info.id++;

            // remplire les informations de dans P
            P->Info = chap;
            strcpy(P->Info.Titre_Chapitre_Suivant, (*livre)->Info.titre);

            // gerer le chainage
            P->Suivant = *livre;
            *livre = P;

            // chercher la fin de la liste
            P = (*livre)->Suivant;
            while (P->Suivant->Info.id != 2)
            {
                P = P->Suivant;
            }

            // enchainé le dernier element avec la tete (liste cirulaire)
            P->Suivant = *livre;

            // remplire le champs titre chapitre suivant
            strcpy(P->Info.Titre_Chapitre_Suivant, (*livre)->Info.titre);
        }
    }

    // cas 2:livre deja existant
    else
    {
        // cas 2.1:position trouvé
        if (pos - 1 == 1)
        {
            // Creer le chapitre
            Chapitre chap = Creer_Chapitre();

            P = malloc(sizeof(Element_Livre));
            // remplire le champs info de l'element de la liste par le chapitre que l'auteur veut ajouter
            P->Info = chap;

            // gerer le titre du chapitre suivant du nouveau chapitre
            strcpy(P->Info.Titre_Chapitre_Suivant, (*livre)->Suivant->Info.titre);

            // gerer l'id du nouveau chapitre
            P->Info.id = (*livre)->Info.id + 1;

            // gerer le titre du chapitre suivant de l'ancien chapitre qui se trouvais a cette position
            strcpy((*livre)->Info.Titre_Chapitre_Suivant, P->Info.titre);

            // gerer le chainnage
            P->Suivant = (*livre)->Suivant;
            (*livre)->Suivant = P;

            // incrementer les id des chapitres qui se trouvent apres le nouveau chapitre inserer
            inc_id(P->Suivant);
        }

        // cas 2.3:position non atteinte
        else
        { // stocker le pointeur livre dans un autre pointeur pour eviter de perdre la tete au 1er appel
            P = (*livre)->Suivant;

            // appel recursive
            Ajouter_Chapitre(&P, pos - 1);
        }
    }
}

// Afficher les chapitres du livre en ordre croissant selon l'Id
void Afficher_Chapitre_Id(Livre l)
{
    // cas de base: Arriver a la fin
    if (l->Suivant->Info.id == 1)
        Afficher_Chapitre(l->Info);
    else
    {
        Afficher_Chapitre(l->Info);

        // appel recursif
        Afficher_Chapitre_Id(l->Suivant);
    }
}

// Afficher les chapitres du livre en ordre croissant selon le nombre de pages

// Fonction qui creer une copie du livre
Livre Copie(Livre l)
{

    // cas 1: la livre est vide
    if (l == NULL)
        return NULL;

    else if (l->Suivant->Info.id != 1)
    {
        // cas 2: parcourir le livre en creant une copie de chaque chapitre
        Livre p = malloc(sizeof(Element_Livre));

        // gestion du chainnage + appel recursive
        p->Info = l->Info;
        // appel recursive
        p->Suivant = Copie(l->Suivant);

        return p;
    }
    else
    {
        // cas 3: le livre contient 1 chapitre qu'on renvoie OU on est arrivé au dernier chapitre du livre
        Livre p = malloc(sizeof(Element_Livre));
        p->Info = l->Info;
        p->Suivant = NULL;

        return p;
    }
}

// Fonction qui trie la liste par ordre croissant par rapport au nombre de pages
void Trie_Pages(Livre *l)
{ // T contient l'addresse de la tete de la litse donc l'appel se fera avec T=&l

    if ((*l)->Suivant == NULL || *l == NULL)
    {
        // cas 1:Liste vide ou a un element
        // ne rien faire
        return;
    }

    // cas de base: arriver a la téte de liste envoyer a l'appel

    // cas recursif
    else if ((*l)->Suivant != NULL)
    {
        // initialiser p pour parcourir et min pour garder le minimum
        Livre p = (*l)->Suivant, min = (*l);

        // parcourir le livre en recherchant le min
        while (p != NULL)
        {
            if (min->Info.nombres_pages > p->Info.nombres_pages)
            {
                min = p;
            }
            p = p->Suivant;
        }

        Chapitre chap = min->Info;
        min->Info = (*l)->Info;
        (*l)->Info = chap;
        p = (*l)->Suivant;

        // appel recursive
        Trie_Pages(&p);
    }
}

// Fonction qui affiche la copie triée
void Afficher_Copie_Trie(Livre l, int id)
{
    // L'appel se fait avec id = l'id du 1er element de la liste

    // cas de base: arriver a la fin de la copie du livre
    if (NULL == l->Suivant)
        Afficher_Chapitre(l->Info);
    else
    {
        Afficher_Chapitre(l->Info);

        // appel recursive
        Afficher_Copie_Trie(l->Suivant, id);
    }
}

// Fonction qui affiche les chapitre par ordre croissant selon le nombre de pages
void Afficher_Chapitre_Pages(Livre l)
{
    Livre CopieDuLivre = Copie(l);
    Trie_Pages(&CopieDuLivre);
    Afficher_Copie_Trie(CopieDuLivre, CopieDuLivre->Info.id);
}

// Supprimer un chapitre (le premier, le dernier ou le n-ème),
void Supprimer_Chapitre(Livre *l, int pos)
{
    // cas 1: Liste vide donc ne rien faire
    if (*l != NULL)
    { // cas 2: liste non vide
        // cas 2.1: Supprimer la tete

        // cas 2.1.1: Supprimer la tete d'une liste a 1 chapitre
        if ((*l)->Suivant->Info.id == (*l)->Info.id)
        {
            free(*l);
            *l = NULL;
        }

        // cas 2.1.2: Supprimer la tete d'une liste a plusieur chapitres
        else if (pos - 1 == 0)
        {
            Livre P = (*l);

            // deplacer la tete au chapitre suivant
            (*l) = (*l)->Suivant;

            // liberer l'ancienne tete
            free(P);

            // reinitialiser P
            P = (*l);

            // rechercher la fin de la liste
            while (P->Suivant->Info.id != 1)
            {
                P = P->Suivant;
            }
            // gerer le chainnage entre la fin de la liste et le debut (liste circulaire)
            P->Suivant = (*l);

            // gerer le titre du chapitre suivant
            strcpy(P->Info.Titre_Chapitre_Suivant, (*l)->Info.titre);

            // decrementer les id de tout les elements se trouvant apres le chapitre supprimer si il y en a
            dec_id(*l);
        }
        else
        {
            if (pos - 1 == 1)
            {
                // cas 2.2: position trouvé non tete
                // gerer le chainnage
                Livre P = (*l)->Suivant;
                (*l)->Suivant = P->Suivant;

                // gerer le titre suivant
                strcpy((*l)->Info.Titre_Chapitre_Suivant, P->Suivant->Info.titre);

                // liberer l'espace du chapitre detacher de la liste
                free(P);

                // decrementer les id de tout les elements se trouvant apres le chapitre supprimer si il y en a
                dec_id((*l)->Suivant);
            }
            else
            {
                // cas 2.3: position non trouvé

                // stocker le pointeur livre dans un autre pointeur pour eviter de perdre la tete au 1er appel
                Livre P = (*l)->Suivant;

                // appel recursive
                Supprimer_Chapitre(&P, pos - 1);
            }
        }
    }
}

// Modifier le contenu d'un chapitre a partir de son Id.
void Modifier(Livre *l, int id)
{
    Livre P;

    // Cas de base: chapitre trouver

    if ((*l)->Info.id == id)
    {
        char reponse[3];

        // modification du titre
        printf("Voulez-vous modifier le titre du chapitre ?(oui/non)\n");
        scanf("%s", &reponse);

        // tester la reponse de l'auteur
        if (strcmp(strlwr(reponse), "oui") == 0)
        // strlw transforme la chaine en minuscule pour facilité le test de la reponse (reponse = OUI,OUi,Oui...)
        {
            printf("Donner le nouveau titre\n");
            fflush(stdin);
            scanf("%[^\n]s", (*l)->Info.titre);

            // modifier le  champs "titre du chapitre suivant" du precendent

            // Rechercher le precedent de l
            Livre q = (*l)->Suivant;
            while (q->Suivant->Info.id != (*l)->Info.id)
            {
                q = q->Suivant;
            }

            // copier le titre
            strcpy(q->Info.Titre_Chapitre_Suivant, (*l)->Info.titre);
        }

        // modification du Contenu
        printf("Voulez-vous modifier le contenu du chapitre ?(oui/non)\n");
        scanf("%s", &reponse);

        if (strcmp(strlwr(reponse), "oui") == 0)
        {
            printf("Donner le nouveau contenu\n");
            fflush(stdin);
            scanf("%[^\n]", (*l)->Info.contenu);
        }

        // modification du nombre de pages
        printf("Voulez-vous modifier le nombre de pages du chapitre ?(oui/non)\n");
        scanf("%s", &reponse);

        if (strcmp(strlwr(reponse), "oui") == 0)
        {
            printf("Donner le nouveau nombre de pages\n");
            scanf("%d", &(*l)->Info.nombres_pages);
        }
    }
    else
    {
        // appel recursive pour avancer dans la liste
        P = (*l)->Suivant;
        Modifier(&P, id);
    }
}

// Afficher le nombre de pages du livre.
int Calculer_Nombre_Pages(Livre l)
{
    if (l == NULL)
        return 0;

    // cas de base: arrivé a le fin de la liste
    if (l->Suivant->Info.id == 1)
        return l->Info.nombres_pages;

    else
        // appel recursive
        return l->Info.nombres_pages + Calculer_Nombre_Pages(l->Suivant);
}

// fonction qui renvoie le plus grand chapitre (avec le plus de caracteres)
void Plus_grand_chapitre(Livre L, Chapitre *PGchap)
{
    // l'appel doit se faire avec PGchap = au premier chapitre du livre

    // si la longueur du contenu du chapitre actuel est plus grande que celle du PGchap on remplace
    // Dans le cas ou les nombres de caractere des deux chapitre sont egaux on garde le 1er
    if (strlen(L->Info.contenu) > strlen((*PGchap).contenu))
    {
        *PGchap = L->Info;
    }

    // cas de base : l'id du chapitre est égal a 1 donc on a parcouru toute le liste
    if (L->Suivant->Info.id == 1)
    {
        return;
    }
    else
    {
        // appel recursive
        Plus_grand_chapitre(L->Suivant, PGchap);
    }
}

// fonction qui renvoie le plus petit chapitre (avec le moins de caracteres)
void Plus_petit_chapitre(Livre L, Chapitre *PPchap)
{
    // l'appel doit se faire avec PPchap = au premier chapitre du livre
    // si la longueur du contenu du chapitre actuel est plus petite que celle du PGchap on remplace
    // Dans le cas ou les nombres de caractere des deux chapitre sont egaux on garde le 1er

    if (strlen(L->Info.contenu) < strlen((*PPchap).contenu))
    {
        *PPchap = L->Info;
    }

    //  cas de base : l'id du chapitre est égal a 1 donc on a parcouru toute le liste
    if (L->Suivant->Info.id == 1)
    {
        return;
    }
    else
    {
        // appel recursive
        Plus_petit_chapitre(L->Suivant, PPchap);
    }
}

// Afficher les informations du plus grand et plus petit chapitre.
void Affiche_PGchap_PPchap(Livre L)
{
    Chapitre PGchap, PPchap;

    // initialisation requise pour le fonctionnement de la procédure
    PGchap = L->Info;
    PPchap = L->Info;

    // Recuperer les PGchap et PPchap
    Plus_grand_chapitre(L, &PGchap);
    Plus_petit_chapitre(L, &PPchap);

    // Affichage plus grand chapitre
    printf("Le plus grand chapitre:\n");
    Afficher_Chapitre(PGchap);

    // Affichage plus petit chapitre
    printf("Le plus petit chapitre:\n");
    Afficher_Chapitre(PPchap);
}

// Fonction taille qui calcule le nombre de chapitres du livre (utilser pour verifier les entrees de l'utilisateur)
int Taille(Livre l)
{
    // cas 1: liste vide
    if (l == NULL)
        return 0;

    // cas 2: cas de base (Arriver a la fin)
    else if (l->Suivant->Info.id == 1)
        return 1;

    else
        // appel recursive
        return 1 + Taille(l->Suivant);
}

int main()
{
    // la variable reponse contient la reponse de l'auteur
    int reponse;
    Livre livre = NULL;
    int pos; // la position ou en insert

    // creation d'un menu pour faciliter l'utilisation du programme

    printf("\t\tBienvenue");

    // appeler la fonction adequate selon la reponse de l'auteur
    do
    {
        printf("\n\nTaper le chiffre correspondant a la fonctionnalite que vous voulez utiliser:\n");
        printf("1-Ajouter un chapitre.\n2-Afficher les chapitres du livre.\n3-Supprimer un chapitre du livre.\n4-Modifier le contenu d'un chapitre.\n5-Afficher le nombre de pages du livre.\n6-Afficher les information du plus grand et du plus petit chapitre.\n0-Pour quitter la programme\n");
        fflush(stdin); // pour eviter une boucle infinie dans le cas ou l'auteur entre un charcatere au lieu d'un entier
        scanf("%d", &reponse);
        switch (reponse)
        {
        case 1:
            // Ajouter un chapitre
            do
            {
                printf("Donner la position de l'insertion\n");
                fflush(stdin); // pour eviter une boucle infinie dans le cas ou l'auteur entre un charcatere au lieu d'un entier
                scanf(" %d", &pos);
                if (pos < 1 || pos > Taille(livre) + 1)
                    printf("Postion incorrect veuillez reessayer\n");
            } while (pos < 1 || pos > Taille(livre) + 1); //  1<=pos<=Taille + 1
            Ajouter_Chapitre(&livre, pos);

            break;
        case 2:
            // Afficher les chapitre d'un livre
            if (Taille(livre) == 1) // Si le livre contient 1 chapitre l'ordre n'est pas important
                Afficher_Chapitre_Id(livre);
            else if (Taille(livre) != 0) // si le livre n'est pas vide
            {
                printf("Voulez vous afficher selon l'ordre croissant des:\n1-IDs\n2-Nombres de pages\n3-Retour\n");
                do
                {
                    fflush(stdin); // pour eviter une boucle infinie dans le cas ou l'auteur entre un charcatere au lieu d'un entier
                    scanf("%d", &reponse);
                    if (reponse == 1)
                        // cas 2.1:Afficher selon l'ordre  les IDs
                        Afficher_Chapitre_Id(livre);
                    else if (reponse == 2)
                    {
                        // cas 2.2:Afficher selon l'ordre
                        Afficher_Chapitre_Pages(livre);
                    }
                    else if (reponse == 3)
                        break;
                    else
                        printf("Le chiffre entrer est incorrect veuillez reessayer\n");
                } while (reponse != 1 && reponse != 2 && reponse != 3); // la reponse doit etre egale a 1,2 ou 3.
            }
            else
                printf("Erreur: affichage impossible le livre est vide\n"); // si le livre est vide
            break;
        case 3:;
            // Supprimer un chapitre
            if (Taille(livre) != 0) // si le livre n'est pas vide
            {
                do
                {
                    printf("Donner la position du chapitre a supprimer (ou l'ID)\n");
                    fflush(stdin); // pour eviter une boucle infinie dans le cas ou l'auteur entre un charcatere au lieu d'un entier
                    scanf("%d", &pos);
                    if (pos < 1 || pos > Taille(livre)) // 1<=pos<=Taille
                        printf("Postion incorrect veuillez reessayer\n");

                } while (pos < 1 || pos > Taille(livre)); // 1<=pos<=Taille
                Supprimer_Chapitre(&livre, pos);
            }
            else
                printf("Erreur: suppresion impossible le livre est vide\n");

            break;
        case 4:;
            // Modifier le contenu d'un chapitre
            if (Taille(livre) != 0) // si le livre n'est vide
            {
                do
                {
                    printf("Donner la position du chapitre a modifer (ou l'ID)\n");
                    fflush(stdin); // pour eviter une boucle infinie dans le cas ou l'auteur entre un charcatere au lieu d'un entier
                    scanf("%d", &pos);
                    if (pos < 1 || pos > Taille(livre)) // 1<=pos<=Taille
                        printf("Postion incorrect veuillez reessayer\n");
                } while (pos < 1 || pos > Taille(livre)); // 1<=pos<=Taille
                Modifier(&livre, pos);
            }
            else
                printf("Erreur: Modification impossible le livre est vide\n");

            break;
        case 5:
            // Afficher le nombre de pages du livre
            printf("Le nombre de pages du livre est :%d\n", Calculer_Nombre_Pages(livre));

            break;
        case 6:
            // Afficher les information du plus grand et du plus petit chapitre
            if (Taille(livre) != 0) // si le livre n'est pas vide
                Affiche_PGchap_PPchap(livre);
            else
                printf("Erreur: Affichage impossible le livre est vide\n");
            break;
        case 0:
            // quitter le programme
            exit(0);
            break;
        default:
            printf("Erreur: vous avez entrer un mauvais chiffre\n");
            break;
        }

    } while (1 == 1); // Boucle infinie pour reafficher le menu jusqu'a ce que l'utilisateur entre '0' pour quitter le programme
    return 0;
}
