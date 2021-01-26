/*
 Name: Xinyue Li
 UID: 116560740
 DID: sarahl
 */
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <err.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "command.h"
#include "executor.h"

/* static void print_tree(struct tree *t); */
int execute_helper(struct tree *t, int input_fd, int output_fd);

int execute(struct tree *t) {
    execute_helper(t, -1, -1);

   /* print_tree(t); */

   return 0;
}

int execute_helper(struct tree *t, int input_fd, int output_fd){
    pid_t pid, pid1, pid2;
    int status, pipe_fd[2];
    
    if (t->conjunction == NONE){
        if (!strcmp(t->argv[0], "exit")) {
            exit(0);
        }else if (!strcmp(t->argv[0], "cd")){
            if(!t->argv[1]) {
              if((chdir(getenv("HOME"))) < 0) {
                perror("Failed to change directory");
              }
            }else{
                if(chdir(t->argv[1]) < 0){
                    perror(t->argv[1]);
                }
            }
        }else {
            if ((pid = fork()) < 0) {
                err(EX_OSERR, "fork error");
            }
            
            if (pid != 0) { /* parent */
               wait(&status);
               return status;
            }else { /* child */
                if (t->input) {
                    if ((input_fd = open(t->input, O_RDONLY)) < 0) {
                        err(EX_OSERR, "Input file couldn't be opened.\n");
                    }
                    if (dup2(input_fd, STDIN_FILENO) < 0) {
                        err(EX_OSERR, "dup2 input failed.\n");
                    }
                    close(input_fd);
                }
                
                if (t->output) {
                    if ((output_fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0) {
                        err(EX_OSERR, "Output file couldn't be opened.\n");
                    }
                    if (dup2(output_fd, STDOUT_FILENO) < 0) {
                        err(EX_OSERR, "dup2 output failed.\n");
                    }
                    close(output_fd);
                }
                
                execvp(t->argv[0],t->argv);
                fprintf(stderr,"Failed to execute %s\n",t->argv[0]);
                fflush(stdout);
                exit(EX_OSERR);
            }
        }
    }else if (t->conjunction == AND){
        if(execute_helper(t->left, input_fd, output_fd) ==0){
           return execute_helper(t->right, input_fd, output_fd);
        }
        return -1;
    }else if (t->conjunction == PIPE){
        if(pipe(pipe_fd) < 0) {
            perror("pipe error");
            exit(EX_OSERR);
        }

        if( (pid1 = fork()) < 0) {
            perror("fork error");
            exit(EX_OSERR);
        }

        if(pid1 == 0) { /* CHILD #1 CODE ****************/
            close(pipe_fd[0]);
            if(dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
                perror("dup2 error");
                exit(EX_OSERR);
            }
            close(pipe_fd[1]);
            execute_helper(t->left, input_fd, output_fd);
            exit(0);
        } else {         /* parent code */
            if( (pid2 = fork()) < 0) {
                perror("fork error");
                exit(EX_OSERR);
            }

            if(pid2 == 0) {  /* CHILD #2 CODE ************/
                close(pipe_fd[1]);
                if(dup2(pipe_fd[0], STDIN_FILENO) < 0) {
                    perror("dup2 error");
                    exit(EX_OSERR);
                }
                close(pipe_fd[0]);
                execute_helper(t->right, input_fd, output_fd);
                exit(0);
            } else {       /* parent code */
                close(pipe_fd[0]);
                close(pipe_fd[1]);
                wait(NULL);
                wait(NULL);
            }
         }
    }else if (t->conjunction == SUBSHELL){
        if (t->input) {
            if ((input_fd = open(t->input, O_RDONLY)) < 0) {
                err(EX_OSERR, "Input file couldn't be opened.\n");
            }
            if (dup2(input_fd, STDIN_FILENO) < 0) {
                err(EX_OSERR, "dup2 input failed.\n");
            }
            close(input_fd);
        }
        
        if (t->output) {
            if ((output_fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC, 0664)) < 0) {
                err(EX_OSERR, "Output file couldn't be opened.\n");
            }
            if (dup2(output_fd, STDOUT_FILENO) < 0) {
                err(EX_OSERR, "dup2 output failed.\n");
            }
            close(output_fd);
        }
        
        if ((pid = fork()) < 0) {
            err(EX_OSERR, "fork error");
        }
        
        if (pid != 0){ /* parent */
            wait(NULL);
        }else { /* child */
            execute_helper(t->left, input_fd, output_fd);
            exit(0);
        }
    }
    
    return 0;
}

/*
static void print_tree(struct tree *t) {
   if (t != NULL) {
      print_tree(t->left);

      if (t->conjunction == NONE) {
         printf("NONE: %s, ", t->argv[0]);
      } else {
         printf("%s, ", conj[t->conjunction]);
      }
      printf("IR: %s, ", t->input);
      printf("OR: %s\n", t->output);

      print_tree(t->right);
   }
}
*/
