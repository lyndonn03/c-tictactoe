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
    void (*move)(char (*board)[UNITS], short *point, struct Player *player);
} Player;

void enter_player(Player players[PLAYER_NO], bool is_ai, short turn);
void print_board(char (*board)[UNITS]);
void player_move(char (*board)[UNITS], short *point, Player *player);
void ai_move(char (*board)[UNITS], short *point, Player *player);

bool validate();
bool validate_hor();
bool validate_ver();
bool validate_diag();

int main()
{

    char board[][UNITS] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    Player players[PLAYER_NO];
    short curr_turn = 0, curr_player = -1;
    short curr_point[] = {0, 0};

    for (int x = 0; x < PLAYER_NO; x++)
        enter_player(players, ((x+1) % 2) == 0, x); // ai the 2nd player automatically for now
    print_board(board);

    while (true)
    {
        curr_player = (++curr_player) % PLAYER_NO;
        players[curr_player].move(board, curr_point, &players[curr_player]);
        curr_turn++;
        if ((curr_player % PLAYER_NO) != 0)
            print_board(board);
        if(curr_turn >= (UNITS*UNITS))
            break;
    }
    print_board(board);
    return 0;
}

void enter_player(Player player[2], bool is_ai, short turn)
{
    char rep = (is_ai) ? 'o' : 'x';
    void *move = (is_ai) ? ai_move : player_move;
    Player new_player = {.rep = rep, .turn = turn, .move = move};
    player[turn] = new_player;
}

void player_move(char (*board)[UNITS], short *point, Player *player)
{
    puts("Enter the row (1 - 3):");
    scanf("%hi", &point[0]);
    puts("Enter the column (1 - 3):");
    scanf(" %hi", &point[1]);
    board[point[0] - 1][point[1] - 1] = 'x';
}

void ai_move(char (*board)[UNITS], short *point, Player *player)
{
    char rep = 'o';
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
    board[point[0]][point[1]] = rep;
}

void print_board(char (*board)[UNITS])
{
    puts("\nLive Board:");
    puts("___________________");

    for (int x = 0; x < UNITS; x++)
    {
        char rep_col[UNITS];
        for (int j = 0; j < UNITS; j++)
        {
            char in = board[x][j];
            rep_col[j] = in;
        }
        puts("|     |     |     |");
        printf("|  %c  |  %c  |  %c  |\n", rep_col[0], rep_col[1], rep_col[2]);
        puts("|_____|_____|_____|");
    }

    puts("\n");
}