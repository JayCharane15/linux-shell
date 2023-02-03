#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include <sys/wait.h>
#include<errno.h>
#include<stdlib.h>
#include <fcntl.h>
#include<sys/stat.h>
#include <signal.h>
#include <ctype.h>

#define MAX 50
#define READ_END 0
#define WRITE_END 1 
#define MIN 0

char PATH[50][50] = {"/usr/local/sbin","/usr/local/bin","/usr/sbin","/usr/bin","/sbin","/bin","/usr/games","/usr/local/games","/snap/bin","/snap/bin"};
int path_num = 10;

char history[50][50];
int commands_count = 0;

void handle_sigtstp(int sig);




char* remove_red_spaces(char* str)
{
    char* res = malloc(sizeof(char)*100);
    int n = strlen(str);

    int i = 0;
    int k = 0;

    while(str[i] == ' ')
    {
        i++;

    }
    while(str[i] != ' ')
    {
        res[k++] = str[i++];
    }

    // int count = 0;
    int j;
    for(j = i; j < n; j++)
    {
        res[k++] = ' ';
        while(str[j] == ' ')
        {
            j++;
        }
        while(str[j] != ' ')
        {
            res[k++] = str[j++];
        }
    

    }

    return res;
    
    
}

char** string_to_word_del_for_path(char* str,char del)
{
    // char newString[10][10]; 
    int i,j,ctr;

    char** newString = malloc(sizeof(char*)*50);
    for(i=0;i<10;i++)
    {
        newString[i] = malloc(sizeof(char)*50);
    }


    j=0; ctr=0;
    for(i=5;i<=(strlen(str));i++)
    {
        // if space or NULL found, assign NULL into newString[ctr]
        if(str[i]==del||str[i]=='\0')
        {
            newString[ctr][j]='\0';
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            newString[ctr][j]=str[i];
            j++;
        }
    }
    newString[ctr] = NULL;

    // for(i=0;i < ctr;i++)
    //     printf(" %s\n",newString[i]);

    return newString;

}

char** string_to_word_del(char* str,char del)
{
    // char newString[10][10]; 
    int i,j,ctr;

    char** newString = malloc(sizeof(char*)*50);
    for(i=0;i<10;i++)
    {
        newString[i] = malloc(sizeof(char)*50);
    }


    j=0; ctr=0;
    for(i=0;i<=(strlen(str));i++)
    {
        // if space or NULL found, assign NULL into newString[ctr]
        if(str[i]==del||str[i]=='\0')
        {
            newString[ctr][j]='\0';
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            newString[ctr][j]=str[i];
            j++;
        }
    }
    newString[ctr] = NULL;

    // for(i=0;i < ctr;i++)
    //     printf(" %s\n",newString[i]);

    return newString;

}


char** string_to_word(char* str)
{
    // char newString[10][10]; 
    int i,j,ctr;

    char** newString = malloc(sizeof(char*)*10);
    for(i=0;i<10;i++)
    {
        newString[i] = malloc(sizeof(char)*10);
    }


    j=0; ctr=0;
    for(i=0;i<=(strlen(str));i++)
    {
        // if space or NULL found, assign NULL into newString[ctr]
        if(str[i]==' '||str[i]=='\0')
        {
            newString[ctr][j]='\0';
            ctr++;  //for next word
            j=0;    //for next word, init index to 0
        }
        else
        {
            newString[ctr][j]=str[i];
            j++;
        }
    }
    newString[ctr] = NULL;

    // for(i=0;i < ctr;i++)
    //     printf(" %s\n",newString[i]);

    return newString;

}

