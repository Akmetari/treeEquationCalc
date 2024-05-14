#pragma once
#include "EquationNode.h"
#include "OperatorNode.h"
#include "VariableNode.h"
#include "ErrorGuard.h"
#include <sstream>
#include <iostream>
#include<list>
#include<vector>

const int ASCII_9 = 57;
const int ASCII_0 = 48;
const string DEFAULT_NODE = "1";


template <typename T> class EquationTree
{
private:
	EquationNode<T>* root;
	string equationString;
	vector <EquationNode<T>*> variables;

	ErrorGuard* guard;

	EquationNode<T>* createNode(string strNode);
	bool growTree(EquationNode<T>** startNode, list<string>* dividedEquation, list<string>::iterator i);
	bool containsVar(string varTag);
	void tree2Str(EquationNode<T>* rootNode, string* str);

public:

	EquationTree<T>();
	EquationTree<T>(EquationTree<T>& original);
	EquationTree<T>(EquationNode<T>* root);
	EquationTree<T>(string equationStr);
	~EquationTree<T>();

	EquationNode<T>* getRoot() { return root; }
	ErrorGuard* getGuard() { return guard; }

	
	string toString();
	string vars2String();
	double evaluate();
	void assignValueToVar(string var,int value);
	list<string> divideStrBySpace(string str);

	void operator=(const EquationTree<T>& originTree);
	EquationTree<T> operator+( EquationTree<T>& treeToAdd); //dolacza drzewo w miejsce pierwszej zmiennej nieposiadajacej przypisanej wartosci
	EquationTree<T>* operator+( EquationTree<T>* treeToAdd);
};

// BODY *************************************************************************************

template <typename T>
EquationTree<T>::EquationTree<T>() {
	root = new EquationNode<T>();
	variables = {};
	guard = new ErrorGuard();
}

template <typename T>
EquationTree<T>::EquationTree(EquationTree<T>& original) {
	guard = new ErrorGuard();
	EquationNode<T>* pointer = original.root;
	if (pointer->isValue()) root = new ValueNode<T>(pointer);
	else if (pointer->isOperator()) root = new OperatorNode<T>(pointer);
	else if (pointer->isVariable()) root = new VariableNode<T>(pointer);
	equationString = original.equationString;
	root->findVars(&variables);
}

template <typename T>
EquationTree<T>::EquationTree<T>(EquationNode<T>* root) {
	guard = new ErrorGuard();
	this->root = root;
	root->findVars(&variables);

}

template <typename T>
EquationTree<T>::EquationTree<T>(string equationStr) {
	guard = new ErrorGuard();
	list<string> dividedEquation = divideStrBySpace(equationStr);
	list<string>::iterator strNodeIndex = dividedEquation.begin();
	root = createNode(*strNodeIndex);
	advance(strNodeIndex, 1);
	growTree(&root, &dividedEquation, strNodeIndex);

}

template <typename T>
list<string> EquationTree<T>::divideStrBySpace(string str) {
	istringstream divisor(str);
	list<string> dividedStr;
	string tmpStr;
	if (str == "") guard->raiseNoEquation();
	while (divisor >> tmpStr) dividedStr.push_back(tmpStr);

	return dividedStr;
}

template <typename T>
EquationNode<T>* EquationTree<T>::createNode(string strNode) {
	EquationNode<T>* node;
	bool isOperator = false;
	int numberOfVariables = 0;
	for (int i = 0; i < NUMBER_OF_OPERATOR_TYPES; i++) if (TAGS[i] == strNode) isOperator = true;

	if (strNode[0] > ASCII_0 && strNode[0] < ASCII_9) node = new ValueNode<T>(stoi(strNode));
	else if (isOperator) node = new OperatorNode<T>(strNode);
	else {
		node = new VariableNode<T>(strNode);
		if (!containsVar(node->toString())) variables.push_back(node);
	}
	return node;
}

