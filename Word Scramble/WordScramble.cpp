#include"TemplateSort.h"
#include<iostream>
#include<fstream>
#include<string>
#include<Windows.h>
#include<conio.h>
#include<time.h>
using namespace std;

#define ROWS 34
#define COLS 130

void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_ECHO_INPUT | ENABLE_INSERT_MODE | ENABLE_LINE_INPUT | ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}

void LoadDictionary(TemplateSort<string>& dictArr, int dictSize)
{
	ifstream Rdr("dictionary.txt");
	for (int i = 0; i < dictSize; i++)
		Rdr >> dictArr.returnTemp()[i];
}

void printSpaces(int sr, int sc, int dr, int dc)
{
	for (int i = sc; i < dc; i++)
	{
		gotoRowCol(sr, i);
		cout << ' ';
	}
}

int STRLEN(string str)
{
	int count = 0, i = 0;
	while (str[i] != '\0')
	{
		count++;
		i++;
	}
	return count;
}

TemplateSort<string> alphaGen(int DIFFLEV)
{
	srand(time(0));
	TemplateSort<string> R(1);
	for (int i = 0; i < DIFFLEV; i++)
		R.returnTemp()[0] = R.returnTemp()[0] + char(rand() % 26 + 97);

	bool check = false;
	for (int i = 0; i < DIFFLEV; i++)
	{
		if (R.returnTemp()[0][i] == 'a' || R.returnTemp()[0][i] == 'e' || R.returnTemp()[0][i] == 'i' ||
			R.returnTemp()[0][i] == 'o' || R.returnTemp()[0][i] == 'u')
		{
			check = true;
			break;
		}
	}
	if (!check)
		R.returnTemp()[0][DIFFLEV - 1] = 'e';
	return R;
}

void Merger(TemplateSort<string>& arr, int si1, int ei1, int si2, int ei2)
{
	bool c;
	int tempsize = ei2 - si1 + 1;
	TemplateSort<string> temp(tempsize);
	int i = 0, j = si1, k = si2;
	for (; j <= ei1 && k <= ei2; i++)
	{
		if (arr[k] < arr[j])
		{
			temp.returnTemp()[i] = arr.returnTemp()[k];
			k++;
			c = false;
		}
		else
		{
			temp.returnTemp()[i] = arr.returnTemp()[j];
			j++;
			c = true;
		}
	}

	if (c)
	{
		for (; k <= ei2; i++, k++)
			temp.returnTemp()[i] = arr.returnTemp()[k];
	}
	else
		for (; j <= ei1; i++, j++)
			temp.returnTemp()[i] = arr.returnTemp()[j];

	for (int i = 0, l = si1; i < (tempsize); i++, l++)
		arr.returnTemp()[l] = temp.returnTemp()[i];
}

void MergeSort(TemplateSort<string>& arr, int size, int si, int ei)
{
	if (si == ei)
		return;
	int mid = (si + ei) / 2;
	MergeSort(arr, size, si, mid);
	MergeSort(arr, size, mid + 1, ei);
	Merger(arr, si, mid, mid + 1, ei);
}

TemplateSort<string> return_uniqueStrings(TemplateSort<string> S, int& uniSize)
{
	TemplateSort<string> str(1, S[0]);
	uniSize++;
	for (int j = 1; j < S.returnSize(); j++)
	{
		int count = 0;
		for (int k = 0 ; k < str.returnSize(); k++)
		{
			if (S[j] == str[k])
				count++;
		}
		if (count == 0)
		{
			TemplateSort<string> HA(uniSize + 1);
			for (int i = 0; i < uniSize; i++)
				HA.returnTemp()[i] = str.returnTemp()[i];
			HA.returnTemp()[uniSize] = S[j];
			uniSize++;
			delete[] str.returnTemp();
			str = HA;
		}
	}
	return str;
}

TemplateSort<string> find_intersection(TemplateSort<string> S1, TemplateSort<string> S2, 
	int& intSize, TemplateSort<string>& orgWord)
{
	TemplateSort<string> str;
	for (int i = 0; i < S1.returnSize(); i++)
	{
		for (int j = 0; j < S2.returnSize(); j++)
		{
			if (S1.returnTemp()[i] == S2.returnTemp()[j])
			{
				if (!(S1.returnTemp()[i] == orgWord.returnTemp()[0]))
				{
					TemplateSort<string> HA(intSize + 1);
					for (int k = 0; k < intSize; k++)
						HA.returnTemp()[k] = str.returnTemp()[k];
					HA.returnTemp()[intSize] = S1[i];
					intSize++;
					delete[] str.returnTemp();
					str = HA;
					break;
				}
				else
					break;
			}	
		}
	}
	return str;
}

