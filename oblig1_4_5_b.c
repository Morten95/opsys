#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void process(int number, int time) {
printf("Prosess %d kjører\n", number);
sleep(time);
printf("Prosess %d kjørte i %d sekunder\n", number, time);
}

int main() {

   pid_t pid1, pid2;


  //starter prosess 0
   pid1 = fork();
   if (pid1 == 0) {
      process(0,1);
      exit(0);
   }
  //starter prosess 2
   pid2 = fork();
   if (pid2 == 0) {
      process(2,3);
      exit(0);
   }
   waitpid(pid1,NULL,0);
  //starter prosess 1
   pid1 = fork();
   if (pid1 == 0) {
      process(1,2);
      exit(0);
   }
  //starter prosess 4
   pid2 = fork();
   if (pid2 == 0) {
      process(4,3);
      exit(0);
   }
   waitpid(pid1,NULL,0);
  //starter prosess 3
   pid1 = fork();
   if (pid1 == 0) {
      process(3,2);
      exit(0);
   }
   waitpid(pid2,NULL,0);
  //starter prosess 5
   pid2 = fork();
   if (pid2 == 0) {
      process(5,3);
      exit(0);
   }
   waitpid(pid2,NULL,0);
   printf("Alle prosessene er ferdige!\n");

   return 0;
}
