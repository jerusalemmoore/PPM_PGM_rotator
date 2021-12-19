//Programmer: Jerusalem Moore
// Purpose: Take directory of ppm or pgm card files and rotate left or
// right based off of suit
// Note: Outputs need a corresponding directory with _rotated appended to the name
// Matrix rotation reference
// https://hplusacademy.com/matrix-rotation-in-c/
// PPM/PGM processing reference
// https://github.com/sol-prog/Perlin_Noise/blob/master/ppm.cpp
#include <time.h>
#include <dirent.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <pthread.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>



#define BILLION  1000000000L;

// PPM image array with channels
pthread_mutex_t lock;


using std::string, std::vector, std::cout,std::endl;
using namespace std;
struct arg_struct {
        vector<string> deck;
        string dirName;

};

//function rotates pgm data to the right and writes to given pgm file
void leftRotate(std::ofstream& pgmFile,int width, int height, unsigned char** data){
        // #pragma omp prallel for
        char aux;
        for(int i=width-1; i >= 0; i--)
        {
                for(int j = 0; j < height; j++) {
                        // cout << "i : " << i << " j : " << j << endl;

                        aux =  data[j][i];
                        pgmFile.write(&aux, 1);
                }
        }
        for (int i = 0; i < height; i++) {
                free(data[i]);


        }
        free(data);

}
//function rotates pgm data to the left and writes to given pgm file
void rightRotate(std::ofstream& pgmFile, int width, int height, unsigned char** data){
        // #pragma omp prallel for collapse(2) num_threads(NUM_THREADS)
        char aux;
        for(int i=0; i < width; i++)
        {
                for(int j = height-1; j >= 0; j--) {
                        // cout << "i : " << i << " j : " << j << endl;
                        aux = data[j][i];
                        pgmFile.write(&aux, 1);
                }
        }
        for (int i = 0; i < height; i++) {
                free(data[i]);


        }
        free(data);

}

