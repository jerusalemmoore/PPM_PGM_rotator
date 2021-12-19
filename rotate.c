//Programmer: Jerusalem Moore
// Purpose: Take directory of ppm or pgm card files and rotate left or
// right based off of suit
// Note: Outputs need a corresponding directory with _rotated appended to the name
// Matrix rotation reference
// https://hplusacademy.com/matrix-rotation-in-c/
// PPM/PGM processing reference
// https://github.com/sol-prog/Perlin_Noise/blob/master/ppm.cpp
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#include <string.h>
#include <dirent.h>
#include <omp.h>
#include <math.h>
// #define IMG_HEIGHT (300)
// #define IMG_WIDTH (400)
//modified to do 1280x960
#define IMG_HEIGHT (920)
// #define FAST_IO
#define NUM_THREADS (4)
#define NUM_CARDS (52)
// #define min(x,y) x < y ? x : y
// #define max(x,y) x > y ? x : ykll;
typedef double FLOAT;

typedef unsigned int UINT32;
typedef unsigned long long int UINT64;
typedef unsigned char UINT8;

// PPM Edge Enhancement Code
//
UINT8 header[38];

// PPM image array with channels
UINT8 RGB[IMG_HEIGHT][IMG_HEIGHT];
//function rotates pgm data to the right and writes to given pgm file
//function rotates pgm data to the right and writes to given pgm file
// https://hplusacademy.com/matrix-rotation-in-c/
void leftRotatePgm(FILE* pgmFile,int width, int height, unsigned char** data){
        // #pragma omp prallel for collapse(2) num_threads(NUM_THREADS)
        // printf("Width: %i height: %i\n", width,height);

        for(int i=width -1 ; i>=0; i--)
        {
                for(int j = 0; j < height; j++) {
                        // printf("i: %i\n", i);
                        // printf("j: %i\n", j);
                        // printf("data[j][i] %i\n", (data[j][i]));

                        fputc(data[j][i],pgmFile);
                }
        }
        for (int i = 0; i < height; i++) {
                free(data[i]);
        }
        free(data);
}
//function rotates pgm data to the left and writes to given pgm file
void rightRotatePgm (FILE* pgmFile, int width,  int height, unsigned char** data){
        // printf("Width: %i height: %i\n", width,height);
        // #pragma omp prallel for collapse(2) num_threads(NUM_THREADS)
        for(int i=0; i < width; i++)
        {
                for(int j = height-1; j >=0; j--) {
                        // printf("i: %i j: %i data: %\n", i,j, data);

                        fputc(data[j][i],pgmFile);
                }
        }
        for (int i = 0; i < height; i++) {
                free(data[i]);
        }
        free(data);

}
void leftRotatePPM(FILE* ppmFile,int width, int height, unsigned char** r,
                   unsigned char** g,unsigned char** b){
        // #pragma omp prallel for collapse(2) num_threads(NUM_THREADS)
        // printf("Width: %i height: %i\n", width,height);

        for(int i=width -1 ; i>=0; i--)
        {
                for(int j = 0; j < height; j++) {
                        // printf("i: %i\n", i);
                        // printf("j: %i\n", j);
                        // printf("data[j][i] %i\n", (data[j][i]));
                        fputc(r[j][i],ppmFile);
                        fputc(g[j][i],ppmFile);
                        fputc(b[j][i],ppmFile);
                }
        }
        for (int i = 0; i < height; i++) {
                free(r[i]);
                free(g[i]);
                free(b[i]);
        }
        free(r);
        free(g);
        free(b);
}
//function rotates pgm data to the left and writes to given pgm file
void rightRotatePPM (FILE* ppmFile,int width, int height, unsigned char** r,
                     unsigned char** g,unsigned char** b){
        // printf("Width: %i height: %i\n", width,height);
        // #pragma omp prallel for collapse(2) num_threads(NUM_THREADS)
        for(int i=0; i < width; i++)
        {
                for(int j = height-1; j >=0; j--) {
                        // printf("i: %i j: %i data: %\n", i,j, data);
                        fputc(r[j][i],ppmFile);
                        fputc(g[j][i],ppmFile);
                        fputc(b[j][i],ppmFile);
                }
        }
        for (int i = 0; i < height; i++) {
                free(r[i]);
                free(g[i]);
                free(b[i]);
        }
        free(r);
        free(g);
        free(b);

}

