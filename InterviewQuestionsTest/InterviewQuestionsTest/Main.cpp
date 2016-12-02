// library includes
#include <stdio.h>
#include <conio.h>
#include <stdint.h>
#include <crtdbg.h>

struct Node
{
	Node* next;
	int val;
} typedef Node;

void PrintLinkedList(Node* head)
{
	Node* curr = head;
	while (curr != nullptr)
	{
		printf("%d ", curr->val);
		curr = curr->next;
	}
}

void PrintLinkedList(Node* head, const size_t num_nodes)
{
	Node* curr = head;
	for (uint32_t i = 0; curr && i < num_nodes; ++i)
	{
		printf("%d ", curr->val);
		curr = curr->next;
	}
}

/*
	Reverse a linked list in place.
*/
Node* ReverseLinkedList(Node* head)
{
	if (!head)
	{
		return head;
	}

	Node* curr = head;
	Node* prev = nullptr;

	while (curr)
	{
		Node* next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}

	return prev;
}

void TestReverseLinkedList()
{
	printf("-------------------- %s started --------------------\n", __FUNCTION__);

	// create linked list
	Node* head = new Node;
	head->next = nullptr;
	head->val = 0;

	const size_t num_nodes = 4;
	for (uint8_t i = 0; i < num_nodes; ++i)
	{
		Node* node = new Node;
		node->next = head;
		node->val = i + 1;
		head = node;
	}

	// print newly created linked list
	printf("Linked list before reverse: ");
	PrintLinkedList(head);
	printf("\n");

	// reverse linked list
	head = ReverseLinkedList(head);

	// print reversed linked list
	printf("Linked list after reverse: ");
	PrintLinkedList(head);
	printf("\n");

	while (head)
	{
		Node* next = head->next;
		delete head;
		head = next;
	}
	head = nullptr;

	printf("-------------------- %s ended --------------------\n", __FUNCTION__);
}

/*
	Detect if a linked list is circular.
*/
bool IsCircularLinkedList(Node* head)
{
	if (!head)
	{
		return false;
	}

	Node* slow = head;
	Node* fast = head;

	do
	{
		if (!fast->next)						return false;

		if (!fast->next->next)					return false;
		fast = (fast->next)->next;

		if (!slow->next)						return false;
		slow = slow->next;

		if (slow == fast)						return true;

	} while (true);

	return false;
}

void TestIsCircularLinkedList()
{
	printf("-------------------- %s started --------------------\n", __FUNCTION__);

	Node* head = new Node;
	head->next = nullptr;
	head->val = 0;
	Node* first = head;
	
	const size_t num_nodes = 4;
	for (uint8_t i = 0; i < num_nodes; ++i)
	{
		Node* node = new Node;
		node->next = head;
		node->val = i + 1;
		head = node;
	}
	first->next = head;

	// print newly created linked list
	printf("Linked list: ");
	PrintLinkedList(head, 1 + num_nodes * 2);
	printf("\n");

	bool is_circular = IsCircularLinkedList(head);
	printf("%s circular!\n", is_circular ? "Is" : "Is not");

	for (uint8_t i = 0; i < num_nodes + 1; ++i)
	{
		Node* next = head->next;
		delete head;
		head = next;
	}
	head = first = nullptr;

	printf("-------------------- %s ended --------------------\n", __FUNCTION__);
}

/*
	Reverse the words in a string in place.
*/
void ReverseWords(char* sentence)
{
	if (!sentence)
	{
		return;
	}

	uint16_t front = 0, back = 0;
	while (true)
	{
		if (*(sentence + back) == ' ' || *(sentence + back) == '\0')
		{
			uint16_t i = front, j = back;
			while (i < j)
			{
				char swap = *(sentence + i);
				*(sentence + i) = *(sentence + j - 1);
				*(sentence + j - 1) = swap;
				++i;
				--j;
			}

			if (*(sentence + back) == '\0')
			{
				break;
			}
			else
			{
				front = ++back;
			}
		}
		else
		{
			++back;
		}
	}
}

void TestReverseWords()
{
	printf("-------------------- %s started --------------------\n", __FUNCTION__);

	char sentence[20] = { 't', 'w', 'i', 'n', ' ', 'c', 'y', 'l', 'i', 'n', 'd', 'e', 'r', 's', '\0' };
	printf("Sentence:%s\n", sentence);

	ReverseWords(sentence);

	printf("Reversed sentence:%s\n", sentence);

	printf("-------------------- %s ended --------------------\n", __FUNCTION__);
}