template <typename T>
bool EquationTree<T>::growTree(EquationNode<T>** startNode, list<string>* dividedEquation, list<string>::iterator i) {
	while ((**startNode).getChildrenNumber() != (**startNode).getExpectedChildrenNumber() && i != dividedEquation->end()) {
		EquationNode<T>* child = createNode(*i);
		(**startNode).addChild(child);
		advance(i, 1);

		growTree(&child, dividedEquation, i);
	}

	while ((**startNode).getChildrenNumber() != (**startNode).getExpectedChildrenNumber()) {
		guard->raiseTooLittleParams();
		EquationNode* child = createNode(DEFAULT_NODE);
		(**startNode).addChild(child);
	}

	return (**startNode).getChildrenNumber() == (**startNode).getExpectedChildrenNumber();
}

template <typename T>
EquationTree<T>::~EquationTree<T>() {
	delete root;
	delete guard;
}


template <typename T>
void EquationTree<T>::tree2Str(EquationNode<T>* rootNode, string* str) {
	*str += (*rootNode).toString();
	for (int i = 0; i < (*rootNode).getChildrenNumber(); i++) {
		tree2Str((*rootNode).getChildren()[i], str);
	}
}

template <typename T>
string EquationTree<T>::toString() {
	string treeStr = "";
	tree2Str(root, &treeStr);
	return treeStr;
}

template <typename T>
string EquationTree<T>::vars2String() {
	string vars = "";
	for (int i = 0; i < variables.size(); i++) vars += ((VariableNode<T>*)variables.at(i))->toStringVal() + NEW_LINE;
	return vars;
}


template <typename T>
double EquationTree<T>::evaluate() {
	return (*root).evaluateNode();
}

template <typename T>
void EquationTree<T>::operator=(const EquationTree<T>& originTree) {
	EquationNode<T>* pointer = originTree.root;
	if (pointer->isValue()) root = new ValueNode<T>(pointer);
	else if (pointer->isOperator()) root = new OperatorNode<T>(pointer);
	else if (pointer->isVariable()) root = new VariableNode<T>(pointer);
	equationString = originTree.equationString;
	root->findVars(&variables);
}


template <typename T>
EquationTree<T> EquationTree<T>::operator+(EquationTree<T>& originTree) {
	EquationTree copyTree(*this);
	if (variables.size() > 0) {
		EquationNode<T>* joint = copyTree.variables.at(0)->getParent();
		EquationNode<T>* newBranch = nullptr;

		if (originTree.getRoot()->isValue()) newBranch = new ValueNode<T>(originTree.getRoot());
		else if (originTree.getRoot()->isOperator()) newBranch = new OperatorNode<T>(originTree.getRoot());
		else if (originTree.getRoot()->isVariable()) newBranch = new VariableNode<T>(originTree.getRoot());
		newBranch->setParent(joint);

		int index = joint->deleteChild(&copyTree.variables.at(0));
		joint->addChild(newBranch, index);
		copyTree.root->findVars(&(copyTree.variables));
	}
	return copyTree;
}

template <typename T>
EquationTree<T>* EquationTree<T>::operator+(EquationTree<T>* originTree) {
	EquationTree* copyTree = new EquationTree(*this);
	if (variables.size() > 0) {
		EquationNode<T>* joint = copyTree->variables.at(0)->getParent();
		EquationNode<T>* newBranch = nullptr;

		if (originTree->getRoot()->isValue()) newBranch = new NumberNode<T>(originTree->getRoot());
		else if (originTree->getRoot()->isOperator()) newBranch = new OperatorNode<T>(originTree->getRoot());
		else if (originTree->getRoot()->isVariable()) newBranch = new VariableNode<T>(originTree->getRoot());
		newBranch->setParent(joint);

		int index = joint->deleteChild(&copyTree->variables.at(0));
		joint->addChild(newBranch, index);
		copyTree->root->findVars(&(copyTree->variables));
	}
	return copyTree;
}

template <typename T>
bool EquationTree<T>::containsVar(string varTag) {
	bool ret = false;
	for (int i = 0; i < variables.size(); i++) {
		ret = ret || ((VariableNode<T>*)variables.at(i))->getTag() == varTag;
	}
	return ret;
}

template <typename T>
void EquationTree<T>::assignValueToVar(string varTag, int value) {
	if (containsVar(varTag)) {
		root->assignValue(varTag, value);
	}
}

