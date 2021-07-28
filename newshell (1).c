#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAXCOM 1000
#define MAXLIST 100
#define PI 3.14159265358979323846
#define clear() printf("\033[H\033[J")

int g = 0;
int interrupt = 0;
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
    //sleep(5);
   // clear();
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
    char *filename, buf[30];
    int fd, l, h, opt;
    printf("Enter filename :\n");
    filename = readline(">");
    printf("Enter Start and end bytes :\n");
    scanf("%d", &l);
    scanf("%d", &h);
    fd = open(filename, O_RDWR);
    lseek(fd, l, SEEK_SET);
    read(fd, buf, h - l);
    printf("\nFile content :\n %s \n\n\n", buf);
    printf("Enter 1 if you want to copy this content to new file :");
    scanf("%d", &opt);
    if (opt)
    {        
        printf("Enter filename :\n");
        filename = readline(">");
        fd = open(filename, O_WRONLY | O_CREAT, 777);
        write(fd, buf, h - l);
    }
    close(fd);
}

void reversefile()
{
    int fd, fdt, len, i = 1, j = 0;
    char filename[30], buff[5], d[50];
    printf("Enter filename : ");
    scanf("%s", filename);
    fd = open(filename, O_RDONLY);
    fdt = open("tempreverse.txt", O_WRONLY | O_CREAT, 777);
    len = lseek(fd, 0, SEEK_END);

    while (i < len + 1 && j < len)
    {
        lseek(fd, -(i++), SEEK_END);
        read(fd, buff, 1);
        printf("%c", buff[0]);        
        write(fdt, buff, 1);
    }

    bzero(d, sizeof(d));
    strcat(d, "cp tempreverse.txt ");
    strcat(d, filename);
    system(d);
    system("rm -f tempreverse.txt");
    close(fd);
    close(fdt);
}

void sighandler(int sig_num)
{
    if (sig_num == SIGQUIT)
    {
        printf("\nquit signal passed\n");
    }
    if (sig_num == SIGINT)
    {
        printf("\ninterrupt signal passed\n");
        g = 1;
        interrupt = 1;
    }
    if (sig_num == SIGALRM)
    {
        if (g == 0)
        {
            printf("\nAlarm signal passed\n");
        }
        g = 1;
    }
}

void math(char **parsed)
{
    if (strcmp(parsed[1], "-p") == 0)
    {
        signal(SIGINT, sighandler);
        signal(SIGQUIT, sighandler); /* ctrl + \ */
        signal(SIGALRM, sighandler);
        int i = 2, m = 0, flag = 0;
        alarm(5);
        while (g != 1)
        {
            flag = 0;
            m = i / 2;
            for (int k = 2; k <= m; k++)
            {
                if (i % k == 0)
                {
                    flag = 1;
                }
            }
            if (flag == 0)
            {
                printf("\n%d", i);
                sleep(1);
            }
            i++;
            
        }
        if (interrupt == 1)
        {
            pause();
        }
        g = 0;
        interrupt = 0;
    }
    if (strcmp(parsed[1], "-f") == 0)
    {
       
    }
}

