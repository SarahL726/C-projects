/*
 NAME: Xinyue Li
 UID: 116560740
 DID: sarahl
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <ctype.h>
#include "document.h"

int valid_command(Document *doc, char *command);

/* This command will add a paragraph to the document. */
int run_add_paragraph_after(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
    int sscanf_return, p_num;

    sscanf_return = sscanf(command, "%s%d%s", name, &p_num, extra);
    if (sscanf_return != 2 || p_num < 0){
       return FAILURE;
    }
    
    if (add_paragraph_after(doc, p_num) == FAILURE){
        printf("add_paragraph_after failed\n");
    }
    
    return SUCCESS;
}

/* This command will add a line after the line with the specified line number. */
int run_add_line_after(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], line[MAX_STR_SIZE + 1], rest[MAX_STR_SIZE + 1];
    int sscanf_return, p_num, l_num;
    
    sscanf_return = sscanf(command, "%s%d%d%s", name, &p_num, &l_num, rest);
    if (sscanf_return != 4 || p_num <= 0 || l_num < 0 || strchr(command, '*') == NULL){
        return FAILURE;
    }
    
    strcpy(line, strchr(command, '*') + 1);
    if (add_line_after(doc, p_num, l_num, line) == FAILURE){
        printf("add_line_after failed\n");
    }
    
    return SUCCESS;
}

/* This command will pring the document information. */
int run_print_document(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
    int sscanf_return;
    
    sscanf_return = sscanf(command, "%s%s", name, extra);
    if (sscanf_return != 1){
        return FAILURE;
    }
    
    if (print_document(doc) == FAILURE){
        printf("print_document failed\n");
    }
    
    return SUCCESS;
}

/* This command will append a line to the specified paragraph. */
int run_append_line(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], line[MAX_STR_SIZE + 1], rest[MAX_STR_SIZE + 1];
    int sscanf_return, p_num;
    
    sscanf_return = sscanf(command, "%s%d%s", name, &p_num, rest);
    if (sscanf_return != 3 || p_num <= 0 || strchr(command, '*') == NULL){
        return FAILURE;
    }
    
    strcpy(line, strchr(command, '*') + 1);
    if (append_line(doc, p_num, line) == FAILURE){
        printf("append_line failed\n");
    }
    
    return SUCCESS;
}

/* This command will remove the specified line from the paragraph. */
int run_remove_line(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
    int sscanf_return, p_num, l_num;
    
    sscanf_return = sscanf(command, "%s%d%d%s", name, &p_num, &l_num, extra);
    if (sscanf_return != 3 || p_num <= 0 || l_num <= 0){
        return FAILURE;
    }
    
    if (remove_line(doc, p_num, l_num) == FAILURE){
        printf("remove_line failed\n");
    }
    
    return SUCCESS;
}

/* This command will load the specified file into the current document. */
int run_load_file(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], file_name[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
    int sscanf_return;
    
    sscanf_return = sscanf(command, "%s%s%s", name, file_name, extra);
    if (sscanf_return != 2){
        return FAILURE;
    }
    
    if (load_file(doc, file_name) == FAILURE){
        printf("load_file failed\n");
    }
    return SUCCESS;
}

/* This command will replace the string "TARGET" with "REPLACEMENT". */
int run_replace_text(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], *str1, *str2, *str3, *str4, tmp1[MAX_STR_SIZE + 1], tmp2[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1], replacement[MAX_STR_SIZE + 1];
    int sscanf_return;
    
    sscanf_return = sscanf(command, "%s%s%s", name, tmp1, tmp2);
    if (sscanf_return != 3){
        return FAILURE;
    }

    str1 = strstr(command, "\"");
    str2 = strstr(str1 + 1, "\"");
    str3 = strstr(str2 + 1, "\"");
    str4 = strstr(str3 + 1, "\"");

    strncpy(target, str1 + 1, str2 - str1);
    strncpy(replacement, str3 + 1, str4 - str3);
    target[str2 - str1 - 1] = '\0';
    replacement[str4 - str3 - 1] = '\0';
    
    if (replace_text(doc, target, replacement) == FAILURE){
        printf("replace_text failed\n");
    }

    return SUCCESS;
}

/* This command will highlight the string "TARGET". */
int run_highlight_text(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], *str1, *str2, tmp[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1];
    int sscanf_return;
    
    sscanf_return = sscanf(command, "%s%s", name, tmp);
    if (sscanf_return != 2){
        return FAILURE;
    }
    
    str1 = strstr(command, "\"");
    str2 = strstr(str1 + 1, "\"");
    
    strncpy(target, str1 + 1, str2 - str1);
    target[str2 - str1 - 1] = '\0';
    
    highlight_text(doc, target);
    return SUCCESS;
}

