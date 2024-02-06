#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

char global_username[100];
char global_email[100];
char local_name[100];
char local_email[100];

int initialize(int argc, char const *argv[]);
int create_configName(char *username);
int create_configEmail(char *email);
int add_file(int argc, char *adress);
int add_dir(int argc, char *adress);
int create_config_globalUsername(int argc, char const *argv[]);
int create_config_globalemail(int argc, char const *argv[]);
int add_n(int argc, char const *argv[]);
int reset_file(int argc, char *addres);
int reset_dir(int argc, char *addres);
int reset_undo(int argc, char const *argv[]);
int commit(int argc, char const *argv[]);

int initialize(int argc, char const *argv[])
{
    char cwd[1000];
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        fprintf(stderr, "%s\n", "Error finding the current-working-directory!");
        return 1;
    }
    char temp_dir[1000];
    struct dirent *entry;
    bool exist = false;
    do
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            fprintf(stderr, "%s\n", "Directory could not be found!");
            return 1;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == 4 && strcmp(entry->d_name, ".neogit") == 0)
            {
                exist = true;
            }
        }
        closedir(dir);

        if (getcwd(temp_dir, sizeof(temp_dir)) == NULL)
        { // output a reason for this error.
            fprintf(stderr, "%s\n", "Error");
        }
        if (strcmp(temp_dir, "/") != 0)
        {
            if (chdir("..") != 0)
            {
                fprintf(stderr, "%s\n", "Error");
            }
        }

    } while (strcmp(temp_dir, "/") != 0);

    if (chdir(cwd) != 0)
    {
        fprintf(stderr, "%s\n", "Error returning to cwd!");
        return 1;
    }

    if (exist)
    {
        fprintf(stdout, "%s\n", "Neogit repository already exists!");
    }
    else
    {
        if (mkdir(".neogit", 0755) != 0)
        {
            fprintf(stderr, "%s\n", "Making directory failed!");
            return 1;
        }
    }
    return 0;
}

int create_configName(char *username)
{
    FILE *file = fopen(".neogit/config", "a");
    if (file == NULL)
    {
        fprintf(stderr, "%s\n", "You have to initialize a repository first!");
        return 1;
    }
    fprintf(file, "local username is : %s\n", username);

    fclose(file);
    return 0;
}

int create_configEmail(char *email)
{
    FILE *file = fopen(".neogit/config", "a");
    if (file == NULL)
    {
        fprintf(stderr, "%s\n", "You have to initialize a repository first!");
        return 1;
    }
    fprintf(file, "local email is : %s\n", email);

    fclose(file);
    return 0;
}

int create_config_globalemail(int argc, char const *argv[])
{

    FILE *file = fopen("/home/parsa/globalconfig", "a");
    if (file == NULL)
    {
        fprintf(stderr, "%s\n", "Opening GlobalConfig failed!");
        return 1;
    }
    fprintf(file, "global email is : %s\n", argv[4]);
    fclose(file);

    return 0;
}

int create_config_globalUsername(int argc, char const *argv[])
{
    FILE *found = fopen("/mnt/c/FOP_Project/.neogit", "r");
    if (found != NULL)
    {
        // To do
    }
    fclose(found);
    FILE *file = fopen("/home/parsa/globalconfig", "a");
    if (file == NULL)
    {
        fprintf(stderr, "%s\n", "Opening GlobalConfig failed!");
        return 1;
    }
    fprintf(file, "global username is : %s\n", argv[4]);
    fclose(file);
    return 0;
}

