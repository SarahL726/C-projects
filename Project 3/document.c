#include <stdio.h>
#include <string.h>
#include "document.h"

/*
 Initializes the document to be empty.
 */
int init_document(Document *doc, const char *name){
    if (doc == NULL || name == NULL || strlen(name) > MAX_STR_SIZE){
        return FAILURE;
    }
    
    strcpy(doc->name, name);
    return SUCCESS;
}

/*
 Sets the number of paragraphs to 0.
 */
int reset_document(Document *doc){
    if (doc == NULL){
        return FAILURE;
    }
    
    doc->number_of_paragraphs = 0;
    return SUCCESS;
}

/*
 Prints the document's name, number of paragraphs, followed by the paragraphs.
 */
int print_document(Document *doc){
    int i, j;
    if (doc == NULL){
        return FAILURE;
    }
    
    printf("Document name: \"%s\"\n", doc->name);
    printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);
    for (i = 0; i < doc->number_of_paragraphs; i++){
        if (doc->paragraphs[i].number_of_lines > 0){
            for (j = 0; j < doc->paragraphs[i].number_of_lines; j++){
                printf("%s\n", doc->paragraphs[i].lines[j]);
            }
            if (i != doc->number_of_paragraphs - 1){
                printf("\n");
            }
        }
    }
    
    return SUCCESS;
}

/*
 Adds a paragraph after the specified paragraph number.
 Paragraph numbers start at 1. If paragraph_number is 0, the paragraph will be added at the beginning of the document.
 */
int add_paragraph_after(Document *doc, int paragraph_number){
    int i;
    
    if (doc == NULL || doc->number_of_paragraphs == MAX_PARAGRAPHS
        || paragraph_number > doc->number_of_paragraphs){
        return FAILURE;
    }
    
    /* 1. there is no paragraph at the moment */
    if (paragraph_number == 0 && doc->number_of_paragraphs == 0){
        doc->paragraphs[0].number_of_lines = 0;
    }
    /* 2. add the paragraph at the end */
    else if (paragraph_number == doc->number_of_paragraphs){
        doc->paragraphs[paragraph_number].number_of_lines = 0;
    }
    /* 3. other circumstances */
    else{
        for (i = doc->number_of_paragraphs; i > paragraph_number; i--){
            doc->paragraphs[i] = doc->paragraphs[i - 1];
        }
        doc->paragraphs[paragraph_number].number_of_lines = 0;
    }
    
    doc->number_of_paragraphs++;
    return SUCCESS;
}

/*
 Adds a new line after the line with the specified line number.
 Line numbers start at 1. If the line number is 0, the line will be added at the beginning of the paragraph.
 */
int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line){
    int i, num_of_lines;
    
    num_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
    
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs
        || num_of_lines == MAX_PARAGRAPH_LINES
        || line_number > MAX_PARAGRAPH_LINES || new_line == NULL){
        return FAILURE;
    }
    
    /* 1. there is no line in this paragraph at the moment */
    if (line_number == 0 && num_of_lines == 0){
        strcpy(doc->paragraphs[paragraph_number - 1].lines[0], new_line);
    }
    /* 2. add the line at the end of this paragraph */
    else if (line_number == num_of_lines){
        strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number],
               new_line);
    }
    /* 3. other circumstances */
    else{
        for (i = num_of_lines; i > line_number; i--){
            strcpy(doc->paragraphs[paragraph_number - 1].lines[i],
                   doc->paragraphs[paragraph_number - 1].lines[i - 1]);
        }
        strcpy(doc->paragraphs[paragraph_number - 1].lines[line_number - 1],
               new_line);
    }
    
    doc->paragraphs[paragraph_number - 1].number_of_lines++;
    
    return SUCCESS;
}

/*
 Returns the number of lines in a paragraph using the number_of_lines out parameter.
 */
int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines){
    if (doc == NULL || number_of_lines == NULL
        || paragraph_number > doc->number_of_paragraphs){
        return FAILURE;
    }
    
    *number_of_lines = doc->paragraphs[paragraph_number].number_of_lines;
    
    return SUCCESS;
}

/*
 Appends a line to the specified paragraph.
 */
int append_line(Document *doc, int paragraph_number, const char *new_line){
    int num_of_lines;
    
    num_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs
        || num_of_lines == MAX_PARAGRAPH_LINES || new_line == NULL){
        return FAILURE;
    }
    
    add_line_after(doc, paragraph_number, num_of_lines, new_line);
    
    return SUCCESS;
}

/*
 Removes the specified line from the paragraph.
 */
