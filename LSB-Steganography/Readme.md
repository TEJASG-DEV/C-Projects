# LSB Steganography Project

In this project, the program encodes a text message bits within LSB of each byte (A pixel is 3 bytes) of the image. <br/>
Therefore the image appers the same and we'll have the message hidden.

## Build Instructions
To compile the project using makefile, type: 

`make steg`
or open the makefile and type the command mentioned in recepies for the targets.

### Execution
Note: The program works only with bitmap images.
type: `./steg -e/d <filename.bmp> <filename.txt> <optional_output.bmp>`
