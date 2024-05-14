#pragma once
#include "EquationTree.h"
#include <iostream>

// consts for messages
const string WELCOME_MESSAGE = "Welcome to tree calculator! Trees are constructed from equations in polish notation.\n\n";
const string HELP_INSTRUCTIONS = "Instructions: \naddTree [s] - addTree representing equation s \ndeleteTree [i] - delete tree from index i \nmergeTree [i] [j] - merge trees from indexes i and j into new tree\n joinTree [i] [s] - join new tree form s to tree i \ncalc [i] - calculate tree from index i\nls - list all trees\nlsv [i] - list variables of tree i\nassignVar [i] [var_name] [val] - assign value to variable from tree i\nTo see this message again type: help\nTo exit type: exit\n";
const string TREE_VAL_MESSAGE = "Tree value= ";
const string LISTING_VARS_MESSAGE = "Variables of tree:\n";
const string LISTING_TREES_MESSAGE = "Trees:\n";
const string OPERATION_CONFIRM = "Operation ended successfully\n";
const string WRONG_ORDER_MESSAGE = "No suitable command. Please try again...";
const string WRONG_INDEX = "Wrong index. Please try again...";
const string ERROR_MESSAGE = "Errors:  ";
const string CONSOLE_POINTER = "TreeCalc> ";
const string END_MESSAGE = "Goodbye!\n";

// wzorcowe komendy
const string EXIT_ORD= "exit";
const string HELP_ORD = "help";
const string ADD_ORD = "addTree";
const string DELETE_ORD = "deleteTree";
const string MERGE_ORD = "mergeTree";
const string JOIN_ORD = "joinTree";
const string CALC_ORD = "calc";
const string LIST_TREES_ORD = "ls";
const string LIST_VARS_ORD = "lsv";
const string ASSIGN_VAR_ORD = "assignVar";

template <typename T> class TreeCalc{

private:
	static vector<EquationTree<T>*> trees;
	static bool exit;

public:

	static void run();
	static void output(string message, string data="");
	
	static void input(int* param1);
	static void input(string* dataStr);
	static void inputEquation(string* dataStr);
	static void input( int* param1, int* param2);
	static void inputIntAndEquation(int* param1, string* dataStr);
	static void input( int* param1, string* param2, int* param3);

	
	static int newTree(string equation);
	static void deleteTree(int treeIndex);
	static int mergeTrees(int tree1index, int tree2index); // merge tree 1 and tree 2 into tree3 (t1+t2=t3)
	static int joinToTree(int tree1index, string equationTree);
	static double evaluateTree(int treeIndex);

	static void assignVarValue(int treeIndex, string varName, int val);
	
	static string listVars(int treeIndex);
	static string listTrees();

	static void printErrors(int treeIndex);


};

// BODY ********************************************************************
template <typename T>
bool TreeCalc<T>::exit = false;

template <typename T>
vector<EquationTree<T>*> TreeCalc<T>::trees = {};

template <typename T>
void TreeCalc<T>::run() {
	string userInput = "";
	int index1;
	int index2;
	string dataStr;
	output(WELCOME_MESSAGE, HELP_INSTRUCTIONS);
	while (!exit) {
		output(CONSOLE_POINTER, "");
		input(&userInput);
		if (userInput == EXIT_ORD) exit = true;
		else {
			if (userInput == HELP_ORD) output(HELP_INSTRUCTIONS, "");
			else if (userInput == ADD_ORD) {
				inputEquation(&dataStr);
				printErrors(newTree(dataStr) - 1);
			}
			else if (userInput == DELETE_ORD) {
				input(&index1);
				input(&index1);
				deleteTree(index1);
			}

			else if (userInput == MERGE_ORD) {
				input(&index1, &index2);
				printErrors(mergeTrees(index1, index2) - 1);
			}
			else if (userInput == JOIN_ORD) {
				inputIntAndEquation(&index1, &dataStr);
				printErrors(joinToTree(index1, dataStr));
			}

			else if (userInput == CALC_ORD) {
				input(&index1);
				output(TREE_VAL_MESSAGE, to_string(evaluateTree(index1)) + NEW_LINE);
			}
			else if (userInput == LIST_TREES_ORD) {
				output(LISTING_TREES_MESSAGE, listTrees());
			}
			else if (userInput == LIST_VARS_ORD) {
				input(&index1);
				output(LISTING_VARS_MESSAGE, listVars(index1));
			}
			else if (userInput == ASSIGN_VAR_ORD) {
				input(&index1, &dataStr, &index2);// index2 użyte jako wartość dla zmiennej zeby na jeden przypadek nei mnozyc zmiennych
				assignVarValue(index1, dataStr, index2);
			}
			else output(WRONG_ORDER_MESSAGE);
		}
	}
	output(END_MESSAGE, "");
}

