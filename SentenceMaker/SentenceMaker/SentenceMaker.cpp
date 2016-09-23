// SentenceMaker.cpp : Defines the entry point for the console application.
//

#include <stdio.h>		// printf
#include <conio.h>		// _getch
#include <stdlib.h>		// malloc, free

char* MakeSentence(const char** words)
{
	// validate input
	if (words == NULL)
	{
		return NULL;
	}

	//printf("%s...\n", __FUNCTION__);
	char* sentence = NULL;
	int num_words = 0, num_characters = 0;
	
	// count the alphabets & words
	int i = 0;
	while (words[i] != NULL) {
		const char* word = words[i];

		int j = 0;
		while (word[j] != NULL) {
			//printf("%c", word[j]);
			++j;
			++num_characters;
		}

		//printf(" = %s\n", words[i]);
		++i;
		++num_words;
	}
	//printf("This sentence has %d words & %d characters...\n", num_words, num_characters);
	
	// allocate memory
	size_t sentence_size = num_characters + num_words + 1;
	sentence = (char*)malloc(sentence_size);

	// check if we were given memory
	if (sentence == NULL)
	{
		return NULL;
	}

	// fill in the sentence
	int k = 0;
	i = 0;
	while (words[i] != NULL)
	{
		int j = 0;
		while (words[i][j] != NULL)
		{
			sentence[k++] = words[i][j];
			++j;
		}
		sentence[k++] = ' ';
		++i;
	}
	sentence[--k] = '.';
	sentence[++k] = '\0';
	//printf("Final sentence length:%d\n", k);

	printf("Your sentence is:%s\n", sentence);

	return sentence;
}

int main()
{
	printf("The Unoriginal Sentence Maker\n");

	const char* words[] = {
		"This",
		"might",
		"not",
		"really",
		"work",
		NULL
	};

	char* sentence = MakeSentence(words);

	_getch();

	// free any memory that may have been allocated
	if (sentence != NULL)
	{
		free(sentence);
		sentence = NULL;
	}

    return 0;
}

