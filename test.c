#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main(int argc, char** argv){
    if(argc==1){
        printf("File not specified\n");
    }
    else{
        FILE *ptr;
        printf("Filename entered : %s\n", argv[1]);
        ptr=fopen(argv[1], "rb");
        if(ptr!=NULL){
            printf("Opened successfully\n");
            unsigned char *data;
            fseek(ptr, 0, SEEK_END);
            int len = ftell(ptr);
            rewind(ptr); 
            data = (unsigned char*) malloc(len);
            fread(data, len, 1, ptr);
            fclose(ptr);

            u_int16_t *spc = (u_int16_t*) malloc(sizeof(u_int16_t));
            u_int16_t *bps = (u_int16_t*) malloc(sizeof(u_int16_t));
            u_int16_t *mft_start = (u_int16_t*) malloc(sizeof(u_int16_t));

            memcpy(bps, data+11, 2);
            memcpy(spc, data+13, 2);
            memcpy(mft_start, data+48, 8);

            int mft_addr = *mft_start*(*spc)*(*bps);
            int i30_addr = mft_addr + (*bps * 2);

            printf("Bytes per sector : %d\n", *bps);
            printf("Sectors per cluster : %d\n", *spc);
            printf("Starting cluster of MFT : %d\n\n", *mft_start);

            int completed = 0;

            printf("Files in root dir : \n\n");
            int temp = mft_addr;
            while(temp < mft_addr+75000) {
                int s = temp+56;
                while(s < (temp+1024)) {
                    int size = data[s+4];
                    if(size == 0) break;
                    s += size;
                    if(data[s]==0x30 && data[s+1]==0x00 && data[s+2]==0x00 && data[s+3]==0x00){
                        int size = data[s+4];
                        char *filename = (char*) malloc(sizeof(char)*(size-88));
                        memcpy(filename, (char*)data+s+88, size-88);

                        int ind = 0;
                        while(ind < size) {
                            if(filename[ind] != 0) {
                                printf("%c", filename[ind]);
                            }
                            ind++;
                        }
                        printf("\n");
                    }
                }
                temp = temp + 1024;
                completed++;
            }
        }
        else{
            printf("File not found\n");
        }
        
    }

}
