#include <stdio.h>  // io methods
#include <stdlib.h> // for randomizer
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define UNITS 3
#define PLAYER_NO 2

typedef struct Player
{
    char name[25];
    char rep;
    int turn;
    bool is_ai;
    void (*move)(char (*board)[UNITS], short *point, struct Player *player);
} Player;

void setup_gameplay(char (*board)[UNITS], Player *player, int *gameplay);
void enter_player(Player players[PLAYER_NO], bool is_ai, short turn, char rep);
void print_board(char (*board)[UNITS]);
void player_move(char (*board)[UNITS], short *point, Player *player);
void ai_move(char (*board)[UNITS], short *point, Player *player);

bool validate(char (*board)[UNITS], short *point, char rep);
bool validate_hor(char (*board)[UNITS], short *point, char rep);
bool validate_ver(char (*board)[UNITS], short *point, char rep);
bool validate_diag(char (*board)[UNITS], short *point, char rep);

int main()
{
    int gameplay = -1;
    char board[UNITS][UNITS] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};
    Player players[PLAYER_NO];
    short curr_turn = 0, curr_player = -1, curr_point[] = {0, 0};

    setup_gameplay(board, players, &gameplay);
    if (gameplay != 1 && gameplay != 2)
        gameplay = -1;

    while (true)
    {
        curr_player = (++curr_player) % PLAYER_NO;
        players[curr_player].move(board, curr_point, &players[curr_player]);
        curr_turn++;
        system("clear");
        if (players[curr_player].is_ai || gameplay == -1)
            print_board(board);

        if (validate(board, curr_point, players[curr_player].rep))
        {
            printf("Player %s is the winner..", players[curr_player].name);
            break;
        }

        if (curr_turn >= (UNITS * UNITS))
            break;
    }
    print_board(board);
    return 0;
}

void setup_gameplay(char (*board)[UNITS], Player *players, int *gameplay)
{
    char reps[] = {'x', 'o'};
    printf("Choose opponent ( 1 => AI - Player, 2 => Player - AI, (any int) => Player - Player ):");
    scanf(" %d", gameplay);
    printf("%d", *gameplay);
    for (int x = 0; x < PLAYER_NO; x++)
    {
        switch (*gameplay)
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
}

void enter_player(Player player[2], bool is_ai, short turn, char rep)
{
    void *move = (is_ai) ? ai_move : player_move;
    Player new_player = {.rep = rep, .turn = turn, .move = move, .is_ai = is_ai};
    if (is_ai)
        strcpy(new_player.name, "Bot");
    else
    {
        printf("\nEnter your name: ");
        scanf(" %s", (new_player.name));
    }
    player[turn] = new_player;
}

void player_move(char (*board)[UNITS], short *point, Player *player)
{
    printf("Enter the row (1 - 3):");
    scanf("%hi", &point[0]);
    printf("Enter the column (1 - 3):");
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

bool validate(char (*board)[UNITS], short *point, char rep)
{

    bool hor_res = validate_hor(board, point, rep);
    bool ver_res = validate_ver(board, point, rep);
    bool diag_res = validate_diag(board, point, rep);

    return ver_res || hor_res || diag_res;
}

bool validate_hor(char (*board)[UNITS], short *point, char rep)
{
    int row = (int)point[0] - 1;
    bool res = false;
    for (int x = 0; x < UNITS; x++)
    {
        res = (board[row][x] == rep);
        if (!res)
            break;
    }
    return res;
}

bool validate_ver(char (*board)[UNITS], short *point, char rep)
{
    int col = (int)point[1] - 1;
    bool res = false;
    for (int x = 0; x < UNITS; x++)
    {
        res = (board[x][col] == rep);
        if (!res)
            break;
    }
    return res;
}

bool validate_diag(char (*board)[UNITS], short *point, char rep)
{
    int row = (int)point[0] - 1, col = (int)point[1] - 1;
    int com = abs(row - col);
    bool res = false;
    if (row == col)
        for (int x = 0; x < UNITS; x++)
        {
            res = board[x][x] == rep;
            if (!res)
                break;
        }

    if (!res && (row == col || com == 2))
        for (int x = 0; x < UNITS; x++)
        {
            res = board[x][com - x] == rep;
            if (!res)
                break;
        }
    return res;
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