#include <iostream>
#include <ctime>
#include <chrono>
#include <cmath>
#include <stdlib.h>
#include<cstdlib>
#include<algorithm>
#include<sstream>
#include<cstring>
#include<map>
#include<list>
#include<queue>
using namespace std;

#define ASCII_NUMBER_SHIFT 48

typedef struct
{
    string arrangement;
    int cost;
} chromosome;

typedef vector<chromosome*> genom_type;
genom_type population;
long generation;
int initialPopSize;
int boardSize;
int printStep;
int mutationRate;

int fitnessValue(string arrangement)
{
    int fitness = (boardSize * (boardSize - 1) )/2;
    for(int i = 0; i < boardSize; i++)
        for(int j = i + 1; j < boardSize; j++)
            if((arrangement[i] == arrangement[j]) || (i-arrangement[i] == j-arrangement[j]) || (i+arrangement[i] == j+arrangement[j]))
                fitness--;
    return fitness;
}

chromosome* createGenom()
{
    chromosome *newGenom  = new chromosome;
    return newGenom;
}

void generateFirstGeneration()
{
    string tmpArrengment = "";
    chromosome *temp;
    for(int i=1; i <= boardSize; i++)
    {
        tmpArrengment += i + ASCII_NUMBER_SHIFT;
    }

    // add to population
    for(int i=0; i < initialPopSize; i++)
    {
        random_shuffle( tmpArrengment.begin(), tmpArrengment.end());
        temp   = createGenom();
        temp->arrangement = tmpArrengment;
        temp->cost = fitnessValue(tmpArrengment);
        population.push_back(temp);
    }
}

chromosome* reproduce(chromosome *x, chromosome *y)
{
    chromosome *child = createGenom();
    int rnd = rand() % boardSize;
    // crossOver
    child->arrangement = (x->arrangement).substr(0, rnd) + (y->arrangement).substr(rnd, boardSize - rnd + 1);
    child->cost = fitnessValue(child->arrangement);
    return child;
}

chromosome* mutate(chromosome *child)
{
    int randomR = (rand() % (boardSize)) + 1;
    int randomC = (rand() % (boardSize)) + 1;
    child->arrangement[randomR] = randomC + ASCII_NUMBER_SHIFT;
    return child;
}

int randomSelection()
{
    int randomC = rand() % population.size() % 2;
    return randomC;
}

bool checkFit(chromosome *test)
{
    if(fitnessValue(test->arrangement)==((boardSize*(boardSize-1))/2))
        return true;
    return false;
}

bool comp(chromosome *a, chromosome*b)
{
    return(a->cost > b->cost);
}

chromosome* Generator()
{
    int randX, randY;
    chromosome *childX, *childY, *child;
    bool found =0;
    while(!found)
    {
        genom_type new_population;
        for(unsigned int i = 0; i < population.size(); i++)
        {
            generation++;
            sort(population.begin(), population.end(), comp);

            randX = randomSelection();
            childX = population[randX];

            randY =randomSelection();
            childY = population[randY];

            child = reproduce(childX, childY);

            if(rand() % mutationRate == 0)
                child = mutate(child);

            if(checkFit(child))
            {
                found = 1;
                return child;
            }
            if (generation % 100 == 0)
            {
                cout << "G[" << generation << "] : " << child->arrangement << endl;
            }
            
            new_population.push_back(child);
        }
        population = new_population;
    }
    return child;
}

void initialize(int chessboardSize, int firstPopSize, int mutatRate, int showStep)
{
    srand(time(0));
    generation = 0;
    initialPopSize = firstPopSize;
    boardSize = chessboardSize;
    mutationRate = mutatRate;
    printStep = showStep;
}

int main()
{
    auto start = chrono::steady_clock::now();
    srand((unsigned) time(0));

    initialize(8, 30, 2, 100);
    generateFirstGeneration();
    chromosome *solution = Generator();
    string IsFit = checkFit(solution) == 1 ? "TRUE" : "FALSE";
    cout << "\n===============================\n";
    cout << "Result:\n" 
        << ">>> IsFit: " << IsFit << endl
        << ">>> Arrangment: " << solution->arrangement << endl;

    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << ">>> ExecutionTime: " << elapsed.count() << " us";
    cout << "\n===============================\n";
    return 0;
}