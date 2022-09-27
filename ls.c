#include "ls.h"
#include "headers.h"

extern char *lsflag[50]; // stores the arguments after ls
char *lscontents[1000];  // stores the directory / file name after ls
char *file[1000];
char *exec[1000];
char curr_dir[1000];
extern char home_path[1000];
char curr[1000];
char names[1000][1000];
char direc[1000];
char host[1000];
char *fll[1000]; // to store names just to sort them

int comparator(const void *a, const void *b) // comparator function for qsort
{
    const char **str_a = (const char **)a;
    const char **str_b = (const char **)b;
    return strcmp(*str_a, *str_b);
}
int isfile(char *name) // return 1 if file is a regular file
{
    struct stat path;
    stat(name, &path);
    return S_ISREG(path.st_mode);
}
int isdirec(char *name) // return 1 if file is a directory
{                       // printf("%s\n",name);
    struct stat path;
    stat(name, &path);
    return S_ISDIR(path.st_mode);
}
int isout(char *name) // return 1 is file is an executable
{
    int len = strlen(name);
    if (name[len - 3] == 'o' && name[len - 2] == 'u' && name[len - 1] == 't' && name[len - 4] == '.')
        return 1;
    return 0;
}
int is_sh(char *name)
{
    int len = strlen(name);
    if (name[len - 3] == '.' && name[len - 2] == 's' && name[len - 1] == 'h')
        return 1;
    return 0;
}
void lsboth(int fl, int fa, char *dir_path)
{
    int total1 = 0, total2 = 0; // total1 - for all files, total2 - for
    gethostname(host, 1000);
    struct dirent *f;
    DIR *dr = opendir(dir_path);
    if (dr == NULL)
    {
        perror("Could not open directory");
        return;
    }
    int x = 0;
    char filename[1000];
    while ((f = readdir(dr)) != NULL)
    {
        struct stat path;
        fll[x] = f->d_name;
        stat(f->d_name, &path);
        total1 += path.st_blocks;
        if (f->d_name[0] != '.')
            total2 += path.st_blocks;
        x++;
    }
    if (fa == 1 && fl == 1)
        printf("total %d\n", total1 / 2);
    else if (fa != 1 && fl == 1)
        printf("total %d\n", total2 / 2);
    qsort(fll, x, sizeof(char *), comparator); // sort the contents
    for (int i = 0; i < x; i++)
    {
        strcpy(filename, fll[i]);      // copy each content into filename and display the info
        if (!fa && filename[0] == '.') // fa flag is 0, and file is hidden - don't print it
            continue;
        if (fl)
        {
            static char file_path[1000];
            strcpy(file_path, dir_path);
            strcat(file_path, "/");
            strcat(file_path, filename);
            struct stat file;
            if (stat(file_path, &file) == -1)
            {
                char err_buf[1100];
                sprintf(err_buf, "Error reading %s", filename);
                perror(err_buf);
                continue;
            }
            if (S_ISDIR(file.st_mode)) // checking all permissions for user, group, others
                printf("d");
            else
                printf("-");
            if (file.st_mode & S_IRUSR)
                printf("r");
            else
                printf("-");
            if (file.st_mode & S_IWUSR)
                printf("w");
            else
                printf("-");
            if (file.st_mode & S_IXUSR)
                printf("x");
            else
                printf("-");
            if (file.st_mode & S_IRGRP)
                printf("r");
            else
                printf("-");
            if (file.st_mode & S_IWGRP)
                printf("w");
            else
                printf("-");
            if (file.st_mode & S_IXGRP)
                printf("x");
            else
                printf("-");
            if (file.st_mode & S_IROTH)
                printf("r");
            else
                printf("-");
            if (file.st_mode & S_IWOTH)
                printf("w");
            else
                printf("-");
            if (file.st_mode & S_IXOTH)
                printf("x");
            else
                printf("-");
            printf("  %3d", (int)file.st_nlink); // number of links of file
            struct passwd *pw;
            pw = getpwuid(file.st_uid);
            printf("  %5s", pw->pw_name); // user name
            printf(" %5s", host);         // host name
            printf("  %5d", (int)file.st_size);
            struct tm *times = localtime(&file.st_mtim.tv_sec); // struct times stores info about time of creation
            if (times->tm_mon == 0)
                printf(" Jan");
            else if (times->tm_mon == 1)
                printf(" Feb");
            else if (times->tm_mon == 2)
                printf(" Mar");
            else if (times->tm_mon == 3)
                printf(" Apr");
            else if (times->tm_mon == 4)
                printf(" May");
            else if (times->tm_mon == 5)
                printf(" Jun");
            else if (times->tm_mon == 6)
                printf(" Jul");
            else if (times->tm_mon == 7)
                printf(" Aug");
            else if (times->tm_mon == 8)
                printf(" Sep");
            else if (times->tm_mon == 9)
                printf(" Oct");
            else if (times->tm_mon == 10)
                printf(" Nov");
            else if (times->tm_mon == 11)
                printf(" Dec");
            else
                printf("     ");
            printf(" %2d", times->tm_mday);       // date
            printf(" %d", 1900 + times->tm_year); // year
            printf(" %0*d:%0*d", 2, times->tm_hour, 2, times->tm_min);
            // struct stat path;
            // stat(filename, &path);
            int len = strlen(filename);
            if (isdirec(filename) == 1)
                printf(Blue);
            else if (len > 4 && isout(filename) == 1)
                printf(Green);
            else if (len > 3 && is_sh(filename) == 1)
                printf(Green);
            else if (isfile(filename) == 1)
                printf(White);
            printf("  %s\n", filename);
            printf(White);
        }
        else
            printf("%s\n", filename);
    }
    closedir(dr);
}

