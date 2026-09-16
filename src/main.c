#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define MAX_COMMAND 100
#define BUFFER_SIZE 4096

/* ---------------- BACKUP ---------------- */

void create_backup(char *filename) {
    int source, destination;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead, bytesWritten;
    char backupPath[200];

    snprintf(backupPath, sizeof(backupPath), "backup/%s", filename);

    source = open(filename, O_RDONLY);

    if (source == -1) {
        perror("Error opening source file");
        return;
    }

    destination = open(
        backupPath,
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (destination == -1) {
        perror("Error creating backup");
        close(source);
        return;
    }

    while ((bytesRead = read(source, buffer, BUFFER_SIZE)) > 0) {

        bytesWritten = write(
            destination,
            buffer,
            bytesRead
        );

        if (bytesWritten != bytesRead) {
            perror("Error writing backup");
            close(source);
            close(destination);
            return;
        }
    }

    close(source);
    close(destination);

    printf("Backup successful: %s\n", filename);
}


/* ---------------- RESTORE ---------------- */

void restore_file(char *filename) {
    int source, destination;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead, bytesWritten;
    char backupPath[200];

    snprintf(backupPath, sizeof(backupPath), "backup/%s", filename);

    source = open(backupPath, O_RDONLY);

    if (source == -1) {
        perror("Backup not found");
        return;
    }

    destination = open(
        filename,
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (destination == -1) {
        perror("Error creating restored file");
        close(source);
        return;
    }

    while ((bytesRead = read(source, buffer, BUFFER_SIZE)) > 0) {

        bytesWritten = write(
            destination,
            buffer,
            bytesRead
        );

        if (bytesWritten != bytesRead) {
            perror("Error restoring file");
            close(source);
            close(destination);
            return;
        }
    }

    close(source);
    close(destination);

    printf("Restore successful: %s\n", filename);
}


/* ---------------- LIST BACKUPS ---------------- */

void list_backups() {

    printf("\nAvailable Backups:\n");

    system("ls -1 backup");

    printf("\n");
}


/* ---------------- HELP ---------------- */

void show_help() {

    printf("\nAvailable Commands:\n");

    printf("  help              - Show available commands\n");
    printf("  backup <file>     - Create a manual backup\n");
    printf("  restore <file>    - Restore a file manually\n");
    printf("  list              - List available backups\n");
    printf("  exit              - Exit FlashOS\n");

    printf("\n");
}


/* ---------------- MAIN ---------------- */

int main() {

    char command[MAX_COMMAND];

    /* Create backup directory if it does not exist */
    mkdir("backup", 0755);

    printf("============================\n");
    printf("        FlashOS v1.0\n");
    printf("============================\n");

    printf("Type 'help' to see commands.\n\n");


    /* REPL LOOP */

    while (1) {

        printf("FlashOS> ");

        if (fgets(command, sizeof(command), stdin) == NULL) {
            break;
        }

        /* Remove newline */
        command[strcspn(command, "\n")] = '\0';


        /* ---------- HELP ---------- */

        if (strcmp(command, "help") == 0) {

            show_help();
        }


        /* ---------- EXIT ---------- */

        else if (strcmp(command, "exit") == 0) {

            printf("Exiting FlashOS...\n");
            break;
        }


        /* ---------- BACKUP ---------- */

        else if (strncmp(command, "backup ", 7) == 0) {

            char *filename = command + 7;

            if (strlen(filename) == 0) {

                printf("Please provide a filename.\n");

            } else {

                create_backup(filename);
            }
        }


        /* ---------- RESTORE ---------- */

        else if (strncmp(command, "restore ", 8) == 0) {

            char *filename = command + 8;

            if (strlen(filename) == 0) {

                printf("Please provide a filename.\n");

            } else {

                restore_file(filename);
            }
        }


        /* ---------- LIST ---------- */

        else if (strcmp(command, "list") == 0) {

            list_backups();
        }


        /* ---------- UNKNOWN COMMAND ---------- */

        else {

            printf("Unknown command: %s\n", command);

            printf("Type 'help' for available commands.\n");
        }
    }

    return 0;
}
