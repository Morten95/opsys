#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


void process(int number, int time)
{
  printf("Prosess %d kjører\n", number);
  sleep(time);
  printf("Prosess %d kjørte i %d sekunder\n", number, time);
}

int main()
{
  pid_t pid1, pid2;
  
  pid1 = fork();
  if (pid1 == 0)
  {
    process(0,1);
    exit(0);  
  }

  pid2= fork();
  if (pid2 == 0)
  {
    process(2,3);
    exit(0);
  }

  waitpid(pid1, NULL, 0);
  pid1=fork();
  if (pid1 == 0)
  {
    process(4,3);
    exit(0);
  }

  pid2=fork();
  if (pid2 == 0)
  {
    process(1,2);
    exit(0);
  }

  waitpid(pid2, NULL, 0);
  pid2=fork();
  if (pid2 == 0)
  {
    process(3,2);
    exit(0);
  }

  waitpid(pid1, NULL, 0);
  pid1=fork();
  if (pid1 == 0)
  {
    process(5,4);
    exit(0);
  }  
  return 0;
}
