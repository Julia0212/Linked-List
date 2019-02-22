// Laboratory 2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>
#include <conio.h> // getch
#include <vector>

#pragma warning(disable : 4996)

struct Node {
	char* data;
	Node* next;
};

Node *evaluating_tokens(Node *head, std::string s, int &index_tokens);
bool isOperator(std::string token);
Node *NewElement(char *data);
Node *AddEnd(Node *head, Node *new_element);
void PrintList(Node *head);
std::string VariableNumber(std::string s);
void PrintTable(std::string result, int &pos_result);
void gotoxy(int x, int y);
void printBin(std::string prefix, int n, int &pos, std::string &result);
int EvaluateResult(std::string bin, Node *head, std::string result);
Node *DNF(std::string result, std::string bin, Node *head);

Node *HEAD = NULL;
Node *HEAD_DNF = NULL;

int main()
{

	std::string s;
	std::cout << "Type the expression ";
	getline(std::cin, s);
	std::string *tokens = new std::string[s.length()];
	int size;

	Node *start = NULL;
	start = evaluating_tokens(start, s, size);
	std::string result = VariableNumber(s);
	HEAD = start;
	int pos_result;
	PrintTable(result, pos_result);
	pos_result = pos_result + 2;
	gotoxy(10, pos_result);
	PrintList(HEAD_DNF);
}

Node *evaluating_tokens(Node *head, std::string s, int &index_tokens) { // returns head of the list

	std::string token;
	index_tokens = 0;
	std::string str = "";

	for (int i = 0; i < s.length(); i++)
	{
		token = s[i];
		if (isOperator(token))
		{
			if (!str.empty())
			{
				char *cstr = new char[str.length() + 1];
				strcpy(cstr, str.c_str());
				head =  AddEnd(head, NewElement(cstr));
				delete[] cstr;
				//tokens[index_tokens] = str;
				index_tokens++;
			}
			str = "";
			//tokens[index_tokens] = token;
			/*char *cstr = new char[token.length() + 1];
			strcpy(cstr, token.c_str());
			head = AddEnd(head, NewElement(cstr));
			delete[] cstr;
			index_tokens++;*/
		}
		else
		{
			// Append the variables    
			if (!token.empty() && token != " ")
			{
				str.append(token);
			}
			else
			{
				if (str != "")
				{
					//tokens[index_tokens] = str;
					char *cstr = new char[str.length() + 1];
					strcpy(cstr, str.c_str());
					head = AddEnd(head, NewElement(cstr));
					delete[] cstr;
					index_tokens++;
					str = "";
				}
			}
		}
	}
	if (str != "") {
		//tokens[index_tokens] = str;
		char *cstr = new char[str.length() + 1];
		strcpy(cstr, str.c_str());
		head = AddEnd(head, NewElement(cstr));
		delete[] cstr;
		
		index_tokens++;
	}

	return head;
}

std::string VariableNumber(std::string s) {

	std::string token;
	std::string result = "";
	for (int i = 0; i < s.length(); i++)
	{
		token = s[i];
		if (isOperator(token) == false) 
			if (!(result.find(token) != std::string::npos))
				result.append(token);
	}

	return result;
}

void PrintTable(std::string result, int &pos_result) {

	gotoxy(10, 5);
	std::cout << "______________________";

	int pos = 10;
	for (int i = 0; i < result.length(); i++) {
		gotoxy(pos, 6);
		std::cout << result[i];
		pos = pos + 1;
	}
	pos = pos + 2;
	gotoxy(pos, 6);
	std::cout << "f";

	gotoxy(10, 7);
	std::cout << "______________________";

	int pos2 = 8;
	printBin("", result.length(), pos2, result);

	pos_result = pos2;
	char ch = getch();
} 

int EvaluateResult(std::string bin, Node *head, std::string result) {
	Node *x = head;
	//make table
	std::string token;
	int functionValue = 0; // final result, what function returns

	while (x != NULL) {
		std::string data = x->data;

		int ConjunctionResult = 1;
		for (int i = 0; i < data.length(); i++) {
			token = data[i];
			std::size_t index = result.find(token);

			std::string text;
			text = bin[index];
			std::istringstream iss(text); // string to int
			int value;
			iss >> value;
			
			ConjunctionResult = ConjunctionResult * value;
		}
		functionValue = functionValue + ConjunctionResult;
		if (functionValue == 2) functionValue = 0;

		x = x->next;
	}


	return functionValue;
}

void gotoxy(int x, int y)
{
	COORD ord;
	ord.X = x;
	ord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), ord);
}

void printBin(std::string prefix, int n, int &pos, std::string &result)
{
	if (prefix.size() == n)
	{
		gotoxy(10, pos);
		std::cout << prefix << std::endl;
		int functionValue = EvaluateResult(prefix, HEAD, result);
		int pos1 = 10 + prefix.length() + 2;
		gotoxy(pos1, pos);
		std::cout << functionValue;
		if (functionValue == 1) {
			HEAD_DNF = DNF(result, prefix, HEAD_DNF);
		}
		pos++;
		return;
	}

	printBin(prefix + '0', n, pos, result);
	printBin(prefix + '1', n, pos, result);

	return;

}

bool isOperator(std::string token)
{
	if (token == "+") return true;
	else return false;
}

Node *NewElement(char *data) {
	Node *new_element = new Node();
	new_element->data = new char[strlen(data) + 1];
	strcpy(new_element->data, data);
	new_element->next = NULL;
	return new_element;
}

Node *AddEnd(Node *head, Node *new_element) {
	Node *x = head;
	if (head == NULL)
		return new_element;

	while (x->next != NULL) {
		x = x->next;
	}

	x->next = new_element;
	return head;
}

void PrintList(Node *head) {
	Node *x = head;
	while (x != NULL) {
		std::cout << x->data;
		x = x->next;
	}
	char ch = getch();
}

Node *DNF(std::string result, std::string bin, Node *head) {
	std::string token;
	std::string str = "";
	for (int i = 0; i < result.length(); i++) {
		token = bin[i];
		std::istringstream iss(token); // string to int
		int value;
		iss >> value;

		if (value == 0) {
			str.append("!");
			token = result[i];
			str.append(token);
		}
		else { // if value == 1
			token = result[i];
			str.append(token);
		}
	}

	char *cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	HEAD_DNF = AddEnd(HEAD_DNF, NewElement(cstr));
	delete[] cstr;

	bool allOne = true;
	
	for (int i = 0; i < (bin.length()); i++) {
		token = bin[i];
		std::istringstream iss2(token); // string to int
		int value2;
		iss2 >> value2;

		if (value2 == 0) allOne = false;
		if (allOne == false) break;
	}
	if (allOne == false) {
		std::string str2 = " OR ";
		char *cstr2 = new char[str2.length() + 1];
		strcpy(cstr2, str2.c_str());
		HEAD_DNF = AddEnd(HEAD_DNF, NewElement(cstr2));
		delete[] cstr2;
		
	}
	

	return HEAD_DNF;
}


