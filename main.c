#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "testScanner.h"
FILE* inputFile;
int main(int argc, char* argv[]) {

if (argc > 2) {
                //If more than 2 arguments provided, display an error message
                printf("FATAL: Too many arguments. Must be 2 or 1\n Usage: %s [filename]\n", argv[0]);
                return 1;
        } else if (argc == 2) {
                //If 2 arguments provided, open the specified file and check for special characters
              inputFile = fopen(argv[1], "r");

                if (inputFile == NULL) {
                        perror("Error opening file");
                        return 1;
                }
                testScanner();
                fclose(inputFile);
        } else { //Get keyboard input if arg less than 2
            inputFile = fopen("output.txt","w"); //Use same global variable so scanner won't error
            if (inputFile == NULL) {
                    perror("Error opening output.txt");
                    return 1;
            }

            char ch;

            printf("Enter text (Ctrl+D to end):\n");

            while ((ch = getchar()) != EOF) {

                    fputc(ch, inputFile); //Write the character to the output file

            }
            fclose(inputFile);

            //re-open inputFile in read mode for testScanner
            inputFile = fopen("output.txt", "r");
            if (inputFile == NULL) {
                    perror("Error re-opening output.txt for reading");
                    return 1;
            }
            testScanner();
            fclose(inputFile);
            //delete output.txt
            system("rm output.txt");

        }
return EXIT_SUCCESS;
}
