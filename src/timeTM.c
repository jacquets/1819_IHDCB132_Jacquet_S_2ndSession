#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include "C:\MinGW\include\time.h"
#include <errno.h>

#include "timeTM.h"

const char * NomJourSemaine[] = {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"};
const char * NomMois[] = {"janvier", "fevrier", "mars", "avril", "mai", "juin", "juillet", "aout", "septembre", "octobre", "novembre", "decembre"};

 /**
 * Affiche la date et l'heure courante (format francais en toute lettre)
 * @param un character (détermine si on retourne le jour ou le mois courant).
 * @return une constante, un pointeur vers une chaîne de characters.
 */
const char *timeString(char temps){
      if(temps != 'J' && temps != 'M'){
          return NULL;
      }
      time_t timestamp;
      struct tm * t;
      timestamp = time(NULL);
      t = localtime(&timestamp);
      if(temps == 'J')
          return NomJourSemaine[t->tm_wday];
      if(temps == 'M')
          return NomMois[t->tm_mon];
      else
          return NULL;
}

 /**
 * Affiche la date et l'heure courante (au choix).
 * @param un character (détermine la valeur de retour).
 * @return un entier naturel non signé (int).
 */
int timeInt(char temps){
     time_t timestamp;
     struct tm * t;
     timestamp = time(NULL);
     t = localtime(&timestamp);
     switch (temps){
         case 'J' : return t->tm_mday; break;
         case 'M' : return t->tm_mon+1; break;
         case 'A' : return (1900 + t->tm_year); break;
         case 'h' : return t->tm_hour; break;
         case 'm' : return t->tm_min; break;
         case 's' : return t->tm_sec; break;
         default:
            fprintf(stderr,"warning : Wrong input.\n");
            break;
            //fprintf(stderr, "error : %s\n",strerror(errno)); /* On affiche sur le canal d'erreur standard, "stderr". */
            //perror("error ");
            //exit(EXIT_FAILURE);
     }
     return ERR;
}

#ifdef _STRUCT_TM
int main(void){
    
    printf("%d-%s-%d",timeInt('J'),timeString('M'),timeInt('A'));

	return 0;
}
#endif
