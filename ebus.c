#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>
#define MAX_VOYAGEURS 100
#define MAX_BUS 15
#define MAX_RESERVATIONS 20
#define MAX_LINE_SIZE 256
#define BUS_FILE_PATH "C:\\Users\\guesm\\OneDrive\\Bureau\\ebus\\bus.txt"
#define VOYAGEUR_FILE_PATH "C:\\Users\\guesm\\OneDrive\\Bureau\\ebus\\voyageur.txt"
#define RESERVATION_FILE_PATH "C:\\Users\\guesm\\OneDrive\\Bureau\\ebus\\reservation.txt"

struct voyageur
{
    char nom[10];
    char prenom[10];
    int ci;
    int fidele;
    int etudiant;
    char mot_de_passe[10];
};

struct Bus
{
    int num_bus;
    char lieu_depart[10];
    char lieu_arrivee[10];
    char date_depart[10];
    char date_arrivee[10];
    float prix_billet;
    int places_disponibles;
};

struct reservation
{
    int id;
    int num_res; //car un seul utilisateur peut faire plusieurs reservation
    char nom[10];
    char prenom[10];
    int num_bus;
    float prix_billet;
    int nb_places_reservees;
};


void creer_compte()
{
    struct voyageur u;
    int x;
    printf("donnez votre nom: ");
    scanf(" %[^\n]",u.nom);
    printf("donnez votre prenom: ");
    scanf(" %[^\n]",u.prenom);
    printf("donnez votre numero de carte d'identite: ");
    scanf(" %d",&u.ci);
    const char *chemin= VOYAGEUR_FILE_PATH;
    struct voyageur t[MAX_VOYAGEURS];
    int nb_voyageur;
    lireFichierVoyageur(chemin,t,&nb_voyageur);
    int test=0;
    for (int i=0;i<nb_voyageur;i++)
    {
        if (t[i].ci==u.ci)
        {
            printf("**compte existe deja** \n");
            test=1;
            break;
        }
    }
    if (test==0){
    u.fidele=0;
    printf("si vous etes un etudiant tapez 1 sinon tapez 2: \n");
    scanf(" %d",&x);
    if (x==1)
    {
        printf("\033[1;32m");
        printf("vous beneficez d'un tarif etudiant \n");
        printf("\033[0m");
        u.etudiant=1;
    }
    else
    {
        u.etudiant=0;
    }
    printf("veuillez creer un mot de passe pour votre compte compose au minimum de 5 caracteres: ");
    scanf(" %s",u.mot_de_passe);
    FILE *in;
    in=fopen(VOYAGEUR_FILE_PATH,"a");
    fprintf(in,"%s,%s,%d,%d,%d,%s \n",u.nom,u.prenom,u.ci,u.fidele,u.etudiant,u.mot_de_passe);
    fclose(in);
    printf("\033[1;34m");
    printf("Votre compte a ete creer avec succes\n");
    printf("\033[0m");}

}





int afficherBusDisponibles(const char *chemin_du_fichier, int places_a_reserver) {
    FILE *fichier = fopen(BUS_FILE_PATH, "r");
    int test=0;
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    char ligne[256];

    while (fgets(ligne, sizeof(ligne), fichier) != NULL) {
        char *token = strtok(ligne, ",");
        int num_bus = atoi(token);


        char lieu_depart[50], lieu_arrivee[50], date_depart[20], date_arrivee[20];
        float prix_billet;
        int places_disponibles;

        strcpy(lieu_depart, strtok(NULL, ","));
        strcpy(lieu_arrivee, strtok(NULL, ","));
        strcpy(date_depart, strtok(NULL, ","));
        strcpy(date_arrivee, strtok(NULL, ","));
        prix_billet = atof(strtok(NULL, ","));
        places_disponibles = atoi(strtok(NULL, ","));


        if (places_disponibles > places_a_reserver) {
            printf("\033[1;34m");
            printf("Bus %d:\n", num_bus);
            printf("\033[0m");
            printf("Lieu de depart : %s\n", lieu_depart);
            printf("Lieu d'arrivee : %s\n", lieu_arrivee);
            printf("Date de depart : %s\n", date_depart);
            printf("Date d'arrivee : %s\n", date_arrivee);
            printf("Prix du billet : %f\n", prix_billet);
            printf("Places disponibles : %d\n\n", places_disponibles);
            test=1;
        }

    }
    if (test==0)
    {   printf("pas de bus contenants assez de places disponibles \n");}
    fclose(fichier);
    return test;
}


