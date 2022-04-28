#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(){
    FILE *ptr;
	ptr=fopen("NTFS.img", "rb");
    if(ptr!=NULL){
        printf("Opened successfully\n");
    }
    unsigned char *data;
    fseek(ptr, 0, SEEK_END);
    int len = ftell(ptr);
    rewind(ptr); 
    data = (unsigned char*) malloc(len);
    fread(data, len, 1, ptr);
    fclose(ptr);

    u_int16_t *spc = (u_int16_t*) malloc(sizeof(u_int16_t));
    u_int16_t *bps = (u_int16_t*) malloc(sizeof(u_int16_t));

    memcpy(bps, data+11, 2);
    memcpy(spc, data+13, 2);
    printf("Bytes per sector : %d\n", *bps);
    printf("Sectors per cluster : %d\n", *spc);
}