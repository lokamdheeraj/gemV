#!/bin/bash
helper(){
    echo "This script takes two file pathes as arguments."
    echo "$0 path/to/file1 path/to/file2"
}

validate_input(){
    if [[ $# != 2 ]]; then 
        helper
        exit 1
    fi

    if [[ ! -f "$1" ]]; then
        echo "$1" file does not exist.
        helper
        exit 1
    fi
    if [[ ! -f "$2" ]]; then
        echo "$2" file does not exist.
        helper
        exit 1
    fi
}

diff_files(){
#Takes number of lines currently in $2 file,and compares it with exactly the same number of lines in $1
    diff -q -a -w <(tail -n+"$ULINES" $1 | head -n "$CURR_LINE") <(tail -n+"$ULINES" $2 | head -n "$CURR_LINE")
}

get_curr_lines(){
# Number of current line minus Unused lines
    echo "$[$(cat $1 | wc -l) - $ULINES]"
}

print_diff_lines(){
    diff -a -w --unchanged-line-format="" --new-line-format=":%dn: %L" "$1" "$2" | grep -o ":[0-9]*:" | tr -d ":"
}

ULINES=15 # count of first unused lines. How many first lines to ignore

validate_input "$1" "$2"
CURR_LINE=$(get_curr_lines "$2") # count of lines currenly have minus ULINES

if [[ $CURR_LINE < 0 ]];then
    exit 0
fi

IS_DIFF=$(diff_files "$1" "$2")
if [[ -z "$IS_DIFF" ]];then
    echo "In match with Golden run"
else
    red='\e[0;31m'
    NC='\e[0m' # No Colour
    echo -e "${red}FAULT!${NC}"
    echo "Line number: " `print_diff_lines "$1" "$2"`
fi


