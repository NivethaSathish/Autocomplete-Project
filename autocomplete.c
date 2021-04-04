#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "autocomplete.h"

int cmpfunc(char *one, char *two);
int compare(struct term* terms1, struct term* terms2);
int startMatch(const char* sub,const char* whole);

int cmpfunc (char *one, char *two) 
{
    int result = strcmp(one, two);
    return result;
}

int compare (struct term* terms1, struct term* terms2) {

    if ((*terms1).weight > (*terms2).weight)
    {
        return -1;
    } else if ((*terms1).weight < (*terms2).weight)
    {
        return 1;
    } else
    {
        return 0;
    }
}

int startMatch(const char* sub,const char* whole)
{
    int comparison = strncmp(sub,whole,strlen(sub));
    if (comparison == 0)
    {
        return 0; // they're both equal, therefore a match
    }
    else
    {
        return 1;
    }
}

void read_in_terms(struct term **terms, int *pnterms, char *filename)
{

    char line[200];
    FILE *fp = fopen(filename, "r");
    fgets(line,sizeof(line),fp); // get the first line (number of cities)
    *pnterms = atoi(line); // turn the first line into an int to put in the for loop

    *terms = (struct term *)malloc((*pnterms) * sizeof(struct term));

    for(int i = 0; i < *pnterms; i++)
    {
        fgets(line, sizeof(line), fp); //read in at most sizeof(line) characters, (including '\0') into line.
        //printf("THE LINE IS: %i %s",i,line);
        //char str[] = "      13076300	Buenos Aires, Argentina";
        //char str[] = line;
        char *token = strtok(line, " \t"); 
        
        int w = atoi(token);
        token = strtok(NULL, ""); 
        token[strcspn(token,"\n")] = '\0';
        //token[strlen(token) - 1] = 0;

        /*printf("THE INDIVIDUAL WEIGHT AND TERM ARE:\n");
        printf("%i\n",w);
        printf("%s\n", token);*/

        (*terms)[i].weight = w;
        strcpy((*terms)[i].term,token);

    }          
    qsort(*terms, *pnterms, sizeof(struct term), cmpfunc);
}

int lowest_match(struct term *terms, int nterms, char *substr)
{

    int low = 0;
    int high = nterms - 1;
    int res = -1;

    while(low <= high)
    {
        int mid = (low + high)/2;
        //printf("%s\n",terms[mid].term);
        //printf("%s\n",substr);
        //printf("%i\n",(strcmp(terms[mid].term,substr)));

        if(startMatch(substr,terms[mid].term) == 0)
        {
            //printf("%s and %s", substr,terms[mid].term);
            //printf("matched\n");
            //printf("%i\n",mid);
            //printf("%i\n",high);
            
            res = mid;
            high = mid - 1;
            
        }
        else if (strcmp(terms[mid].term,substr) > 0)// first one is greater
            //if (strstr(terms[mid].term,substr) != NULL)
        {
            high = mid - 1;
        } 
        else if(strcmp(terms[mid].term,substr)) // if second one is greater
        {
            low = mid + 1;
            //printf("working");
        }  
   
    }
    return res;
}

int highest_match(struct term *terms, int nterms, char *substr)
{
    int low = 0;
    int high = nterms - 1;
    int res = -1;

    while(low <= high)
    {
        int mid = (low + high)/2;
        //printf("%s\n",terms[mid].term);
        //printf("%s\n",substr);
        //printf("%i\n",(strcmp(terms[mid].term,substr)));

        if(startMatch(substr,terms[mid].term) == 0)
        {
            //printf("%s and %s", substr,terms[mid].term);
            //printf("matched\n");
            //printf("%i\n",mid);
            //printf("%i\n",high);
            
            res = mid;
            low = mid + 1;
            
        }
        else if (strcmp(terms[mid].term,substr) > 0)// first one is greater
            //if (strstr(terms[mid].term,substr) != NULL)
        {
            high = mid - 1;
        } 
        else if(strcmp(terms[mid].term,substr)) // if second one is greater
        {
            low = mid + 1;
            //printf("working");
        }  
   
    }
    return res;
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr)
{
    // loop through the terms and put all the terms that start with the substr in answers
    // sort by weight

    int counter = 0;
    for (int i = 0; i < nterms; i++)
    {
        if(startMatch(substr,terms[i].term) == 0)
        {
            //printf("add");
            counter = counter + 1;
        }
    }

    //counter = *n_answer;

    *answer = (struct term *)malloc((counter) * sizeof(struct term));

   //*answer = (struct term *)malloc((nterms) * sizeof(struct term));
   //*terms = (struct term *)malloc((*pnterms) * sizeof(struct term));

    int oc = 0;

    for (int i = 0; i < nterms; i++)
    {
        if(startMatch(substr,terms[i].term) == 0)
        {
            
            //struct term *answer[i] = (struct term *)malloc(sizeof(struct term));
            int w = terms[i].weight;
            char *token = terms[i].term;

            (*answer)[oc].weight = w;
            strcpy((*answer)[oc].term,token);

            oc = oc + 1;

            //printf("Begin %i",w);
            //printf(" %s\n",token);
        }
    }

    qsort(*answer, counter, sizeof(struct term), compare);
    //printf("%i",counter);
} 




