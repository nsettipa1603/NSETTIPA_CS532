/* 
Name: NEELIMA SETTIPALLI
BlazerId: nsettipa
Project #: Homework_2
To compile: gcc -o nsettipa_HW2 nsettipa_HW2.c
To run: ./nsettipa_HW2 <commands> <path of file>
Commands:- 
-S :- List all files in the directory along with the required attributes
>> ./nsettipa_HW2 -S ./
-s :- List all files with size <= 1024 bytes in the current directory.
>> ./nsettipa_HW2 -s <file size in bytes> <file path>
-f :- List all files in the current directory that includes the given extension.
>> ./nsettipa_HW2 -f <file extension> <depth of file>
*/ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <dirent.h>

char *sub_str= NULL;
char *PATH = ".";
int len_max = -1, size_max = -1, attribs = 0;


char *f_name(char *);

void prt_name(char *path)
{
    char buf[1024];
    int len = readlink(path, buf, sizeof(buf) - 1);
    if (len != -1)
    {
        buf[len] = '\0';
        char *name = f_name(buf);
        if (name == NULL)
            name = path;
        else
            name++;
        printf("(%s)\t", name);
    }
}

char *f_name(char *path)
{
    for (int i = 0; i < strlen(path); i++)
    {
        if (path[i] == '/')
        {
            return path + i + 1;
        }
    }
    return path;
}

void file_attribs(struct stat statbuf)
{
    if (S_ISDIR(statbuf.st_mode))
        printf("\t0\t");
    else
        printf("\t%ld\t", statbuf.st_size);
    
    printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
    printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
    printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
    printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
    printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
    printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
    printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
    printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
    printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
    printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");
    printf("\t%s", ctime(&statbuf.st_atime));
}

int prt_details(char *path, struct dirent *dir_entry, int len_curr)
{
    for (int i = 0; i < len_curr; i++)
    {
        printf("\t");
    }
    struct stat fileStat;
    lstat(path, &fileStat);

    printf("%s\t", dir_entry->d_name);
    if (S_ISDIR(fileStat.st_mode) && attribs == 1)
    {
        printf("\t0\t");
    }
    else if (S_ISLNK(fileStat.st_mode))
    {
        prt_name(path);
    }
    if (attribs)
    {
        file_attribs(fileStat);
    }
    printf("\n");
    return 0;
}

char *link_path(char *s1, char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 2);
    strcpy(result, s1);
    strcat(result, "/");
    strcat(result, s2);
    return result;
}

int cont(char* str, char* substr)
{
    if (substr == NULL)
        return 1;
    if (strstr(str, substr) != NULL)
        return 1;
    return 0;
}

int find(char *path, int len_curr)
{
    DIR *dir;
    struct dirent *dirEntry;
    struct stat fileStat;

    dir = opendir(path);
    while (dirEntry = readdir(dir))
    {
        if (!((dirEntry->d_name[0] == '.' && dirEntry->d_name[1] == '\0') || (dirEntry->d_name[0] == '.' && dirEntry->d_name[1] == '.' && dirEntry->d_name[2] == '\0')))
            lstat(dirEntry->d_name, &fileStat);
        else continue;

        char *newPath = link_path(path, dirEntry->d_name);
        if (S_ISDIR(fileStat.st_mode))
        {
            if (!(len_max == -1 || len_curr <= len_max))
            {
                continue;
            }
            prt_details(newPath, dirEntry, len_curr);
            find(newPath, len_curr + 1);
        }
        else if ((size_max == -1 || fileStat.st_size <= size_max) && cont(dirEntry->d_name, sub_str))
        {
            {
                prt_details(newPath, dirEntry, len_curr);
            }
        }
    }
}

void p_args(int argc, char *argv[])
{
    int i;
    int path_curr = 0;
    for (i = 1; i < argc; i++)
    {
        switch (argv[i][0])
        {
        case '-':
            switch (argv[i][1])
            {
            case 's':
                size_max = atoi(argv[i + 1]);
                i++;
                break;
            case 'f':
                sub_str = argv[i + 1];
                len_max = atoi(argv[i + 2]);
                i += 2;
                break;
            case 'S':
                attribs = 1;
                break;
            }
            break;
        default:
            PATH = !path_curr ? argv[i] : PATH;
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    p_args(argc, argv);
    find(PATH, 0);
}