//this function generates all possible combinations of strings of a single string.
TemplateSort<string> combiner(string str, char fl, int& comSize)
{
	comSize = STRLEN(str) + 1;
	TemplateSort<string> com(comSize, "\0");
	for (int i = 0; i < comSize; i++)
	{
		for (int j = 0; j < i; j++)
			com.returnTemp()[i] = com[i] + str[j];

		com.returnTemp()[i] = com[i] + fl;

		for (int k = i; k < STRLEN(str); k++)
			com.returnTemp()[i] = com[i] + str[k];
	}
	return com;
}

//this function merges segments of all possible strings of strings array generated by combiner function.
TemplateSort<string> coalescer(TemplateSort<string> str, int& asaSize, char firstLetter, int size)
{
	asaSize = asaSize * size + 1;
	TemplateSort<string> allStringsArr(asaSize);
	int ind = 0;
	for (int i = 0; i < (asaSize - 1) / size; i++, ind++)
		allStringsArr.returnTemp()[ind] = str[i];

	for (int i = 0; i < (asaSize - 1) / size; i++)
	{
		int comSize = 0;
		TemplateSort<string> com = combiner(str[i], firstLetter, comSize);
		for (int j = 0; j < comSize; j++, ind++)
			allStringsArr.returnTemp()[ind] = com[j];
	}
	return allStringsArr;
}

//this function recursively breaks down each character from a given string.
TemplateSort<string> allPossibleStrings(TemplateSort<string> str, int size, int& asaSize)
{
	if (str[0][0] == '\0')
	{
		TemplateSort<string> aps(1, "\0");
		asaSize++;
		return aps;
	}
	char firstLetter = str[0][0];
	TemplateSort<string> restOfWord(1);
	restOfWord.returnTemp()[0] = str[0][1];
	for (int i = 2; i < STRLEN(str[0]); i++)
		restOfWord.returnTemp()[0] = restOfWord[0] + str[0][i];
	TemplateSort<string> split = allPossibleStrings(restOfWord, size - 1, asaSize);
	TemplateSort<string> allStringsArr = coalescer(split, asaSize, firstLetter, size);
	return allStringsArr;
}

TemplateSort<string> setup(TemplateSort<string>& Word, int& awaSize, int DIFFLEV, int& HS)
{
	int dictSize = 79339;
	TemplateSort<string> dictArr(dictSize);
	LoadDictionary(dictArr, dictSize);
	
	ifstream Rdr("HighScore.txt");
	Rdr >> HS;

	while (true)
	{
		Word = alphaGen(DIFFLEV);

		int size = STRLEN(Word[0]), asaSize = 0;
		TemplateSort<string> allStringArr = allPossibleStrings(Word, size, asaSize);

		int uniSize = 0;
		TemplateSort<string> uni = return_uniqueStrings(allStringArr, uniSize);

		MergeSort(uni, uniSize, 0, uniSize - 1);

		TemplateSort<string> allWordArr = find_intersection(uni, dictArr, awaSize, Word);
		if(awaSize != 0)
			return allWordArr;
	}
}

void Display(TemplateSort<string>& Word, TemplateSort<string>& allWordArr, int awaSize, int DIFFLEV, 
	int score, int HS, int Level)
{
	printSpaces(0, 0, 0, 15);

	string mode;
	if (DIFFLEV == 4)
	{
		gotoRowCol(1, (COLS / 2) - 5);
		cout << Word.returnTemp()[0][0];

		gotoRowCol(3, (COLS / 2) - 10);
		cout << Word.returnTemp()[0][1];

		gotoRowCol(5, (COLS / 2) - 5);
		cout << Word.returnTemp()[0][2];

		gotoRowCol(3, COLS / 2);
		cout << Word.returnTemp()[0][3];

		mode = "Easy";
	}
	else if (DIFFLEV == 5)
	{
		gotoRowCol(1, (COLS / 2) - 5);
		cout << Word.returnTemp()[0][0];

		gotoRowCol(3, (COLS / 2) - 10);
		cout << Word.returnTemp()[0][1];

		gotoRowCol(5, (COLS / 2) - 10);
		cout << Word.returnTemp()[0][2];

		gotoRowCol(3, COLS / 2);
		cout << Word.returnTemp()[0][3];

		gotoRowCol(5, COLS / 2);
		cout << Word.returnTemp()[0][4];

		mode = "Medium";
	}
	else if (DIFFLEV == 6)
	{
		gotoRowCol(1, (COLS / 2) - 5);
		cout << Word.returnTemp()[0][0];

		gotoRowCol(3, (COLS / 2) - 10);
		cout << Word.returnTemp()[0][1];

		gotoRowCol(5, (COLS / 2) - 10);
		cout << Word.returnTemp()[0][2];

		gotoRowCol(7, (COLS / 2) - 5);
		cout << Word.returnTemp()[0][3];

		gotoRowCol(5, COLS / 2);
		cout << Word.returnTemp()[0][4];

		gotoRowCol(3, COLS / 2);
		cout << Word.returnTemp()[0][5];

		mode = "Hard";
	}

	for (int i = 0, j = 10; i < awaSize; i++, j++)
	{
		char w = -37;
		gotoRowCol(j, 0);
		cout << w << ' ';
		for (int k = 0; k < STRLEN(allWordArr[i]); k++)
			cout << '*';
		cout << endl;
	}

	gotoRowCol(1, 5);
	cout << "Mode: " << mode;
	gotoRowCol(2, 5);
	cout << "Level: " << Level;
	gotoRowCol(5, 5);
	cout << "Score = " << score;
	gotoRowCol(6, 5);
	cout << "High Score = " << HS;

	gotoRowCol(5, 100);
	cout << "Hint: 0";
	gotoRowCol(6, 100);
	cout << "Exit: -1";
}

