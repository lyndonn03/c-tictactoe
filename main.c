#include <stdio.h>  // io methods
#include <stdlib.h> // for randomizer
#include <stdbool.h>
#include <time.h>
#define UNITS 3
#define PLAYER_NO 2

typedef struct Player
{
    char rep;
    int turn;
    bool is_ai;
    void (*move)(char (*board)[UNITS], short *point, struct Player *player);
} Player;

void enter_player(Player players[PLAYER_NO], bool is_ai, short turn, char rep);
void print_board(char (*board)[UNITS]);
void player_move(char (*board)[UNITS], short *point, Player *player);
void ai_move(char (*board)[UNITS], short *point, Player *player);

bool validate();
bool validate_hor();
bool validate_ver();
bool validate_diag();

int main()
{
    int gameplay = -1;
    char board[][UNITS] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    char reps[] = {'x', 'o'};
    Player players[PLAYER_NO];
    short curr_turn = 0, curr_player = -1, curr_point[] = {0, 0};

    puts("Choose opponent ( 1 => AI - Player, 2 => Player - AI, (any int) => Player - Player ):");
    scanf(" %d", &gameplay);
    if(gameplay != 1 || gameplay != 2)
        gameplay = -1;

    for (int x = 0; x < PLAYER_NO; x++)
    {
        switch (gameplay)
        {
        case 1:
            enter_player(players, ((x + 2) % 2) == 0, x, reps[(x + 2) % 2]);
            break;
        case 2:
            enter_player(players, ((x + 1) % 2) == 0, x, reps[x]);
            break;
        default:
            enter_player(players, false, x, reps[x]);
            break;
        }
    }

    print_board(board);
    while (true)
    {
        curr_player = (++curr_player) % PLAYER_NO;
        players[curr_player].move(board, curr_point, &players[curr_player]);
        curr_turn++;
        if (players[curr_player].is_ai || gameplay == -1)
            print_board(board);
        if (curr_turn >= (UNITS * UNITS))
            break;
    }
    print_board(board);
    return 0;
}

void enter_player(Player player[2], bool is_ai, short turn, char rep)
{
    void *move = (is_ai) ? ai_move : player_move;
    Player new_player = {.rep = rep, .turn = turn, .move = move, .is_ai = is_ai};
    player[turn] = new_player;
}

void player_move(char (*board)[UNITS], short *point, Player *player)
{
    puts("Enter the row (1 - 3):");
    scanf("%hi", &point[0]);
    puts("Enter the column (1 - 3):");
    scanf(" %hi", &point[1]);
    board[point[0] - 1][point[1] - 1] = player->rep;
}

void ai_move(char (*board)[UNITS], short *point, Player *player)
{
    srand(time(0));
    printf("AI thinking...");
    point[0] = rand() % UNITS;
    point[1] = rand() % UNITS;
    while (board[point[0]][point[1]] != ' ')
    {
        printf(".");
        point[0] = rand() % UNITS;
        point[1] = rand() % UNITS;
    }
    board[point[0]][point[1]] = player->rep;
    puts("");
}

void print_board(char (*board)[UNITS])
{
    puts("\nLive Board:");
    puts("___________________");

    for (int x = 0; x < UNITS; x++)
    {
        char rep_col[UNITS];
        for (int j = 0; j < UNITS; j++)
            rep_col[j] = board[x][j];
        puts("|     |     |     |");
        printf("|  %c  |  %c  |  %c  |\n", rep_col[0], rep_col[1], rep_col[2]);
        puts("|_____|_____|_____|");
    }

    puts("\n");
}