void writePgm(char imageName[], int width, int height, char* suit,
              unsigned char** data){
        FILE *pgmFile;
        char folderOutName[255];
        strcpy(folderOutName, "cards_3x4_pgm_rotated/");
        char* fullOutPath = strcat(folderOutName,imageName);
        // printf("fopen called on path: %s\n", fullOutPath);
        //open file to place rotated image
        pgmFile = fopen(fullOutPath, "wb");
        if(pgmFile == NULL) {
                perror("cannot open file to write");
                exit(EXIT_FAILURE);
        }
        //print header to file
        fprintf(pgmFile, "P5\n");
        fprintf(pgmFile, "# Created by IrfanView\n");
        // printf("writing height : %s width : %s\n" );
        fprintf(pgmFile, "%d %d\n", height, width);
        fprintf(pgmFile, "255\n");
        //depending on the suit call left or right rotate
        if(suit[0] == 'D'|| suit[0] == 'H' ) {
                // printf("suit is %c, D or H calls left rotate function\n/////////////////////////////////////////////////\n", suit[0]);

                leftRotatePgm(pgmFile, width, height,data);
                // rotate(pgmFile, intWidth, intHeight, 45);
        }
        else if(suit[0] == 'S' || suit[0] == 'C') {
                // printf("suit is %c, S or C calls right rotate function\n/////////////////////////////////////////////////\n", suit[0]);
                rightRotatePgm(pgmFile, width, height,data);
                // rotate(pgmFile, intWidth, intHeight, 45);

        }
        else{
                printf("No suit identified\n");
        }
        //close newly written file
        fclose(pgmFile);

}
void writePPM(char imageName[], int width, int height, char* suit,
              unsigned char** r,unsigned char** g,unsigned char** b){
        FILE *ppmFile;
        char folderOutName[255];
        strcpy(folderOutName, "cards_3x4_ppm_rotated/");
        char* fullOutPath = strcat(folderOutName,imageName);
        // printf("fopen called on path: %s\n", fullOutPath);
        //open file to place rotated image
        ppmFile = fopen(fullOutPath, "wb");
        if(ppmFile == NULL) {
                perror("cannot open file to write");
                exit(EXIT_FAILURE);
        }
        //print header to file
        fprintf(ppmFile, "P6\n");
        fprintf(ppmFile, "# Created by IrfanView\n");
        // printf("writing height : %s width : %s\n" );
        fprintf(ppmFile, "%d %d\n", height, width);
        fprintf(ppmFile, "255\n");
        //depending on the suit call left or right rotate
        if(suit[0] == 'D'|| suit[0] == 'H' ) {
                // printf("suit is %c, D or H calls left rotate function\n/////////////////////////////////////////////////\n", suit[0]);

                leftRotatePPM(ppmFile, width, height,r,g,b);
                // rotate(pgmFile, intWidth, intHeight, 45);
        }
        else if(suit[0] == 'S' || suit[0] == 'C') {
                // printf("suit is %c, S or C calls right rotate function\n/////////////////////////////////////////////////\n", suit[0]);
                rightRotatePPM(ppmFile, width, height,r,g,b);
                // rotate(pgmFile, intWidth, intHeight, 45);

        }
        else{
                printf("No suit identified\n");
        }
        //close newly written file
        fclose(ppmFile);

}
void processImage(char imageName[], char** argv){
        int fdin, bytesRead=0, bytesLeft,rc;

        char* suit;
        char suits[] = "CDHS";
        suit = strpbrk(imageName,suits);
        if(suit == NULL) {
                printf("Error, couldn't find suit\n");
                return;
        }
        char folderName[255];
        strcpy(folderName, argv[1]);
        strcat(folderName, "/");
        char* fullPath = strcat(folderName,imageName);
        if((fdin = open(fullPath, O_RDONLY, 0644)) < 0)
        {
                printf("Error opening %s\n", fullPath);
                return;
        }
        // printf("File opened successfully\n");
        //process file to read
        bytesLeft=38;
        // #pragma omp critical
        {
                do
                {
                        //printf("bytesRead=%d, bytesLeft=%d\n", bytesRead, bytesLeft);
                        bytesRead=read(fdin, (void *)header, bytesLeft);
                        bytesLeft -= bytesRead;
                } while(bytesLeft > 0);
        }
        // header[38]='\0';
        //parse header
        // printf("Header:\n%s\n", header);
        char magicNum[3];
        char width[4];
        char height[4];
        char maxVal[4];
        memcpy(magicNum, &header,2);
        memcpy(width, &header[26], 3);
        memcpy(height, &header[30],3);
        memcpy(maxVal, &header[34],3);
        magicNum[2] = '\0';
        width[3] = '\0';
        height[3] = '\0';
        maxVal[3] = '\0';
        // printf("imageName: %s\n", imageName);
        // printf("magicNum: %s\n", magicNum);
        // printf("width: %s\n", width);
        // printf("height: %s\n", height);
        // printf("maxVal: %s\n", maxVal);
        int intHeight = atoi(height);
        int intWidth = atoi(width);
        // printf("header: %s", header);
        // #pragma omp prallel for collapse(2) num_threads(NUM_THREADS)
        if(strcmp(magicNum, "P5") == 0) {
                unsigned char** data = (unsigned char **)  malloc(intHeight * sizeof(unsigned char*));
                for(int i=0; i<intHeight; i++)
                {
                        data[i] = (unsigned char*) malloc (intWidth * sizeof(unsigned char));
                        for(int j = 0; j < intWidth; j++) {
                                rc = read(fdin, &data[i][j],1);

                                if(rc < 0) {
                                        printf("Read error\n");
                                }
                                // printf("i: %i j: %i data: %i\n", i,j, data[i][j]);

                        }
                }
                // //close current file being read
                writePgm(imageName, intWidth, intHeight, suit, data);
        }
        //WE ASSUME ANY OTHER CASE WE GET A PPM
        else{
                unsigned char** r = (unsigned char **)  malloc(intHeight * sizeof(unsigned char*));
                unsigned char** g = (unsigned char **)  malloc(intHeight * sizeof(unsigned char*));
                unsigned char** b = (unsigned char **)  malloc(intHeight * sizeof(unsigned char*));

                for(int i=0; i<intHeight; i++)
                {
                        r[i] = (unsigned char*) malloc (intWidth * sizeof(unsigned char));
                        g[i] = (unsigned char*) malloc (intWidth * sizeof(unsigned char));
                        b[i] = (unsigned char*) malloc (intWidth * sizeof(unsigned char));
                        for(int j = 0; j < intWidth; j++) {
                                rc = read(fdin, &r[i][j],1);
                                rc = read(fdin, &g[i][j],1);
                                rc = read(fdin, &b[i][j],1);

                                if(rc < 0) {
                                        printf("Read error\n");
                                }
                                // printf("i: %i j: %i data: %i\n", i,j, data[i][j]);

                        }
                }
                writePPM(imageName, intWidth, intHeight, suit, r,g,b);
                // //close current
                // printf("ppm\n");
        }
        close(fdin);




}


