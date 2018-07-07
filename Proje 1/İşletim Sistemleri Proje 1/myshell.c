#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int myCd(char **args);
int myHelp(char **args);
int myExit(char **args);
int plusTwo(char **args);
int minusTwo(char **args);
int factorial(char **args);
int otherCommand(char **args);

char *builtin_str[] = {
"cd",
"help",
"exit",
"plusTwo",
"minusTwo",
"factorial",
"otherCommand"};


int (*builtin_func[]) (char **) = 
{&myCd,
&myHelp,
&myExit,
&plusTwo,
&minusTwo,
&factorial,
&otherCommand};


int numBuiltins() {
  return sizeof(builtin_str) / sizeof(char *);
}



int myCd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "Arguman girilmedi! \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
//cd yap
      perror("lsh");
    }
  }
  return 1;
}



int myHelp(char **args)
{
  int i;
  printf("Erdem Çağrı GÖRÜCÜ\n");
  printf("Tanımlanan Komut Listesi\n");

  for (i = 0; i < numBuiltins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  printf("Örnek Komut Çağrıları\n");
  printf("plusTwo Arguman1 Arguman2\n");
  printf("minusTwo Arguman1 Arguman2\n");
  printf("factorial Arguman1\n");
  return 1;
}



int myExit(char **args)
{
  printf("Program Sonlandırıldı.\n");
  return 0;
}




int plusTwo(char **args){

pid_t pid;
int p,status;

pid = fork();
if (pid == 0) {
    // Child process
   if(args[1] == NULL && args[2] == NULL){
	fprintf(stderr,"Komuttan sonra iki parametre girilmeli !\n");
   }else if(args[1] == NULL || args[2] == NULL){
	fprintf(stderr,"Eksik parametre girdiniz !\n");
    }else{
	p=execve(args[0],args,NULL);
	}
    return 0;
  } else if (pid < 0) {
    // Error forking
    perror("error");
  } else {
    // Parent process
    status = wait(&status);
  }
  return 1;
}



int minusTwo(char **args){

pid_t pid;
int m,status;

pid = fork();
if (pid == 0) {
    // Child process
   if(args[1] == NULL && args[2] == NULL){
	fprintf(stderr,"Komuttan sonra iki parametre girilmeli !\n");
   }else if(args[1] == NULL || args[2] == NULL){
	fprintf(stderr,"Eksik parametre girdiniz !\n");
   }else{
	m=execve(args[0],args,NULL);
   }
    return 0;
  } else if (pid < 0) {
    // Error forking
    perror("error");
  } else {
    // Parent process
    status = wait(&status);
  }
  return 1;
}



int factorial(char **args){

pid_t pid,wpid;
int f,status;

pid = fork();
if (pid == 0) {
    // Child process
   if(args[1] == NULL){
	fprintf(stderr,"lsh : Komuttan sonra bir parametre girilmeli !\n");
   }else if(args[1] != NULL && args[2] != NULL){
 	fprintf(stderr,"lsh : Sadece bir parametre girebilirsiniz !\n");
   }else{
	f=execve(args[0],args,NULL);
   }
    return 0;
  } else if (pid < 0) {
    // Error forking
    perror("error");
  } else {
    // Parent process
//samet hocayla konuş
    //status = wait(&status);
  }
  return 1;
}


int yanlisKomut(char **args){
int i;
for (i = 0; i < numBuiltins(); i++) {
    if (strcmp(args[0], builtin_str[i]) != 0) {     
	printf("Yanlış komut girdiniz!\n");
	break;
    }
  }
}


int otherCommand(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}


int execute(char **args){
  int i,j=0;

  if (args[0] == NULL) {
    return 1;
  }
 
  if(strcmp(args[0], "clear") == 0 || strcmp(args[0], "echo") == 0 || strcmp(args[0], "cat") == 0 || strcmp(args[0], "ls") == 0){
	j=1;
	otherCommand(args);
  }

for (i = 0; i < numBuiltins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
	j=1;     
	return (*builtin_func[i])(args);
    }
  }

if(j == 0){
   	yanlisKomut(args);
    }

   return 1;
}




char *readLine(void){
  char *line = NULL;
  ssize_t bufsize = 0;
  getline(&line, &bufsize, stdin);
  return line;
}

#define BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
char **splitLine(char *line){
  int bufsize = BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens){
    fprintf(stderr, "Buffer Hatası\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "Buffer Hatası\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}


void myLoop(void){
  char *line;
  char **args;
  int status;

  do {
    printf("myshell>> ");
    line = readLine();
    args = splitLine(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status);
}


int main(){

myLoop();

return EXIT_SUCCESS;
}
