#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

struct Node 
{
    struct RecordType record;
    struct Node* next;
};

// Fill out this structure
struct HashType
{
    struct Node* head;
};

// Compute the hash function
int hash(int x, int size)
{
    return x % size;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		struct Node* hashnode = pHashArray[i].head;
        if (hashnode == NULL)
        {
            continue;
        }
        printf("index %d -> ", i);
        while (hashnode != NULL)
        {
            printf(" %d,  %c,  %d -> ", hashnode->record.id, hashnode->record.name, hashnode->record.order);
            hashnode = hashnode->next;
        }
        printf("Null\n");
	}
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    int hashSz = 15;
    struct HashType* hashArray = malloc(sizeof(struct HashType) * hashSz);
    for (int i = 0; i < hashSz; ++i)
    {
        hashArray[i].head = NULL;
    }

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].order, hashSz);
        struct Node* newNode = malloc(sizeof(struct Node));
        newNode->record = pRecords[i];
        newNode->next = hashArray[index].head;
        hashArray[index].head = newNode;
    }

    displayRecordsInHash(hashArray, hashSz);


    free(pRecords);
    for (int i = 0; i < hashSz; ++i)
    {
        struct Node* freenode = hashArray[i].head;
        while (freenode != NULL)
        {
            struct Node* temp = freenode;
            freenode = freenode->next;
            free(temp);
        }
    }
    free(hashArray);

    return 0;
}