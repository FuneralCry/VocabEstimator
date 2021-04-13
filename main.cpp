#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <math.h>
#include <ctime>

#define DICT_FILE "words_ru_sorted.txt"
#define DISP_CONST_W 0.2
#define DISP_CONST_R 0.03
#define DISP_CONST 600
#define EPS 10e-10

typedef std::vector<std::string> Dict;

std::vector<std::string> getDict(Dict& dict)
{
    std::fstream df;
    df.open(DICT_FILE);
    if(df.is_open())
    {
        std::string word;
        while(std::getline(df,word))
            dict.push_back(word);
    }
    else
        throw 1;
    df.close();

    return dict;
}

void getWord(Dict& dict, unsigned int& mean, unsigned int disp)
{
    std::random_device rd;
    std::mt19937 gen{rd()};
    std::normal_distribution<> rnd{0,(double)disp};
    mean += std::abs(std::round(rnd(gen)));
}

double* getDistr(int* word_answ, unsigned int size)
{
    int w,r;
    double* distr = new double[size];
    for(int i(0); i < 2000; ++i)
    {
        if(word_answ[i] == 0)
            ++w;
        else if(word_answ[i] == 1)
            ++r;
    }
    distr[1000] = (double)r/double(w+r);
    for(int i(1001); i < size-1001; ++i)
    {
        if(word_answ[i+1000] == 1)
            ++r;
        else if(word_answ[i+1000] == 0)
            ++w;
        if(word_answ[i-1001] == 1)
            --r;
        else if(word_answ[i-1001] == 0)
            --w;
        distr[i] = (double)r/double(w+r+EPS);
    }
    w = r = 0;
    for(int i(0); i < 1000; ++i)
    {
        if(word_answ[i] == 0)
            ++w;
        else if(word_answ[i] == 1)
            ++r;
    }
    for(int i(0); i < 1000; ++i)
    {
        if(word_answ[i+1000] == 1)
            ++r;
        else if(word_answ[i+1000] == 0)
            ++w;
        distr[i] = (double)r/double(w+r+EPS);
    }
    w = r = 0;
    for(int i(size-1001); i < size; ++i)
    {
        if(word_answ[i-1001] == 1)
            --r;
        else if(word_answ[i-1001] == 0)
            --w;
        distr[i] = (double)r/double(w+r+EPS);
    }

    return distr;
}

int getVocabSize(double* distr, int size)
{
    double res(0);

    for(int i(0); i < size; ++i)
        res += distr[i];
    
    return (int)res / 100 * 100;
}

int main()
{
    Dict dict;
    getDict(dict);

    unsigned int wrong(0),right(0),mean(0),disp(100);
    int* word_answ = new int[dict.size()];

    for(int i(0); i < dict.size(); ++i)
        word_answ[i] = -1;
    
    while(true)
    {
        getWord(dict,mean,disp);
        if(mean >= dict.size())
            break;
        std::string word(dict[mean]),answ;
        std::cout << "Do you know word '" << word << "'?(Y/n)" << '\n';
        std::cin >> answ;
        if(answ == "N" or answ == "n")
        {
            ++wrong;
            word_answ[mean] = 0;
        }
        else
        {
            ++right;
            word_answ[mean] = 1;
        }
        disp = (double)DISP_CONST/(std::max((double)1, DISP_CONST_W*double(wrong)+1) * (std::max((double)0.5,-DISP_CONST_R*double(right)+1.5)));
        std::cout << disp << '\n';
        std::cout << mean << '\n';
    }

    std::cout << '\n' << "Your vocabulary size is: " << getVocabSize(getDistr(word_answ,dict.size()),dict.size());

    return 0;
}