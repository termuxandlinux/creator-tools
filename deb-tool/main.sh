#!/bin/bash

# Prompt for input for each variable
read -p "Name: " name
read -p "Version: " version
read -p "Architecture: " arch
read -p "Author: " author
read -p "Email: " mail
read -p "Description: " description
read -p "Homepage: " homepage
read -p "Dependencies (comma-separated): " depends
read -p "Filename: " file

# Erstelle JSON-Vorlage
template=$(cat <<EOF
{
  "name": "$name",
  "version": "$version",
  "arch": "$arch",
  "maintainer": "$author <$mail>",
  "description": "$description",
  "homepage": "$homepage",
  "depends": ["$depends"],
  "files": {
    "$file": "/data/data/com.termux/files/usr/bin/$name"
  }
}
EOF
)

# Speichere JSON-Vorlage in Datei
echo "$template" > "${name}.json"

echo "JSON file created: ${name}.json"

# Optionen verarbeiten
while [[ $# -gt 0 ]]
do
    key="$1"
    case $key in
        --setup)
            # Führe setup aus
            pkg install termux-create-package
            mkdir pkg
            shift # Gehe zum nächsten Argument
            ;;
        --build)
            # Führe build aus
            termux-create-package *.json
            mv ${name}*.deb ${name}.deb
            mv ${name}.deb pkg/
            shift # Gehe zum nächsten Argument
            ;;
        --help)
            # Zeige Hilfe
            echo "Commands:"
            echo "--setup: Führt pkg install termux-create-package und mkdir pkg aus"
            echo "--build: Führt termux-create-package *.json und mv *.deb pkg/ aus"
            echo "--help: Zeigt diese Hilfe"
            exit 0
            ;;
        *)
            # Unbekanntes Argument
            echo "Unrecognized command: $key"
            exit 1
            ;;
    esac
done
