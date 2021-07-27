#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
//#include<iostream>

#define MAXCOM 1000
#define MAXLIST 100

#define clear() printf("\033[H\033[J")

//using namespace std;

void openHelp()
{
    puts("\n***WELCOME TO MY SHELL HELP***"

         "\nList of Commands supported:"
         "\n>cd"
         "\n>ls"
         "\n>ShowFileDetails"
         "\n>Output redirection ex : ls > file.txt"
         "\n>exit"
         "\n>all other general commands available in UNIX shell"
         "\n>pipe handling"
         "\n>improper space handling");

    return;
}

void init_shell()
{
    clear();
    printf("\n\n\n\n******************"
           "************************");
    printf("\n\n\n\t****MY SHELL****");

    printf("\n\n\n\n*******************"
           "***********************");
    char *username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
    openHelp();
    sleep(5);
    clear();
}

int takeInput(char *str)
{
    char *buf;

    buf = readline("\n>>> ");
    if (strlen(buf) != 0)
    {
        add_history(buf);
        strcpy(str, buf);
        return 0;
    }
    else
    {
        return 1;
    }
}

void printDir()
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("\nDir: %s", cwd);
}

void execArgs(char **main_command)
{
    pid_t pid = fork();

    if (pid == -1)
    {
        printf("\nFailed forking child..");
        return;
    }
    else if (pid == 0)
    {
       //  printf("\n inside execargs ..... child process  %s\n",main_command[0]);
        if (execvp(main_command[0], main_command) < 0)
        {
            printf("\nCould not execute command..");
        }
        exit(0);
    }
    else
    {
        wait(NULL);
        return;
    }
}

void execArgsPiped(char **pipe_1st_command, char **pipe_2nd_command)
{
    int pipefd[2];
    pid_t p1, p2;

    if (pipe(pipefd) < 0)
    {
        printf("\nPipe could not be initialized");
        return;
    }
    p1 = fork();
    if (p1 < 0)
    {
        printf("\nCould not fork");
        return;
    }

    if (p1 == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        if (execvp(pipe_1st_command[0], pipe_1st_command) < 0)
        {
            printf("\nCould not execute command 1..");
            exit(0);
        }
    }
    else
    {
        p2 = fork();

        if (p2 < 0)
        {
            printf("\nCould not fork");
            return;
        }

        if (p2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if (execvp(pipe_2nd_command[0], pipe_2nd_command) < 0)
            {
                printf("\nCould not execute command 2..");
                exit(0);
            }
        }
        else
        {

            wait(NULL);
            wait(NULL);
        }
    }
}

void filedetails()
{
    struct stat s;
    char *filename, *atime, *mtime;
    int size, inodes, userid, groupid, links;
    printf(" Files in current directory :\n");
    system("ls -p | egrep -v /$");
    printf("Enter file name :\n");
    filename = readline("> ");
    int fd = open(filename, O_WRONLY | O_CREAT);
    mode_t file_perm;
    fstat(fd, &s);
    printf("File Name : %s\n", filename);
    printf("File type : ");
    if (S_ISREG(s.st_mode))
        printf("Regular file \n\n");

    else if (S_ISDIR(s.st_mode))
        printf("Directory file\n\n");

    else if (S_ISLNK(s.st_mode))
        printf("Symbolic Link\n\n");

    else
        printf("Other\n\n");

    file_perm = s.st_mode & ~S_IFMT;
    printf("User permissions: ");
    if (file_perm && S_IRUSR)
        printf("Readable ");
    else
        printf("Non Readable ");

    if (file_perm && S_IWUSR)
        printf("Writable ");
    else
        printf("Not Writable ");

    if (file_perm && S_IXUSR)
        printf("Executable\n");
    else
        printf("not Executable\n");

    printf("Group permissions :");
    if (file_perm && S_IRGRP)
        printf("Readable ");
    else
        printf("Non Readable ");

    if (file_perm && S_IWGRP)
        printf("Writable ");
    else
        printf("Not Writable ");

    if (file_perm && S_IXGRP)
        printf("Executable\n");
    else
        printf("not Executable\n");

    printf("Other :");
    if (file_perm && S_IROTH)
        printf("Readable ");
    else
        printf("Non Readable ");

    if (file_perm && S_IWOTH)
        printf("Writable ");
    else
        printf("Not Writable ");

    if (file_perm && S_IXOTH)
        printf("Executable\n");
    else
        printf("not Executable\n");

    size = s.st_size;
    inodes = s.st_ino;
    userid = s.st_uid;
    groupid = s.st_gid;
    links = s.st_nlink;
    atime = ctime(&s.st_atime);
    mtime = ctime(&s.st_mtime);

    printf("\nFile size in bytes : %d \n", size);
    printf("File Inode number : %d \n", inodes);
    printf("User ID : %d \n", userid);
    printf("Group ID : %d \n", groupid);
    printf("Number of links : %d \n", links);
    printf("Time of last access : %s", atime);
    printf("Time of last modification : %s", mtime);
}

