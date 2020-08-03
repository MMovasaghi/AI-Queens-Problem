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

genom generations[GENERATION_NUMBER];
genom gens[QUEENS_NUMBER][GENERATION_NUMBER];
int gensLength[QUEENS_NUMBER + 1] = {0};
int maxProfit = 0;

int numbers[QUEENS_NUMBER] = {0};
int ptrNumbers = 0;
int totalNumber = 0;

void generator() //generator with simple x and y checker
{
    for (long i = 0; i < GENERATION_NUMBER; i++)
    {
        int x[8] = {0};
        int y[8] = {0};
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
            generations[i].queens[j].x = newX;

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
            generations[i].queens[j].y = newY;
        }        
    }
    
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

void classification(queen queens[])
{
    //totalNumber = 0
    //numbers[] = 0
    int number = fitness(queens);    
    bool check = true;
    for (int i = 0; i < QUEENS_NUMBER; i++)
    {
        gens[number][gensLength[number]].queens[i] = queens[i];
        if (numbers[i] == number)
            check = false;        
    }    

    gensLength[number]++;

    if(check)
    {
        numbers[ptrNumbers] = number;
        ptrNumbers++;
        totalNumber += number;
    }
    
    if (number > maxProfit)
    {
        maxProfit = number;
    }    
}

int compare (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}

int main()
{
    auto start = chrono::steady_clock::now();

    srand((unsigned) time(0));
    generator();

    int max = 0;
    for (int i = 0; i < GENERATION_NUMBER; i++)
    {
        classification(generations[i].queens);              
    }   
    qsort (numbers, QUEENS_NUMBER, sizeof(int), compare);
    for (int i = 0; i < QUEENS_NUMBER; i++)
    {
        cout << i << " : " << floor((numbers[i]/(double)totalNumber)*100) <<  endl;
    }
    cout << "Total : " << totalNumber << endl;
    
    double persent = 0;
    int ee[QUEENS_NUMBER] = {0};
    for (int i = 0; i < 20; i++)
    {
        double rnd = rand() % 100;
        persent = 0;
        for (int j = QUEENS_NUMBER - 1; j >= 0; j--)
        {
            //if(numbers[j] != 0)
            {
                persent += floor((numbers[j]/(double)totalNumber)*100) + 1;
                cout << rnd << "  " << persent << endl;
                if (rnd < persent)
                {
                    ee[j]++;
                    break;
                }
            }            
        }       
    }

    for (int i = 0; i < QUEENS_NUMBER; i++)
    {
        cout << i << " Number[" << numbers[i] << "] Rand " << " : " << ee[i] <<  endl;
    }

    
    
    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Time: " << elapsed.count() << " microseconds" << endl;
    return 0;
}