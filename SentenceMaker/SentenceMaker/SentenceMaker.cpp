// SentenceMaker.cpp : Defines the entry point for the console application.
//

#include <stdio.h>		// printf
#include <conio.h>		// _getch
#include <stdlib.h>		// malloc, free

void CountCharactersAndWords(const char** words, int& num_characters, int& num_words)
{
	if (words == NULL)
	{
		return;
	}

	while (*(words + num_words) != NULL)
	{
		int char_in_word = 0;
		while (*(*(words + num_words) + char_in_word) != NULL)
		{
			++char_in_word;
			++num_characters;
		}
		++num_words;
	}
}

char* MakeSentence(const char** words)
{
	// validate input
	if (words == NULL)
	{
		return NULL;
	}

	char* sentence = NULL;
	
	// count the characters & words
	int num_words = 0, num_characters = 0;
	CountCharactersAndWords(words, num_characters, num_words);
	
	// allocate memory
	size_t sentence_size = num_characters + num_words + 1;
	sentence = (char*)malloc(sentence_size);

	// check if we were given memory
	if (sentence == NULL)
	{
		return NULL;
	}

	// fill in the sentence
	int i = 0, k = 0;
	while (*(words + i) != NULL)
	{
		int j = 0;
		while (*(*(words + i) + j) != NULL)
		{
			*(sentence + k++) = *(*(words + i) + j);
			++j;
		}
		*(sentence + k++) = ' ';
		++i;
	}
	*(sentence + --k) = '.';
	*(sentence + ++k) = '\0';

	return sentence;
}

int main()
{
	printf("The Unoriginal Sentence Maker\n");

	const char* words[] = {
		"May",
		"the",
		"force",
		"be",
		"with",
		"you",
		NULL
	};

	char* sentence = MakeSentence(words);

	printf("Your sentence is:%s\n", sentence);

	_getch();

	// free any memory that may have been allocated
	if (sentence != NULL)
	{
		free(sentence);
		sentence = NULL;
	}

    return 0;
}