int main()
{
    struct sigaction sa;
    sa.sa_handler = &handle_sigtstp;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa, NULL);

    
    
    
    int pid;

    char prompt[100];
    getcwd(prompt,sizeof(prompt));
    strcpy(PATH[path_num++],prompt);
    strcat(prompt,"$ ");
    
    

    char** part1 = malloc(sizeof(char*)*50);
    char** part2 = malloc(sizeof(char*)*50);
    for(int i = 0; i < 50; i++)
    {
        part1[i] = malloc(sizeof(char)*50);
        part2[i] = malloc(sizeof(char)*50);
    }
    char full_path1[100];
    char full_path2[100];

    int fd[2];





    char* buf = malloc(sizeof(char)*MAX);
    int flag = 0;
    while(1)
    {
        char current_wd[100];
        getcwd(current_wd,sizeof(current_wd));
        
        printf("%s",prompt);
        if(fgets(buf,MAX,stdin) == NULL)
        {
            printf("Bye\n");
            exit(1);
        }
        else
        {
            // printf("%s\n",buf);
            char* res = malloc(sizeof(char)*50);
            res = remove_red_spaces(buf);
            strcpy(buf,res);
            free(res);
            
            if(strcmp(buf,"exit\n") == 0)
            {
                printf("Bye");
                exit(1);
                // return 0;
            }
            char* token = strtok(buf, "\n");
            if(token == NULL)
            {
                fprintf(stderr, "Only Delimiters");
            }

            
            // FILE* fptr;
            // fptr = fopen("/home/jayraj/os/Assignments/shell1/history.txt", "a+");
            // if(fptr == NULL)
            // {
            //     // printf("Entered here");
            //     return -1;
            // }
            // fprintf(fptr, "%s\n",token);
            // // printf("Checkpoint 1\n");
            // // if(fclose(fptr) == EOF)
            // // {
            // //     printf("Error closing file");

            // // }

  
            // // printf("Command enetered is %s",token);
            strcpy(history[commands_count++], token);


            
            if(token[0] == 'P' && token[1] == 'S')
            {
                if(token[5] != '\\' && token[6] != 'w' && token[7] != '$')
                {
                    flag = 1;
                    int k = 0;
                    for(int i = 5; i < strlen(token)-1; i++)
                    {
                        prompt[k++] = token[i];
            

                    }
                    prompt[k] = '\0';
                }
                else
                {
                    strcpy(prompt,current_wd);
                    strcat(prompt,"$ ");
                    

                }
                
                
                
        
            
            }
            else if(strcmp(token, "history") == 0)
            {
                // FILE* fp = fopen("/home/jayraj/os/Assignments/history.txt", "r");
                // char str[100];
                // int i = 0;
                // while(fgets(str,100, fp) != NULL)
                // {
                //     printf("%d %s\n",i,str);
                //     i++;
                // }
                // fclose(fp);
                int k = 0;
                for(k=commands_count-1;k>=0;k--)
                {
                    printf("%d %s\n",k+1, history[k]);
                }

            }
            else if(strcmp(token,"echo $PATH") == 0)
            {
                for(int i = 0; i < path_num; i++)
                {
                    printf("%s:",PATH[i]);
                }
                printf("\n");

        
            }
            else if(token[0] == 'P' && token[1] == 'A' && token[2] == 'T')
            {
                // printf("Came inside");
                char** newString = string_to_word_del_for_path(token,':');
                // for(int i=0;i<10;i++)
                // {
                //     newString[i] = malloc(sizeof(char)*50);

                // }
                



                int i = 0;
                path_num = 0;
                while(newString[i] != NULL)
                {
                    
                    strcpy(PATH[path_num++], newString[i]);
                    i++;
                }
                // continue;
                // for(int i = 0; i < path_num; i++)
                // {
                //     printf("%s\n",PATH[i]);
                // }
                getcwd(current_wd,sizeof(current_wd));
                strcpy(PATH[path_num++],current_wd);


                


            }
            else if(token[0] == 'c' && token[1] == 'd')
            {
                char** newString = malloc(sizeof(char*)*10);
                for(int i=0;i<10;i++)
                {
                    newString[i] = malloc(sizeof(char)*10);
                }

                newString = string_to_word(token);

                chdir(newString[1]);

                getcwd(prompt,sizeof(prompt));
                strcpy(PATH[path_num++],prompt);

                strcat(prompt,"$ ");


            }
            else
            {
                char** newString = malloc(sizeof(char*)*10);
                for(int i=0;i<10;i++)
                {
                    newString[i] = malloc(sizeof(char)*10);
                }

                newString = string_to_word(token);
                // printf("Checkpoint 2\n");


                
                char** temp = string_to_word(token);
                char** redirect = malloc(sizeof(char*)*20);
                int fd_redirect;
                int k = 0;
                int redirect_flag = 0;
                int rediect_flag1 = 0;

                int pipe_flag = 0;
                int pipe_count = 0;
                while(temp[k])
                {
                    if(strcmp(temp[k], ">") == 0)
                    {
                        // printf("Strcmp is 0");
                        redirect_flag = 1;
                        break;

                    }
                    if(strcmp(temp[k], "<") == 0)
                    {
                        rediect_flag1 = 1;
                        break;

                    }
                    if(strcmp(temp[k], "|") == 0)
                    {
                        pipe_flag = 1;
                        int k1 = k;
                        k1++;
                        pipe_count++;
                        while(temp[k1])
                        {
                            if(strcmp(temp[k1], "|") == 0)
                            {
                                pipe_count++;
                            }

                            k1++;

                        }
                        // printf("Pipe coutn is %d", pipe_count);

                        break;
                        // int pipe_greater_than1 = 0;

                    }
                    k++;
                }

                if(pipe_flag && pipe_count == 1)
                {

                    // printf("Entered here \n");
                    for(int i = 0; i < k; i++)
                    {
                        strcpy(part1[i], temp[i]);
                        // printf("%s\n",part1[i]);

                        if(i == k-1)
                        {
                            part1[k] = NULL;

                        }
                    }

                    k++;
                    int i = 0;
                    // printf("PART2: ");
                    while(temp[k])
                    {
                        
                        //printf("temp[k] is %s\n",temp[k]);
                        strcpy(part2[i], temp[k++]);
                        // printf("part[2] is %s\n",part2[i]);
                        i++;
                    }
                    part2[i] = NULL;

                    for(int i = 0; i < path_num; i++)
                    {
                        strcpy(full_path1, PATH[i]);
                        strcat(full_path1,"/");
                        strcat(full_path1, part1[0]);
                        FILE* fp = fopen(full_path1,"r");
                        if(fp != NULL)
                        {
                            // printf("PATH1 is %s\n",full_path1);
                            break;
                        }
                    }

                     for(int i = 0; i < path_num; i++)
                    {
                        strcpy(full_path2, PATH[i]);
                        strcat(full_path2,"/");
                        strcat(full_path2, part2[0]);
                        FILE* fp = fopen(full_path2,"r");
                        if(fp != NULL)
                        {
                            // printf("PATH2 is %s\n",full_path2);
                            
                            break;
                        }
                    }

                    if(pipe(fd) == -1)
                    {
                        fprintf(stderr, "Pipe failed");
                        return 1;
                    }
                    pid = fork();
                // printf("hii");
                    int pid2;
                    if(pid == 0)
                    {
                        pid2 = fork();
                        if(pid2 == 0)
                        {
                            dup2(fd[READ_END], STDIN_FILENO);
                            close(fd[READ_END]);
                            close(fd[WRITE_END]);
                            execv(full_path2, part2);
                            // printf("hii");
                            // exit(0);
                            waitpid(pid2, NULL, 0);

                        }
                        else
                        {

                            dup2(fd[WRITE_END], STDOUT_FILENO);
                            close(fd[READ_END]);
                            close(fd[WRITE_END]);
                            execv(full_path1, part1);
                            waitpid(pid2, NULL, 0);

                            // wait(NULL);
                        
                        }
                        
                    }
                    else
                    {
                        wait(NULL);
                    }

                    // pid = fork();


                }
                else if(pipe_count > 1)
                {
                    char*** pipe_parts = malloc(sizeof(char**)*(pipe_count+1));
                    for(int i = 0 ; i < pipe_count+1; i++)
                    {
                        pipe_parts[i] = malloc(sizeof(char*)*10);
                        for(int j = 0; j < 10; j++)
                        {
                            pipe_parts[i][j] = malloc(sizeof(char)*50);
                        }
                    }

                    char** command_in_words = string_to_word(token);
                    int i = 0;
                    int c1 = 0;
                    int c2 = 0;
                    while(command_in_words[i])
                    {
                        if(strcmp(command_in_words[i], "|") == 0)
                        {
                            pipe_parts[c1][c2] = NULL;
                            // printf("Null string is %s",pipe_parts[c1][c2]);
                            c2 = 0;
                            c1++;
                            // printf("\n");

                        }
                        else
                        {
                            strcpy(pipe_parts[c1][c2++], command_in_words[i]);
                            // printf("%s\n", pipe_parts[c1][c2-1]);
                        }
                        i++;
                    }
                    pipe_parts[c1][c2] = NULL;

                    pid = fork();

                    if(pid == 0)
                    {

                        int pipes[pipe_count][2];
                        for(int i = 0; i < pipe_count; i++)
                        {
                            if(pipe(pipes[i]) == -1)
                            {
                                printf("Error creating pipes");
                            }
                        }

                        int pids[pipe_count];
                        // int pids[pipe_count];
                        // int i;
                        char full_path_n[100];
                        for(i = 0 ;i < pipe_count; i++)
                        {
                            pids[i] = fork();
                            if(pids[i] == 0)
                            {
                                for (int j = 0; j < pipe_count; j++) 
                                {
                                    if (i != j) {
                                        close(pipes[j][0]);
                                    }
                                    if (i + 1 != j) {
                                        close(pipes[j][1]);
                                    }
                                }

                                close(0);
                                dup(pipes[i][0]);


                                if(i != pipe_count-1)
                                {
                                    close(1);
                                    dup(pipes[i+1][1]);

                                }


                                for(int i = 0; i < path_num; i++)
                                {

                                    strcpy(full_path_n,PATH[i]);
                                    strcat(full_path_n,"/");
                                    strcat(full_path_n, pipe_parts[i+1][0]);
                                    // printf("Checkpoint 3\n");

                                    int ret = execv(full_path_n,pipe_parts[i+1]);
                                    if(ret == -1 && i == path_num-1)
                                    {
                                        perror("Execution failed: ");
                                        exit(errno);

                                    }
                                }

                                close(pipes[i][0]);
                                close(pipes[i+1][0]);

                                return 0;

                                




                                
                            }

                        }

                        
                        close(1);
                        dup(pipes[1][1]);

                        close(pipes[0][0]);

                        for(int i = 1; i < pipe_count; i++)
                        {
                            close(pipes[i][0]);
                            if(i != 1)
                            {
                                close(pipes[i][1]);

                            }
                        }

                        for(int i = 0; i < path_num; i++)
                        {

                            strcpy(full_path_n,PATH[i]);
                            strcat(full_path_n,"/");
                            strcat(full_path_n, pipe_parts[0][0]);
                            // printf("Checkpoint 3\n");

                            int ret = execv(full_path_n,pipe_parts[0]);
                            if(ret == -1 && i == path_num-1)
                            {
                                perror("Execution failed: ");
                                exit(errno);

                            }
                        }

                        close(pipes[1][1]);

                        for(int i = 0; i < pipe_count; i++)
                        {
                            wait(NULL);
                        }
                    }
                    else
                    {
                        wait(NULL);
                    }



                    // printf("%s %s",pipe_parts[c1][c2-1], pipe_parts[c1][c2]);





                }
                else
                {

                    pid = fork();
                    // printf("Fork is being called");

                    char full_path[100];

                    if(pid == 0)
                    {
                        if(redirect_flag)
                        {
                            // printf("\nOp redirection detected");
                            close(1);
                            fd_redirect = open(temp[++k],O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                            if(fd_redirect == -1)
                            {
                                perror("Open failed: ");
                            }
                            k--;
                            int i;
                            for(i=0;i<k;i++)
                            {
                                redirect[i] = temp[i];
                            }
                            redirect[i] = NULL;

                            strcpy(full_path,"/bin/");
                            strcat(full_path,redirect[0]);
                            
                            int ret = execv(full_path,redirect);
                            if(ret == -1)
                            {
                                perror("Execution failed: ");
                                exit(errno);

                            }
                            close(1);
                            // char* file = stdout;
                            // open();
                            

                        }
                        else if(rediect_flag1)
                        {
                            close(0);
                            fd_redirect = open(temp[++k],O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
                            if(fd_redirect == -1)
                            {
                                perror("Open failed: ");
                            }
                            k--;
                            int i;
                            for(i=0;i<k;i++)
                            {
                                redirect[i] = temp[i];
                            }
                            redirect[i] = NULL;

                            strcpy(full_path,"/bin/");
                            strcat(full_path,redirect[0]);
                        
                            
                            int ret = execv(full_path,redirect);
                            if(ret == -1)
                            {
                                // printf("Entered here");
                                perror("Execution failed: ");
                                exit(errno);

                            }


                        }
                        // else if(pipe_flag)
                        // {
                        //     // printf("Child entered here");
                        //     // close(0);
                        //     // dup(fd[READ_END]);
                        //     // close(fd[WRITE_END]);
                        //     // int ret1 = execv(full_path2, part2);
                        //     // if(ret1 == -1)
                        //     // {
                        //     //     perror("Execution failed");
                        //     //     return errno;
                        //     // }
                            
                        //     dup2(fd[READ_END], STDIN_FILENO);
                        //     close(fd[READ_END]);
                        //     close(fd[WRITE_END]);
                        //     execv(full_path2, part2);
        

                        // }
                        else
                        {
                            for(int i = 0; i < path_num; i++)
                            {

                                strcpy(full_path,PATH[i]);
                                strcat(full_path,"/");
                                strcat(full_path,newString[0]);
                                // printf("Checkpoint 3\n");

                                int ret = execv(full_path,newString);
                                if(ret == -1 && i == path_num-1)
                                {
                                    perror("Execution failed: ");
                                    exit(errno);

                                }
                            }

                        }
                        
                    

                    }
                    else
                    {
                        // if(pipe_flag)
                        // {
                        //     // printf("Parent entered here");
                        //     // close(1);
                        //     // dup(fd[WRITE_END]);
                        //     // close(fd[READ_END]);
                        //     // execv(full_path1, part1);
                        //     // printf("Exiting now");
                        //     // int ret2 = execv(full_path2, part2);
                        //     // if(ret2 == -1)
                        //     // {
                        //     //     perror("Execution failed");
                        //     //     return errno;
                        //     // }
                        //     // wait(NULL);


                        //     dup2(fd[WRITE_END], STDOUT_FILENO);
                        //     close(fd[READ_END]);
                        //     close(fd[WRITE_END]);
                        //     execv(full_path1, part1);

                        //     wait(0);


                            

                        // }
                        wait(0);
                    
                    }
                }
                // int fd[2];



            }



        }

    }

    // close(fd[0]);
    // close(fd[1]);

    // waitpid(pid, NULL, 0);
    return 0;
}


void handle_sigtstp(int sig)
{
    // fflush(stdout)
    printf("\n");
    // main();
}