void call()
{
    char *dir = ".";
    int i = 0;
    struct dirent *ent;
    DIR *dh = opendir(dir);
    while ((ent = readdir(dh)) != NULL)
    {
        if (ent->d_name[0] == '.') // if file is a hidden file, don't store in array
            continue;
        else
        {
            lscontents[i] = ent->d_name;
            i++;
        }
    }
    qsort(lscontents, i, sizeof(char *), comparator);
    for (int j = 0; j < i; j++)
    {
        int len = strlen(lscontents[j]);
        if (isdirec(lscontents[j]) == 1)
            printf(Blue);
        else if (len > 4 && isout(lscontents[j]) == 1)
            printf(Green);
        else if (len > 3 && is_sh(lscontents[j]) == 1)
            printf(Green);
        else if (isfile(lscontents[j]) == 1)
            printf(White);
        printf("%s\n", lscontents[j]);
        printf(White);
    }
}
int cntlsArgs(char *token) // count number of arguments after ls
{
    int count = 0;
    while (token != NULL)
    {
        lsflag[count] = token;
        count++;
        token = strtok(NULL, " ");
    }
    return count;
}
void ls(int count) // count is number of argumenst after ls
{
    if (count == 0) // no arguments -- only ls. displays all files and direcs in current directory
    {
        call();
    }
    else if (count == 1) // one flag after ls
    {
        if (strcmp(lsflag[1], ".") == 0) // ls .  --  is same as ls
        {
            call();
        }
        else if (strcmp(lsflag[1], "-a") == 0) // ls -a : no condition on hidden files
        {
            char *dir = ".";
            int i = 0;
            struct dirent *ent;
            DIR *dh = opendir(dir);
            while ((ent = readdir(dh)) != NULL)
            {
                lscontents[i] = ent->d_name;
                i++;
            }
            qsort(lscontents, i, sizeof(char *), comparator);
            for (int j = 0; j < i; j++)
            {
                int len = strlen(lscontents[j]);
                if (isdirec(lscontents[j]) == 1)
                    printf(Blue);
                else if (len > 4 && isout(lscontents[j]) == 1)
                    printf(Green);
                else if (len > 3 && is_sh(lscontents[j]) == 1)
                    printf(Green);
                else if (isfile(lscontents[j]) == 1)
                    printf(White);
                printf("%s\n", lscontents[j]);
                printf(White);
            }
        }
        else if (strcmp(lsflag[1], "..") == 0) // ls ..  -- display contents of previous directory in hierarchy
        {
            getcwd(curr_dir, 1000);
            chdir("..");
            call();
            chdir(curr_dir);
        }
        else if (strcmp(lsflag[1], "-l") == 0) // display file name and info in detail
        {
            lsboth(1, 0, ".");
        }
        else if (strcmp(lsflag[1], "~") == 0) // display contents of home directory
        {
            getcwd(curr_dir, 1000);
            chdir(home_path);
            call();
            chdir(curr_dir);
        }
        else if (strcmp(lsflag[1], "-al") == 0)
        {
            lsboth(1, 1, ".");
        }
        else if (strcmp(lsflag[1], "-la") == 0)
        {
            lsboth(1, 1, ".");
        }
        else // directory/file  name after ls
        {
            struct stat path;
            stat(lsflag[1], &path);
            if (!S_ISDIR(path.st_mode)) // if not directory, print it directly
            {
                FILE *file;
                if (file = fopen(lsflag[1], "r"))
                {
                    printf("%s\n", lsflag[1]);
                }
                else
                    printf("File does not exist\n");
            }
            else
            {
                getcwd(curr_dir, 1000);
                chdir(lsflag[1]);
                getcwd(curr, 1000);
                int i = 0;
                struct dirent *ent;
                DIR *dh = opendir(curr);
                while ((ent = readdir(dh)) != NULL)
                {
                    if (ent->d_name[0] == '.')
                        continue;
                    else
                    {
                        lscontents[i] = ent->d_name;
                        i++;
                    }
                }
                qsort(lscontents, i, sizeof(char *), comparator);
                for (int j = 0; j < i; j++)
                {
                    int len = strlen(lscontents[j]);
                    if (isdirec(lscontents[j]) == 1)
                        printf(Blue);
                    else if (len > 4 && isout(lscontents[j]) == 1)
                        printf(Green);
                    else if (len > 3 && is_sh(lscontents[j]) == 1)
                        printf(Green);
                    else if (isfile(lscontents[j]) == 1)
                        printf(White);
                    printf("%s\n", lscontents[j]);
                    printf(White);
                }
                chdir(curr_dir);
            }
        }
    }    // count = 1 ends
    else // more than one flag
    {
        int aflag = 0, lflag = 0;
        int k = 0;
        for (int i = 1; i <= count; i++)
        {
            if (strcmp(lsflag[i], "-a") == 0)
                aflag = 1;
            else if (strcmp(lsflag[i], "-l") == 0)
                lflag = 1;
            else if (strcmp(lsflag[i], "-al") == 0)
            {
                aflag = 1;
                lflag = 1;
            }
            else if (strcmp(lsflag[i], "-la") == 0)
            {
                aflag = 1;
                lflag = 1;
            }
            else
            {
                strcpy(names[k], lsflag[i]);
                k++;
            }
        }
        if (k == 0) // only flags
        {
            if (aflag == 0 && lflag == 0)
                lsboth(0, 0, ".");
            else if (aflag == 1 && lflag == 1)
                lsboth(1, 1, ".");
            else if (aflag == 1 && lflag == 0)
            {
                int i = 0;
                struct dirent *ent;
                DIR *dh = opendir(".");
                while ((ent = readdir(dh)) != NULL)
                {
                    lscontents[i] = ent->d_name;
                    i++;
                }
                qsort(lscontents, i, sizeof(char *), comparator);
                for (int j = 0; j < i; j++)
                {
                    int len = strlen(lscontents[j]);
                    if (isdirec(lscontents[j]) == 1)
                        printf(Blue);
                    else if (len > 4 && isout(lscontents[j]) == 1)
                        printf(Green);
                    else if (len > 3 && is_sh(lscontents[j]) == 1)
                        printf(Green);
                    else if (isfile(lscontents[j]) == 1)
                        printf(White);
                    printf("%s\n", lscontents[j]);
                    printf(White);
                    printf("%s\n", lscontents[j]);
                }
            }
            else if (aflag == 0 && lflag == 1)
                lsboth(1, 0, ".");
        }
        else /// directory name + flags
        {
            if (aflag == 0 && lflag == 0)
            {
                for (int i = 0; i < k; i++)
                {
                    if (isdirec(names[i]))
                    {
                        printf("%s:\n", names[i]);
                        getcwd(curr_dir, 1000);
                        chdir(names[i]);
                        getcwd(curr, 1000);
                        call();
                        chdir(curr_dir);
                    }
                    else
                        printf("%s\n", names[i]);
                }
            }
            else if (aflag == 0 && lflag == 1)
            {
                for (int i = 0; i < k; i++)
                {
                    if (isdirec(names[i]))
                    {
                        printf("%s:\n", names[i]);
                        lsboth(1, 0, names[i]);
                    }
                    else
                        printf("%s\n", names[i]);
                }
            }
            else if (aflag == 1 && lflag == 0)
            {
                for (int i = 0; i < k; i++)
                {
                    if (isdirec(names[i]))
                    {
                        printf("%s:\n", names[i]);
                        getcwd(curr_dir, 1000);
                        chdir(names[i]);
                        char *dir = ".";
                        int i = 0;
                        struct dirent *ent;
                        DIR *dh = opendir(dir);
                        while ((ent = readdir(dh)) != NULL)
                        {
                            lscontents[i] = ent->d_name;
                            i++;
                        }
                        qsort(lscontents, i, sizeof(char *), comparator);
                        for (int j = 0; j < i; j++)
                        {
                            int len = strlen(lscontents[j]);
                            if (isdirec(lscontents[j]) == 1)
                                printf(Blue);
                            else if (len > 4 && isout(lscontents[j]) == 1)
                                printf(Green);
                            else if (len > 3 && is_sh(lscontents[j]) == 1)
                                printf(Green);
                            else if (isfile(lscontents[j]) == 1)
                                printf(White);
                            printf("%s\n", lscontents[j]);
                            printf(White);
                            chdir(curr_dir);
                        }
                    }
                    else
                        printf("%s\n", names[i]);
                }
            }
            else // both 1
            {
                for (int i = 0; i < k; i++)
                {
                    if (isdirec(names[i]))
                    {
                        printf("%s:\n", names[i]);
                        lsboth(1, 1, names[i]);
                    }
                    else
                        printf("%s\n", names[i]);
                }
            }
        }
    }
}