int add_file(int argc, char *adress)
{
    FILE *foundL = fopen("/mnt/c/FOP_Project/.neogit/config", "r");
    FILE *foundG = fopen("/home/parsa/globalconfig", "r");
    if (foundL == NULL && foundG == NULL)
    {
        printf("Please enter you're username and email first\n");
        return 1;
    }
    if (foundL == NULL && foundG != NULL)
        fclose(foundG);
    if (foundL != NULL && foundG == NULL)
        fclose(foundL);
    chdir(adress);
    char filename[100];
    char *lastBackslash = strrchr(adress, '/');
    strcpy(filename, lastBackslash + 1);
    chdir("/mnt/c/FOP_Project");
    DIR *dir = opendir(".neogit");
    if (dir == NULL)
    {
        printf("You have to initialize a repository first!\n");
        return 1;
    }
    else
    {
        FILE *found = fopen("/mnt/c/FOP_Project/.neogit/stage.txt", "r");
        if (found == NULL)
        {
            FILE *create = fopen("/mnt/c/FOP_Project/.neogit/stage.txt", "a");
            // fclose(create);
        }
        // fclose(found);
    }
    chdir(".neogit");
    DIR *openFolder = opendir("staging");
    if (openFolder == NULL)
    {
        if (mkdir("staging", 0755) != 0)
        {
            printf("Could not make staging directory!\n");
            return 1;
        }
    }
    else
    {
        chdir("/mnt/c/FOP_Project/.neogit/staging");
        FILE *open = fopen(filename, "r");
        if (open == NULL)
        {
            FILE *create = fopen(filename, "w");
            if (create == NULL)
            {
                printf("Could not create file!");
                return 1;
            }
            else
            {
                printf("%s : File added!\n", filename);
                FILE *write = fopen("/mnt/c/FOP_Project/.neogit/stage.txt", "a");
                fprintf(write, "\n%s", filename);
                fclose(write);
            }
            char ch;
            FILE *source = fopen(adress, "r");
            while ((ch = fgetc(source)) != EOF)
            {
                fputc(ch, create);
            }
        }
        else
        {
            FILE *diff = fopen(adress, "r");
            chdir("/mnt/c/FOP_Project/.neogit/staging");
            FILE *dest = fopen(filename, "r+");
            if (diff == NULL || dest == NULL)
            {
                printf("Error!\n");
                return 1;
            }

            int ch1, ch2;
            char ch3;
            do
            {
                ch1 = fgetc(diff);
                ch2 = fgetc(dest);

                if (ch1 != ch2)
                {
                    fseek(diff, 0, SEEK_SET);
                    fseek(dest, 0, SEEK_SET);
                    while ((ch3 = fgetc(diff)) != EOF)
                    {
                        fputc(ch3, dest);
                    }
                    break;
                }
            } while (ch1 != EOF && ch2 != EOF);

            if (ch1 == EOF && ch2 == EOF)
            {
                printf("%s : File is already on stage!\n", filename);
            }
            else
            {
                printf("%s : File Updated!\n", filename);
                FILE *write = fopen("/mnt/c/FOP_Project/.neogit/stage.txt", "a");
                fprintf(write, "\n%s", filename);
                fclose(write);
            }
            fclose(diff);
            fclose(dest);
        }
    }

    return 0;
}
int add_dir(int argc, char *adress)
{
    FILE *foundL = fopen("/mnt/c/FOP_Project/.neogit/config", "r");
    FILE *foundG = fopen("/home/parsa/globalconfig", "r");
    if (foundL == NULL && foundG == NULL)
    {
        printf("Please enter you're username and email first\n");
        return 1;
    }
    if (foundL == NULL && foundG != NULL)
        fclose(foundG);
    if (foundL != NULL && foundG == NULL)
        fclose(foundL);
    DIR *dir = opendir("/mnt/c/FOP_Project/.neogit");
    if (dir == NULL)
    {
        printf("You have to initialize a repository first!\n");
        return 1;
    }
    else
    {
        FILE *found = fopen("/mnt/c/FOP_Project/.neogit/stage.txt", "r");
        if (found == NULL)
        {
            FILE *create = fopen("/mnt/c/FOP_Project/.neogit/stage.txt", "a");
            fclose(create);
        }
        fclose(found);
    }
    closedir(dir);
    char direname[100];
    char *lastBackslash = strrchr(adress, '/');
    strcpy(direname, lastBackslash + 1);

    if (chdir("/mnt/c/FOP_Project/.neogit/staging") != 0)
    {
        mkdir("/mnt/c/FOP_Project/.neogit/staging", 0755);
    }
    DIR *found = opendir(direname);
    if (found == NULL)
    {
        if (mkdir(direname, 0755) != 0)
        {
            printf("Could not make directory\n");
            return 1;
        }
    }
    else
    {
        printf("%s : Directrory is already added!\n", direname);
        return 1;
    }
    struct dirent *entry;
    DIR *dir_1 = opendir(adress);
    while ((entry = readdir(dir_1)) != NULL)
    {
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        {
            continue;
        }
        chdir(adress);
        FILE *file = fopen(entry->d_name, "r");
        if (file == NULL)
        {
            printf("ERROR!\n");
            return 1;
        }
        char temp[1000];
        char ch;
        chdir("/mnt/c/FOP_Project/.neogit/staging");
        chdir(direname);

        FILE *copy = fopen(entry->d_name, "w");
        if (copy == NULL)
        {
            printf("!\n");
            return 1;
        }

        while ((ch = fgetc(file)) != EOF)
        {
            fputc(ch, copy);
        }
    }
    printf("%s : Directory added!\n",direname);
    FILE *write = fopen("/mnt/c/FOP_Project/.neogit/stage.txt", "a");
    fprintf(write, "\n%s", direname);
    fclose(write);

    return 0;
}
int add_n(int argc, char const *argv[])
{
    FILE *foundL = fopen("/mnt/c/FOP_Project/.neogit/config", "r");
    FILE *foundG = fopen("/home/parsa/globalconfig", "r");
    if (foundL == NULL && foundG == NULL)
    {
        printf("Please enter you're username and email first\n");
        return 1;
    }
    if (foundL == NULL && foundG != NULL)
        fclose(foundG);
    if (foundL != NULL && foundG == NULL)
        fclose(foundL);
    DIR *dir = opendir("/mnt/c/FOP_Project/.neogit");
    if (dir == NULL)
    {
        printf("You have to initialize a repository first!\n");
        return 1;
    }
    closedir(dir);
    DIR *check = opendir("/mnt/c/FOP_project");
    if (check == NULL)
    {
        printf("Could not open directory!\n");
        return 1;
    }

    struct dirent *entry;
    struct dirent *entry2;

    while ((entry = readdir(check)) != NULL)
    {
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        {
            continue;
        }
        if (entry->d_type == 4) // folder
        {
            DIR *open = opendir("/mnt/c/FOP_Project/.neogit/staging");
            int flag = 0;
            while ((entry2 = readdir(open)) != NULL)
            {
                if (strcmp(entry->d_name, entry2->d_name) == 0)
                {
                    flag = 1;
                }
            }
            if (flag)
            {
                printf("%s : directory is on stage!\n", entry->d_name);
            }
            else
            {
                printf("%s : directory is NOT on stage!\n", entry->d_name);
            }
            closedir(open);
        }
        if (entry->d_type == 8) 
        {
            DIR *open1 = opendir("/mnt/c/FOP_Project/.neogit/staging");
            if (open1 == NULL)
            {
                printf("Could not open directory!\n");
                return 1;
            }
            int flag = 0;
            while ((entry2 = readdir(open1)) != NULL)
            {
                if (strcmp(entry->d_name, entry2->d_name) == 0)
                {
                    flag = 1;
                }
            }
            if (flag)
            {
                printf("%s : file is on stage!\n", entry->d_name);
            }
            else
            {
                printf("%s : file is NOT on stage!\n", entry->d_name);
            }
            closedir(open1);
        }
    }

    return 0;
}

