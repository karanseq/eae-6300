// SentenceMaker.cpp : Defines the entry point for the console application.
//

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif // _DEBUG

#include <stdio.h>		// printf
#include <conio.h>		// _getch
#include <stdlib.h>		// malloc, free

#define MAX_WORDS 128

char* MakeSentence(char** words, const int num_words, const int num_characters)
{
	// validate input
	if (words == NULL)
	{
		return NULL;
	}

	char* sentence = NULL;
	
	// allocate memory
	int sentence_size = num_characters + num_words + 1;
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

bool AcceptInput(char** words, int& num_words, int& num_characters)
{
	printf("Type a word or simply hit enter when you're done:");

	// capture input into buffer
	char input_str[256] = { 0 };
	fgets(input_str, sizeof(input_str), stdin);

	// count the characters in the input string
	int i = 0, num_characters_in_word = 0;
	while (input_str[i] != NULL)
	{
		++num_characters_in_word;
		++i;
	}

	// stop accepting input if an empty string was entered
	if (num_characters_in_word == 1)
	{
		return false;
	}

	// reserve memory for the word
	*(words + num_words) = (char*)malloc(num_characters_in_word);
	if (*(words + num_words) == NULL)
	{
		return false;
	}

	// copy all characters except the last one...because 'fgets' appends a '\n' to the result
	for (int i = 0; i < num_characters_in_word - 1; ++i)
	{
		*(*(words + num_words) + i) = input_str[i];
	}
	*(*(words + num_words) + num_characters_in_word - 1) = '\0';

	// update the total number of words and characters
	++num_words;
	num_characters += num_characters_in_word;

	return true;
}

int main()
{
	printf("The Unoriginal Sentence Maker...\nSentences having more than %d words are an abomination!\n\n", MAX_WORDS);

	int num_words = 0, num_characters = 0;
	char* words[MAX_WORDS + 1] = { 0 };
	while (num_words < MAX_WORDS && AcceptInput(words, num_words, num_characters) != false) {}

	// nullify end of words
	*(words + num_words) = NULL;

	char* sentence = NULL;
	if (num_words > 0)
	{
		sentence = MakeSentence(words, num_words, num_characters);
	}

	printf("\nYour sentence is:%s\n", sentence);

	// free any memory that may have been allocated...
	// first check if any words were allocated
	if (words != NULL && num_words > 0)
	{
		// free memory allocated for each word
		for (int i = 0; i < num_words; ++i)
		{
			free(*(words + i));
			*(words + i) = NULL;
		}
	}

	// free memory allocated for the sentence
	if (sentence != NULL)
	{
		free(sentence);
		sentence = NULL;
	}

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	printf("Press any key to exit...\n");
	_getch();

    return 0;
}

