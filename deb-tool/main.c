#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_LENGTH 1000

void create_json(const char *name, const char *version, const char *arch, const char *author, const char *mail, const char *description, const char *homepage, const char *depends, const char *file) {
    FILE *fp;
    char filename[MAX_INPUT_LENGTH];
    snprintf(filename, sizeof(filename), "%s.json", name);
    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    fprintf(fp, "{\n");
    fprintf(fp, "  \"name\": \"%s\",\n", name);
    fprintf(fp, "  \"version\": \"%s\",\n", version);
    fprintf(fp, "  \"arch\": \"%s\",\n", arch);
    fprintf(fp, "  \"maintainer\": \"%s <%s>\",\n", author, mail);
    fprintf(fp, "  \"description\": \"%s\",\n", description);
    fprintf(fp, "  \"homepage\": \"%s\",\n", homepage);
    fprintf(fp, "  \"depends\": [\"%s\"],\n", depends);
    fprintf(fp, "  \"files\": {\n");
    fprintf(fp, "    \"%s\": \"/data/data/com.termux/files/usr/bin/%s\"\n", file, name);
    fprintf(fp, "  }\n");
    fprintf(fp, "}\n");
    fclose(fp);
    printf("JSON file created: %s\n", filename);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // Erstelle JSON-Script
        char name[MAX_INPUT_LENGTH], version[MAX_INPUT_LENGTH], arch[MAX_INPUT_LENGTH], author[MAX_INPUT_LENGTH], mail[MAX_INPUT_LENGTH], description[MAX_INPUT_LENGTH], homepage[MAX_INPUT_LENGTH], depends[MAX_INPUT_LENGTH], file[MAX_INPUT_LENGTH];
        printf("Enter name: ");
        scanf("%s", name);
        printf("Enter version: ");
        scanf("%s", version);
        printf("Enter architecture: ");
        scanf("%s", arch);
        printf("Enter author: ");
        scanf("%s", author);
        printf("Enter email: ");
        scanf("%s", mail);
        printf("Enter description: ");
        scanf("%s", description);
        printf("Enter homepage: ");
        scanf("%s", homepage);
        printf("Enter dependencies (comma-separated): ");
        scanf("%s", depends);
        printf("Enter filename: ");
        scanf("%s", file);
        create_json(name, version, arch, author, mail, description, homepage, depends, file);
    } else if (argc == 2) {
        // Führe das angegebene Argument aus
        if (strcmp(argv[1], "--setup") == 0) {
            // Führe setup aus
            system("pkg install termux-create-package");
            system("mkdir pkg");
        } else if (strcmp(argv[1], "--build") == 0) {
            // Führe build aus
            char name[MAX_INPUT_LENGTH];
            printf("Enter name: ");
            scanf("%s", name);
            char command[MAX_INPUT_LENGTH];
            snprintf(command, sizeof(command), "termux-create-package %s.json", name);
            system(command);
            snprintf(command, sizeof(command), "mv %s*.deb %s.deb", name, name);
            system(command);
            snprintf(command, sizeof(command), "mv %s.deb pkg/", name);
            system(command);
        } else if (strcmp(argv[1], "--help") == 0) {
            // Zeige Hilfe
            printf("Commands:\n");
            printf("--setup: Führt pkg install termux-create-package und mkdir pkg aus\n");
            printf("--build: Führt termux-create-package *.json und mv *.deb pkg/ aus\n");
            printf("--help: Zeigt diese Hilfe\n");
            return 0;
        } else {
            printf("Unrecognized command: %s\n", argv[1]);
            return 1;
        }
    } else {
        printf("Usage: %s [--setup | --build | --help]\n", argv[0]);
        return 1;
    }

    return 0;
}
