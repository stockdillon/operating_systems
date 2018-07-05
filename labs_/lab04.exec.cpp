
/******************************************************************************
  Demonstrate the use of "execle" and "execlp"
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

const char* env_init[] = { "USER=unknown", "PATH=/tmp", NULL };

int main()
{
  pid_t pid;
  int flag, status;


  /***** Create child process #1 *****/

  pid = fork();

  if (pid < 0)
  {
    perror( "fork error" );
  }
  else if (pid == 0)
  { 
    /* Child process -- replace with different program */

    flag = execle( "/bin/ls",
      "ls", "-lF", NULL, env_init );

    if (flag < 0)
    {
      perror( "execle error" );
    }
  }
  else
  {
    /* Parent process -- wait for child to terminate */

    if (wait( &status ) != pid)
    {
      perror( "wait error" );
    }
  }


  /***** Create child process #2 *****/

  pid = fork();

  if (pid < 0)
  {
    perror( "fork error" );
  }
  else if (pid == 0)
  { 
    /* Child process -- replace with different program */

    flag = execlp( "lab04.script",
      "lab04.script", "lab03", "lab04", NULL );

    if (flag < 0)
    {
      perror( "execlp error" );
    }
  }
  else
  {
    /* Parent process -- wait for child to terminate */

    if (wait( &status ) != pid)
    {
      perror( "wait error" );
    }
  }

  exit( 0 );
}
