#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#endif
#define MAX 100
#define MAX_TIME 100
#define MAX_STUDENTS 100

typedef struct
{
    int code;
    char nom[MAX];
    char prenom[MAX];
    char nomRef[MAX];
} ETUDIANT;


typedef struct
{
    int codes[MAX_STUDENTS];
    int count;
} StudentsPresent;

typedef struct
{
    char username[MAX];
    char password[MAX];
} USER;

typedef struct {
    char message[MAX];
    int lu;
} MESSAGE;
typedef struct {
    char *message;
    int lu;
} Message;

void libererMessages(Message *messages, int nbMessages) {
    for (int i = 0; i < nbMessages; i++) {
        if (messages[i].message != NULL) {
            free(messages[i].message);
        }
    }
    free(messages);
}
MESSAGE messages[MAX]; 
int nbMessages = 0; // 
#ifdef _WIN32
char getch()
{
    return _getch();
}
#else
char getch()
{
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}
#endif

void getHiddenInput(char *password, int maxLength)
{
    int i = 0;
    char ch;
    while (1)
    {
        ch = getch();
        if (ch == '\n' || ch == '\r')
        {
            password[i] = '\0';
            break;
        }
        else if (ch == 127 && i > 0)
        {
            printf("\b \b");
            i--;
        }
        else if (isprint(ch) && i < maxLength - 1)
        {
            password[i++] = ch;
            printf("•");
        }
    }
}

void EtudianMarkPresent(){
     int tr = 0, codeInput;
    char rep[MAX];
    ETUDIANT etudiant;
    int i;
    FILE *file = fopen("listesEtudiants.txt", "r");
    if (file == NULL)
    {
        printf("Impossible d'ouvrir le fichier\n");
        exit(1);
    }

    
    StudentsPresent studentsPresent;
    studentsPresent.count = 0;

    i = 1;
    while (i == 1)
    {
        printf("Donner le code\n");
        scanf("%d", &codeInput);
        while (getchar() != '\n')
        {
            continue;
        }
        
        tr = 0;

        
        while (fscanf(file, "%d %s %s %s\n", &etudiant.code, etudiant.prenom, etudiant.nom, etudiant.nomRef) == 4)
        {
            if (codeInput == etudiant.code)
            {
                tr = 1;
                break;
            }
        }
        fclose(file); 

        if (tr == 1)
        {
            // printf("Continuer cet etudiant, il est dans la base de donnees de l'ecole!\n");

    
            if (isCodePresent(&studentsPresent, codeInput))
            {
                printf("vous etes etudiant deja marque present\n");
            }
            else
            {
                FILE *filePresents = fopen("listesEtudiantsPresents.txt", "a");
                if (filePresents == NULL)
                {
                    printf("Impossible d'ouvrir le fichier\n");
                    exit(1);
                }

                time_t t;
                struct tm *timeinfo;
                char time_str[MAX_TIME];

                time(&t);
                timeinfo = localtime(&t);
                strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);

                printf("\U00002705 Code valide %s %s vous etes present a %s\n", etudiant.prenom, etudiant.nom, time_str);
                i = 0;

                
                fprintf(filePresents, "%d %s %s %s %s\n", etudiant.code, etudiant.prenom, etudiant.nom, etudiant.nomRef, time_str);
                fclose(filePresents);

            
                addCode(&studentsPresent, codeInput);
            }
        }
        else
        {
            printf("\U0000274C Code invalide !\n");
        }

        // Ouvrir le fichier "listesEtudiants.txt" à chaque itération
        file = fopen("listesEtudiants.txt", "r");         
    }
   
}

int isCodePresent(StudentsPresent *studentsPresent, int code)
{
    for (int i = 0; i < studentsPresent->count; ++i)
    {
        if (studentsPresent->codes[i] == code)
        {
            return 1; // Code trouvé
        }
    }
    return 0; // Code non trouvé
}

// Fonction pour ajouter un code à la liste des codes présents
void addCode(StudentsPresent *studentsPresent, int code)
{
    studentsPresent->codes[studentsPresent->count++] = code;
}

