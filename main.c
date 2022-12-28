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

//Structure de données qui represente un livre composé de N chapitre
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

// Fonction qui incremonte les id apres l'insertion d'un chapitre si nécessaire
void inc_id(Livre Q)
{
    if (Q->Info.id != 1)
    {
        Q->Info.id++;
        inc_id(Q->Suivant);
    }
}

// Fonction qui decremonte les id apres l'insertion d'un chapitre si nécessaire
void dec_id(Livre Q)
{
    if (Q->Info.id != 1)
    {
        Q->Info.id--;
        dec_id(Q->Suivant);
    }
}

// Afficher un chapitre dans le livre
void Afficher_Chapitre(Chapitre chap)
{
    printf("\tChapitre %d: %s \n", chap.id, chap.titre);
    printf("\tNombre de pages: %d\n", chap.nombres_pages);
    printf("\tContenu: %s\n", chap.contenu);
    printf("\tChapitre Suivant:%s\n\n", chap.Titre_Chapitre_Suivant);
    printf("\t*******\n");
}

//fonction qui renvoie le plus grand chapitre
void Plus_grand_chapitre(Livre L,Chapitre *PGchap){
    if (L->Suivant->Info.id == 1)
    {
        if (strlen(L->Info.contenu) > strlen((*PGchap).contenu) ){
        *PGchap=L->Info;
        }
        return;
    }
    if (strlen(L->Info.contenu) > strlen((*PGchap).contenu) )
    {
        *PGchap=L->Info;
    }
    return Plus_grand_chapitre(L->Suivant, PGchap);    
}

//fonction qui renvoie le plus petit chapitre
void Plus_petit_chapitre(Livre L, Chapitre *PPchap){
    if (L->Suivant->Info.id == 1)
    {
        if (strlen(L->Info.contenu) < strlen((*PPchap).contenu) ){
        *PPchap=L->Info;
        }
        return;
    }
    if (strlen(L->Info.contenu) < strlen((*PPchap).contenu) )
    {
        *PPchap=L->Info;
    }
    return Plus_petit_chapitre(L->Suivant, PPchap);
}

// L'auteur peut à tout moment

// Ajouter un chapitre (au début, au milieu ou à la fin).
void Ajouter_Chapitre(Livre *livre, int pos)
{
    Livre P;

    // cas 1: 1er chapitre d'un nouveau livre
    if (*livre == NULL && pos == 1)
    {
        // Creer le chapitre
        Chapitre chap = Creer_Chapitre();

        strcpy(chap.Titre_Chapitre_Suivant, chap.titre);
        chap.id = 1;

        P = malloc(sizeof(Element_Livre));
        P->Info = chap;
        P->Suivant = P;
        (*livre) = P;
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
            P->Info = chap;

            strcpy(P->Info.Titre_Chapitre_Suivant, (*livre)->Suivant->Info.titre);
            P->Info.id = (*livre)->Info.id + 1;

            strcpy((*livre)->Info.Titre_Chapitre_Suivant, P->Info.titre);
            P->Suivant = (*livre)->Suivant;
            (*livre)->Suivant = P;
            inc_id(P->Suivant);
        }

        // cas 2.2:position non trouvé
        else
        {
            P = (*livre)->Suivant;
            Ajouter_Chapitre(&P, pos - 1);
        }
    }
}

// Afficher les chapitres du livre en ordre croissant selon l'Id
void Afficher_Chapitre_Id(Livre l)
{
    if (l->Suivant->Info.id == 1)
        Afficher_Chapitre(l->Info);
    else
    {
        Afficher_Chapitre(l->Info);
        Afficher_Chapitre_Id(l->Suivant);
    }
}


// Afficher les chapitres du livre en ordre croissant selon le nombre de pages
// Fonction qui creer une copie de la liste
Livre Copie(Livre l)
{

    if (l == NULL)
        return NULL;
    else if (l->Suivant->Info.id != 1)
    {
        Livre p = malloc(sizeof(Element_Livre));
        p->Info = l->Info;
        p->Suivant = Copie(l->Suivant);
    }
    else
    {
        return l;
    }
}

