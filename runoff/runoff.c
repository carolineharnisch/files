#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates (definition of two constants)
#define MAX_VOTERS 100 // max number of voters in election
#define MAX_CANDIDATES 9 // max number of candidates in election

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES]; // preferences[i] represents all preferences for voter number i, and the int preferences[i][j] will store the index of candidate who is jth preference for voter i (i is the voter, j is the votes for voter i)

// Candidates have name, vote count, eliminated status
typedef struct // represents data of each candidate
{
    string name; // candidate name
    int votes; // candidate votes
    bool eliminated; // indicates whether candidate has been eliminated
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES]; // keeps track of all candidates in election

// Numbers of voters and candidates (global variables)
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank (keep track of voting preferences)
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4; // program exits
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate(); // looks at all voters' preferences and computes the current vote totals, by looking at each voter's top choice candidate who hasn't yet been eliminated.

        // Check if election has been won
        bool won = print_winner(); // print out winner, if there is one
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min(); // determine fewest number of votes abyone still in the election received.
        bool tie = is_tie(min);

        // If tie, everyone in election is tied with same number of votes, election is declared tie.
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min); // Last-place candidate is eliminated from election.

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name) 
{
    for (int k = 0; k < candidate_count; k++) // and each rank, 
    {
        if (strcmp(candidates[k].name, name) == 0) // if the vote exists in candidate array,
        {
            preferences[voter][rank] = k; // set rank for chosen candidate in array for 1st, 2nd and 3rd vote,
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            int k = preferences[i][j]; // Index (int) that stores the place the candidate has in the candidate array (this was set when user input in command-line)
            if (candidates[k].eliminated == false)
            {
                candidates[k].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            int highestvote = (voter_count / 2);
            if (candidates[i].votes > highestvote)
            {
                printf("%s\n", candidates[i].name);
                return true;
            }
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int lowestvote = 9999999;
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            if (candidates[i].votes < lowestvote)
            {
                lowestvote = candidates[i].votes;
            }
        }
    }
    return lowestvote;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
   for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].votes != min && candidates[i].eliminated == false) // If all remaining candidates have the same votes, 
        {
            return false; // declare a tie.
        }
    }
    return true;
}

// Eliminate the candidate (or candidiates) in last place
void eliminate(int min)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i].eliminated == false)
        {
            if (candidates[i].votes == min)
            {
                candidates[i].eliminated = true;
            }
        }
    }
    return;
}