struct Element {
    int donnee;
    struct Element* suivant;
};

typedef struct {
    struct Element* debut;
    struct Element* fin;
} File;


File* creerFile() {
    File* nouvelleFile = (File*)malloc(sizeof(File));
    if (nouvelleFile == NULL) {
        fprintf(stderr, "Erreur : Impossible de créer la file\n");
        exit(EXIT_FAILURE);
    }
    nouvelleFile->debut = NULL;
    nouvelleFile->fin = NULL;
    return nouvelleFile;
}


void enfiler(File* file, int valeur) {

    struct Element* nouvelElement = (struct Element*)malloc(sizeof(struct Element));
    if (nouvelElement == NULL) {
        fprintf(stderr, "Erreur : Impossible d'allouer la mémoire pour l'élément\n");
        exit(EXIT_FAILURE);
    }
    nouvelElement->donnee = valeur;
    nouvelElement->suivant = NULL;

    if (file->debut == NULL) {
        file->debut = nouvelElement;
        file->fin = nouvelElement;
    } else {
        file->fin->suivant = nouvelElement;
        file->fin = nouvelElement;
    }
}


int defiler(File* file) {
    if (file->debut == NULL) {
        fprintf(stderr, "Erreur : La file est vide\n");
        exit(EXIT_FAILURE);
    }

    int valeurDefilee = file->debut->donnee;
    struct Element* elementADefiler = file->debut;
    file->debut = file->debut->suivant;
    free(elementADefiler);
    if (file->debut == NULL) {
        file->fin = NULL;
    }
    return valeurDefilee;
}

void afficherFile(File* file) {
    struct Element* actuel = file->debut;
    printf("Votre file de reservations : ");
    while (actuel != NULL) {
        printf("%d ", actuel->donnee);
        actuel = actuel->suivant;
    }
    printf("\n");
}



void lireFichierVoyageur(char *nom_fichier, struct voyageur t[], int *nb) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    *nb = 0;
    char ligne[256];
    while (*nb < MAX_VOYAGEURS && fgets(ligne, sizeof(ligne), fichier) != NULL) {
        int result = sscanf(ligne, "%49[^,],%49[^,],%d,%f,%d,%49[^,]\n",
                            t[*nb].nom,
                            t[*nb].prenom,
                            &t[*nb].ci,
                            &t[*nb].fidele,
                            &t[*nb].etudiant,
                            t[*nb].mot_de_passe);

        if (result == 6) {
            (*nb)++;
        } else {
            fprintf(stderr, "Erreur de lecture pour la ligne : %s", ligne);
        }
    }

    fclose(fichier);
}

void lireFichierBus(char *nom_fichier, struct Bus t[], int *nb) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    *nb = 0;
    char ligne[256];

    while (*nb < MAX_BUS && fgets(ligne, sizeof(ligne), fichier) != NULL) {
        int fieldsRead = sscanf(ligne, "%d,%49[^,],%49[^,],%19[^,],%19[^,],%f,%d",
                                &t[*nb].num_bus,
                                t[*nb].lieu_depart,
                                t[*nb].lieu_arrivee,
                                t[*nb].date_depart,
                                t[*nb].date_arrivee,
                                &t[*nb].prix_billet,
                                &t[*nb].places_disponibles);

        if (fieldsRead == 7) {
            (*nb)++;
        } else {
            fprintf(stderr, "Erreur de lecture pour le bus %d\n", *nb + 1);
        }
    }

    fclose(fichier);
}