void readfile()
{
  printf("Inside readfile function\n");


}

int ownCmdHandler(char **parsed)
{
    int NoOfOwnCmds = 6, i, switchOwnArg = 0;
    char *ListOfOwnCmds[NoOfOwnCmds];
    char *username;

    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "ShowFileDetails";
    ListOfOwnCmds[5] = "ReadFile";

    for (i = 0; i < NoOfOwnCmds; i++)
    {
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0)
        {
            switchOwnArg = i + 1;
            break;
        }
    }

    switch (switchOwnArg)
    {
    case 1:
        printf("\nGoodbye\n");
        exit(0);
    case 2:
        chdir(parsed[1]);
        return 1;
    case 3:
        openHelp();
        return 1;
    case 4:
        username = getenv("USER");
        printf("\nHello %s.\nMind that this is "
               "not a place to play around."
               "\nUse help to know more..\n",
               username);
        return 1;
    case 5:
        filedetails();
        return 1;
    case 6:
          readfile();
          return 1;
    default:
        break;
    }

    return 0;
}

int parsePipe(char *str, char **strpiped)
{
    int i;
    for (i = 0; i < 2; i++)
    {
        strpiped[i] = strsep(&str, "|");
        if (strpiped[i] == NULL)
            break;
    }

    if (strpiped[1] == NULL)
        return 0;

    else
    {
        return 1;
    }
}

void parseSpace(char *str, char **main_command)
{
    int i;

    for (i = 0; i < MAXLIST; i++)
    {
        main_command[i] = strsep(&str, " ");

        if (main_command[i] == NULL)
            break;
        if (strlen(main_command[i]) == 0)
            i--;
    }
    
   // printf("\nInside parse space str = %s and main[0] = %s , main[1]=%s \n",str,main_command[0],main_command[1]);
}

int outputredirection(char *str)
{

   
    int i=0;
    char *strpiped[2];
    
    
    
    
    for (i = 0; i < 2; i++)
    {
        strpiped[i] = strsep(&str, ">");
        if (strpiped[i] == NULL)
            return 0;
    }

    if (strpiped[1] == NULL)
        return 0;

    else
    {
        char d[50];
        bzero(d,sizeof(d));
        strcat(d,strpiped[0]);
        strcat(d,">");
        strcat(d,strpiped[1]);

        system(d);
        

        return 1;
    }
}

int processString(char *str, char **main_command, char **pipe_2nd_command)
{

    char *strpiped[2];
    int piped = 0;
    //strcpy(str1,str);
    
     if (strstr(str,">")!=NULL)
    {
         outputredirection(str);
        return 0;
    }

    piped = parsePipe(str, strpiped);
    
   

    if (piped)
    {
        parseSpace(strpiped[0], main_command);
        parseSpace(strpiped[1], pipe_2nd_command);
    }
    else
    {
  //  printf("\nInside process string str = %s and main = %s \n",str,main_command[0]);

        parseSpace(str, main_command);
    }

    if (ownCmdHandler(main_command))
        return 0;
   
    else
        return 1 + piped;
}

int main()
{
    char inputString[MAXCOM], *parsedArgs[MAXLIST];
    char *parsedArgsPiped[MAXLIST];
    int execFlag = 0;
    init_shell();

    while (1)
    {
        printDir();
        if (takeInput(inputString))
            continue;
        execFlag = processString(inputString,parsedArgs, parsedArgsPiped);

        if (execFlag == 1)
            execArgs(parsedArgs);

        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    return 0;
}
