#!/bin/bash

webhook_setup() {
    mkdir -p $HOME/.webhook
}

set_webhook_url() {
    read -p "Enter webhook name: " name
    read -p "Enter webhook URL: " url
    echo $url > $HOME/.webhook/${name}.txt
    echo "Webhook URL set for ${name}."
}

delete_webhook() {
    rm -rf $HOME/.webhook
    echo "Webhook directory deleted."
}

delete_url() {
    read -p "Enter webhook name to delete its URL: " name
    rm -f $HOME/.webhook/${name}.txt
    echo "Webhook URL for ${name} deleted."
}

remove_script() {
    rm /data/data/com.termux/files/usr/bin/wh-tool
    echo "Script removed."
}

show_help() {
    echo "Usage: wh-tool [OPTION]"
    echo "Options:"
    echo "  --setup        Create directory for webhook URLs"
    echo "  --seturl       Set webhook URL"
    echo "  --delete       Delete webhook directory"
    echo "  --deleteurl    Delete specific webhook URL"
    echo "  --remove       Remove this script"
}

if [ $# -eq 0 ]; then
    read -p "Enter webhook name: " name
    webhook_url=$(< $HOME/.webhook/${name}.txt)
    read -p "Enter message: " message
    curl -X POST -H "Content-Type: application/json" -d "{\"content\":\"$message\"}" $webhook_url
else
    case $1 in
        --setup) webhook_setup;;
        --seturl) set_webhook_url;;
        --delete) delete_webhook;;
        --deleteurl) delete_url;;
        --remove) remove_script;;
        --help) show_help;;
        *) echo "Invalid parameter. Use --help to see available options.";;
    esac
fi