void calculator(char **parsed)
{
    char operation;
    int option;
    int i = 1, temp, Check, N, a, b, c, d;
    double Num1, Num2, ans, CheckF, A, B, C, A1, A2, Z, temp1;
    if (strcmp(parsed[1], "-s") == 0)
    {
        printf("Special Functions Mode.\n\n[1] Sine Function\n[2] Cosine Function\n[3] Tangent Function\n[4] Log (With base 10)\n[5] Quadratic Equation Solver\n[6] Factorial\n\nWhich mode do you want: ");
        scanf("%d", &N);
        if (N == 1)
        {
            printf("Your angle: ");
            scanf("%lf", &Num1);
            temp1 = (Num1 * 3.14 / 180);
            ans = sin(temp1);
            printf("\nanswer:\nSine(%lf) = %.2lf \n\n", Num1, ans);
        }
        else if (N == 2)
        {
            printf("Your angle: ");
            scanf("%lf", &Num1);
            temp1 = (Num1 * 3.14 / 180);
            ans = cos(temp1);
            printf("answer:\nCosine(%lf) = %.2lf \n\n", Num1, ans);
        }
        else if (N == 3)
        {
            printf("Your angle: ");
            scanf("%lf", &Num1);
            temp1 = Num1 * 3.14 / 180;
            ans = (tan(temp1));
            printf("answer:\nTangent(%lf) = %.2lf \n\n", Num1, ans);
        }
        else if (N == 4)
        {
            printf("Your number: ");
            scanf("%lf", &Num1);
            ans = log10(Num1);
            if (Num1 < 0)
            {
                printf("Cannot log a negative number; ERROR 1 Syntax\a\n");
                system("pause");
            }
            else if (Num1 == 0)
            {
                printf("Cannot log(0); ERROR 1 Syntax\a\n");
                system("pause");
            }
            else if (Num1 > 0)
                printf("answer:\nLg(%lf) = %.2lf \n\n", Num1, ans);
        }

        else if (N == 5)
        {
            printf("Welcome to Quadratic Equation solver. Enter the coefficient of X^2, followed by\nthe coefficient of X, followed by the integer value.\n\nEnter values: ");
            scanf("%lf%lf%lf", &A, &B, &C);
            CheckF = (B * B - 4 * A * C);
            if (A == 0)
            {
                ans = -(C / B);
                printf("Root of equation is %lf \n", ans);
            }
            else if (CheckF < 0)
            {
                printf("This calculator cannot handle complex numbers.\n");
            }
            else if (CheckF >= 0)
            {
                Z = pow(CheckF, 0.5);
                A1 = (-B + Z) / (A + A);
                A2 = (-B - Z) / (A + A);
                if (A1 == A2)
                {
                    ans = A1;
                    printf("\nRoots of equation are %lf and %lf \n", ans, ans);
                }
                else if (A1 != A2)
                    printf("Roots of equation are %lf and %lf \n", A1, A2);
            }
        }
        else if(N==6)
        {
           int n,fact=1,i;
           printf("Enter NUmber : ");
           scanf("%d",&n);
           for(i=0;i<=n;i++)
           {
              fact=fact*i;           
           }
           printf("Factorial of %d : %d\n",n,fact);
          
         }
        else
        {
            printf("Your input optionion is not found! ERROR 404\a\n");
            system("pause");
        }
    }
    else if (strcmp(parsed[1], "-n") == 0)
    {
        printf(" Normal maths operation Mode.\n\nYour two numbers: ");
        scanf("%lf%lf", &Num1, &Num2);
        printf("\nAVAILABLE SYMBOLS:\n\n+ for Addition\n- for Subtraction\n/ for Division\n* for Multiplication\n^ for Power function\n\\ to find root\nYour input: ");
        scanf(" %c", &operation);
        if (operation == '+')
        {
            ans = (Num1 + Num2);
            printf("answer:\n%lf  %c %lf = %.2lf \n\n", Num1, operation, Num2, ans);
        }
        else if (operation == '-')
        {
            ans = (Num1 - Num2);
            printf("answer:\n%lf  %c %lf = %.2lf \n\n", Num1, operation, Num2, ans);
        }
        else if (operation == '/')
        {
            ans = (Num1 / Num2);
            printf("answer:\n%lf  %c %lf = %.2lf \n\n", Num1, operation, Num2, ans);
        }
        else if (operation == '*')
        {
            ans = (Num1 * Num2);
            printf("answer:\n%lf  %c %lf = %.5g \n\n", Num1, operation, Num2, ans);
        }
        else if (operation == '^')
        {
            ans = (pow(Num1, Num2));
            printf("answer:\n%lf  %c %lf = %.5g \n\n", Num1, operation, Num2, ans);
        }
        else if (operation == '\\')
        {
            ans = pow(Num2, 1 / Num1);
            Check = Num1;
            temp = Check % 2;
            if (Num2 < 0)
            {
                printf("Cannot root a negative number; ERROR 1 Syntax\a\n");
                system("pause");
            }
            else if (temp == 0)
                printf("answer:\n%lf root(%lf) = - %.2lf or + %.2lf \n\n", Num1, Num2, ans, ans);

            else if (!temp == 0)
                printf("answer:\n%lf root(%lf) = + %.2lf \n\n", Num1, Num2, ans);
        }
        else
        {
            printf("\n\nYour input operator is incorrect; ERROR 1 Syntax\n");
            printf("\a\n");
            system("pause");
        }
    }
}

void showhistory()
{
   HISTORY_STATE *hisd=history_get_history_state();
   HIST_ENTRY **list=history_list();
   char tempstr[20];
   int max,i=0;
   //if(argc==NULL)
  // {
      max=hisd->length;
 //  }
  // else
 //  {
  //   max=atoi(argc);
 //  }
 
 
 
 //strcpy(tempstr,list[i]->line);
 //printf(" %d : %s \n",i+1,tempstr);
   while(i<max)
   {
      
       strcpy(tempstr,list[i]->line);
     //cout<<i+1<<":"<<list[i]->line<<endl;
     printf(" %d : %s \n",i+1,tempstr);
     i++;
     
     
   }
}



int ownCmdHandler(char **parsed)
{
    int NoOfOwnCmds = 10, i, switchOwnArg = 0;
    char *ListOfOwnCmds[NoOfOwnCmds];
    char *username;
    ListOfOwnCmds[0] = "exit";
    ListOfOwnCmds[1] = "cd";
    ListOfOwnCmds[2] = "help";
    ListOfOwnCmds[3] = "hello";
    ListOfOwnCmds[4] = "ShowFileDetails";
    ListOfOwnCmds[5] = "ReadFile";
    ListOfOwnCmds[6] = "ReverseFile";
    ListOfOwnCmds[7] = "math";
    ListOfOwnCmds[8] = "calculator";
    ListOfOwnCmds[9] = "history";
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
    case 7:
        reversefile();
        return 1;
    case 8:
        math(parsed);
        return 1;
    case 9:
        calculator(parsed);
        return 1;
    case 10:
        showhistory();
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
}

int outputredirection(char *str)
{
    int i = 0;
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
        bzero(d, sizeof(d));
        strcat(d, strpiped[0]);
        strcat(d, ">");
        strcat(d, strpiped[1]);
        system(d);
        return 1;
    }
}

int processString(char *str, char **main_command, char **pipe_2nd_command)
{
    char *strpiped[2];
    int piped = 0;
    if (strstr(str, ">") != NULL)
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
        execFlag = processString(inputString, parsedArgs, parsedArgsPiped);

        if (execFlag == 1)
            execArgs(parsedArgs);

        if (execFlag == 2)
            execArgsPiped(parsedArgs, parsedArgsPiped);
    }
    return 0;
}

