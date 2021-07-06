// If you are not compiling with the gcc option --std=gnu99, then
// uncomment the following line or you might get a compiler warning

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct for movie information */
struct movie
{
    char *title;
    int year;
    char *language;
    double rating;
    struct movie *next;
};

/* Parse the current line which is comma delimited and create a
*  movie struct with the data in this line
*/
struct movie *createMovie(char *currLine)
{
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;

    // The first token is the Title
    // Comma is used as the delimeter that splits the columns
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the Year
    token = strtok_r(NULL, ",", &saveptr);
    // Convert a string to int.
    currMovie->year = atoi(token);

    // The next token is the Language
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->language = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->language, token);

    // The last token is the Rating
    token = strtok_r(NULL, "\n", &saveptr);
    // To convert a string to a double.
    currMovie->rating = strtod(token, NULL);

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;

    return currMovie;
}

/*
* Return a linked list of students by parsing data from
* each line of the specified file.
*/
struct movie *processFile(char *filePath)
{
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");

    char *currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    char *token;

    // The head of the linked list
    struct movie *head = NULL;
    // The tail of the linked list
    struct movie *tail = NULL;

    // First Line is the Column Titles
    nread = getline(&currLine, &len, movieFile);

    // Read the file line by line
    while ((nread = getline(&currLine, &len, movieFile)) != -1)
    {
        // Get a new movie node corresponding to the current line
        struct movie *newNode = createMovie(currLine);

        // Is this the first node in the linked list?
        if (head == NULL)
        {
            // This is the first node in the linked link
            // Set the head and the tail to this node
            head = newNode;
            tail = newNode;
        }
        else
        {
            // This is not the first node.
            // Add this node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
    }
    free(currLine);
    fclose(movieFile);
    return head;
}

/*
* Print data for the given movie
*/
void printMovie(struct movie *aMovie)
{
/* To print a variable of data type double
*  to one decimal space, use "%.1f" for this 
* variable in the format string passed to printf */
    printf("%s, %d %s, %.1f\n", aMovie->title,
           aMovie->year,
           aMovie->language,
           aMovie->rating);
}

/*
* Print the linked list of movies
*/
void printMovieList(struct movie *list)
{
    while (list != NULL)
    {
        printMovie(list);
        list = list->next;
    }
}

/*
* Prints the Movie with the highest rating per year
* Helper Function to highestRatedEachYear
*/
void HighestRatingPrintMovie(int currYear, struct movie *list)
{
    // create a temp Movie to hold data for the movie with highest rating
    struct movie *tempMovie = malloc(sizeof(struct movie));
    tempMovie->rating = 0;
    // Traverse through the list of movies 
    while (list != NULL)
    {
        // Get the year of movie in list, and compare year with current year selection (i.e. 2008)
        if (list->year == currYear)
        {
            // compare the rating for current year with the other movie of current year
            if (list->rating > tempMovie->rating)
            {
                // assign title and rating of movie with highest rating for that year to temp
                tempMovie->title = list->title;
                tempMovie->rating = list->rating;
            }
        }
        list = list->next;
    }
    printf("%d %.1f %s\n", currYear, tempMovie->rating, tempMovie->title);
    free(tempMovie);
}

/*
* Print the Titles of movies in given year
*/
void moviesInSpecificYear(struct movie *list, int yearChoice)
{
    int noMatch = 0;    // 0 signals False match for year given
    while (list != NULL)
    {
        if (list->year == yearChoice)
        {
            printf("%s\n", list->title);
            noMatch = 1;
        }
        list = list->next;
    }
    // No data match for the user's input year
    if (noMatch == 0)
    {
        printf("No data about movies released in the year %d\n", yearChoice);
    }
    printf("\n");
    printf("\n");
}

/*
* Helper Function to highestRatedEachYear
* Tracks if the years of the movies have been 
* seen when comparing the highest Ratings per year.
*/
int NeedHighestYearRating(int listYear, int *yearsCheckedList)
{
    // iterate through the indexes of the list 
    // list items at indexes are the years of the movies
    int i = 0;
    // base case [i] == -1
    while (yearsCheckedList[i] != -1)
    { 
        // If Year already compared in the Print function rating has been determined. 
        // Can pass process and move on to next year (increment i). 
        if (listYear == yearsCheckedList[i])
        {
            return 0;
        }
        i++;
    }
    return 1;
}

/*
* Helper Function to highestRatedEachYear
* Tracks if the years of the movies have been 
* seen when comparing the highest Ratings per year.
*/
void updateHaveYearRating(int currYear, int *yearsCheckedList)
{
    int i = 0;
    while (yearsCheckedList[i] != -1)
    {
        i++;
    }
    yearsCheckedList[i] = currYear;
}

/*
* Getting Movie of highest rated movie for each year
* Calls NeedHighestYearRating to check if a rating for that year is already seen.
* Calls printMovieHighRating to print highest rated of year Movie details.
* Calls updateHaveYearRating to update tracking of seen years. 
*/
void highestRatedEachYear(struct movie *list, int count)
{
    count += 1;
    // initialize a list to track the years that have been visited.
    int *yearsCheckedList = calloc(count, sizeof(int));
    // make all movie indexes -1 
    for (int i = 0; i < count; i++)
    {
        yearsCheckedList[i] -= 1;
    }
    // Traverse through the list of movies 
    while (list != NULL)
    {
        // year is a year we already have a rating for, go to next year. 
        if (NeedHighestYearRating(list->year, yearsCheckedList))
        {
            HighestRatingPrintMovie(list->year, list);
            // add current list->year to list of years seen
            updateHaveYearRating(list->year, yearsCheckedList);
        }
        list = list->next;
    }
    free(yearsCheckedList);
}

/*
* Print the Year and Title of movies in given langauge
*/
//function to show movies released in specific language
void moviesInSpecificLanguage(struct movie *list, int count, char *userInputlanguage)
{
    int match = 0;
    count += 1;
    while (list != NULL)
    {
        //tokenizing the list of languages in the column
        int toknum = 0;
        char movieLang[1000];
        strcpy(movieLang, list->language);
        const char delimiters[] = "[, ;, ]";
        char *token = strtok(movieLang, delimiters);
        while (token != NULL)
        {
            if (strcmp(token, userInputlanguage) == 0)
            {
                printf("%d %s\n", list->year, list->title);
                match = 1;
            }
            token = strtok(NULL, delimiters);
        }
        list = list->next;
    }
    //if no movie is found for that language
    if (match == 0)
    {
        printf("No data about movies released in language %s\n", userInputlanguage);
    }
    printf("\n\n");
}

/*
* Return the number of movies in list
*/
int moviesCounter(struct movie *list)
{
    int moviesCount = 0;
    while (list != NULL)
    {
        moviesCount++;
        list = list->next;
    }
    return moviesCount;
}

/*
*   Process the file provided as an argument to the program to
*   create a linked list of student structs and print out the list.
*   Compile the program as follows:
*       gcc --std=gnu99 -o movies main.c
*/
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies_sample_1.csv\n");
        return EXIT_FAILURE;
    }
    struct movie *list = processFile(argv[1]);
    //printMovieList(list); //For Testing
    int count = moviesCounter(list);

    // print the following message to stdout
    printf("Processed file %s and parsed data for %i movies\n\n", argv[1], count);
    int selection;
    int yearChoice;
    int test;
    char userInputlanguage[1000];

    int i = 1;
    while (i == 1)
    {
        printf("1. Show movies released in the specified year\n");
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n\n");
        printf("Enter a choice from 1 to 4: ");
        // user's input
        scanf("%i", &selection);

        switch (selection)
        {
        // Show movies release in the specified year
        case 1:
            printf("Enter the year for which you want to see movies: ");
            // user's year input
            scanf("%i", &yearChoice);
            moviesInSpecificYear(list, yearChoice);
            break;
        // Show highest rated movie for each year
        case 2:
            highestRatedEachYear(list, count);
            printf("\n\n");
            break;
        // Show movies and their year of release for a specific language
        case 3:
            printf("Enter the language for which you want to see movies: ");
            // user's language input
            scanf("%s", userInputlanguage);
            moviesInSpecificLanguage(list, count, userInputlanguage);
            break;
        case 4:
            return EXIT_SUCCESS;
        default:
            printf("Invalid input, please enter a number from 1 to 4.\n");
        }
    }
}