void lireFichierReservation(const char *nom_fichier, struct reservation t[], int *nb) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    *nb = 0;
    char ligne[MAX_LINE_SIZE];

    while (*nb < MAX_RESERVATIONS && fgets(ligne, sizeof(ligne), fichier) != NULL) {
        int fieldsRead = sscanf(ligne, "%d,%d,%49[^,],%49[^,],%d,%f,%d",
                                &t[*nb].id,
                                &t[*nb].num_res,
                                t[*nb].nom,
                                t[*nb].prenom,
                                &t[*nb].num_bus,
                                &t[*nb].prix_billet,
                                &t[*nb].nb_places_reservees);

        if (fieldsRead == 7) {
            (*nb)++;
        } else {
            fprintf(stderr, "Erreur de lecture pour la reservation %d\n", *nb + 1);
        }
    }

    fclose(fichier);
}



float reduire_le_prix_du_billet_pour_etudiant(int x, int numero)
{
    struct voyageur V[MAX_VOYAGEURS];
    struct Bus B[MAX_BUS];
    int nb_voyageurs, nb_bus,test=0;
    float prix;
    lireFichierBus(BUS_FILE_PATH, B, &nb_bus);
    lireFichierVoyageur(VOYAGEUR_FILE_PATH, V, &nb_voyageurs);
    for (int i=0;i<nb_voyageurs;i++)
    {
        if (V[i].ci==x)
        {
            if (V[i].etudiant==0)
            {
                printf("erreur: nest pas etudiant");
            }
            else
            {
                test=1;
            }
        }
    }
    if (test==1)
    {
        for(int i=0;i<nb_bus;i++)
        {
            if (B[i].num_bus==numero)
            {
                prix=B[i].prix_billet-B[i].prix_billet*0.1;
                return prix;
            }
        }
    }
}

//fonction pour mettre a jour le nombre de places encore disponibles dans un bus apres reservation

void mettreAJourPlacesDisponibles(int numBus, int nouvellesPlaces) {

    struct Bus buses[MAX_BUS];
    int nbBuses = 0;

    lireFichierBus(BUS_FILE_PATH,buses, &nbBuses);

    for (int i = 0; i < nbBuses; i++) {
        if (buses[i].num_bus == numBus) {
            // Mettre à jour le nombre de places disponibles
            if (nouvellesPlaces > 0) {
                buses[i].places_disponibles = nouvellesPlaces;
            } else {
                // Supprimer le bus s'il n'y a plus de places disponibles

                for (int j = i; j < nbBuses - 1; j++) {
                    buses[j] = buses[j + 1];
                }
                nbBuses--;
            }
            break;
        }
    }

    // Reecrire le fichier avec les mises à jour
    FILE *fichier = fopen(BUS_FILE_PATH, "w");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nbBuses; i++) {
        fprintf(fichier,"%d,%s,%s,%s,%s,%f,%d\n",
                buses[i].num_bus,buses[i].lieu_depart,buses[i].lieu_arrivee,
                buses[i].date_depart,buses[i].date_arrivee,buses[i].prix_billet,
                buses[i].places_disponibles);
    }

    fclose(fichier);
}


