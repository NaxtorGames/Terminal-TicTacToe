#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

enum FieldAssignment
{
    CPU = -1,
    Clear = 0,
    Player = 1
};

enum BoardDrawMode
{
    DrawNone = 0,
    DrawRow = 1,
    DrawColumn = 2
};

enum GameCommand
{
    CommandStart = 0,
    CommandQuit = 1,
    CommandInvalid = 1
};

int _field[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

void ClearInputBuffer()
{
    // Check if end of line.
    if (feof(stdin))
    {
        return;
    }

    // Clear commandline.
    char character;
    while ((character = getchar()) != '\n' && character != EOF)
        ;

    // With block would be as followed:
    /*
    int condition;
    do
    {
        condition = ((c = getchar()) != '\n') && (c != EOF);
    }while (condition);
    */
}

int IsRowWon(int rowOne, int columnOne,
             int rowTwo, int columnTwo,
             int rowThree, int columnThree)
{
    int fieldOne = _field[rowOne][columnOne];
    if (fieldOne == Clear)
    {
        return 0;
    }
    int fieldTwo = _field[rowTwo][columnTwo];
    if (fieldTwo == Clear)
    {
        return 0;
    }
    int fieldThree = _field[rowThree][columnThree];
    if (fieldThree == Clear)
    {
        return 0;
    }

    if (fieldOne == fieldTwo && fieldTwo == fieldThree)
    {
        // Check whether the average is -1 (CPU index) or 1 (Player index) to decide how owns this row.
        // 0 is clear so at the same time no one owns this row.
        int owner = (fieldOne + fieldTwo + fieldThree) / 3;
        if (owner == CPU)
        {
            printf("CPU won!\n");
        }
        else if (owner == Player)
        {
            printf("Player won!\n");
        }
        return owner;
    }
    else
    {
        return 0;
    }
}

int IsGameDraw()
{
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            if (_field[row][column] == Clear)
            {
                return 0;
            }
        }
    }

    printf("Game draw!\n");
    return 1;
}

int IsGameWonOrDraw()
{
    return IsRowWon(0, 0, 0, 1, 0, 2) != 0 ||
           IsRowWon(1, 0, 1, 1, 1, 2) != 0 ||
           IsRowWon(2, 0, 2, 1, 2, 2) != 0 ||
           IsRowWon(0, 0, 1, 0, 2, 0) != 0 ||
           IsRowWon(0, 1, 1, 1, 2, 1) != 0 ||
           IsRowWon(0, 2, 1, 2, 2, 2) != 0 ||
           IsRowWon(0, 0, 1, 1, 2, 2) != 0 ||
           IsRowWon(2, 0, 1, 1, 0, 2) != 0 ||
           IsGameDraw();

    // 00 | 01 | 02
    // 10 | 11 | 12
    // 20 | 21 | 22
}

int Get(int row, int column)
{
    return _field[row][column];
}

int CheckValidValues(int row, int column, int playerIndex)
{
    if (row < 0 || row >= 3)
    {
        return 0;
    }
    if (column < 0 || column >= 3)
    {
        return 0;
    }

    if (playerIndex != Clear)
    {
        if (Get(row, column) != Clear)
        {
            return 0;
        }
    }

    return 1;
}

void Set(int row, int column, int playerIndex)
{
    if (CheckValidValues(row, column, playerIndex))
    {
        _field[row][column] = playerIndex;
    }
}

void ResetBoard()
{
    for (int row = 0; row < 3; row++)
    {
        for (int column = 0; column < 3; column++)
        {
            Set(row, column, Clear);
        }
    }
}

char PlayerIndexToChar(int playerIndex)
{
    switch (playerIndex)
    {
    case Player:
        return 'X';
    case CPU:
        return 'O';
    default:
        return ' ';
    }
}

void DrawSpace()
{
    printf("\n------------------\n");
}

