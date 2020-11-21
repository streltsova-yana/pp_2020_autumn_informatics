// Copyright 2020 Kolesin Andrey
#ifndef MODULES_TASK_1_KOLESIN_A_COUNT_WORDS_COUNT_WORDS_H_
#define MODULES_TASK_1_KOLESIN_A_COUNT_WORDS_COUNT_WORDS_H_

#include <string>

std::string getRandomString(int  words);
int shareStrings(int rank, int size, std::string str, char** buff_out);
int count(const char* str);
int countWords(std::string str);

#endif  // MODULES_TASK_1_KOLESIN_A_COUNT_WORDS_COUNT_WORDS_H_
