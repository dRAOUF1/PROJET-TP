#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definition des types
typedef struct Chapitre
{
    int id;
    char titre[100];
    int nombres_pages;
    char contenu[1000];
    char Titre_Chapitre_Suivant[100];
} Chapitre;

typedef struct Element_Livre *Livre;

typedef struct Element_Livre
{
    Chapitre Info;
    Livre Suivant;
} Element_Livre;

// Creer un chapitre
Chapitre Creer_Chapitre()
{
    Chapitre chap;
    printf("Donner le titre du chapite\n");
    scanf("%s", &chap.titre);
    printf("Donner le nombre de pages du chapitre\n");
    scanf("%d", &chap.nombres_pages);
    printf("Donner le contenu du chapitre\n");
    scanf("%s", &chap.contenu);
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

// Ajouter un chapitre dans le livre
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

void Afficher_Chapitre(Chapitre chap)
{
    printf("\tChapitre %d: %s \n", chap.id, chap.titre);
    printf("%s\n", chap.contenu);
    printf("\tChapitre Suivant:%s\n\n", chap.Titre_Chapitre_Suivant);
}
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

// Fonction qui decremonte les id apres l'insertion d'un chapitre si nécessaire
void dec_id(Livre Q)
{
    if (Q->Info.id != 1)
    {
        Q->Info.id--;
        dec_id(Q->Suivant);
    }
}
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
            while(P->Suivant->Info.id!=1){
                P=P->Suivant;
            }
            P->Suivant=(*l);
            strcpy(P->Info.Titre_Chapitre_Suivant,(*l)->Info.titre);
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
            scanf("%s", l->Info.titre);
        }

        // modification du Contenu
        printf("Voulez-vous modifier le contenu du chapitre ?(oui/non)\n");
        scanf("%s", &reponse);

        if (strcmp(reponse, "oui") == 0)
        {
            printf("Donner le nouveau contenu\n");
            scanf("%s", l->Info.contenu);
        }

        // modification du nombre de pages
        printf("Voulez-vous modifier le nombre de pages du chapitre ?(oui/non)\n");
        scanf("%s", &reponse);

        if (strcmp(reponse, "oui") == 0)
        {
            printf("Donner le nouveau nombre de pages\n");
            scanf("%s", l->Info.nombres_pages);
        }
    }
    else
        Modifier(l->Suivant, id);
}

int Calculer_Nombre_Pages(Livre l)
{
    if (l->Suivant->Info.id == 1)
        return l->Info.nombres_pages;
    else
        return l->Info.nombres_pages + Calculer_Nombre_Pages(l->Suivant);
}

//fonction qui renvoie le plus grand chap
Chapitre Plus_grand_chapitre(Livre L,Chapitre PGchap){
    if (L->Suivant->Info.id == 1)
    {
        if (L->Info.nombres_pages > PGchap.nombres_pages ){
        PGchap=L->Info;
        }
        return PGchap;
    }
    if (L->Info.nombres_pages > PGchap.nombres_pages )
    {
        PGchap=L->Info;
    }
    return Plus_grand_chapitre(L->Suivant, PGchap);
    
    
    
}

/*fonction qui renvoie le plus petit chap
Chapitre Plus_petit_chapitre(Livre L){

}*/

//fonction qui affiche les informations du plus grand et plus petit chapitre
void Affiche_PGchap_PPchap(Livre L){
    Chapitre PGchap,PPchap;
    
    PGchap=L->Info;
    Plus_grand_chapitre(L,PGchap);
    printf("%s",PGchap.titre);
}
int main(){

    Livre livre = NULL;
    
    Ajouter_Chapitre(&livre, 1);
    Ajouter_Chapitre(&livre, 2);
    Ajouter_Chapitre(&livre, 3);
    Ajouter_Chapitre(&livre, 4);

    Affiche_PGchap_PPchap(livre);
   
    return 0;
}
