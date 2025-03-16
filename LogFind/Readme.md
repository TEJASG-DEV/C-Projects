# Project Logfind

This program is based off of the vague description given in the Exercise 26 of the book "Learn C The Hard Way" by Zed A. Shaw.<br/>
This program searches for text or source files mentioned in the '.logfind' file (dot_logfind in this repository since it's a hidden file.) and <br/>
searches for words mentioned in the command line arguments. If all the words are found in the file, it echoes the file name.

## Instructions
Rename the "dot_logfind" file to '.logfind' and you will be good to go.

## Build Instructions
To Compile the program using the makefile type: <br/>
`make logfind`

### Execution

type: <br/>
`./logfind <multiple words seperated by spaces>` for the 'AND' operation.<br/>
`./logfind -o <multiple words seperated by spaces>` for the 'OR' operation.