// Fonction qui trie la liste par ordre croissant par rapport au nombre de pages
void Trie_Pages(Livre *l, Livre *T)
{ // T contient l'addresse de la tete de la litse donc l'appel se fera avec T=&l

    // Livre p=Copie(*l);

    // int id=(*l)->Info.id;
    if ((*l)->Suivant->Info.id == (*l)->Info.id || *l == NULL) // cas 1:Liste vide ou a une element
    {
        return;
    }
    else if ((*l)->Suivant != *T)
    {
        Livre p = (*l)->Suivant, min = (*l);
        while (p != *T)
        {
            if (min->Info.nombres_pages > p->Info.nombres_pages)
                min = p;
            p = p->Suivant;
        }

        if (min != NULL)
        {
            Chapitre chap = min->Info;
            min->Info = (*l)->Info;
            (*l)->Info = chap;
        }
        p = *l;
        Trie_Pages(&p->Suivant, T);
    }
}
void Afficher_Chapitre_Pages(Livre l, int id)
{
    // L'appel se fait avec id = l'id du 1er element de la liste
    if (id == l->Suivant->Info.id)
        Afficher_Chapitre(l->Info);
    else
    {
        Afficher_Chapitre(l->Info);
        Afficher_Chapitre_Pages(l->Suivant, id);
    }
}

// Fonction qui decremonte les id apres l'insertion d'un chapitre si nécessaire
void dec_id(Livre Q)
{
    //L'appel se fait avec id = l'id du 1er element de la liste
    if (id == l->Suivant->Info.id)
        Afficher_Chapitre(l->Info);
    else
    {
        Afficher_Chapitre(l->Info);
        Afficher_Chapitre_Pages(l->Suivant, id);
    }
}

//Supprimer un chapitre (le premier, le dernier ou le n-ème),
void Supprimer_Chapitre(Livre *l, int pos)
{
    // cas 1: Liste vide donc ne rien faire
    if (*l != NULL)
    { // cas 2: liste non vide
        // cas 2.1: Supprimer la tete
        if (pos - 1 == 0)
        {
            Livre P = (*l);
            // dec_id((*l)->Suivant);
            (*l) = (*l)->Suivant;
            while (P->Suivant->Info.id != 1)
            {
                P = P->Suivant;
            }
            P->Suivant = (*l);
            strcpy(P->Info.Titre_Chapitre_Suivant, (*l)->Info.titre);
            dec_id(*l);
        }
        else
        {
            if (pos - 1 == 1)
            {
                // cas 2.2: position trouvé non tete
                Livre P = (*l)->Suivant;
                (*l)->Suivant = P->Suivant;
                strcpy((*l)->Info.Titre_Chapitre_Suivant, P->Suivant->Info.titre);
                free(P);
                dec_id((*l)->Suivant);
            }
            else
            {
                // cas 2.2: position non trouvé
                Livre P = (*l)->Suivant;
                Supprimer_Chapitre(&P, pos - 1);
            }
        }
    }
}

//Modifier le contenu d'un chapitre a partir de son Id.
void Modifier(Livre l, int id)
{
    if (l->Info.id == id)
    {
        char reponse[3];
        // modification du titre
        printf("Voulez-vous modifier le titre du chapitre ?(oui/non)\n");
        scanf("%s", &reponse);

        if (strcmp(reponse, "oui") == 0)
        {
            printf("Donner le nouveau titre\n");
            fflush(stdin);
            scanf("%[^\n]s", l->Info.titre);
        }

        // modification du Contenu
        printf("Voulez-vous modifier le contenu du chapitre ?(oui/non)\n");
        scanf("%s", &reponse);

        if (strcmp(reponse, "oui") == 0)
        {
            printf("Donner le nouveau contenu\n");
            fflush(stdin);
            scanf("%[^\n]", l->Info.contenu);
        }

        // modification du nombre de pages
        printf("Voulez-vous modifier le nombre de pages du chapitre ?(oui/non)\n");
        scanf("%s", &reponse);

        if (strcmp(reponse, "oui") == 0)
        {
            printf("Donner le nouveau nombre de pages\n");
            fflush(stdin);
            scanf("%[^\n]s", l->Info.nombres_pages);
        }
    }
    else
        Modifier(l->Suivant, id);
}

// Afficher le nombre de pages du livre.
int Calculer_Nombre_Pages(Livre l)
{
    if (l->Suivant->Info.id == 1)
        return l->Info.nombres_pages;
    else
        return l->Info.nombres_pages + Calculer_Nombre_Pages(l->Suivant);
}

// Afficher les informations du plus grand et plus petit chapitre.
void Affiche_PGchap_PPchap(Livre L){
    Chapitre PGchap,PPchap;
    
    PGchap=L->Info;
    Plus_grand_chapitre(L,&PGchap);
    PPchap=L->Info;
    Plus_petit_chapitre(L,&PPchap);
    printf("L'affichage du plus grand chapitre:\n");
    Afficher_Chapitre(PGchap);
    printf("L'affichage du plus petit chapitre:\n");
    Afficher_Chapitre(PPchap);
}

int main(){

    return 0;
}
