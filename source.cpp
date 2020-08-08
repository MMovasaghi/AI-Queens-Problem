#include <iostream>
#include <ctime>
#include <chrono>
#include <cmath>
#include <stdlib.h> 
using namespace std;

#define GENERATION_NUMBER 5000
#define QUEENS_NUMBER 8

class queen
{
    public:
        int x;
        int y;
        queen()
        {
            x = -1;
            y = -1;
        }     
};
class genom
{
    public:
        queen queens[8];
        int checkeds;
        genom()
        {
            checkeds = 0;
        }
};

int compare (const void * a, const void * b) // compare function for QuickSort
{
    return ( *(int*)a - *(int*)b );
}

genom generations[GENERATION_NUMBER];
genom gens[QUEENS_NUMBER][GENERATION_NUMBER];
int gensLength[QUEENS_NUMBER + 1] = {0};
int maxProfit = 0;

int numbers[QUEENS_NUMBER] = {0};
int ptrNumbers = 0;
int totalNumber = 0;

int mutationRate = 5;

int generation = 0;

int crossOverCallNumber = 0;

void printResult(queen queens[])
{
    cout << "--------------------------------" << endl;
    cout << "Result #" << generation << endl;
    for (int j = 0; j < QUEENS_NUMBER; j++)
    {
        cout << "Q[" << j << "] : [ " << queens[j].x << " , " << queens[j].y << " ]" << endl;
    }
}

genom mutation()
{
    int x[8] = {0};
    int y[8] = {0};
    genom newGenom;
    for (int j = 0; j < QUEENS_NUMBER; j++)
    {
        bool checked = true;
        int newX;
        while (checked)
        {
            newX = rand() % QUEENS_NUMBER;
            int k;
            for (k = 0; k < j; k++)
            {
                if (x[k] == newX)
                {
                    break;
                }                    
            }
            if(k == j)
            {
                checked = false;
                x[j] = newX;
            }                           
        }
        newGenom.queens[j].x = newX;

        checked = true;
        int newY;
        while (checked)
        {
            newY = rand() % QUEENS_NUMBER;
            int k;
            for (k = 0; k < j; k++)
            {
                if (y[k] == newY)
                {
                    break;
                }                    
            }
            if(k == j)
            {
                checked = false;
                y[j] = newY;
            }               
        }
        newGenom.queens[j].y = newY;
    }
    return newGenom;
}

int fitness(queen queens[])
{
    int checkedNumber = 0;
    for (int i = 0; i < QUEENS_NUMBER; i++)
    {
        int j;    
        for (j = 0; j < QUEENS_NUMBER; j++)
        {
            if (i != j)
            {
                if (queens[i].x == queens[j].x || queens[i].y == queens[j].y)
                break;
                if ((queens[i].x + queens[i].y) == (queens[j].x + queens[j].y) || abs(queens[i].x - queens[i].y) == abs(queens[j].x - queens[j].y))
                    break;
            }           
        }
        if (j == QUEENS_NUMBER)
            checkedNumber++;  
    }
    return checkedNumber;
}

bool classificationAndCheck(queen queens[])
{
    int number = fitness(queens);    
    bool check = true;
    for (int i = 0; i < QUEENS_NUMBER; i++)
    {
        if(gensLength[number] > GENERATION_NUMBER)
            gensLength[number] = 50;
        gens[number][gensLength[number]].queens[i] = queens[i];
        if (numbers[i] == number)
            check = false;        
    }    

    gensLength[number]++;

    if(check)
    {
        numbers[0] = number;
        totalNumber += number;
    }
    
    if (number > maxProfit)
    {
        maxProfit = number;
        if (maxProfit == QUEENS_NUMBER)
        {
            return true;
        }
        else
        {
            return false;
        }        
    } 
    return false;
}

void generateFirstGeneration() //generator with simple x and y checker
{
    for (long i = 0; i < GENERATION_NUMBER; i++)
    {
        genom createdGenom = mutation();
        for (int j = 0; j < QUEENS_NUMBER; j++)
        {
            generations[i].queens[j].x = createdGenom.queens[j].x;
            generations[i].queens[j].y = createdGenom.queens[j].y;
        }
        if(classificationAndCheck(generations[i].queens))
        {
            printResult(generations[i].queens);
            break;
        }          
    }    
}

//select random genom by its score normalization
genom selectRandom()
{
    double persent = 0;
    double rnd = rand() % 100;
    int num = 0;
    persent = 0;
    qsort (numbers, QUEENS_NUMBER, sizeof(int), compare);
    for (int j = QUEENS_NUMBER - 1; j >= 0; j--)
    {
        if(gensLength[j] != 0)
        {
            persent += floor((numbers[j]/(double)totalNumber)*100) + 1;
            if (rnd < persent)
            {
                num = rand() % gensLength[j];
                return gens[j][num];
            }
        }
    }
    num = rand() % gensLength[numbers[7]];
    return gens[numbers[7]][num];
}

genom crossOver(genom g1, genom g2)
{
    crossOverCallNumber++;
    genom newGenom;
    if(crossOverCallNumber % 100 < mutationRate)
    {
        genom mutatedGenom = mutation();
        for (int i = 0; i < QUEENS_NUMBER; i++)
        {
            g2.queens[i].x = mutatedGenom.queens[i].x;
            g2.queens[i].y = mutatedGenom.queens[i].y;
        }        
    }
    int randX = rand() % QUEENS_NUMBER;
    int randY = rand() % QUEENS_NUMBER;
    for (int i = 0; i < QUEENS_NUMBER; i++)
    {
        if(g1.queens[i].x < randX)
        {
            newGenom.queens[i].x = g2.queens[i].x;
        }
        else
        {
            newGenom.queens[i].x = g1.queens[i].x;
        }
        if(g1.queens[i].y < randY)
        {
            newGenom.queens[i].y = g2.queens[i].y;
        }
        else
        {
            newGenom.queens[i].y = g1.queens[i].y;
        }
    }
    return newGenom;  
}


int main()
{
    auto start = chrono::steady_clock::now();

    srand((unsigned) time(0));

    generateFirstGeneration();

    while (true)
    {
        generation++;

        genom newG = selectRandom();
        bool res = classificationAndCheck(newG.queens);

        if(res)
        {
            cout << "DONE";
            printResult(newG.queens);
            break;
        }
        else if(generation % 1000000 == 0)
        {
            cout << "G[" << generation << "] : " << fitness(newG.queens) << endl;
        }
    }
    

    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Time: " << elapsed.count() << " microseconds" << endl;
    return 0;
}