#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int i, a = 0;
int maxY, maxX;
float fx = 7.0, fy = 5.0;
int x = 7, y = 5;
int ax = 5, ay = 3;
int snX[100], snY[100];
int x0, y0;
int speed = 1;
BOOL runW = FALSE;
BOOL runS = FALSE;
BOOL runA = FALSE;
BOOL runD = FALSE;
BOOL tail = FALSE;

void setcur(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void initialization(int maxX, int maxY, char **mas)
{
    for (int i = 0; i < maxY; i++)
    {
       for (int j = 0; j < maxX; j++)
       {
            if (i == 0 || j == 0 || i == (maxY - 1) || j == (maxX - 1))
                mas[i][j] = '#';

            else mas[i][j] = ' ';
       }
        mas[i][maxX] = '\0';
    }
}

void putSnake(char **mas)
{
    for (i = a; i >= 0; i--)
    {
        if (x0 == x && y0 == y)
        {
            mas[snY[i]][snX[i]] = 'o';
            continue;
        }
        if (i == 0)
        {
            snX[i] = x;
            snY[i] = y;
        }
        else
        {
            snX[i] = snX[i-1];
            snY[i] = snY[i-1];
        }
        mas[snY[i]][snX[i]] = 'o';

        if (snY[0] == snY[i] && snX[0] == snX[i] && i > 1) tail = TRUE;
    }
    mas[snY[0]][snX[0]] = 'O';
}

void autoMove()
{
    if (GetKeyState('W') < 0 && runS != TRUE)
    {
        runW = TRUE;
        runA = runD = FALSE;
    }
    if (GetKeyState('S') < 0  && runW != TRUE)
    {
        runS = TRUE;
        runA = runD = FALSE;
    }
    if (GetKeyState('A') < 0  && runD != TRUE)
    {
        runA = TRUE;
        runS = runW = FALSE;
    }
    if (GetKeyState('D') < 0  && runA != TRUE)
    {
        runD = TRUE;
        runS = runW = FALSE;
    }

    if (runW)  fy-= 0.003 * speed;
    if (runS)  fy+= 0.003 * speed;
    if (runA)  fx-= 0.003 * speed;
    if (runD)  fx+= 0.003 * speed;

    y = (int)round(fy);
    x = (int)round(fx);
}

void walls()
{
    if (x == 0)
    {
    x = maxX - 2;
    fx = (float)x;
    }
    if (x == maxX - 1)
    {
        x = 1;
        fx = (float)x;
    }
    if (y == 0)
    {
        y = maxY - 2;
        fy = (float)y;
    }
    if (y == maxY - 1)
    {
        y = 1;
        fy = (float)y;
    }
}

int main()
{
    srand(time(NULL));

    printf("\n\tSNAKE\n\nPress ENTER to start game!");
    getch();
    system("cls");

    do
    {
        printf("\nSpecify the speed of the snake (1-9):");
        scanf("%d", &speed);
        system("cls");

        printf("\nEnter the dimensions of the field\nWidth(15-100):");
        scanf("%d", &maxX);
        printf("Height(15-25):");
        scanf("%d", &maxY);

        if (maxX < 15 || maxX > 100 || maxY < 15 || maxY > 25)
        {
            maxX = 20;
            maxY = 10;
        }

        if (speed > 9 || speed < 1)
            speed = 5;

        speed *= maxY/10;

        char **mas;
        mas = malloc(sizeof(*mas) * (maxY));

        for (int i = 0; i < maxY; i++)
            mas[i] = malloc(sizeof(**mas) * (maxX +1));

        system("cls");

        do
        {
            setcur(0,0);

            initialization(maxX, maxY, mas);

            mas[ay][ax] = '*';

            putSnake(mas);

            for ( i = 0; i < maxY; i++)
                printf("%s\n", mas[i]);

            if (tail)
            {
                system("cls");
                printf("\t\ngame over\n");
                break;
            }

            if (a < maxX - speed)
                printf("Score:%d", a);
            else
            {
            system("cls");
            printf("\n\tYOU WIN!\n");
            break;
            }

            x0 = x;
            y0 = y;

            autoMove();

            walls();

            if (x == ax && y == ay)
            {
                ax = rand() * 1.0 / RAND_MAX *(maxX - 2) + 1;
                ay = rand() * 1.0 / RAND_MAX *(maxY - 2) + 1;
                a++;
            }
        }
        while(GetKeyState(VK_ESCAPE) >= 0);

        system("pause");
        system("cls");

        FILE *f;
        int scoreCount = 5;
        char score[scoreCount];

        f = fopen("Score.txt", "r");
        fgets(score, scoreCount, f);
        printf("Best result:");

        for (int i = 0; i < scoreCount; i++)
            printf("%c", score[i]);

        fclose(f);

        if(a > atoi(score))
        {
            f = fopen("Score.txt", "w");
            fprintf(f, "%d", a);
            fclose(f);
        }
        printf("\nYour result:%d\n", a);

        int snX[100], snY[100];
        a = 0; i = 0;
        x = 7; y = 5;
        tail = FALSE;
        system("pause");
        system("cls");

        for(int i = 0; i < maxY; i++)
            free(mas[i]);
        free(mas);
    }
    while(GetKeyState(VK_ESCAPE) >= 0);
}
