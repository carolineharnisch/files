#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
}
candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int j = 0; j < candidate_count; j++) // For the number of candidates,
    {
        if (strcmp(candidates[j].name, name) == 0) // compare candidates name with the name put in by user, and if the name exists,
        {
            candidates[j].votes++; // add one vote to the candidates vote count.
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    int highestvote = 0; // Declare a highest vote variable to keep track of the highest votes.
    for (int i = 0; i < candidate_count; i++) // For the number of candidates,
    {
        if ((candidates[i].votes) > highestvote) // if the candidates vote is greater than the highest vote stored,
        {
            highestvote = candidates[i].votes; // assign the candidates vote as the newest, highest vote stored.
        }
    }
    
    for (int n = 0; n < candidate_count; n++) // For the number of candidates,
    {
        if (candidates[n].votes == highestvote) // if the number of votes of each candidate is equal to the highest vote recorded,
        {
            printf("%s\n", candidates[n].name); // print the names of candidates with highest votes.
        }
    }
    return;
}