int remove_line(Document *doc, int paragraph_number, int line_number){
    int i, j, num_of_lines;
    char *original_line, *covered_line;
    
    num_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
    if (doc == NULL || paragraph_number > doc->number_of_paragraphs
        || line_number > num_of_lines){
        return FAILURE;
    }

    for (i = line_number - 1; i < num_of_lines - 1; i++){
        original_line = doc->paragraphs[paragraph_number - 1].lines[i];
        covered_line = doc->paragraphs[paragraph_number - 1].lines[i + 1];
        for (j = 0; j < MAX_STR_SIZE; j++){
            original_line[j] = covered_line[j];
        }
    }
    doc->paragraphs[paragraph_number - 1].number_of_lines--;
    
    return SUCCESS;
}

/*
 Adds the first data_lines number of lines from the data array to the document.
 An empty string in the array will create a new paragraph.
 By default, the function will create the first paragraph.
 You can assume that if data_lines is different than 0, the appropriate number of lines will be present in the data array.
 */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){
    int i;
    
    if (doc == NULL || data == NULL || data_lines == 0){
        return FAILURE;
    }
    
    add_paragraph_after(doc, doc->number_of_paragraphs);
    for (i = 0; i < data_lines; i++){
        if (strcmp(data[i], "") == 0){
            add_paragraph_after(doc, doc->number_of_paragraphs);
        }else{
            append_line(doc, doc->number_of_paragraphs, data[i]);
        }
    }
    
    return SUCCESS;
}

/*
 Replaces the text "target" with the text "replacement" everywhere it appears in the document.
 You can assume the replacement will not generate a line that exceeds the maximum line length;
 also you can assume the target will not be the empty string.
 */
int replace_text(Document *doc, const char *target, const char *replacement){
    int paragraph, line, num_of_lines;
    char *current, tmp[MAX_STR_SIZE + 1] = {0};

    if (doc == NULL || target == NULL || replacement == NULL){
        return FAILURE;
    }

    for (paragraph = 0; paragraph < doc->number_of_paragraphs; paragraph++){
        num_of_lines = doc->paragraphs[paragraph].number_of_lines;

        for (line = 0; line < num_of_lines; line++){
            current = strstr(doc->paragraphs[paragraph].lines[line], target);

            while (current != '\0'){
                strcpy(tmp, current);
                strcpy(current, "");
                strcat(current, replacement);
                strcat(current + strlen(replacement), tmp + strlen(target));

                current = strstr(current + strlen(replacement), target);
            }
        }
    }

    return SUCCESS;
}

/*
 Highlights the text associated with "target" everywhere it appears in the document by surrounding the text with the strings HIGHLIGHT_START_STR and HIGHLIGHT_END_STR (see document.h).
 You can assume the highlighting will not exceed the maximum line length;
 also you can assume the target will not be the empty string.
 */
int highlight_text(Document *doc, const char *target){
    char result[MAX_STR_SIZE + 1] = "";
    
    if (doc == NULL || target == NULL){
        return FAILURE;
    }
    
    strcat(result, HIGHLIGHT_START_STR);
    strcat(result, target);
    strcat(result, HIGHLIGHT_END_STR);
    replace_text(doc, target, result);
    
    return SUCCESS;
}

/*
 Removes the text "target" everywhere it appears in the document.
 You can assume the "target" will not be the empty string.
 */
int remove_text(Document *doc, const char *target){
    if (doc == NULL || target == NULL){
        return FAILURE;
    }
    
    replace_text(doc, target, "");
    
    return SUCCESS;
}

int load_file(Document *doc, const char *filename){
    FILE *input;
    char line[MAX_STR_SIZE + 1], line1, line2[MAX_STR_SIZE + 1];
    int num, paragraph_number = 1;
    
    if (doc == NULL || filename == NULL || (input = fopen(filename, "r")) == NULL){
        return FAILURE;
    }
    
    /* default: add a paragraph */
    add_paragraph_after(doc, 0);
    
    while(fgets(line, MAX_STR_SIZE + 1, input)){
        num = sscanf(line, "%c%s", &line1, line2);
        strtok(line, "\n");
        
        if (line1 == '#'){
            if (num < 2){
                add_paragraph_after(doc, paragraph_number);
                paragraph_number++;
            }
            else{
                append_line(doc, paragraph_number, line);
            }
        }
    }
    
    return SUCCESS;
}

int save_document(Document *doc, const char *filename){
    FILE *output;
    int paragraph, line, num_of_lines;
    
    if (doc == NULL || filename == NULL || (output = fopen(filename, "w")) == NULL){
        return FAILURE;
    }
    
    for (paragraph = 0; paragraph <= doc->number_of_paragraphs; paragraph++){
        num_of_lines = doc->paragraphs[paragraph].number_of_lines;
        
        for (line = 0; line <= num_of_lines; line++){
            fputs(doc->paragraphs[paragraph].lines[line], output);
            
            if (line < num_of_lines){
                fputs("\n", output);
            }
        }
        
        if ((paragraph + 1) < doc->number_of_paragraphs){
            fputs("\n", output);
        }
    }
    fclose(output);
    
    return SUCCESS;
}
