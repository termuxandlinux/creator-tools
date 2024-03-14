#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h> // Für mkdir()

#define MAX_LENGTH 256

void webhook_setup() {
    char dir[MAX_LENGTH];
    snprintf(dir, sizeof(dir), "%s/.webhook", getenv("HOME"));
    mkdir(dir, 0700);
}

void set_webhook_url() {
    char name[MAX_LENGTH];
    char url[MAX_LENGTH];
    printf("Enter webhook name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter webhook URL: ");
    fgets(url, sizeof(url), stdin);
    url[strcspn(url, "\n")] = 0;

    char filename[MAX_LENGTH];
    snprintf(filename, sizeof(filename), "%s/.webhook/%s.txt", getenv("HOME"), name);
    FILE *file = fopen(filename, "w");
    if (file) {
        fprintf(file, "%s\n", url);
        fclose(file);
        printf("Webhook URL set for %s.\n", name);
    } else {
        printf("Error: Unable to open file for writing.\n");
    }
}

void delete_webhook() {
    char dir[MAX_LENGTH];
    snprintf(dir, sizeof(dir), "%s/.webhook", getenv("HOME"));
    if (rmdir(dir) == 0) {
        printf("Webhook directory deleted.\n");
    } else {
        printf("Error: Unable to delete webhook directory.\n");
    }
}

void delete_url() {
    char name[MAX_LENGTH];
    printf("Enter webhook name to delete its URL: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    char filename[MAX_LENGTH];
    snprintf(filename, sizeof(filename), "%s/.webhook/%s.txt", getenv("HOME"), name);
    if (remove(filename) == 0) {
        printf("Webhook URL for %s deleted.\n", name);
    } else {
        printf("Error: Unable to delete webhook URL.\n");
    }
}

void remove_script() {
    if (remove("/data/data/com.termux/files/usr/bin/wh-tool") == 0) {
        printf("Script removed.\n");
    } else {
        printf("Error: Unable to remove script.\n");
    }
}

void show_help() {
    printf("Usage: wh-tool [OPTION]\n");
    printf("Options:\n");
    printf("  --setup        Create directory for webhook URLs\n");
    printf("  --seturl       Set webhook URL\n");
    printf("  --delete       Delete webhook directory\n");
    printf("  --deleteurl    Delete specific webhook URL\n");
    printf("  --remove       Remove this script\n");
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        char name[MAX_LENGTH];
        printf("Enter webhook name: ");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;

        char filename[MAX_LENGTH];
        snprintf(filename, sizeof(filename), "%s/.webhook/%s.txt", getenv("HOME"), name);
        FILE *file = fopen(filename, "r");
        if (file) {
            char webhook_url[MAX_LENGTH];
            fgets(webhook_url, sizeof(webhook_url), file);
            fclose(file);
            webhook_url[strcspn(webhook_url, "\n")] = 0;

            char message[MAX_LENGTH];
            printf("Enter message: ");
            fgets(message, sizeof(message), stdin);
            message[strcspn(message, "\n")] = 0;

            char command[MAX_LENGTH];
            snprintf(command, sizeof(command), "curl -X POST -H 'Content-Type: application/json' -d '{\"content\":\"%s\"}' %s", message, webhook_url);
            system(command);
        } else {
            printf("Error: Webhook URL not found.\n");
        }
    } else {
        if (strcmp(argv[1], "--setup") == 0) {
            webhook_setup();
        } else if (strcmp(argv[1], "--seturl") == 0) {
            set_webhook_url();
        } else if (strcmp(argv[1], "--delete") == 0) {
            delete_webhook();
        } else if (strcmp(argv[1], "--deleteurl") == 0) {
            delete_url();
        } else if (strcmp(argv[1], "--remove") == 0) {
            remove_script();
        } else if (strcmp(argv[1], "--help") == 0) {
            show_help();
        } else {
            printf("Invalid parameter. Use --help to see available options.\n");
        }
    }

    return 0;
}
