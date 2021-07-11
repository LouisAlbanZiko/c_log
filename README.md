## c_log

A small library for logging in the C programming language. Features include:
- different levels of logging being trace, info, warning, error and fatal.
- multiple loggers can be created.
- a custom output pattern can be created for each logger.
- colors for each level of logging.

#### Setup

###### With Premake

Clone the repository to the location of your choice.
Create a new premake workspace and include the file `c_log/premake5.lua` inside your own premake file. 
Then link your project to the c_log project using this line `links "c_log"`.
For a full setup of premake go to the premake [github page](https://github.com/premake/premake-core).

###### Without Premake

Copy the files `c_log/c_log.c`, `c_log/c_log.h` and `c_log/internal.h` into your project directory and compile your project with these files.

#### Documentation

Check the wiki page.