int reset_file(int argc, char *addres)
{
    FILE *foundL = fopen("/mnt/c/FOP_Project/.neogit/config", "r");
    FILE *foundG = fopen("/home/parsa/globalconfig", "r");
    if (foundL == NULL && foundG == NULL)
    {
        printf("Please enter you're username and email first\n");
        return 1;
    }
    if (foundL == NULL && foundG != NULL)
        fclose(foundG);
    if (foundL != NULL && foundG == NULL)
        fclose(foundL);
    DIR *dir = opendir("/mnt/c/FOP_Project/.neogit");
    if (dir == NULL)
    {
        printf("You have to initialize a repository first!\n");
        return 1;
    }
    closedir(dir);
    char filename[100];
    char *lastBackslash = strrchr(addres, '/');
    strcpy(filename, lastBackslash + 1);
    chdir("/mnt/c/FOP_Project/.neogit/staging");
    FILE *find = fopen(filename, "r");
    if (find == NULL)
    {
        printf("%s : The file is not on stage!\n", filename);
        return 1;
    }
    if (remove(filename) == 0)
    {
        printf("%s : file was unstaged successfully!\n", filename);
    }
    else
    {
        printf("Error unstaging the file!\n");
        return 1;
    }
    fclose(find);

    return 0;
}

int reset_dir(int argc, char *addres)
{
    FILE *foundL = fopen("/mnt/c/FOP_Project/.neogit/config", "r");
    FILE *foundG = fopen("/home/parsa/globalconfig", "r");
    if (foundL == NULL && foundG == NULL)
    {
        printf("Please enter you're username and email first\n");
        return 1;
    }
    if (foundL == NULL && foundG != NULL)
        fclose(foundG);
    if (foundL != NULL && foundG == NULL)
        fclose(foundL);
    DIR *dir = opendir("/mnt/c/FOP_Project/.neogit");
    if (dir == NULL)
    {
        printf("You have to initialize a repository first!\n");
        return 1;
    }
    closedir(dir);
    char dirname[100];
    char *lastBackslash = strrchr(addres, '/');
    strcpy(dirname, lastBackslash + 1);
    chdir("/mnt/c/FOP_Project/.neogit/staging");

    DIR *open = opendir(dirname);
    if (open == NULL)
    {
        printf("%s : Directory is not on stage!\n", dirname);
        return 1;
    }

    struct dirent *entry;
    while ((entry = readdir(open)) != NULL)
    {
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        {
            continue;
        }
        char fullpath[256];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dirname, entry->d_name);

        if (remove(fullpath) != 0)
        {
            printf("%s : Could not delete file or directory!\n", fullpath);
        }
    }

    if (rmdir(dirname) == 0)
    {
        printf("%s : Directory was unstaged successfully!\n", dirname);
    }
    else
    {
        printf("Error unstaging directory!\n");
        return 1;
    }
    closedir(open);

    return 0;
}
int reset_undo(int argc, char const *argv[])
{
    FILE *foundL = fopen("/mnt/c/FOP_Project/.neogit/config", "r");
    FILE *foundG = fopen("/home/parsa/globalconfig", "r");
    if (foundL == NULL && foundG == NULL)
    {
        printf("Please enter you're username and email first\n");
        return 1;
    }
    if (foundL == NULL && foundG != NULL)
        fclose(foundG);
    if (foundL != NULL && foundG == NULL)
        fclose(foundL);
    DIR *dir = opendir("/mnt/c/FOP_Project/.neogit");
    if (dir == NULL)
    {
        printf("You have to initialize a repository first!\n");
        return 1;
    }
    closedir(dir);

    FILE *open = fopen("/mnt/c/FOP_Project/.neogit/stage.txt", "r");
    if (open == NULL)
    {
        printf("You have to add files or directories first!\n");
        return 1;
    }
    fseek(open, 0, SEEK_END);

    long pos = ftell(open);

    while (pos > 0)
    {
        pos--;
        fseek(open, pos, SEEK_SET);

        char c = fgetc(open);
        if (c == '\n')
        {
            break;
        }
    }

    size_t bufsize = ftell(open);
    char *lastLine = malloc(bufsize + 1);
    if (fgets(lastLine, bufsize + 1, open) != NULL)
    {
        chdir("/mnt/c/FOP_Project/.neogit/staging");
        if (remove(lastLine) == 0)
        {
            printf("%s : File was unstaged successfully!\n", lastLine);
        }
        else
        {
            printf("!\n");
            struct dirent *entry;
            DIR *open = opendir(lastLine);
            chdir(lastLine);
            while ((entry = readdir(open)) != NULL)
            {
                if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
                {
                    continue;
                }

                if (remove(entry->d_name) != 0)
                {
                    printf("Error deleting!\n");
                    return 1;
                }
            }
            chdir("/mnt/c/FOP_Project/.neogit/staging");
            if (rmdir(lastLine) == 0)
            {
                printf("%s : Directory was unstaged successfully!\n", lastLine);
            }
        }
    }
    else
    {
        printf("Error reading last line\n");
        return 1;
    }

    free(lastLine);
    fclose(open);
    return 0;
}