void marquerPresent()
{
    int tr = 0, codeInput;
    char rep[MAX];
    ETUDIANT etudiant;
    int i;
    FILE *file = fopen("listesEtudiants.txt", "r");
    if (file == NULL)
    {
        printf("Impossible d'ouvrir le fichier\n");
        exit(1);
    }

    // Initialisation de la structure pour les étudiants présents
    StudentsPresent studentsPresent;
    studentsPresent.count = 0;

    i = 1;
    while (i == 1)
    {
        printf("Donner le code\n");
        scanf("%d", &codeInput);
        while (getchar() != '\n')
        {
            continue;
        }
        // Réinitialiser le flag tr à 0 à chaque itération
        tr = 0;

        // Vérification si l'étudiant existe dans la base de données de l'école
        while (fscanf(file, "%d %s %s %s\n", &etudiant.code, etudiant.prenom, etudiant.nom, etudiant.nomRef) == 4)
        {
            if (codeInput == etudiant.code)
            {
                tr = 1;
                break;
            }
        }
        fclose(file); // Fermer le fichier après la première vérification

        if (tr == 1)
        {
            printf("Continuer cet etudiant, il est dans la base de donnees de l'ecole!\n");

            // Vérification si l'étudiant est déjà présent
            if (isCodePresent(&studentsPresent, codeInput))
            {
                printf("Cet etudiant est deja marque present\n");
            }
            else
            {
                FILE *filePresents = fopen("listesEtudiantsPresents.txt", "a");
                if (filePresents == NULL)
                {
                    printf("Impossible d'ouvrir le fichier\n");
                    exit(1);
                }

                time_t t;
                struct tm *timeinfo;
                char time_str[MAX_TIME];

                time(&t);
                timeinfo = localtime(&t);
                strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);

                printf("\U00002705 Code valide %s %s present a %s\n", etudiant.prenom, etudiant.nom, time_str);

                // Écrire les informations de l'étudiant avec l'heure de présence
                fprintf(filePresents, "%d %s %s %s %s\n", etudiant.code, etudiant.prenom, etudiant.nom, etudiant.nomRef, time_str);
                fclose(filePresents);

                // Ajouter le code à la liste des étudiants présents
                addCode(&studentsPresent, codeInput);
            }
        }
        else
        {
            printf("\U0000274C Code invalide !\n");
        }

        // Ouvrir le fichier "listesEtudiants.txt" à chaque itération
        file = fopen("listesEtudiants.txt", "r");

        do
        {
            printf("Voulez-vous continuer ? Si oui, tapez [o], sinon tapez [q] : ");
            scanf("%s", rep);
        } while (strcmp(rep, "o") != 0 && strcmp(rep, "q") != 0);

        if (strcmp(rep, "o") == 0)
        {
            i = 1;
        }
        else if (strcmp(rep, "q") == 0)
        {
            char mdp[MAX];
            do
            {
                printf("Mot de passe : ");
                getHiddenInput(mdp, MAX);
                printf("\n");
                if (strcmp(mdp, "passer1") != 0)
                {
                    printf("Mot de passe incorrect!, veuillez le ressaisir\n\n");
                }
                if (strlen(mdp) == 0 || strspn(mdp, " \n") == strlen(mdp) || isspace((unsigned char)mdp[0]))
                {
                    printf("Vous n'avez rien saisi, réessayez SVP\n");
                }

            } while (strcmp(mdp, "passer1") != 0 || strlen(mdp) == 0 || strspn(mdp, " \n") == strlen(mdp) || isspace((unsigned char)mdp[0]));
            mdp[strcspn(mdp, "\n")] = '\0';

            if (strcmp(mdp, "passer1") == 0)
            {
                break;
            }
        }
    }
}
void menuAdmin()
{

    printf("\n");
    printf("\t╔════════════════════════════════════════════════╗\n");
    printf("\t║             Espace Administrateur              ║\n");
    printf("\t╠════════════════════════════════════════════════╣\n");
    printf("\t║                                                ║\n");
    printf("\t║ 1. GESTION DES ÉTUDIANTS                       ║\n");
    printf("\t║ 2. MARQUER LES PRÉSENCES                       ║\n");
    printf("\t║ 3. GÉNÉRATION DE FICHIERS                      ║\n");
    printf("\t║ 4. ENVOYER UN MESSAGE                          ║\n");
    printf("\t║ 5. DECONNEXION                                 ║\n");
    printf("\t║                                                ║\n");
    printf("\t╚════════════════════════════════════════════════╝\n\n\n");
}