template <typename T>
void TreeCalc<T>::output(string message, string data) {
	if (data != "") cout << message + NEW_LINE + data;
	else cout << message;
}

template <typename T>
void TreeCalc<T>::input(string* dataStr) { // INPUT MA PRZYJMOWAC ODPOWIEDNI TYP lub dodac gdzies konwersje na T!!!
	cin >> *dataStr;
}

template <typename T>
void TreeCalc<T>::inputEquation(string* dataStr) {
	getline(cin, *dataStr);
	istringstream divisor(*dataStr);
	*dataStr = "";
	string tmp = "";
	while (divisor >> tmp) *dataStr = *dataStr + tmp + " ";
	*dataStr = dataStr->substr(0, dataStr->size() - 1);

}

template <typename T>
void TreeCalc<T>::input(int* param1) {
	cin >> *param1;
	if (*param1 < 0) *param1 = 0;
}

template <typename T>
void TreeCalc<T>::input(int* param1, int* param2) {
	cin >> *param1 >> *param2;
	if (*param1 < 0) *param1 = 0;
	if (*param2 < 0) *param2 = 0;
}

template <typename T>
void TreeCalc<T>::inputIntAndEquation(int* param1, string* dataStr) {
	cin >> *param1;
	if (*param1 < 0) *param1 = 0;

	inputEquation(dataStr);
}

template <typename T>
void TreeCalc<T>::input(int* param1, string* dataStr, int* param2) {
	cin >> *param1 >> *dataStr >> *param2;
	if (*param1 < 0) *param1 = 0;
	if (*param2 < 0) *param2 = 0;
}

template <typename T>
int TreeCalc<T>::newTree(string equation) {
	trees.push_back(new EquationTree(equation));
	return trees.size();

}

template <typename T>
void TreeCalc<T>::deleteTree(int treeIndex) {
	if (treeIndex < trees.size()) {
		delete trees.at(treeIndex);
		trees.erase(trees.begin() + treeIndex); //UWAGA erase moze nie usuwac pola wektora tylko jego zawartso!! w takim prypadku napisac tak, zeby po prostu tworzyc krotszy wektor
	}
	else output(WRONG_INDEX);
}

template <typename T>
int TreeCalc<T>::mergeTrees(int tree1index, int tree2index) {
	if (tree1index < trees.size() && tree2index < trees.size()) {
		EquationTree* newTree = *(trees.at(tree1index)) + trees.at(tree2index);
		trees.push_back(newTree);

	}
	else output(WRONG_INDEX);
	return trees.size();
}
template <typename T>
int TreeCalc<T>::joinToTree(int tree1index, string equationTree) {
	if (tree1index < trees.size()) {
		EquationTree* tree2 = new EquationTree(equationTree);
		EquationTree* newTree = *(trees.at(tree1index)) + tree2;
		trees.at(tree1index) = newTree;
		delete tree2;
	}
	else output(WRONG_INDEX);
	return tree1index;
}

template <typename T>
double TreeCalc<T>::evaluateTree(int treeIndex) {
	if (treeIndex < trees.size())	return trees.at(treeIndex)->evaluate();
	else {
		output(WRONG_INDEX);
		return 0;
	}
}

template <typename T>
string TreeCalc<T>::listVars(int treeIndex) {
	if (treeIndex < trees.size())	return trees.at(treeIndex)->vars2String();
	else {
		output(WRONG_INDEX);
		return 0;
	}
}

template <typename T>
string TreeCalc<T>::listTrees() {
	string treesStr = "";
	for (int i = 0; i < trees.size(); i++) treesStr += to_string(i) + " " + trees.at(i)->toString() + NEW_LINE;
	return treesStr;
}

template <typename T>
void TreeCalc<T>::assignVarValue(int treeIndex, string varName, int val) {

	if (treeIndex < trees.size())	trees.at(treeIndex)->assignValueToVar(varName, val);
	else output(WRONG_INDEX);
}

template <typename T>
void TreeCalc<T>::printErrors(int index) {
	if (index < trees.size()) {
		if (trees.at(index)->getGuard()->newErrors()) output(ERROR_MESSAGE, trees.at(index)->getGuard()->generateErrorList());
		else output(OPERATION_CONFIRM);
	}
	else output(WRONG_INDEX);
}
