#include "TreeCalc.h"

bool TreeCalc::exit = false;
vector<EquationTree*> TreeCalc::trees = {};

template <typename T>
void TreeCalc::run() {
	string userInput="";
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
				printErrors(newTree(dataStr)-1);
			}
			else if (userInput == DELETE_ORD) {
				input(&index1);
				input(&index1);
				deleteTree(index1);
			}

			else if (userInput == MERGE_ORD) {
				input(&index1, &index2);
				printErrors(mergeTrees(index1, index2)-1);
			}
			else if (userInput == JOIN_ORD) {
				inputIntAndEquation(&index1, &dataStr);
				printErrors(joinToTree(index1, dataStr));
			}

			else if (userInput == CALC_ORD) {
				input(&index1);
				output(TREE_VAL_MESSAGE, to_string(evaluateTree(index1))+NEW_LINE);
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
void TreeCalc::output(string message, string data) {
	if(data!="") cout << message + NEW_LINE + data;
	else cout << message;
}

template <typename T>
void TreeCalc::input(string* dataStr) {
	cin >> *dataStr;
}

template <typename T>
void TreeCalc::inputEquation(string* dataStr) {
	getline(cin, *dataStr);
	istringstream divisor(*dataStr);
	*dataStr = "";
	string tmp="";
	while (divisor >> tmp) *dataStr=*dataStr+ tmp+ " ";
	*dataStr = dataStr->substr(0,dataStr->size()-1);

}

template <typename T>
void TreeCalc::input(int* param1) {
	cin >> *param1;
	if (*param1 < 0) *param1 = 0;
}

template <typename T>
void TreeCalc::input(int* param1, int* param2) {
	cin >> *param1>>*param2;
	if (*param1 < 0) *param1 = 0;
	if (*param2 < 0) *param2 = 0;
}

template <typename T>
void TreeCalc::inputIntAndEquation(int* param1, string* dataStr) {
	cin >> *param1;
	if (*param1 < 0) *param1 = 0;

	inputEquation(dataStr);
}

template <typename T>
void TreeCalc::input(int* param1, string* dataStr,int* param2) {
	cin >> *param1 >> *dataStr>>*param2;
	if (*param1 < 0) *param1 = 0;
	if (*param2 < 0) *param2 = 0;
}

template <typename T>
int TreeCalc::newTree(string equation) {
	trees.push_back(new EquationTree(equation));
	return trees.size();

}

template <typename T>
void TreeCalc::deleteTree(int treeIndex) {
	if(treeIndex<trees.size()){
		delete trees.at(treeIndex);
		trees.erase(trees.begin() + treeIndex); //UWAGA erase moze nie usuwac pola wektora tylko jego zawartso!! w takim prypadku napisac tak, zeby po prostu tworzyc krotszy wektor
	}else output(WRONG_INDEX);
}

template <typename T>
int TreeCalc::mergeTrees(int tree1index, int tree2index) {
	if (tree1index < trees.size() && tree2index < trees.size()) {
		EquationTree* newTree = *(trees.at(tree1index)) + trees.at(tree2index);
		trees.push_back(newTree);
		
	}else output(WRONG_INDEX);
	return trees.size();
}

template <typename T>
int TreeCalc::joinToTree(int tree1index, string equationTree) { 
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
double TreeCalc::evaluateTree(int treeIndex) {
	if (treeIndex < trees.size())	return trees.at(treeIndex)->evaluate();
	else {
		output(WRONG_INDEX);
		return 0;
	}
}

template <typename T>
string TreeCalc::listVars(int treeIndex) {
	if (treeIndex < trees.size())	return trees.at(treeIndex)->vars2String();
	else {
		output(WRONG_INDEX);
		return 0;
	}
}

template <typename T>
string TreeCalc::listTrees() {
	string treesStr = "";
	for (int i = 0; i < trees.size(); i++) treesStr += to_string(i)+" " + trees.at(i)->toString() + NEW_LINE;
	return treesStr;
}

template <typename T>
void TreeCalc::assignVarValue(int treeIndex, string varName, int val) {
	
	if (treeIndex < trees.size())	trees.at(treeIndex)->assignValueToVar(varName, val);
	else output(WRONG_INDEX);
}

template <typename T>
void TreeCalc::printErrors(int index) {
	if (index < trees.size()) {
		if (trees.at(index)->getGuard()->newErrors()) output(ERROR_MESSAGE, trees.at(index)->getGuard()->generateErrorList());
		else output(OPERATION_CONFIRM);
	}
	else output(WRONG_INDEX);
}