void MenuGenererFichier()
{
    printf("\n");
    printf("\t╔════════════════════════════════════════════════╗\n");
    printf("\t║             GÉNÉNERATION DE FICHIERS           ║\n");
    printf("\t╠════════════════════════════════════════════════╣\n");
    printf("\t║                                                ║\n");
    printf("\t║ 1. GÉNÉNER LISTE PRESENTS                      ║\n");
    printf("\t║ 2. GÉNÉNER LISTE PRESENTS PAR DATE             ║\n");
    printf("\t║ 3. RETOUR                                      ║\n");
    printf("\t║                                                ║\n");
    printf("\t╚════════════════════════════════════════════════╝\n\n\n");
}
 void GenererListePresent(){
    // Obtenir la date actuelle
time_t t = time(NULL);
struct tm *timeinfo = localtime(&t);
char dateStr[20];
strftime(dateStr, sizeof(dateStr), "%d_%m_%Y", timeinfo);

// Construire le nom de fichier avec la date
char nomFichier[40];
sprintf(nomFichier, "Presents_%s.txt", dateStr);

// Ouvrir le fichier pour écriture
FILE *fichierPresents = fopen(nomFichier, "w");
if (fichierPresents == NULL) {
    printf("Impossible de créer le fichier des présences.\n");
    return 1;
}

// Écrire l'en-tête avec la date
fprintf(fichierPresents, "Présences du %s\n\n", dateStr);
fprintf(fichierPresents, "%-10s %-20s %-20s %-20s %-20s\n", "Code", "Prénom", "Nom", "Classe", "Heure");
fprintf(fichierPresents, "%-10s %-20s %-20s %-20s %-20s\n", "------", "----------------", "----------------", "----------------", "----------------");

// Ouvrir le fichier listesEtudiantsPresents.txt en mode lecture
FILE *oldFilePresents = fopen("listesEtudiantsPresents.txt", "r");
if (oldFilePresents == NULL) {
    printf("Impossible d'ouvrir le fichier listesEtudiantsPresents.txt\n");
    fclose(fichierPresents);
    return 1;
}

// Copier les informations des étudiants présents dans le nouveau fichier
char ligne[MAX];
while (fgets(ligne, sizeof(ligne), oldFilePresents) != NULL) {
    int code, heurePresence;
    char prenom[MAX], nom[MAX], nomRef[MAX], heureStr[MAX_TIME];
    sscanf(ligne, "%d %s %s %s %s", &code, prenom, nom, nomRef, heureStr);
    fprintf(fichierPresents, "%-10d %-20s %-20s %-20s %-20s\n", code, prenom, nom, nomRef, heureStr);
}

fclose(oldFilePresents);
fclose(fichierPresents);
printf("Les informations des étudiants présents ont été copiées dans le fichier %s.\n", nomFichier);
 }