bool IsGuessCorrect(string Guess, TemplateSort<string>& allWordArr, int& place)
{
	for (int i = 0; i < allWordArr.returnSize(); i++)
	{
		if (Guess == allWordArr.returnTemp()[i])
		{
			place = i;
			return true;
		}
	}
	place = -1;
	return false;
}

int Play(TemplateSort<string>& allWordArr, bool& check)
{
	gotoRowCol(10, 100);
	cout << "Guess?";
	int count = 0, score = 0;
	while (count != allWordArr.returnSize())
	{
		printSpaces(12, 100, 12, COLS);
		gotoRowCol(12, 100);
		string Guess;
		cin >> Guess;

		if (Guess == "-1")
		{
			check = false;
			break;
		}
		if (Guess == "0")
		{
			int rpos, cpos;
			getRowColbyLeftClick(rpos, cpos);
			if (cpos == 0 && rpos >= 10 && rpos < 10 + allWordArr.returnSize())
			{
				count++;
				score -= 5;
				printSpaces(5, 13, 5, 16);
				gotoRowCol(5, 13);
				cout << score;
				gotoRowCol(rpos, cpos);
				cout << ' ';
				gotoRowCol(rpos, cpos + 2);
				cout << allWordArr[rpos - 10];
				continue;
			}
		}

		int place = -1;
		if (IsGuessCorrect(Guess, allWordArr, place))
		{
			count++;
			score += 10;
			printSpaces(5, 13, 5, 16);
			gotoRowCol(5, 13);
			cout << score;
			gotoRowCol(10 + place, 0);
			cout << ' ';
			gotoRowCol(10 + place, 2);
			cout << allWordArr[place];
		}
		else
		{
			gotoRowCol(15, 100);
			cout << "Have A Go Again";
			_getch();
			printSpaces(15, 100, 15, COLS);
		}
	}
	return score;
}

int main()
{
	gotoRowCol(0, 0);
	cout << "WELCOME TO WORD SCRAMBLE!";
	gotoRowCol(1, 0);
	cout << "Press any key to continue.";
	_getch();
	printSpaces(0, 0, 0, 30);
	printSpaces(1, 0, 1, 30);

	int mode;
	while (true)
	{
		gotoRowCol(3, 3);
		cout << "Select Difficulty Mode:\n"
			<< "1. Easy\n"
			<< "2. Medium\n"
			<< "3. Hard\n\n"
			<< "opt<<";
		gotoRowCol(8, 5);
		cin >> mode;
		if (mode < 1 || mode > 3)
			continue;
		for (int r = 3; r <= 8; r++)
			printSpaces(r, 0, r, 30);
		break;
	}

	int score = 0, HS = 0, Level = 0;
	bool check = true;
	while (check)
	{
		Level++;
		gotoRowCol(0, 0);
		cout << "Please Wait..";

		int awaSize = 0, DIFFLEV = mode + 3;
		TemplateSort<string> Word(1);
		TemplateSort<string> allWordArr = setup(Word, awaSize, DIFFLEV, HS);
		Display(Word, allWordArr, awaSize, DIFFLEV, score, HS, Level);
		score = score + Play(allWordArr, check);
	}
	if (score > HS)
	{
		ofstream Wtr("HighScore.txt");
		Wtr << score;
	}
	gotoRowCol(30, 0);
}