int commit(int argc, char const *argv[])
{
    FILE *foundL = fopen("/mnt/c/FOP_Project/.neogit/config", "r");
    FILE *foundG = fopen("/home/parsa/globalconfig", "r");
    if (foundL == NULL && foundG == NULL)
    {
        printf("Please enter you're username and email first\n");
        return 1;
    }
    if (foundL == NULL && foundG != NULL)
        fclose(foundG);
    if (foundL != NULL && foundG == NULL)
        fclose(foundL);
    DIR *dir = opendir("/mnt/c/FOP_Project/.neogit");
    if (dir == NULL)
    {
        printf("You have to initialize a repository first!\n");
        return 1;
    }
    closedir(dir);

    chdir("/mnt/c/FOP_Project/.neogit");
    DIR *open = opendir("commits");
    if (open == NULL)
    {
        if (mkdir("commits", 0755) != 0)
        {
            printf("Error making commits directory!\n");
            return 1;
        }
    }
    closedir(open);

    if (argc == 3)
    {
        printf("You have to define a message!\n");
        return 1;
    }
    if (strlen(argv[3]) > 72)
    {
        printf("Commit message is too long!\n");
        return 1;
    }
    chdir("/mnt/c/FOP_Project/.neogit");

    struct dirent *entry;
    DIR *open2 = opendir("/mnt/c/FOP_Project/.neogit/staging");
    mkdir("/mnt/c/FOP_Project/.neogit/commits/commit1", 0755);
    chdir("/mnt/c/FOP_Project/.neogit/commits/commit1");
    FILE *create = fopen("commit1Log", "a");
    if (create == NULL)
    {
        printf("Couldn't create file!\n");
        return 1;
    }
    while ((entry = readdir(open2)) != NULL)
    {
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        {
            continue;
        }
        if (entry->d_type == 8) // file
        {
            chdir("/mnt/c/FOP_Project/.neogit/staging");
            FILE *source = fopen(entry->d_name, "r");
            if (source == NULL)
            {
                printf("ERROR!\n");
                return 1;
            }
            chdir("/mnt/c/FOP_Project/.neogit/commits/commit1");
            char mmd;
            FILE *dest = fopen(entry->d_name, "w");
            if (dest == NULL)
            {
                printf("RIDI!\n");
                return 1;
            }
            while ((mmd = fgetc(source)) != EOF)
            {
                fputc(mmd, dest);
            }
            fclose(source);
            fclose(dest);
        }
    }

    FILE *commit = fopen("commit1Log", "a");
    if (commit == NULL)
    {
        printf("wrong!\n");
        return 1;
    }
    time_t t;
    struct tm *current_time;

    // Get current time
    t = time(NULL);
    current_time = localtime(&t);

    // Print date and time components
    chdir("/mnt/c/FOP_Project/.neogit");
    FILE *config = fopen("config", "r");
    char name[100];
    char email[100];
    fscanf(config, "local username is : %s\n", name);
    fscanf(config, "local email is : %s", email);
    fprintf(commit, "%s\n", "Commit 1");
    fprintf(commit, "commited by : %s %s\n", name, email);
    fprintf(commit, "date : %02d-%02d-%d %02d:%02d:%02d\n",
            current_time->tm_mday, current_time->tm_mon + 1, current_time->tm_year + 1900,
            current_time->tm_hour, current_time->tm_min, current_time->tm_sec);
    fprintf(commit, "commit message : %s\n", argv[3]);
    static uint64_t counter = 0;

    // Combine time and counter to create a unique ID
    uint64_t unique_id = ((uint64_t)current_time << 32) | (counter++);
    fprintf(commit, "commit ID : %llu\n", unique_id);
    printf("Added files were successfully commited!\n");


    fclose(commit);
    fclose(config);
    return 0;
}