/* This command will remove the string "TARGET". */
int run_remove_text(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], *str1, *str2, tmp[MAX_STR_SIZE + 1], target[MAX_STR_SIZE + 1];
    int sscanf_return;
    
    sscanf_return = sscanf(command, "%s%s", name, tmp);
    if (sscanf_return != 2){
        return FAILURE;
    }
    
    str1 = strstr(command, "\"");
    str2 = strstr(str1 + 1, "\"");
    
    strncpy(target, str1 + 1, str2 - str1);
    target[str2 - str1 - 1] = '\0';
    
    remove_text(doc, target);
    return SUCCESS;
}

/* This command will save the current document to the specified file. */
int run_save_document(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], file_name[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
    int sscanf_return;
    
    sscanf_return = sscanf(command, "%s%s%s", name, file_name, extra);
    if (sscanf_return != 2 || extra[0] != '\0'){
        return FAILURE;
    }
    
    if (save_document(doc, file_name) == FAILURE){
        printf("save_document failed\n");
    }
    
    return SUCCESS;
}

/* This command will reset the current document. */
int run_reset_document(Document *doc, char *command){
    char name[MAX_STR_SIZE + 1], extra[MAX_STR_SIZE + 1];
    int sscanf_return;
    
    sscanf_return = sscanf(command, "%s%s", name, extra);
    if (sscanf_return != 1 || extra[0] != '\0'){
        return FAILURE;
    }
    
    reset_document(doc);
    return SUCCESS;
}

/*
 Judge the validity of command
 */
int valid_command(Document *doc, char *command){
    int valid = FAILURE;
    
    if (strstr(command, "add_paragraph_after") != NULL){
        if (!run_add_paragraph_after(doc, command)){
            valid = SUCCESS;
        }
    }else if (strstr(command, "add_line_after") != NULL){
        if (!run_add_line_after(doc, command)){
            valid = SUCCESS;
        }
    }else if (strstr(command, "print_document") != NULL){
        if (!run_print_document(doc, command)){
            valid = SUCCESS;
        }
    }else if (strstr(command, "append_line") != NULL){
        if (!run_append_line(doc, command)){
            valid = SUCCESS;
        }
    }else if (strstr(command, "remove_line") != NULL){
        if (!run_remove_line(doc, command)){
            valid = SUCCESS;
        }
    }else if (strstr(command, "load_file") != NULL){
        if (!run_load_file(doc, command)){
            valid = SUCCESS;
        }
    }else if (strstr(command, "replace_text") != NULL){
        if (!run_replace_text(doc, command)){
            valid = SUCCESS;
        }
    }else if (strstr(command, "highlight_text") != NULL){
        if (!run_highlight_text(doc, command)){
            valid = SUCCESS;
        }
    }else if (strstr(command, "remove_text") != NULL){
        if (!run_remove_text(doc, command)){
            valid = SUCCESS;
        }
    }else if (strstr(command, "save_document") != NULL){
        if (!run_save_document(doc, command)){
            valid = SUCCESS;
        }
    }else if (strstr(command, "reset_document") != NULL){
        if (!run_reset_document(doc, command)){
            valid = SUCCESS;
        }
    }
    
    if (valid == FAILURE){
        printf("Invalid Command\n");
    }
    
    return valid;
}

int main(int argc, char *argv[]){
    Document doc;
    FILE *input;
    const char *doc_name = "main_document";
    char command[MAX_STR_SIZE + 1], line1, line2[MAX_STR_SIZE + 1];
    int sscanf_return = 0;
    
    init_document(&doc, doc_name);
    
    if (argc == 1){
        input = stdin;
        printf("> ");
        fgets(command, MAX_STR_SIZE + 1, stdin);
        
        while (strcmp(command, "quit\n") && strcmp(command, "exit\n")){
            sscanf(command, " %c%s", &line1, line2);
            strtok(command, "\n");
            
            if (strcmp(command, "\n") != 0 && line1 != '#') {
               valid_command(&doc, command);
            }
            printf("> ");
            fgets(command, MAX_STR_SIZE + 1, stdin);
        }
    }else if (argc == 2){
        input = fopen(argv[1], "r");
        
        if (input == NULL){
            fprintf(stderr, " %s cannot be opened.\n", argv[1]);
            exit(EX_OSERR);
        }else {
            while (fgets(command, MAX_STR_SIZE + 1, input)) {
               if (strcmp(command, "quit\n") && strcmp(command, "exit\n")) {
                   sscanf_return = sscanf(command, " %c%s", &line1, line2);
                   strtok(command, "\n");
                   
                   if (sscanf_return > 0 && line1 != '#') {
                       valid_command(&doc, command);
                   }
               }
            }
        }
    }else {
        fprintf(stderr, "Usage: user_interface");
        fprintf(stderr, "Usage: user_interface <filename>");
        exit(EX_USAGE);
    }
    
    fclose(input);
    exit(EXIT_SUCCESS);
}
