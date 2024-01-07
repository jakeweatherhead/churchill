#!/bin/bash

# Define the menu options
HEIGHT=15
WIDTH=40
CHOICE_HEIGHT=4
BACKTITLE="Churchill. Copyright: Jake Weatherhead (2024)"
TITLE="Main Menu"
MENU="Choose one of the following options:"

OPTIONS=(1 "Run Churchill"
         2 "Run tests")

CHOICE=$(dialog --clear \
                --backtitle "$BACKTITLE" \
                --title "$TITLE" \
                --menu "$MENU" \
                $HEIGHT $WIDTH $CHOICE_HEIGHT \
                "${OPTIONS[@]}" \
                2>&1 >/dev/tty)

clear

case $CHOICE in
        1)
            /churchill/src/build/churchill
            ;;
        2)
            # Run your test command here
            # Example: /churchill/test/build/my_tests
            ;;
esac
