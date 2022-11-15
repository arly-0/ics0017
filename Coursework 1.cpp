#include <iostream>
#include "DateTime.h"
#include "Items.h"
#include "Headers.h"
#include "ICS0017DataSource.h"
#include <string>
#include "Coursework2.h"

int main()
{
	//// Task 1
	//HEADER_D* DS = GetStruct4(1, 30);
	//PrintDataStructure(DS);
	//printf("\n");
	//// TASK 2
	//DS = InsertItem(DS, (char*)"Z A");
	//DS = InsertItem(DS, (char*)"Z Z");
	//DS = InsertItem(DS, (char*)"Z K");
	//DS = InsertItem(DS, (char*)"A Z");
	//DS = InsertItem(DS, (char*)"A A");
	//DS = InsertItem(DS, (char*)"A K");
	//DS = InsertItem(DS, (char*)"G Z");
	//DS = InsertItem(DS, (char*)"G A");
	//DS = InsertItem(DS, (char*)"G K");
	//DS = InsertItem(DS, (char*)"M A");
	//DS = InsertItem(DS, (char*)"M Ba");
	//DS = InsertItem(DS, (char*)"M Bb");
	//DS = InsertItem(DS, (char*)"M Z");
	//DS = InsertItem(DS, (char*)"M Ba");
	//DS = InsertItem(DS, (char*)"Mba");
	//printf("\n");
	//PrintDataStructure(DS);
	//printf("\n");
	//// TASK 3
	//DS = RemoveItem(DS, (char*)"Z A");
	//DS = RemoveItem(DS, (char*)"Z Z");
	//DS = RemoveItem(DS, (char*)"Z K");
	//DS = RemoveItem(DS, (char*)"A Z");
	//DS = RemoveItem(DS, (char*)"A A");
	//DS = RemoveItem(DS, (char*)"A K");
	//DS = RemoveItem(DS, (char*)"G Z");
	//DS = RemoveItem(DS, (char*)"G A");
	//DS = RemoveItem(DS, (char*)"G K");
	//DS = RemoveItem(DS, (char*)"M A");
	//DS = RemoveItem(DS, (char*)"M Ba");
	//DS = RemoveItem(DS, (char*)"M Bb");
	//DS = RemoveItem(DS, (char*)"M Z");
	//DS = RemoveItem(DS, (char*)"M Ba");
	//DS = RemoveItem(DS, (char*)"Mba");
	//printf("\n");
	//PrintDataStructure(DS);

	// 1 - Creates empty data structure
	DataStructure testDS;

	//2 - Inserts 10 items into data structure(in a simple loop that calls 10 times operator += )
	for (char i = 0; i < 10; i++)
	{
		testDS += (ITEM1*)GetItem(1, nullptr);
	}
	std::cout << std::endl;

	// 3 - Prints the resulting data structure
	std::cout << testDS;
	std::cout << std::endl;

	// 4 - Prints the number of elements indata structure (using GetItemsNumber)
	std::cout << std::endl << "Number of items in the dat structure object: " << testDS.GetItemsNumber() << std::endl;
	std::cout << std::endl;

	// 5 - Retrieves item with ID Light Cyan
	ITEM1* retrievedItem = testDS.GetItem((char*)"Light Cyan");
	std::cout << std::endl;

	// 6 - Tries to retrieve non-existing element X X
	retrievedItem = testDS.GetItem((char*)"X X");
	std::cout << std::endl;

	// 7 - Using the copy constructor creates the copy of current structure
	DataStructure duplicateTestDataStructure{ testDS };
	std::cout << std::endl;

	// 8 - Removes items Banana Mania, Persian Green and Vegas Gold from the initial structure
	testDS -= (char*)"Banana Mania";
	std::cout << std::endl;
	testDS -= (char*)"Persian Green";
	std::cout << std::endl;
	testDS -= (char*)"Vegas Gold";
	std::cout << std::endl;


	// 9 - Compares the initial structure (now with 7 items) with the copy structure
	std::cout << std::endl << "Result of comparison between modified original and duplicated structures: " << (testDS == duplicateTestDataStructure);
	std::cout << std::endl;

	// 10 - Writes the initial structure (now 7 items) into a data file
	// path example 	char folderPath[] = "C:\\Users\\Your_User\\Downloads\\random_files\\file";
	testDS.Write((char*)"C:\\Users\\Artur\\Desktop\\DS.txt");
	std::cout << std::endl;

	// 11 - Creates a new structure from thisdata file and compares it with initial stucture.
	DataStructure readInTestDS{ (char*)"C:\\Users\\Core\\Downloads\\file" };
	std::cout << std::endl << "Result of comparison between original and read in structures: " << (testDS == readInTestDS);
	std::cout << std::endl;

	// 12 - Assigns to the structure just created (7 items) the copy created in step 7 (10 items) and printsthe result
	std::cout << std::endl << std::endl;
	readInTestDS = duplicateTestDataStructure;
	std::cout << std::endl << std::endl;
	std::cout << readInTestDS;

}