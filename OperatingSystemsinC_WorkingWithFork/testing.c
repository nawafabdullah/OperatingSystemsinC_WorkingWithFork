/* 
* Nawaf Alharbi
* Operating Systems
*/ 
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LENGTH 80 // The maximum length of the commands

int ANALAYZE (char *args[]);

int LOCATE_POSITION (char *args[]);

bool AMPERSAND (char *args[]);

void SORT_COMMAND (char *args[], char command[]);

void PURE_COMMAND (char *args []);

void INPUTSTREAM_COMMAND (char *args []);

void OUTPUTSTREAM_COMMAND (char *args []);

void REPEAT_COMMAND (char *args[]);

void PIPE_COMMAND (char *args []);
 
int main(void) {
  char command[MAX_LENGTH];
  char *args[MAX_LENGTH/2 + 1];
  int should_run = 1;

  while (should_run)
  {
    printf("ssh>>");
    fflush(stdout);
    fgets(command, MAX_LENGTH, stdin);
    SORT_COMMAND(args, command);
    int decider = ANALAYZE(args);
        
    if (decider == 1)
    {
      OUTPUTSTREAM_COMMAND(args); 

    } else if (decider == 2)
    {
      INPUTSTREAM_COMMAND (args);

    } else if (decider == 3)
    {
      PIPE_COMMAND (args);
    }
      else if (decider == 4)
    {
//      REPEAT_COMMAND (args);
                    
    }
     else if (decider == 0)
    {
      PURE_COMMAND(args); 
    } 
 }	

}

void SORT_COMMAND (char *args[], char command[])
{
  int i = 0;    
  args[i] = strtok (command," \r\t\n");
  while (args[i] != NULL)
  {		
//    printf ("index %i has value %s ", i, args[i]);	             
    i++;
    args[i] = strtok (NULL," \r\t\n");     				
  }
  args[i] = NULL;
  return; 
}

int ANALAYZE (char *args[])
{
  int i =0;
  while (args[i] != NULL)
  {
    printf ("\n");
    if (*args[i] == '>')
    {
//      printf (" I detect an output stream attempt \n");			
      return 1;
    } else if (*args[i] == '<')
    { 
//      printf (" I detect an input stream attempt \n"); 
      return 2;
    } else if (*args[i] == '|')
    {
//      printf (" I detect a pip attempt \n");
      return 3;
    } else if (*args[i] == '!' && *args[i+1] == '!')
    {
//      printf (" I detect a pip attempt \n");
      return 4;
    }
    i++;
  }
//  printf("I detect PURE COMMAND \n");
  return 0;
}

bool AMPERSAND (char *args[])
{
  int i = 0;
  while (args[i] != NULL)
  { 
  printf ("\n");
  if (*args[i] == '&')
  {
//    printf (" arguments contain ampersand \n");
    args[i] = NULL;  	        
    return true;
  }          
  i++;
  } 
//  printf (" arguments do NOT contain ampersand \n");
  return false;
}

/*
void REPEAT_COMMAND (char *args[])
{ 

    bool ampersand_test = AMPERSAND (args);

//  printf ("inside PURE_COMMAND function \n");
//  printf ("result of ampercent search is %d \n \n", ampersand_test);

    pid_t child_ID = fork ();
    
    if (child_ID == 0) 
    {
    int r = execvp (args[0], args);
    } else  
    {        
    if (!(ampersand_test))
    {
      wait(NULL);
    }
  }
  return;
}

*/

void PURE_COMMAND (char *args []) 
{
  bool ampersand_test = AMPERSAND (args);

//  printf ("inside PURE_COMMAND function \n");
//  printf ("result of ampercent search is %d \n \n", ampersand_test);

  pid_t child_ID = fork ();
    
  if (child_ID == 0) 
  {
  int r = execvp (args[0], args);
  } else  
  {        
    if (!(ampersand_test))
    {
      wait(NULL);
    }
  }
  return;
}

int LOCATE_POSITION (char *args[])
{   
  int i = 0;
  while (args[i] != NULL)
  {      
//    if (*args[i] == '>' || *args[i] == '<' || *args[i] == '|') 
  
    if (*args[i] == '>' || *args[i] == '<' || *args[i] == '|' || *args[i] == '!' ) 

  {
      printf ("\n");
      printf (" I detect an %s in position %i \n", args[i], i);
      args[i] = NULL;
      return i;
    }
  i++;
  }
}   

void OUTPUTSTREAM_COMMAND (char *args []) 
{
  bool ampersand_test = AMPERSAND (args);
  int position = LOCATE_POSITION (args);
  char* file_name = args[position + 1];
 
//  printf ("inside OUTPUTSTREAM_COMMAND function \n");

//  printf ("result of ampercent search is %d \n \n", ampersand_test);
   
//  printf ("FILE EXTRACTED NAME IS %s ", file_name); 

  pid_t child_ID = fork();
  if (child_ID == 0) 
  {
    int output_stream = open(file_name,O_CREAT | O_WRONLY, 0600);
    dup2(output_stream, STDOUT_FILENO);
    close (output_stream);
    execvp (args[0], args);       
  } else  
  {
    if (!(ampersand_test))
    {
      wait(NULL);
    }
  }
  return;
}

void INPUTSTREAM_COMMAND (char *args [])
{
  bool ampersand_test = AMPERSAND (args);
  int position = LOCATE_POSITION (args);
  char* file_name = args[position + 1];
  
//  printf ("inside INPUTSTREAM_COMMAND function \n");

//  printf ("result of ampercent search is %d \n \n", ampersand_test);

//  printf ("FILE EXTRACTED NAME IS %s ", file_name); 

  pid_t child_ID = fork ();
  if (child_ID == 0) 
  {
    int input_stream = open(file_name, O_RDONLY, 0600);
    dup2(input_stream, STDIN_FILENO);
    close (input_stream);
    execvp (args[0], args);       
  } else  
    {        
      if (!(ampersand_test))
      {
	wait(NULL);
      }
   }
   return;
}

void PIPE_COMMAND (char *args [])
{
  bool ampersand_test = AMPERSAND (args);
  int position = LOCATE_POSITION (args);

//  printf ("inside PIPE_COMMAND function \n");

//  printf ("result of ampercent search is %d \n \n", ampersand_test);

  int i = position + 1;
  char* r[2];    
  int j = 0;
  args[position] = NULL;
  while (args[i] != NULL) 
  {
    r[j] = args[i];
    args[i] = NULL;
    i++;
    j++;    
  }
  r[j] = NULL;

  pid_t child_ID = fork ();
  if (child_ID == 0) 
  {
    int arr[2];
    pipe (arr); 
    pid_t child2_ID = fork ();
    if (child2_ID == 0) 
    { 
      close (arr[1]);   
      dup2(arr[0], STDIN_FILENO);
      execvp (r[0],r); // FIX - COMMAND AFTER PIPE 
    }
    close (arr[0]); 
    dup2(arr[1], STDOUT_FILENO);
    execvp (args[0], args);  // before pipe     
  } else  
  {        
    if (!(ampersand_test))
    {
      wait(NULL);
    }       
 }
 return;
}



