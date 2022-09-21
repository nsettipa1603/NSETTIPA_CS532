#include <stdio.h>
#include <stdlib.h>
int main(int argc,char *argv[])
{
    char ch;
	FILE *fp1;
    FILE *fp2;
    if(argc!=3)
    {
        printf("invalid ");
        exit(1);
    }
	fp1 = fopen(argv[1], "a+");
	fp2 = fopen(argv[2], "r");
	if (fp1==NULL || fp2==NULL) 
    {
		printf("Error! File name is invalid/"
			"File names are same\n");
        exit(1);
	}
	
     while(1)
     {
         ch=fgetc(fp2);
         if(ch==EOF)
            break;
        fputc(ch,fp1);
     }
     printf("\n successful ");
     fclose(fp1);
     fclose(fp2);
     //getch();
}

// Steps for compilation:
// cat file1.txt
// cat file2.txt
// 'gcc -Wall nsettipa_lab3 -o nsettipa_lab3.c'


// Steps for Execution:
//./nsettipa_lab3 file1.txt file2.txt