void rightRotate(std::ofstream& ppmFile, int width, int height, unsigned char** r,
                 unsigned char** g, unsigned char** b){
        char aux;
        // #pragma omp prallel for collapse(2)
        // #pragma omp parallel num_threads(3)
        // {
        //         int threadNum = omp_get_thread_num();
        //         cout <<threadNum << endl;
        // #pragma omp parallel for
        for(int i=0; i < width; i++)
        {
                for(int j = height-1; j >= 0; j--) {
                        // if(threadNum == 0) {
                        aux = r[j][i];
                        ppmFile.write(&aux, 1);
                        // }
                        // else if(threadNum == 1) {
                        aux =  g[j][i];
                        ppmFile.write(&aux, 1);
                        // }
                        // else if(threadNum == 2) {
                        aux = b[j][i];
                        ppmFile.write(&aux, 1);
                        // }


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

        // }

}
//function rotates pgm data to the right and writes to given pgm file
void leftRotate(std::ofstream& ppmFile, int width, int height,unsigned char** r,
                unsigned char** g, unsigned char** b){
        char aux;
        // #pragma omp prallel for
        for(int i=width-1; i >= 0; i--)
        {
                for(int j = 0; j < height; j++) {
                        aux = r[j][i];
                        ppmFile.write(&aux, 1);
                        aux =  g[j][i];
                        ppmFile.write(&aux, 1);
                        aux =  b[j][i];
                        ppmFile.write(&aux, 1);
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

void writePPM(string targetDirName, string imageName, int width, int height,
              int max_col_val, unsigned char** r, unsigned char** g, unsigned char** b){
        string outDir = targetDirName + "_rotated/";
        // cout << "writing " << imageName << endl;

        std::ofstream inp2(outDir+imageName, std::ios::out | std::ios::binary);
        if (inp2.is_open()) {
                //NOTE WE ARE ROTATING IMAGE SO WE NEED TO SWAP WIDTH AND HEIGHT
                //FOR CORRECT ORIENTATION
                inp2 << "P6\n";
                inp2 << height;
                inp2 << " ";
                inp2 << width << "\n";
                inp2 << max_col_val << "\n";
                if(imageName.find('H') != string::npos || imageName.find('D') != string::npos) {
                        leftRotate(inp2,width, height,r,g,b);
                }
                else if(imageName.find('S') != string::npos || imageName.find('C') != string::npos) {
                        rightRotate(inp2,width,height,r,g,b);
                }
                //right rotate any image that isn't a card from deck
                else{
                        rightRotate(inp2,width,height,r,g,b);

                        // cout << "rotate error\n";
                        // pthread_exit(NULL);
                        // return;
                }


        }

        else {
                std::cout << "Error. Unable to open outfile " << outDir+imageName << std::endl;
                // pthread_exit(NULL);
                return;
        }
        inp2.close();
}
void writePGM(string targetDirName, string imageName, int width, int height,
              int max_col_val, unsigned char** data){
        string outDir = targetDirName + "_rotated/";
        // cout << "writing " << imageName << endl;
        std::ofstream inp2(outDir+imageName, std::ios::out | std::ios::binary);
        if (inp2.is_open()) {
                inp2 << "P5\n";
                inp2 << height;
                inp2 << " ";
                inp2 << width << "\n";
                inp2 << max_col_val << "\n";
                if(imageName.find('H') != string::npos || imageName.find('D') != string::npos) {
                        leftRotate(inp2,width, height,data);
                }
                else if(imageName.find('S') != string::npos || imageName.find('C') != string::npos) {
                        rightRotate(inp2,width,height,data);
                }
                //right rotate any image that isn't a card from deck
                else{
                        rightRotate(inp2,width,height,data);
                }
        }

        else {
                std::cout << "Error. Unable to open outfile " << outDir + imageName<< std::endl;
                // pthread_exit(NULL);
                return;
        }
        inp2.close();
}
void* processImage(void* arguments){

        while(1) {
                // struct timespec start, stop;
                // double accum;
                // if( clock_gettime( CLOCK_MONOTONIC, &start) == -1 ) {
                //         perror( "clock gettime" );
                //         exit( EXIT_FAILURE );
                // }
                struct arg_struct * args = (struct arg_struct *)arguments;
                pthread_mutex_lock(&lock);
                // cout << "mutex locked" << endl;
                if((args->deck).empty()) {
                        // cout << "yes" << endl;
                        pthread_mutex_unlock(&lock);
                        // pthread_exit(NULL);
                        return 0;
                }
                // pthread_mutex_unlock(&lock);
                // pthread_mutex_lock(&lock);
                string imageName = (args->deck).back();
                string targetDirName =(args->dirName);
                (args->deck).pop_back();
                pthread_mutex_unlock(&lock);
                string fullPath = targetDirName + '/' + imageName;
                // cout << fullPath << endl;
                std::ifstream inp(fullPath, std::ios::in | std::ios::binary);
                if (inp.is_open()) {
                        std::string line;
                        std::getline(inp, line);
                        // cout << line << endl;
                        if (line == "P6") {

                                unsigned int height;
                                unsigned int width;
                                unsigned int max_col_val;
                                // unsigned int nr_lines;
                                // unsigned int nr_columns;
                                // //total number of elements (pixels)
                                // unsigned int size;

                                // std::cout << "Error. Unrecognized file format." << std::endl;
                                // return;
                                std::getline(inp, line);

                                while (line[0] == '#') {
                                        // cout << line << endl;
                                        std::getline(inp, line);
                                }
                                std::stringstream dimensions(line);

                                try {
                                        dimensions >> width;
                                        dimensions >> height;
                                        // nr_lines = height;
                                        // nr_columns = width;
                                } catch (std::exception &e) {
                                        std::cout << "Header file format error. " << e.what() << std::endl;
                                        // pthread_exit(NULL);
                                        return 0;
                                }

                                std::getline(inp, line);
                                std::stringstream max_val(line);

                                try {
                                        max_val >> max_col_val;
                                } catch (std::exception &e) {
                                        std::cout << "Header file format error. " << e.what() << std::endl;
                                        // pthread_exit(NULL);
                                        return 0;
                                }
                                // cout << width << endl;
                                // cout << height << endl;
                                // cout << max_col_val<< endl;
                                // size = width*height;

                                // std::vector<vector <unsigned char> > r(height);
                                // std::vector<vector <unsigned char> > g(height);
                                // std::vector<vector <unsigned char> > b(height);
                                unsigned char** r = (unsigned char **)  malloc(height * sizeof(unsigned char*));
                                unsigned char** g = (unsigned char **) malloc(height * sizeof(unsigned char*));
                                unsigned char** b = (unsigned char **) malloc(height * sizeof(unsigned char*));

                                char aux;
                                for (unsigned int i = 0; i < height; ++i) {
                                        r[i] = (unsigned char*)malloc(width * sizeof(unsigned char));
                                        g[i] = (unsigned char*)malloc(width * sizeof(unsigned char));
                                        b[i] = (unsigned char*)malloc(width * sizeof(unsigned char));

                                        // r[i].reserve(width);
                                        // g[i].reserve(width);
                                        // b[i].reserve(width);
                                        for(unsigned int j = 0; j < width; ++j) {
                                                inp.read(&aux, 1);
                                                r[i][j] = aux;
                                                // cout << aux << endl;
                                                // r[i].push_back((unsigned char) aux);
                                                inp.read(&aux, 1);
                                                g[i][j] = aux;

                                                // cout << aux << endl;
                                                // g[i].push_back((unsigned char) aux);
                                                inp.read(&aux, 1);
                                                b[i][j] = aux;

                                                // cout << aux << endl;

                                                // b[i].push_back((unsigned char) aux);
                                        }

                                }
                                inp.close();
                                writePPM(targetDirName, imageName, width, height, max_col_val, r,g,b);
                        }
                        //PROCESS PGM
                        else{
                                // cout << "process pgm\n";
                                unsigned int height;
                                unsigned int width;
                                unsigned int max_col_val;
                                std::getline(inp, line);

                                while (line[0] == '#') {
                                        // cout << line << endl;
                                        std::getline(inp, line);
                                }
                                std::stringstream dimensions(line);

                                try {
                                        dimensions >> width;
                                        dimensions >> height;
                                        // nr_lines = height;
                                        // nr_columns = width;
                                } catch (std::exception &e) {
                                        std::cout << "Header file format error. " << e.what() << std::endl;
                                        // pthread_exit(NULL);
                                        return 0;
                                }

                                std::getline(inp, line);
                                std::stringstream max_val(line);

                                try {
                                        max_val >> max_col_val;
                                } catch (std::exception &e) {
                                        std::cout << "Header file format error. " << e.what() << std::endl;
                                        // pthread_exit(NULL);
                                        return 0;
                                }
                                unsigned char** data = (unsigned char **)  malloc(height * sizeof(unsigned char*));

                                char aux;
                                for (unsigned int i = 0; i < height; ++i) {
                                        data[i] = (unsigned char*)malloc(width * sizeof(unsigned char));


                                        // r[i].reserve(width);
                                        // g[i].reserve(width);
                                        // b[i].reserve(width);
                                        for(unsigned int j = 0; j < width; ++j) {
                                                inp.read(&aux, 1);
                                                data[i][j] = aux;
                                                // cout << aux << endl;
                                                // r[i].push_back((unsigned char) aux);
                                                // cout << aux << endl;

                                                // b[i].push_back((unsigned char) aux);
                                        }

                                }
                                inp.close();
                                //WRITE PGM
                                writePGM(targetDirName, imageName, width, height, max_col_val,data);

                        }
                        // if( clock_gettime( CLOCK_MONOTONIC, &stop) == -1 ) {
                        //         perror( "clock gettime" );
                        //         exit( EXIT_FAILURE );
                        // }
                        // accum = ( stop.tv_sec - start.tv_sec )
                        //         + (double)( stop.tv_nsec - start.tv_nsec )
                        //         / (double)BILLION;
                        // printf( "Elapsed Time for thread: %lf seconds\n", accum );


                } else {
                        std::cout << "Error. Unable to open infile " << fullPath << std::endl;
                        // pthread_exit(NULL);
                        return 0;
                }

        }


}
void runThreads(vector<string>& deck, string dirName){
        //creating pthreads
        int numThreads =13;
        pthread_t threads[numThreads];
        int rc;
        if (pthread_mutex_init(&lock, NULL) != 0)
        {
                printf("\n mutex init failed\n");
                return;
        }
        // //YOU NEED TO REWORK PROCESSIMAGE FUNCTION
        // //PASS THE SUIT OF NAMES TO EACH THREAD
        // // LEAKING HERE !!!
        // for(int i =0; i< 4; i++){
        // struct arg_struct* args = malloc(sizeof(arg_struct));
        struct arg_struct args;
        args.deck = deck;
        args.dirName = dirName;
        // cout << deck.size() << endl;
        for(int i = 0; i < numThreads; i++) {
                rc = pthread_create(&threads[i], NULL, processImage, &args);
                if(rc < 0) {
                        cout << "thread creation error\n";
                }

        }
        for(int i = 0; i < numThreads; i++) {
                pthread_join(threads[i], NULL);
        }
        // args = NULL;
        // free(args);
        pthread_mutex_destroy(&lock);


}

int main(int argc, char *argv[])
{
        int testRuns = 10;
        float runTimes[testRuns];
        for(int i = 0; i < testRuns; i++) {
                struct timespec start, stop;
                double accum;

                struct dirent *de;
                DIR *dr;
                //map to hold suits in deck
                vector<string> deck;
                //load map with vector for cards
                // vector<string> cards;
                // deck["H"] = cards;
                // deck["C"] = cards;
                // deck["S"] = cards;
                // deck["D"] = cards;
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
                        // cout << argv[1] << endl;
                        // cout << dr << endl;
                        // closedir(dr);
                        // printf("Directory %s opened successfully\n\n", argv[1]);

                        if( clock_gettime( CLOCK_MONOTONIC, &start) == -1 ) {
                                perror( "clock gettime" );
                                exit( EXIT_FAILURE );
                        }
                        //iterate files in directory and store in array
                        // cout << fstart << endl;
                        // int i = 0;
                        while ((de = readdir(dr)) != NULL) {
                                string entry;
                                entry = de->d_name;
                                // free(de);
                                if(entry == "." || entry == "..") {
                                }
                                else{
                                        deck.push_back(entry);
                                        // cout << "added " << de->d_name << " to deck\n";
                                }
                        }
                        runThreads(deck,argv[1]);

                }
                if( clock_gettime( CLOCK_MONOTONIC, &stop) == -1 ) {
                        perror( "clock gettime" );
                        exit( EXIT_FAILURE );
                }
                accum = ( stop.tv_sec - start.tv_sec )
                        + (double)( stop.tv_nsec - start.tv_nsec )
                        / (double)BILLION;
                printf( "Elapsed Time: %lf seconds\n", accum );
                // free(de);
                closedir(dr);
                runTimes[i] = (accum);
        }
        double averageTime = 0;
        for(int i =0; i < testRuns; i++) {
                // printf("runtimes at %i %lf\n", i, runTimes[i]);
                averageTime += runTimes[i];
        }
        averageTime = averageTime/(double)testRuns;
        cout << "average seconds elapsed after " << testRuns << " runs: " << averageTime << endl;

}


// print contents of map
// for(auto &it : deck){
//   std::cout << "suit " << it.first << std::endl;
//   for(int i = 0
