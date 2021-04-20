#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <math.h>
#include <ctime>
#include "const.h"

typedef std::vector<std::string> Dict;

std::vector<std::string> getDict(Dict& dict);

unsigned int getWord1(Dict& dict);
unsigned int getWord2(Dict& dict, unsigned int& mean, unsigned int disp);
double* getDistr(int* word_answ, size_t size);
int getVocabSize(double* distr, size_t size);

template <typename T>
double* smooth(T* arr, size_t size, long long gap)
{
    double sum(0);
    double* res = new double[size];
    for(size_t i(0); i < gap; ++i)
        sum += arr[i];
    
    res[0] = sum/(double)gap;
    size_t l(0),r(gap-1);
    for(long long i(1); i < size; ++i)
    {
        if(std::max((long long)0,i-gap) != l)
        {
            sum -= arr[l];
            ++l;
        }
        if(std::min(size-1,size_t(i+gap)) != r)
        {
            ++r;
            sum += arr[r];
        }
        res[i] = sum/double(std::min(i,gap)+std::min(size-1-(size_t)i,(size_t)gap)+1);
    }

    return res;
}