void faire_reservation()
{
    char nom[10];
    char prenom[10];
    int nb,numbs,x,id,et,b=0;
    float prix;
    const char *chemin_du_fichier = BUS_FILE_PATH;
    printf("donnez votre numero de carte d'identite (cest votre code de reservation): "); //qui sera le code de la reservation et pour garantir l'unicite
    scanf("%d", &x);
    int test=0;
    while((test==0)) //pour s'assurer que l'utilisateur entre un nombre de places à reserver inferieur au nombre de places disponibles
    {
        printf("combien de place voulez vous reserver: ");
        scanf(" %d", &nb);
        printf("voici les bus encore disponibles \n");
        test=afficherBusDisponibles(chemin_du_fichier,nb);
    }
    printf("veuillez choisir le bus que vous souhaitez prendre: ");
    scanf(" %d", &numbs);
    File* f=creerFile();

    //recuperer les informations de l'utilisateur a fin de les stocker dans le fichier reservation
    struct voyageur voyageurs[MAX_VOYAGEURS];
    int nb_voyageurs;

    lireFichierVoyageur(VOYAGEUR_FILE_PATH, voyageurs, &nb_voyageurs);
    for (int i=0;i<nb_voyageurs;i++)
    {
        if(voyageurs[i].ci==x)
        {
            strcpy(nom,voyageurs[i].nom);
            strcpy(prenom,voyageurs[i].prenom);
            id=voyageurs[i].ci;
        }
    }
    //calculer le prix d'un billet pour un etudiant
    prix=reduire_le_prix_du_billet_pour_etudiant(x,numbs);
    if (prix>0){
            printf("Le prix de votre billet avec tarif etudiant: %f\n \n", prix);}

    float prixf=prix*nb; //le prix final à payer
    //calcul du numero de la reservation
    const char *chemin=RESERVATION_FILE_PATH;
    int nb_reservation;
    struct reservation tab[MAX_RESERVATIONS];
    lireFichierReservation(chemin,tab,&nb_reservation);
    for (int i=0;i<nb_reservation;i++)
    {
        if (tab[i].id==x)
        {
            b++;
        }
    }
    b++;
    FILE *in;
    in=fopen(RESERVATION_FILE_PATH,"a");
    fprintf(in,"%d,%d,%s,%s,%d,%f,%d \n",id,b,nom,prenom,numbs,prixf,nb); //stocker les informations dans reservation.txt
    fclose(in);
    enfiler(f,id); //file qui contient les identifiants de toutes les reservations
    printf("\033[1;34m");
    printf("Votre reservation a ete faite avec succes\n");
    printf("votre code de reservation est %d. \nVous l'utiliserez pour afficher ou modifier votre reservation, ou faire de nouvelles reservation \n \n",id);
    printf("\033[0m");

    //afficher la reservation

    printf("voici les details de votre reservation\n");
    printf("id_reservation= %d ** numero reservartion=%d ** nom= %s ** prenom= %s ** numero de bus= %d **nombre de places reservees= %d ** prix billet(s)= %f \n \n",id,b,nom,prenom,numbs,nb,prixf);

    //modification apres reservation

    struct Bus B[MAX_BUS];
    int nb_bus,r;
    lireFichierBus(BUS_FILE_PATH,B, &nb_bus);
    for (int i=0;i<nb_bus;i++)
    {
        if (B[i].num_bus==numbs)
        {
            r=B[i].places_disponibles;
        }
    }
    int nouvellesPlaces;
    nouvellesPlaces=r-nb;
    mettreAJourPlacesDisponibles(numbs, nouvellesPlaces);
}


void afficher_historique_reservation(int x)
{
    struct reservation R[MAX_RESERVATIONS];
    int nb_reservation,test=0;
    lireFichierReservation(RESERVATION_FILE_PATH,R,&nb_reservation);
    for (int i=0;i<nb_reservation;i++)
    {
        if (R[i].id==x)
        {
            printf("\033[1;34m");
            printf("voici les details de votre reservation numero %d: \n",R[i].num_res);
            printf("\033[0m");
            printf("**id_reservation= %d\n**nom= %s \n**prenom= %s \n**numero de bus= %d \n**places reservees= %d \n**prix billet(s)= %f. \n",R[i].id,R[i].nom,R[i].prenom,R[i].num_bus,R[i].nb_places_reservees,R[i].prix_billet);
            test=1;
        }
    }
    if (test==0)
    {
        printf("pas de reservation faite \n");
    }
}