int main(int argc, char const *argv[])
{
    if (argc == 1 && strcmp(argv[0], "neogit") == 0)
    {
        fprintf(stdout, "%s\n", "Neogit is installed and ready to use!");
        return 0;
    }
    else if (argc < 2)
    {
        fprintf(stderr, "%s\n", "Please enter a valid command");
        return 1;
    }

    else if ((strcmp(argv[1], "config") == 0) && (strcmp(argv[2], "-global") == 0) && (strcmp(argv[3], "user.name") == 0))
    {
        strcpy(global_username, argv[4]);
        create_config_globalUsername(argc, argv);
    }
    else if ((strcmp(argv[1], "config") == 0) && (strcmp(argv[2], "-global") == 0) && (strcmp(argv[3], "user.email") == 0))
    {
        strcpy(global_email, argv[4]);
        create_config_globalemail(argc, argv);
    }
    else if ((strcmp(argv[1], "config") == 0) && (strcmp(argv[2], "user.name") == 0))
    {
        strcpy(local_name, argv[3]);
        create_configName(local_name);
    }
    else if ((strcmp(argv[1], "config") == 0) && (strcmp(argv[2], "user.email") == 0))
    {
        strcpy(local_email, argv[3]);
        create_configEmail(local_email);
    }
    else if (strcmp(argv[1], "init") == 0)
    {
        initialize(argc, argv);
    }
    else if (strcmp(argv[1], "add") == 0)
    {
        if (strcmp(argv[2], "-f") == 0)
        {
            for (int i = 3; i < argc; i++)
            {
                FILE *file = fopen(argv[i], "r");
                DIR *dir = opendir(argv[i]);

                if (file == NULL && dir == NULL)
                {
                    char filename[100];
                    char *lastBackslash = strrchr(argv[i], '/');
                    strcpy(filename, lastBackslash + 1);
                    printf("%s : File or directory doesn't exists!\n", filename);
                }
                if (dir != NULL)
                {
                    char folderadress[100];
                    strcpy(folderadress, argv[i]);
                    add_dir(argc, folderadress);
                    continue;
                }
                if (file != NULL)
                {
                    char fileadress[100];
                    strcpy(fileadress, argv[i]);
                    add_file(argc, fileadress);
                    continue;
                }
            }
        }
        else if (strcmp(argv[2], "-n") == 0)
        {
            add_n(argc, argv);
        }
        else
        {
            for (int i = 2; i < argc; i++)
            {
                FILE *file = fopen(argv[i], "r");
                DIR *dir = opendir(argv[i]);

                if (file == NULL && dir == NULL)
                {
                    char filename[100];
                    char *lastBackslash = strrchr(argv[i], '/');
                    strcpy(filename, lastBackslash + 1);
                    printf("%s : File or directory doesn't exists!\n", filename);
                }
                if (dir != NULL)
                {
                    char folderadress[100];
                    strcpy(folderadress, argv[i]);
                    add_dir(argc, folderadress);
                    continue;
                }
                if (file != NULL)
                {
                    char fileadress[100];
                    strcpy(fileadress, argv[i]);
                    add_file(argc, fileadress);
                    continue;
                }
            }
        }
    }
    else if (strcmp(argv[1], "reset") == 0)
    {
        if (strcmp(argv[2], "-f") == 0)
        {
            for (int i = 3; i < argc; i++)
            {
                FILE *file = fopen(argv[i], "r");
                DIR *dir = opendir(argv[i]);

                if (file == NULL && dir == NULL)
                {
                    char filename[100];
                    char *lastBackslash = strrchr(argv[i], '/');
                    strcpy(filename, lastBackslash + 1);
                    printf("%s : File or directory doesn't exists!\n", filename);
                }
                if (dir != NULL)
                {
                    char folderadress[100];
                    strcpy(folderadress, argv[i]);
                    reset_dir(argc, folderadress);
                    continue;
                }
                if (file != NULL)
                {
                    char fileadress[100];
                    strcpy(fileadress, argv[i]);
                    reset_file(argc, fileadress);
                    continue;
                }
            }
        }
        else if (strcmp(argv[2], "-undo") == 0)
        {
            reset_undo(argc, argv);
        }
        else
        {
            for (int i = 2; i < argc; i++)
            {
                FILE *file = fopen(argv[i], "r");
                DIR *dir = opendir(argv[i]);

                if (file == NULL && dir == NULL)
                {
                    char filename[100];
                    char *lastBackslash = strrchr(argv[i], '/');
                    strcpy(filename, lastBackslash + 1);
                    printf("%s : File or directory doesn't exists!\n", filename);
                }
                if (dir != NULL)
                {
                    char folderadress[100];
                    strcpy(folderadress, argv[i]);
                    reset_dir(argc, folderadress);
                    // continue;
                }
                if (file != NULL)
                {
                    char fileadress[100];
                    strcpy(fileadress, argv[i]);
                    reset_file(argc, fileadress);
                    // continue;
                }
            }
        }
    }
    else if (strcmp(argv[1], "commit") == 0)
    {
        commit(argc, argv);
    }
    else
    {
        fprintf(stderr, "%s\n", "Invalid command!");
        return 1;
    }

    return 0;
}