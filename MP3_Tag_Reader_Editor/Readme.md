# MP3 Tag Reader and Editor Project

This program is used to edit ID3 V2.3.0 Mp3 tags. Also functionality is limited to edit only the Title, Artist Name, Album Name, Year, Content Type and Comment Tags.

## Build Instructions
To compile the project using makefile, type:<br/> 

`make`

### Execution

type: `./reader -v <filename.mp3>` to view the ID3 V2.3 Mp3 audio's tag.<br/>
type `./reader -e -t/a/A/m/y/c <filename.mp3>` to edit specific frames of the ID3 V2.3 Mp3 tag.