int main(int argc, char *argv[])
{
        int testRuns = 10;

        FLOAT runTimes[testRuns];

        FLOAT fstart, fnow;
        struct timespec start, now;
        struct dirent *de;
        DIR *dr;
        char names[52][10];
        // for(int i =0; i < testRuns; i++) {
        //         runTimes[i] = 2;
        //         printf("runtimes at %i %lf\n", i, runTimes[i]);
        //
        // }
        //
        // double averageTime= 0;
        // for(int i =0; i < testRuns; i++) {
        //         printf("runtimes at %i %lf\n", i, runTimes[i]);
        //         averageTime += runTimes[i];
        // }
        // averageTime = averageTime/(double)testRuns;
        // printf("average sadkoiphvafdsjh;oiadfsg;jilsfdgbaok;sfgdhb%lf\n", averageTime);
        for(int i = 0; i < testRuns; i++) {

                if(argc < 2)
                {
                        printf("Usage: sharpen directory");
                        exit(-1);
                }
                else
                {
                        //open directory given in cmdline arg
                        if((dr = opendir(argv[1])) == NULL)
                        {
                                printf("Error opening %s\n", argv[1]);
                                return 0;
                        }
                        // printf("Directory %s opened successfully\n\n", argv[1]);

                        clock_gettime(CLOCK_MONOTONIC, &start);
                        fstart = (FLOAT)start.tv_sec  + (FLOAT)start.tv_nsec / 1000000000.0;
                        //iterate files in directory and store in array
                        int i = 0;
                        while ((de = readdir(dr)) != NULL) {
                                if(strcmp(de->d_name, ".") == 0|| strcmp(de->d_name, "..") == 0) {
                                        continue;
                                }
                                strcpy(names[i], de->d_name);
                                // processImage(de->d_name, argv);
                                // printf("string at %i, %s\n", i, names[i]);
                                i++;
                        }
                        //process images by name
                        for(int i = 0; i < NUM_CARDS; i++) {
                                processImage(names[i], argv);
                        }

                        clock_gettime(CLOCK_MONOTONIC, &now);
                        fnow = (double)now.tv_sec  + (double)now.tv_nsec / 1000000000.0;
                        printf("@ %lf sec elapsed\n", (fnow-fstart));

                        closedir(dr);

                        // return 1;
                }
                runTimes[i] = (fnow-fstart);


        }
        double averageTime= 0;
        for(int i =0; i < testRuns; i++) {
                // printf("runtimes at %i %lf\n", i, runTimes[i]);
                averageTime += runTimes[i];
        }
        averageTime = averageTime/(double)testRuns;
        printf("average seconds elapsed after %i runs: %lf\n", testRuns,averageTime);


}
