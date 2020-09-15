#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX]; // This int represents the amount of voters that prefer candidate i over candidate j.

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX]; // Represents candidate graph - if true there is an edge pointing from candidate i to candidate j. If false, there is no edge.

// Each pair has a winner, loser
typedef struct // Represents a pair of candidates, where each pair includes winner's index and loser's index.
{
    int winner; // Index of winner
    int loser; // Index of loser
}
pair;

// Array of candidates
string candidates[MAX]; // Names of candidates
pair pairs[MAX * (MAX - 1) / 2]; // Represents all pairs of candidates, for which one is preferred over the other.

int pair_count; // Number of pairs.
int candidate_count; // Number of candidates.

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes // Loops over all voters and collectd their preferences in an array called ranks, where ranks[i] is the INDEX of the candidate who is the ith preference for the voter.
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks); // Votes are passed on to here - takes ranks and update preferences variable.

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    rank = candidate_count;
    for (int i = 0; i < rank; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[]) // Each voter's preferences are recorded, taken from ranks[i] array.
{
    for (int j = 0; j < candidate_count; j++) // Will record previous candidate as preference over
    {
        for (int k = j + 1; k < candidate_count; k++) // next candidate and so on.
        {
            preferences[ranks[j]][ranks[k]]++; // 1st rank preferred over 2nd rank, 2nd rank over 3rd etc. Goes through all candidates and ranks.
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i+1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i]) // Add to PAIR if one candidate is preferred to the other.
            {
                pairs[pair_count].winner = i; // Declare winner of the pair.
                pairs[pair_count].loser = j; // Declare loser of the pair.
                pair_count++; // Add one to pair_count.
            }

            if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }

            if (preferences[i][j] == preferences[j][i])
            {
                pair_count += 0; // Do not count candidates that are tied.
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int margin[pair_count]; // Set array for margin of victory for each candidate pair count.
    for (int i = 0; i < pair_count; i++)
    {
        margin[i] = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
        // Margin of victory = how many times voters preferred candidate i over j - how many times voters preferred candidate j over i.
    }

    int swapcounter = -1; // Set swap counter to non-zero value for bubble sort.
    int temp; // Set temporary integer.
    pair temp2; // Set temporary placeholder for pairs array.
    do {
        swapcounter = 0; // Set swap counter to 0.
        for (int j = 0; j < pair_count - 1; j++) // Equivalent of setting i = 0 where i < pair_count, and j = i - 1 where j < pair_count.
        {
            if (margin[j + 1] > margin[j]) // If next margin of victory is greater than previous,
            {
                temp = margin[j + 1];
                margin[j + 1] = margin[j];
                margin[j] = temp; // swap margin of victories, to sort in decreasing order.

                temp2 = pairs[j + 1];
                pairs[j + 1] = pairs[j];
                pairs[j] = temp2; // Repeat same process but this time sorting pairs array.
                swapcounter++; // Add one to swap counter.
            }
        }
    } while (swapcounter != 0); // Repeat until swap counter is equal to zero.
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    locked[pairs[0].winner][pairs[0].loser] = true; // Lock first pair, as there is no locked pairs to compare to yet.
    int a = pairs[0].winner;
    int b = pairs[0].loser;
    
    for (int i = 1; i < pair_count - 1; i++) // For the remaining pair counts,
    {
        if (pairs[i].loser > b)
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