void DrawBoard(int drawMode)
{
    printf("\n");

    switch (drawMode)
    {
    case DrawRow:
        printf("0 -> %c | %c | %c", PlayerIndexToChar(_field[0][0]), PlayerIndexToChar(_field[0][1]), PlayerIndexToChar(_field[0][2]));
        printf("\n");
        printf("    -----------");
        printf("\n");
        printf("1 -> %c | %c | %c", PlayerIndexToChar(_field[1][0]), PlayerIndexToChar(_field[1][1]), PlayerIndexToChar(_field[1][2]));
        printf("\n");
        printf("    -----------");
        printf("\n");
        printf("2 -> %c | %c | %c", PlayerIndexToChar(_field[2][0]), PlayerIndexToChar(_field[2][1]), PlayerIndexToChar(_field[2][2]));
        break;
    case DrawColumn:
        printf(" 0   1   2\n");
        printf(" v   v   v\n");
        printf(" %c | %c | %c", PlayerIndexToChar(_field[0][0]), PlayerIndexToChar(_field[0][1]), PlayerIndexToChar(_field[0][2]));
        printf("\n");
        printf("-----------");
        printf("\n");
        printf(" %c | %c | %c", PlayerIndexToChar(_field[1][0]), PlayerIndexToChar(_field[1][1]), PlayerIndexToChar(_field[1][2]));
        printf("\n");
        printf("-----------");
        printf("\n");
        printf(" %c | %c | %c", PlayerIndexToChar(_field[2][0]), PlayerIndexToChar(_field[2][1]), PlayerIndexToChar(_field[2][2]));
        break;
    default:
        printf(" %c | %c | %c", PlayerIndexToChar(_field[0][0]), PlayerIndexToChar(_field[0][1]), PlayerIndexToChar(_field[0][2]));
        printf("\n");
        printf("-----------");
        printf("\n");
        printf(" %c | %c | %c", PlayerIndexToChar(_field[1][0]), PlayerIndexToChar(_field[1][1]), PlayerIndexToChar(_field[1][2]));
        printf("\n");
        printf("-----------");
        printf("\n");
        printf(" %c | %c | %c", PlayerIndexToChar(_field[2][0]), PlayerIndexToChar(_field[2][1]), PlayerIndexToChar(_field[2][2]));
        break;
    }

    printf("\n");
}

void PlayerAction()
{
    int playerSelectedRow;
    int playerSelectedColumn;
    do
    {
        DrawBoard(DrawRow);
        printf("\nSelect a Row (0,1,2): ");
        scanf("%d", &playerSelectedRow);
        ClearInputBuffer();
        DrawBoard(DrawColumn);
        printf("\nSelect a Column (0,1,2): ");
        scanf("%d", &playerSelectedColumn);
        ClearInputBuffer();
        printf("\nPlayer selected: (%d, %d)\n", playerSelectedRow, playerSelectedColumn);
        DrawSpace();

    } while (!CheckValidValues(playerSelectedRow, playerSelectedColumn, Player));
    Set(playerSelectedRow, playerSelectedColumn, Player);
}

int GetRandomInt(int maxInclusive)
{
    const int LOWER = 0;
    const int IS_INCLUSIVE = 1;

    return (rand() % (maxInclusive - LOWER + IS_INCLUSIVE)) + LOWER;
}

void CPUAction()
{
    printf("\nCpu is selecting...\n");

    srand(time(0));
    int cpuSelectedRow;
    int cpuSelectedColumn;
    do
    {
        cpuSelectedRow = GetRandomInt(2);
        cpuSelectedColumn = GetRandomInt(2);
    } while (!CheckValidValues(cpuSelectedRow, cpuSelectedColumn, CPU));

    printf("Cpu selected: (%d,%d)\n", cpuSelectedRow, cpuSelectedColumn);
    Set(cpuSelectedRow, cpuSelectedColumn, CPU);

    DrawSpace();
}

int BeforeGameStart()
{
    printf("\nWhat next?\nType one of the following options:\n'Start','Exit'\n");

    char answer[8];
    fgets(answer, sizeof(answer), stdin);

    // Removing \n form answer to get only input.
    answer[strcspn(answer, "\n")] = 0;

    DrawSpace();

    if (strcmp("Start", answer) == 0 || strcmp("start", answer) == 0)
    {
        printf("\nStarting new Game\n");
        return CommandStart;
    }
    else if (strcmp("Exit", answer) == 0 || strcmp("exit", answer) == 0)
    {
        printf("\nQuiting Game.");
        return CommandQuit;
    }
    else
    {
        printf("\nWow you can't even follow simple instructions... Door is this way -> |_|");
        return CommandInvalid;
    }
}

void GameLoop()
{
    ResetBoard();

    do
    {
        PlayerAction();
        if (IsGameWonOrDraw())
        {
            break;
        }
        CPUAction();

    } while (!IsGameWonOrDraw());

    printf("Game end!");
    DrawSpace();
}

int main()
{
    DrawSpace();
    printf("Welcome! Lets play: TicTacToe!");
    DrawSpace();

    int startCommand;
    startCommand = BeforeGameStart();

    while (startCommand == CommandStart)
    {
        GameLoop();
        startCommand = BeforeGameStart();
    }

    return 0;
}