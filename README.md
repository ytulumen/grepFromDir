# grepFromDir
this project finds a given string in a given path. might be an example for multiprogramming. 

HOW IT WORKS?

1) Source code compilation done with running 'make' command in terminal
2) Run command in terminal, 
    >./wordcount "directoryname (/home/yasin/Desktop/sampleFolder)" " search word (ece)"  
3) If there is any fault about arguments, program gives an error and exits.
4) Otherwise it finds all files under the given directory.
5) Searches for search word in found files.
6) For found locations, it creates grepFromDir.log file and writes it there which path is word found with line and column        informations.
7) Sample screen print is like that:
  Total number of 'ece' in /home/yasin/Masaüstü/deneme : 4