void annulerReservation(int id, int num_res) {
    //stocker les informations des reservations
    FILE *fichier_reservation = fopen(RESERVATION_FILE_PATH, "r");
    if (fichier_reservation == NULL) {
        perror("Erreur lors de l'ouverture du fichier de réservation");
        exit(EXIT_FAILURE);
    }

    struct reservation reservations[MAX_RESERVATIONS];
    int nbReservations = 0;

    while (nbReservations < MAX_RESERVATIONS &&
           fscanf(fichier_reservation, "%d,%d,%49[^,],%49[^,],%d,%f,%d\n",
                  &reservations[nbReservations].id,
                  &reservations[nbReservations].num_res,
                  reservations[nbReservations].nom,
                  reservations[nbReservations].prenom,
                  &reservations[nbReservations].num_bus,
                  &reservations[nbReservations].prix_billet,
                  &reservations[nbReservations].nb_places_reservees) == 7) {
        nbReservations++;
    }

    fclose(fichier_reservation);

    // Trouver l'index de la reservation à annuler
    int indexReservationAnnulee = -1;

    for (int i = 0; i < nbReservations; i++) {
        if (reservations[i].id== id && reservations[i].num_res == num_res) {
            indexReservationAnnulee = i;
            break;
        }
    }


    if (indexReservationAnnulee == -1) {
        fprintf(stderr, "Reservation avec le numero %d non trouvee pour l'utilisateur avec l'id %d\n", num_res, id);
        return;
    }

    // Supprimer la reservation annulee du tableau
    for (int i = indexReservationAnnulee; i < nbReservations - 1; i++) {
        reservations[i] = reservations[i + 1];
    }

    nbReservations--;

    // mettre a jour le fichier reservation
    fichier_reservation = fopen(RESERVATION_FILE_PATH, "w");
    if (fichier_reservation == NULL) {
        perror("Erreur lors de l'ouverture du fichier de réservation");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nbReservations; i++) {
        fprintf(fichier_reservation, "%d,%d,%s,%s,%d,%f,%d\n",
                reservations[i].id,
                reservations[i].num_res,
                reservations[i].nom,
                reservations[i].prenom,
                reservations[i].num_bus,
                reservations[i].prix_billet,
                reservations[i].nb_places_reservees);
    }

    fclose(fichier_reservation);

    // informations du bus choisi pendant la reservation qui a ete annulee
    FILE *fichier_bus = fopen(BUS_FILE_PATH, "r");
    if (fichier_bus == NULL) {
        perror("Erreur lors de l'ouverture du fichier de bus");
        exit(EXIT_FAILURE);
    }

    struct Bus buses[MAX_BUS];
    int nbBuses = 0;

    while (nbBuses < MAX_BUS &&
           fscanf(fichier_bus, "%d,%49[^,],%49[^,],%19[^,],%19[^,],%f,%d\n",
                  &buses[nbBuses].num_bus, buses[nbBuses].lieu_depart,
                  buses[nbBuses].lieu_arrivee, buses[nbBuses].date_depart,
                  buses[nbBuses].date_arrivee, &buses[nbBuses].prix_billet,
                  &buses[nbBuses].places_disponibles) == 7) {
        nbBuses++;
    }

    fclose(fichier_bus);

    // Mettre à jour le nombre de places disponibles pour le bus associe a la reservation annulee
    int numBusAnnule = reservations[indexReservationAnnulee].num_bus;

    for (int i = 0; i < nbBuses; i++) {
        if (buses[i].num_bus == numBusAnnule) {
            buses[i].places_disponibles += reservations[indexReservationAnnulee].nb_places_reservees;
            break;
        }
    }

    // mise a jour du fichier bus
    fichier_bus = fopen(BUS_FILE_PATH, "w");
    if (fichier_bus == NULL) {
        perror("Erreur lors de l'ouverture du fichier de bus");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < nbBuses; i++) {
        fprintf(fichier_bus, "%d,%s,%s,%s,%s,%.2f,%d\n",
                buses[i].num_bus, buses[i].lieu_depart, buses[i].lieu_arrivee,
                buses[i].date_depart, buses[i].date_arrivee, buses[i].prix_billet,
                buses[i].places_disponibles);
    }

    fclose(fichier_bus);

    printf("Reservation annulee avec succes\n");
}


