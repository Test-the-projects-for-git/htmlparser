/*Programm download html page after parse it by two tags <p> <src>*/

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>
#include<curl/curl.h>
#include<stdint.h>

/*for entrie http request in file*/
static uint32_t writereq( char *ptr, size_t size, size_t nmemb, FILE* data){
  return fwrite(ptr, size, nmemb, data);
}

/*for define size of file*/
static uint32_t filesz(FILE* fd)
{
  uint32_t fsz;
  fseek(fd, 0, SEEK_END);
  fsz = ftell(fd);
  rewind(fd);
  return fsz;
}

/*openfile to read and verify errors*/
static void opnfread(FILE** fd, const char* filename)
{
  if(*fd > 0)
  {
    *fd = fopen(filename,"r");
    puts("file was open and done to read\n");
  }
  else
  {
    perror("error open file\n");
    exit(EXIT_FAILURE);
  }
}

/*openfile to write and verify errors*/ 
static void opnfwrite(FILE** fd, const char* filename)
{ 
  if(*fd > 0)
  { 
    *fd = fopen(filename,"w");
    puts("file was open and done to write\n");
  }    
  else
  {
    perror("error open file\n");
    exit(EXIT_FAILURE);
  }
}


static void writeresult(char** temp_buffer, FILE** fd)
{
  puts("going writeback results\n");

  *fd = fopen("result.txt", "w");
  if(*fd != NULL)
  {
    fputs(*temp_buffer, *fd);
    puts("data was saved\n");
  }
  else
  {
    perror("error open file for writeback\n");
    exit(EXIT_FAILURE);
  }
}

/*allocate block memory under html page e t.c and verify errr*/
static void buffalloc(char** buff, uint32_t fsz)
{
  *buff = (char*)malloc(fsz * sizeof(char));

  if(*buff == NULL)
  {
    perror("error allocate memory\n");
    exit(EXIT_FAILURE);
  }
}

/*функция для записи из файла в буфер*/
static void wrtfbuff(FILE* fd, char* mem_buffer, uint32_t fsz){
  puts("going write data into buffer\n");
  fread(mem_buffer, sizeof(char), fsz, fd);
}

/*for find <p></p> and it record in memory buff*/
static void cpybuffp(char* mem_buffer, char* temp_buffer, uint32_t fsz)
{
  int i = 0, j = 0, k = 0;
  puts("parsing <p></p> tags...\n");
  while(i < fsz)
  {
    if(mem_buffer[i] == '<' && mem_buffer[i + 1] == 'p' && mem_buffer[i + 2] == '>' && mem_buffer[i + 3] == '<')
    {
	    i += 3;
	    while(mem_buffer[i] != '>')
	    {
	      i++;
	    }
    }

    if(mem_buffer[i] == '<' && mem_buffer[i + 1] == 'p' && mem_buffer[i + 2] == '>')
    {
     puts("found entry");
     j = i;
     j += 3;
         
     while(mem_buffer[j] != '\0')
     {
       if(mem_buffer[j] == '<' && mem_buffer[j + 1] == '/' && mem_buffer[j + 2] == 'p' && mem_buffer[j + 3] == '>')
       {
         break;
       }
       temp_buffer[k] = mem_buffer[j];
       j++;
       k++;
     }
    }
    i++;
   }
}      


/*for find <a href></a> and record it memory buffer*/
/*
void cpybuffhref(char* mem_buffer, char* temp_buffer, uint32_t fsize, FILE* fd)
{
  int i = 0, j = 0, k = 0;

  printf("parsing href tags\n");

  while(i != fsize -1)
  {
    if(mem_buffer[i] == '<'
      && mem_buffer[i + 1] == 'a'
      && mem_buffer[i + 2] == ' '
      && mem_buffer[i + 3] == 'h'
      && mem_buffer[i + 4] == 'r'
      && mem_buffer[i + 5] == 'e'
      && mem_buffer[i + 6] == 'f'
      && mem_buffer[i + 7] == '='
      && mem_buffer[i + 8] == '"')
      {
        
        printf("found entry\n\n");

        i = i + 9;
        j = i;
        k = 0;

	while(mem_buffer[j] != '\0')
	{
          if((mem_buffer[j] == '<' && mem_buffer[j + 1] == '/' && mem_buffer[j + 2] == 'a' && mem_buffer[j + 3] == '>'))
          {
            break;
          }
          temp_buffer[k] = mem_buffer[j];
          j++;
          k++;
        }

       }

         i++;
   }

    printf("\nparsing ended\n\n");	
}*/


int main(int argc, char** argv)
{
  FILE* fd_resolv;
  FILE* fd_index;
  uint32_t fsz;
  char* mem_buffer;
  char* temp_buffer;
  char ch = '0';
  CURL* curl; 
  CURLcode res_curl;
/*
  if(argc == 2)
  {
    puts("go opening file\n");
  }
  else
  {
    puts("paramets should be two\n");
    exit(EXIT_FAILURE);
  }
 
  opnfwrite(&fd_index, "index.html");

  if(fd_index == NULL)
  {
    puts("error openning file\n");
    exit(EXIT_FAILURE);
  }

  curl = curl_easy_init();

  if(curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fd_index);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writereq);
    res_curl = curl_easy_perform(curl);
  }
  
  if(res_curl != CURLE_OK)
  {
    curl_easy_cleanup(curl);
    puts("error http request\n");
    exit(EXIT_FAILURE);
  }

  puts("Done page was downloaded\n");
  fclose(fd_index);
  curl_easy_cleanup(curl);
  */ 
   opnfread(&fd_index, "index.html");   
   fsz = filesz(fd_index);

   buffalloc(&mem_buffer, fsz);
   buffalloc(&temp_buffer, fsz);
   
   wrtfbuff(fd_index, mem_buffer, fsz);
   
   fclose(fd_index);
   
   cpybuffp(mem_buffer, temp_buffer, fsz);
   printf("%s\n", temp_buffer);
   puts("--------------------------the end--------------------");
   
   free(mem_buffer);
   free(temp_buffer);
  return EXIT_SUCCESS;
}
