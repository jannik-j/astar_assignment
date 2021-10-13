#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
    size_t buffer_size = 32;
    size_t num_chars;
    char *buffer, *field;

    if ((buffer = (char*) malloc(buffer_size*sizeof(char))) == NULL){
        printf("Unable to allocate memory");
        exit(1);
    }

    FILE *fp = fopen("data/test_reading.txt", "r");
    if (!fp){
        printf("Unable to open file.");
        exit(1);
    }

    int i = 1;
    while(!feof(fp)){
        printf("===== Line %d =====\n", i);
        num_chars = getline(&buffer, &buffer_size, fp);
        for (int j = 1; ; j++){
            field = strsep(&buffer, "|");
            if (field == NULL)
                break;
            printf("Field %d: '%s | '\n", j, field);
        }
        i++;
    }
}