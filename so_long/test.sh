#!/bin/bash

make
clear
for FILE in unvalid/*; do echo "$FILE :"; ./so_long $FILE; echo -e "====\n"; done