/*
	Implement the CRT strcmp(): int strcmp( const char * i_lhs, const char * i_rsh).
*/
int StringCompare(const char* s1, const char* s2)
{
	if (s1 && !s2)							return 1;
	if (!s1 && s2)							return -1;
	if (!s1 && !s2)							return 0;

	uint16_t i = 0;
	int return_value = 0;

	// loop until characters are equal
	while (*(s1 + i) && *(s2 + i))
	{
		if (*(s1 + i) == *(s2 + i))
		{
			++i;
			continue;
		}

		return_value = (*(s1 + i) > *(s2 + i)) ? 1 : -1;
		++i;
		break;
	}

	// loop until end of string
	while (*(s1 + i) && *(s2 + i))
	{
		++i;
	}

	if (*(s1 + i) && !*(s2 + i))			return 1;
	else if (*(s2 + i) && !*(s1 + i))		return -1;

	return return_value;
}

void TestStringCompare()
{
	printf("-------------------- %s started --------------------\n", __FUNCTION__);

	const char* s1 = "baa";
	const char* s2 = "bbb";

	printf("StringCompare(%s, %s) = %d\n", s1, s2, StringCompare(s1, s2));

	printf("-------------------- %s ended --------------------\n", __FUNCTION__);
}

/*
	Reverse a string in place.
*/
void ReverseString(char* string)
{
	if (!string)
	{
		return;
	}

	uint16_t back = 0;
	while (*(string + back) != '\0')
	{
		++back;
	}

	--back;
	uint16_t front = 0;
	while (front < back)
	{
		char swap = *(string + front);
		*(string + front) = *(string + back);
		*(string + back) = swap;
		++front;
		--back;
	}
}

void TestReverseString()
{
	printf("-------------------- %s started --------------------\n", __FUNCTION__);

	char string[7] = { '1', '3', '0', '0', 'c', 'c', '\0' };
	printf("String:%s\n", string);

	ReverseString(string);

	printf("Reversed string:%s\n", string);

	printf("-------------------- %s ended --------------------\n", __FUNCTION__);
}

/*
	Remove a given character from a string in place ( "AABBAABBC" - 'A' becomes "BBBBC").
*/
void RemoveCharFromString(const char c, char* s)
{
	if (!s)
	{
		return;
	}

	uint16_t i = 0;
	while (*(s + i) != '\0')
	{
		if (*(s + i) == c)
		{
			uint16_t j = i + 1;
			while (*(s + j) != '\0')
			{
				if (*(s + j) != c)
				{
					char swap = *(s + j);
					*(s + j) = *(s + i);
					*(s + i) = swap;
					break;
				}
				++j;
			}

			if (*(s + j) == '\0')
			{
				*(s + i) = '\0';
			}
		}
		++i;
	}
}

void TestRemoveCharFromString()
{
	printf("-------------------- %s started --------------------\n", __FUNCTION__);

	char string[7] = { '1', '3', '0', '0', 'c', 'c', '\0' };
	const char remove = '\0';

	printf("String before removing %c:%s\n", remove, string);

	RemoveCharFromString(remove, string);

	printf("String after removing %c:%s\n", remove, string);

	printf("-------------------- %s ended --------------------\n", __FUNCTION__);
}

/*
	TODO:
	- Delete all the nodes in a linked list given a pointer to a function that returns true if the link data should be removed.
	- Hard: You are given a linked list where the link nodes contain not only a next pointer but an additional pointer to another random node in the list. Write a function that creates a deep copy of this list.
	- Split a sentence, given as const char *, into the individual words. Return an array of char *'s containing the words.
	- Find the maximum depth of a binary tree.
	- Implement the CRT itoa(): const char * itoa( int i_int );
	- Given an array of ints move all the 0's to the end while maintaining the order of the non 0's in the list. Do this in place.
	- Remove side-by-side duplicate characters in a string ("AAA BBB" becomes "A B") in place.
*/

int main()
{
	printf("InterviewQuestionsTest\n");

	TestReverseLinkedList();
	printf("\n\n");
	
	TestIsCircularLinkedList();
	printf("\n\n");

	TestReverseWords();
	printf("\n\n");

	TestStringCompare();
	printf("\n\n");

	TestReverseString();
	printf("\n\n");

	TestRemoveCharFromString();
	printf("\n\n");

	TestRemoveAdjacentDuplicatesFromString();
	printf("\n\n");

	_getch();

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}