# movies

### This program:
Reads a CSV file with movie data given as an argument to the program. \
Processes the data in the file to create structs to hold data for each movie. \
Creates a linked list of all these structs. \
Gives the user choice questions for input about the movies in the data. \
Prints out the data about the movies per user input.

### To compile:
You need to have all files in the same folder. 
The files (in no specific order): 
- main
- main.c
- movies
- movies_data.csv (or whatever test input file you are using would sub this out.) 
You can compile and run the program by using the following commands:
- `gcc --std=gnu99 -o movies main.c`
- `./movies movies_data.csv`  
(or similiar, "movies_data.csv" can be filled in with the name of the input file)
