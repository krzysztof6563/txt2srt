# txt2srt
A simple application that lets you convert subtitles from MicroDVD format (txt) to SubRip format (.srt)

To use it you just have to drag'n'drop a file onto it. By default it's configured to use 23.976 FPS for conversion.
If you want to change it you need to either edit main.cpp and change FPS value or use a command line.

Command line usage:

  txt2srt source_file output_file FPS