#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NCHAR 1024
void FromInput();
void FromOutput();
char * replace();

int main(void)
{
  FILE * output;
  output = fopen("Output.csv", "w");
  fprintf(output, "DateAndTime,Climate\n");
  fclose(output);

  printf("From Input:\n");
  FromInput();

  printf("\nFrom Output:\n");
  FromOutput();

  return 0;
}

void FromInput()
{
  char lines[11][1024], buffer[1024];
  int x = 0;
  char * result, * time, * year, * month, * day, * hour, * minute, * sec, * mlls, * tzhr, * tzmin, * weather, * min, * max, * percent;
  FILE * input;
  input = fopen("Input.csv", "r");

  fgets(buffer, 1024, input);

  while(fgets(buffer, 1024, input) && x < 11)
  {
    strcpy(lines[x],buffer);
    time = strtok(lines[x], ",");
    weather = strtok(NULL, " ");
    // printf("%s\n", weather);
    min = strtok(weather, ",");
    max = strtok(NULL, ",");
    percent = strtok(NULL, ",");

    // Para determinar si el timezone es positivo
    char * hyphen;
    int index;
    hyphen = strrchr(time, '-');
    index = (int)(hyphen - time);

    if(index < 23)
    {
      index = 0; // Positivo
    }
    else
    {
      index = 1; // Negativo
    }


    // while(time != NULL){
    // printf("%s\n", time);
    char c[] = "-", d[] = " ", e[] = "T", f[] = ":", g[] = ".", h[] = "+";

    // Eliminar los guiones
    result = replace(time, c, d);
    // Eliminar la T
    result = replace(result, e, d);
    // Eliminar los dos puntos
    result = replace(result, f, d);
    // Eliminar los  puntos
    result = replace(result, g, d);
    // Eliminar el positivo
    result = replace(result, h, d);
    // printf("%s\n", result);

    year = strtok(result, " ");
    // printf("%s\n", year);
    month = strtok(NULL, " ");
    // printf("%s\n", month);
    day = strtok(NULL, " ");
    // printf("%s\n", day);
    hour = strtok(NULL, " ");
    // printf("%s\n", hour);
    minute = strtok(NULL, " ");
    // printf("%s\n", minute);
    sec = strtok(NULL, " ");
    // printf("%s\n", sec);
    mlls = strtok(NULL, " ");
    // printf("%s\n", mlls);
    tzhr = strtok(NULL, " ");
    // printf("%s\n", tzhr);
    tzmin = strtok(NULL, " ");
    // printf("%s\n", tzmin);

    long binary = atol(year) << 4;
    binary = (binary | atol(month)) << 5;
    binary = (binary | atol(day)) << 5;
    binary = (binary | atol(hour)) << 6;
    binary = (binary | atol(minute)) << 6;
    binary = (binary | atol(sec)) << 10;
    binary = (binary | atol(mlls)) << 1;
    binary = (binary | index) << 5;
    binary = (binary | atol(tzhr)) << 6;
    binary = (binary | atol(tzmin));
    
    int newbinary = (atoi(min)) << 7;
    newbinary = (newbinary | atoi(max)) << 7;
    newbinary = (newbinary | atoi(percent));
    printf("%ld,%d\n", binary, newbinary);
    // }

    FILE * output;
    output = fopen("Output.csv", "a");
    fprintf(output, "%ld,%d\n", binary, newbinary);
    fclose(output);

    x++;
  }


  fclose(input);

  return;
}

void FromOutput()
{
  char lines[11][1024], buffer[1024];
  int x = 0, precipitation, maxTemp, minTemp;
  unsigned long numdate, year, month, day, hour, minute, sec, mlls, tzhr, tzmin, tzsign;
  char * result, * datetime, * climate, * weather, * min, * max, * percent;
  FILE * input;
  input = fopen("Output.csv", "r");

  fgets(buffer, 1024, input);

  while(fgets(buffer, 1024, input) && x < 11)
  {
    strcpy(lines[x],buffer);
    datetime = strtok(lines[x], ",");
    climate = strtok(NULL, " ");
    // printf("%s\n", datetime);
    // min = strtok(weather, ",");
    // max = strtok(NULL, ",");
    // percent = strtok(NULL, ",");

    // Lectura binaria del tiempo
    numdate = (atol(datetime));
    tzmin = (numdate) & 63;
    tzhr = (numdate >> 6) & 31;
    tzsign = (numdate >> 11) & 1;
    mlls = (numdate >> 12) & 1023;
    sec = (numdate >> 22) & 63;
    minute = (numdate >> 28) & 63;
    hour = (numdate >> 34) & 31;
    day = (numdate >> 39) & 31;
    month = (atol(datetime) >> 44) & 15;
    year = (atol(datetime) >> 48) & 4095;

    // Lectura binaria del clima
    precipitation = (atoi(climate) & 127);
    maxTemp = ((atoi(climate) >> 7) & 127);
    minTemp = ((atoi(climate) >> 14) & 127);

    // Deteminar si timezone es positivo
    char * tzsignLead;
    if(tzsign == 0)
    {
      tzsignLead = "+";
    }
    else
    {
      tzsignLead = "-";
    }

    printf("%lu-%02lu-%02luT%02lu:%02lu:%02lu.%03lu%s%02lu:%02lu,%d,%d,%d\n", year, month, day, hour, minute, sec, mlls, tzsignLead, tzhr, tzmin, minTemp, maxTemp, precipitation);
    // minute = (datetime >> 27) & 63
    // minuteLead = str(minute).zfill(2)
    // hour = (datetime >> 33) & 31
    // hourLead = str(hour).zfill(2)
    // day = (datetime >> 38) & 31
    // dayLead = str(day).zfill(2)
    // month = (datetime >> 43) & 15
    // monthLead = str(month).zfill(2)
    // year = (datetime >> 47) & 4095
    
    // int newbinary = (atoi(min)) << 7;
    // newbinary = (newbinary | atoi(max)) << 7;
    // newbinary = (newbinary | atoi(percent));
    // printf("%ld,%d\n", binary, newbinary);
    // }

    x++;
  }
}

char * replace(const char *s, const char *oldW, const char *newW) 
{ 
    char *result; 
    int i, cnt = 0; 
    int newWlen = strlen(newW); 
    int oldWlen = strlen(oldW); 
  
    // Counting the number of times old word 
    // occur in the string 
    for (i = 0; s[i] != '\0'; i++) 
    { 
        if (strstr(&s[i], oldW) == &s[i]) 
        { 
            cnt++; 
  
            // Jumping to index after the old word. 
            i += oldWlen - 1; 
        } 
    } 
  
    // Making new string of enough length 
    result = (char *)malloc(i + cnt * (newWlen - oldWlen) + 1); 
  
    i = 0; 
    while (*s) 
    { 
        // compare the substring with the result 
        if (strstr(s, oldW) == s) 
        { 
            strcpy(&result[i], newW); 
            i += newWlen; 
            s += oldWlen; 
        } 
        else
            result[i++] = *s++; 
    } 
  
    result[i] = '\0'; 
    return result; 
}
