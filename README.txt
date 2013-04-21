Kim Cooperrider
CS 1550
Project 4
Spring 2013
kac162@pitt.edu

README.txt

====================================================================================================
  INSTRUCTIONS
====================================================================================================


--COMPILATION INSTRUCTIONS--
To compile this code, just run the following command:
  make


--RUNNING INSTRUCTIONS--
Run program with following command: 
  ./pittar <values...>

Alternatively, you can provide flags at the command line for custom initializaton. For a guide to possible flags, type:
  ./pittar -f

When the program is running, you will see some minor log output. To view a more detailed trace of the program's execution, simply open my_header.txt and change the value of DEBUG to 1.


--OTHER FILES INCLUDED--
Some files have been including for test running the program. These include:
  testfile.txt
  testfile_long.txt 



====================================================================================================
  WRITEUP
====================================================================================================

My program is designed to be minimally effective at compressing files into an archive. The archive's name is, by default, "archive.pitt" (this cannot be changed). The program includes a variety of possible command line flags for use; view them by running the program with -f. The program also is equipped with a -k flag to decompress compressed files (suffixed with .Z) present in the current working directory. This is useful for re-testing the program with the same files repeatedly.

This Pittar program also allows viewing data about the current working directory via flags -h and -y (heirarchy and meta-data info, respectively). This is useful for testing the program's correctness.

My program doesn't allow unarchiving of select files from the archive. It also doesn't allow compression of directories; only individual file names will be accepted via command line as input for the archive. Additionally, the program requires that the files be compressed before they can be added to the archive (via the -j flag).

Additionally, the program validates that a file to be added to the archive is not already present. This helps ensure that no issues with duplicate data arise.