int estDateValide(char *dateStr) {
    int jour, mois, annee;
    if (sscanf(dateStr, "%d_%d_%d", &jour, &mois, &annee) != 3)
        return 0; 

    if (jour < 1 || jour > 31 || mois < 1 || mois > 12 || annee < 2000)
        return 0; 
    int maxJours[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (jour > maxJours[mois - 1] && (mois != 2 || jour > 28))
        return 0; 
    if (mois == 2 && jour == 29 && (annee % 4 != 0 || (annee % 100 == 0 && annee % 400 != 0)))
        return 0; 

    return 1; 
}

void GenererlisteEParDate() {
    char dateStr[20];

    // Demander à l'administrateur de saisir une date valide au format français (jj_mm_aaaa)
    do {
        printf("Veuillez saisir une date au format jj_mm_aaaa : ");
        scanf("%19s", dateStr);

        if (!estDateValide(dateStr)) {
            printf("Date invalide. Veuillez saisir une date valide.\n");
        }
    } while (!estDateValide(dateStr));

    // Construire le nom de fichier avec la date
    char nomFichier[40];
    snprintf(nomFichier, sizeof(nomFichier), "presence_date_%s.txt", dateStr);

    // Ouvrir le fichier pour écriture
    FILE *fichierPresents = fopen(nomFichier, "w");
    if (fichierPresents == NULL) {
        printf("Impossible de créer le fichier des présences.\n");
        return;
    }

    // Écrire l'en-tête avec la date
    fprintf(fichierPresents, "Présences du %s\n\n", dateStr);
    fprintf(fichierPresents, "%-10s %-20s %-20s %-20s %-20s\n", "Code", "Prénom", "Nom", "Classe", "Heure");
    fprintf(fichierPresents, "%-10s %-20s %-20s %-20s %-20s\n", "------", "----------------", "----------------", "----------------", "----------------");

    // Ouvrir le fichier listesEtudiantsPresents.txt en mode lecture
    FILE *oldFilePresents = fopen("listesEtudiantsPresents.txt", "r");
    if (oldFilePresents == NULL) {
        printf("Impossible d'ouvrir le fichier listesEtudiantsPresents.txt\n");
        fclose(fichierPresents);
        return;
    }

    // Copier les informations des étudiants présents dans le nouveau fichier
    char ligne[MAX];
    while (fgets(ligne, sizeof(ligne), oldFilePresents) != NULL) {
        int code;
        char prenom[MAX], nom[MAX], nomRef[MAX], heureStr[MAX_TIME];

        // Utilisez un format plus précis pour l'heure
        if (sscanf(ligne, "%d %s %s %s %s", &code, prenom, nom, nomRef, heureStr) == 5) {
            fprintf(fichierPresents, "%-10d %-20s %-20s %-20s %-20s\n", code, prenom, nom, nomRef, heureStr);
        } else {
            printf("Erreur de lecture dans le fichier listesEtudiantsPresents.txt\n");
        }
    }

    fclose(oldFilePresents);
    fclose(fichierPresents);

    printf("Le fichier des présences pour la date %s a été généré avec succès.\n", dateStr);
}

void envoyerMessage() {
    char nouveauMessage[MAX];
    printf("Entrez le message à envoyer : ");
    scanf(" %[^\n]", nouveauMessage);

  \
    FILE *fichierMessages = fopen("messages.txt", "a");
    
    if (fichierMessages == NULL) {
        printf("Erreur lors de l'ouverture du fichier des messages.\n");
        return;
    }

    
    fprintf(fichierMessages, "%s\n", nouveauMessage);

    
    fclose(fichierMessages);

    printf("Message envoyé avec succès.\n");
}

void freeMessages() {
    
   for (int i = 0; i < nbMessages; i++) {
    
    if (messages[i].message != NULL) {
        
        free(messages[i].message);
    }
}
}
void afficherMessages() {
    
    FILE *fichierMessages = fopen("messages.txt", "r");

    if (fichierMessages == NULL) {
        printf("Aucun message à afficher.\n");
        return;
    }

    char message[MAX];
    int nbMessages = 0;

    
    while (fgets(message, MAX, fichierMessages) != NULL) {
        printf("%d. %s", nbMessages + 1, message);
        nbMessages++;
    }

    
    fclose(fichierMessages);
}

void recevoirMessage(char *message) {
  
    strcpy(messages[nbMessages].message, message);
    messages[nbMessages].lu = 0; 
}

void MenuEtudiant() {
    FILE *fichierMessages = fopen("messages.txt", "r");

    if (fichierMessages == NULL) {
        printf("Erreur lors de l'ouverture du fichier des messages.\n");
        return;
    }

    int nbMessages = 0;
    char c;

    while ((c = fgetc(fichierMessages)) != EOF) {
        if (c == '\n') {
            nbMessages++;
        }
    }

    fclose(fichierMessages);

    // Afficher le menu avec le nombre de messages
    printf("\n\t\t===== Bienvenue dans l'espace Etudiant ======\n\n");
    printf("\t╔════════════════════════════════════════════════╗\n");
    printf("\t║             Espace Apprenant                   ║\n");
    printf("\t╠════════════════════════════════════════════════╣\n");
    printf("\t║                                                ║\n");
    printf("\t║ 1. MARQUER MA PRÉSENCE                         ║\n");
    printf("\t║ 2. NOMBRE DE MINUTES D'ABSENCE                 ║\n");
    printf("\t║ 3. MES MESSAGES (%d)                            ║\n", nbMessages);
    printf("\t║ 4. DECONNEXION                                 ║\n");
    printf("\t║                                                ║\n");
    printf("\t╚════════════════════════════════════════════════╝\n\n");

    // Mettre à jour le fichier des messages (marquer comme lus, etc.)
    FILE *fichierMessagesMaj = fopen("messages.txt", "a");  // Utilisation de "a" pour ajouter à la fin du fichier

    if (fichierMessagesMaj == NULL) {
        printf("Erreur lors de la mise à jour du fichier des messages.\n");
        return;
    }

    // Ajoutez votre logique pour mettre à jour le fichier ici...

    fclose(fichierMessagesMaj);

    // Mettez à jour nbMessages après la lecture et la mise à jour
  
    nbMessages = 0;
    fichierMessages = fopen("messages.txt", "r");

    while ((c = fgetc(fichierMessages)) != EOF) {
        if (c == '\n') {
            nbMessages++;
        }
    }

    fclose(fichierMessages);

    printf("\n\t\tVotre choix : ");
}


void LancerApp()
{
    USER user;
    int tr = 0;
    char inputUser[MAX], inputPass[MAX];
    FILE *file;

    file = fopen("login.txt", "r");
    if (file == NULL)
    {
        printf("Impossible d'ouvrir le fichier");
        exit(1);
    }

    do
    {
        do
        {
            printf("Nom d'utilisateur : ");
            if (fgets(inputUser, MAX, stdin) == NULL || strlen(inputUser) == 0 || strspn(inputUser, " \n") == strlen(inputUser) || isspace((unsigned char)inputUser[0]))
            {
                printf("Vous n'avez rien saisi, réessayez SVP\n");
            }
        } while (strlen(inputUser) == 0 || strspn(inputUser, " \n") == strlen(inputUser) || isspace((unsigned char)inputUser[0]));
        inputUser[strcspn(inputUser, "\n")] = '\0';

        do
        {
            printf("Mot de passe : ");
            getHiddenInput(inputPass, MAX);
            printf("\n");
            if (strlen(inputPass) == 0 || strspn(inputPass, " \n") == strlen(inputPass) || isspace((unsigned char)inputPass[0]))
            {
                printf("Vous n'avez rien saisi, réessayez SVP\n");
            }
        } while (strlen(inputPass) == 0 || strspn(inputPass, " \n") == strlen(inputPass) || isspace((unsigned char)inputPass[0]));
        inputPass[strcspn(inputPass, "\n")] = '\0';

        rewind(file); 

        while (fscanf(file, "%s %s", user.username, user.password) == 2)
        {
            if (strcmp(inputUser, user.username) == 0 && strcmp(inputPass, user.password) == 0)
            {
                tr = 1;
                break;
            }
        }

        if (tr == 1)
        {
            if (strcmp("admin", user.username) == 0)
            {
                int choix;
                do
                {
                    menuAdmin();
                    printf("Votre choix : ");
                    scanf("%d", &choix);
                    switch (choix)
                    {
                    case 1:
                        // Traiter le choix 1
                        break;
                    case 2:
                        marquerPresent();
                        break;
                    case 3:
                        // Générer un fichier
                        MenuGenererFichier();
                        do
                        {
                            printf("Votre choix : ");
                            scanf("%d", &choix);
                            while ((getchar()) != '\n'); 

                            switch (choix)
                            {
                            case 1:
                                
                                printf("Génération liste des étudiants en cours... \n\n");
                                GenererListePresent();
                            
                                break;
                            case 2:
                                
                                printf("Génération liste des étudiants par date en cours... ");
                                GenererlisteEParDate();
                                
                                break;
                            case 3:
                            
                                break;
                            default:
                                printf("Choix invalide\n");
                                break;
                            }
                        } while (choix != 3);
                        break;
                    case 4:
                        // Envoyer un message
                        envoyerMessage();
                        break;
                    case 5:
                        printf("Deconnexion en cours ...\n");
                        break;
                    default:
                        printf("Le choix n'existe pas! Réessayez...\n\n");
                    }
                } while (choix != 5);
            }
            else if (strcmp("etudiant", user.username) == 0 ||  strcmp("bass1", user.username) == 0 || strcmp("fatou1", user.username) == 0)
            {
                printf("\t\t===== Bienvenue dans l'espace Etudiant ======\n\n");
                int chx;
                do
                {
                    MenuEtudiant();
                    printf("Votre choix : ");
                    scanf("%d", &chx);
                    switch (chx)
                    {
                    case 1:
                        // MARQUER MA PRÉSENCE
                        EtudianMarkPresent();
                        
                        break;
                    case 2:
                        // NOMBRE DE MINUTES D’ABSENCE
                        break;
                    case 3:
                        // MES MESSAGES (0)
                        afficherMessages();
                        
                        break;
                    case 4:
                        printf("Deconnexion en cours ...\n");
                        break;
                    default:
                        printf("Le choix n'existe pas! Réessayez...\n\n");
                    }
                } while(chx != 4);

            }
        }
        else
        {
            printf("Nom utilisateur ou mot de passe incorrect. Veuillez réessayer!\n");
        }

    } while (tr != 1);

    fclose(file);
}