int main()
{
    int a,b,c,x,nb,num,q;


    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("\033[4m");
    printf("BIENVENUE DANS NOTRE APPLICATION EBUS\n \n");
    printf("\033[0m");
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("si vous etes un nouvel utilisateur tapez 1 **** sinon tapez 2: ");
    scanf(" %d",&a);
    if (a==1) //nouvel utilisateur
    {
        printf("\033[1;35m");
        printf("veuillez creer un nouveau compte \n");
        printf("\033[0m");
        creer_compte();
        printf("\033[1;35m");
        printf("passez maintenant a letape de reservation \n \n");
        printf("\033[0m");
        faire_reservation();
        while (c!=5)
        {printf("\033[1;35m");
        printf("si vous voulez faire une nouvelle reservation tapez 1\n");
        printf("si vous voulez afficher l'historique de vos reservation tapez 2\n");
        printf("si vous voulez modifier une reservation tapez 3\n");
        printf("si vous voulez annuler une reservation tapez 4\n");
        printf("si vous voulez quitter l'application tapez 5\n");
        printf("\033[0m");
        printf("votre reponse: ");
        scanf(" %d",&c);
        if (c==1)
        {
            faire_reservation();
        }
        if (c==2)
        {
            printf("veuillez taper votre code: ");
            scanf(" %d", &x);
            afficher_historique_reservation(x);
        }
        if (c==3)
        {
            printf("veuillez taper votre code: ");
            scanf(" %d", &x);
            printf("tapez le numero de la reservation que vous voulez modifier: **remarque: la reservation sera annulee et vous allez faire une nouvelle reservation**");
            scanf(" %d", &num);
            annulerReservation(x,num);
            faire_reservation();
        }
        if (c==4)
        {
            printf("veuillez taper votre code: ");
            scanf(" %d", &x);
            afficher_historique_reservation(x);
            printf("donnez le numero de la reservation que vous voulez annuler: ");
            scanf(" %d", &q);
            annulerReservation(x,q);
        }
        }
    }
    else  //ancien utilisateur
    {
        while (b!=5){
        printf("* si vous voulez faire une nouvelle reservation tapez 1 \n");
        printf("* si vous voulez modifier une reservation deja faite tapez 2 \n");
        printf("* si vous voulez afficher l'historique de vos reservations tapez 3 \n");
        printf("* si vous voulez annuler une reservation tapez 4 \n");
        printf("* si vous voulez quitter l'apllication tapez 5 \n");
        printf("votre reponse: ");
        scanf(" %d", &b);

        if (b==1)
        {
            faire_reservation();
        }
        else if (b==2)
        {
            printf("veuillez taper votre code: ");
            scanf(" %d",&x);
            printf("tapez le numero de la reservation que vous voulez modifier: **remarque: la reservation sera annulee et vous allez faire une nouvelle reservation**");
            scanf(" %d", &num);
            annulerReservation(x,num);
            faire_reservation();
        }
        else if (b==3)
        {
            printf("veuillez taper votre code: ");
            scanf(" %d",&x);
            afficher_historique_reservation(x);
        }
        else if (b==4)
        {
            printf("veuillez taper votre code: \n \n");
            scanf(" %d", &x);
            afficher_historique_reservation(x);
            printf("veuillez taper le numero de la reservation que vous voulez annuler: ");
            scanf("%d", &nb);
            annulerReservation(x,nb);
        }}
    }
}
