/*
Name: NEELIMA SETTIPALLI
BlazerId: nsettipa
Project #: Homework_03
To compile: gcc -o nsettipa_HW3 nsettipa_HW3.c
To run: ./nsettipa_HW3 <options as stated below> <path>
*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int len_max = -1, size_max = -1, attribs = 0;
char *sub_str = NULL;
char *PATH = ".";
char type_File = 'a';
char *Cmd_Ind = NULL;
char *Cmd_Com = NULL;
char **Total_Files = NULL;
int File_Count = 0;

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

void file_attribs(struct stat file_structure)
{
    if (S_ISDIR(file_structure.st_mode))
        printf("\t0\t");
    else
        printf("\t%ld\t", file_structure.st_size);
    
    printf((S_ISDIR(file_structure.st_mode)) ? "d" : "-");
    printf((file_structure.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_structure.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_structure.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_structure.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_structure.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_structure.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_structure.st_mode & S_IROTH) ? "r" : "-");
    printf((file_structure.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_structure.st_mode & S_IXOTH) ? "x" : "-");
    printf("\t%s", ctime(&file_structure.st_atime));
}

int prt_details(char *path, struct dirent *directory_info, int len_curr)
{
    if ((directory_info->d_type == DT_DIR && type_File == 'f') || (directory_info->d_type == DT_REG && type_File == 'd'))
    {
        return 0;
    }
    char *path_new = (char *)malloc(sizeof(char) * (strlen(path) + strlen(directory_info->d_name) + 2));
    strcpy(path_new, path);
    strcat(path_new, "/");
    strcat(path_new, directory_info->d_name);
    Total_Files[File_Count] = (char *)malloc(sizeof(char) * (strlen(directory_info->d_name) + 1));
    strcpy(Total_Files[File_Count], directory_info->d_name);
    File_Count++;

    for (int i = 0; i < len_curr; i++)
    {
        printf("\t");
    }
    struct stat fileStat;
    lstat(path, &fileStat);

    printf("%s\t", directory_info->d_name);
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

void com_exe()
{
    if (Cmd_Ind != NULL)
    {
        for (int i = 0; i < File_Count; i++)
        {
            char *cmd = (char *)malloc(sizeof(char) * (strlen(Cmd_Ind) + strlen(Total_Files[i]) + 2));
            strcpy(cmd, Cmd_Ind);
            strcat(cmd, " ");
            strcat(cmd, Total_Files[i]);
            system(cmd);
        }
    }
    if (Cmd_Com != NULL)
    {
        char *cmd = (char *)malloc(sizeof(char) * (strlen(Cmd_Com) + 1));
        strcpy(cmd, Cmd_Com);
        for (int i = 0; i < File_Count; i++)
        {
            strcat(cmd, " ");
            strcat(cmd, Total_Files[i]);
        }
        system(cmd);
    }
}

int find(char *path, int len_curr)
{
    DIR *dir_entry;
    struct dirent *directory_info;
    struct stat fileStat;

    dir_entry = opendir(path);
    while (directory_info = readdir(dir_entry))
    {
        if (!((directory_info->d_name[0] == '.' && directory_info->d_name[1] == '\0') || (directory_info->d_name[0] == '.' && directory_info->d_name[1] == '.' && directory_info->d_name[2] == '\0')))
            lstat(directory_info->d_name, &fileStat);
        else continue;

        char *path_new = link_path(path, directory_info->d_name);
        if (directory_info->d_type == DT_DIR)
        {
            if (!(len_max == -1 || len_curr <= len_max))
            {
                continue;
            }
            prt_details(path_new, directory_info, len_curr);
            find(path_new, len_curr + 1);
        }
        else if ((size_max == -1 || fileStat.st_size <= size_max) && cont(directory_info->d_name, sub_str))
        {
            {
                prt_details(path_new, directory_info, len_curr);
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
            case 't':
                type_File = argv[i + 1][0];
                i++;
                break;
            case 'e':
                Cmd_Ind = argv[i + 1];
                i++;
                break;
            case 'E':
                Cmd_Com = argv[i + 1];
                i++;
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
    Total_Files = (char **)realloc(Total_Files, sizeof(char *) * 512);
    p_args(argc, argv);
    find(PATH, 0);
    com_exe();
}
