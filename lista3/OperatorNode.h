# pragma once
#include "EquationNode.h"


const int NUMBER_OF_OPERATOR_TYPES = 7;

const string DEFAULT_TAG = "+";
const string TAGS[NUMBER_OF_OPERATOR_TYPES] = { "+","-", "*", "/", "sin", "cos", "#" };
const int NUMBER_OF_PARAMS_FOR_TAG[NUMBER_OF_OPERATOR_TYPES] = { 2,2,2,2,1,1,2 };
const string NOT_AN_OPERATOR = "Error! not an operator on second position";
const string NOT_A_VARIABLE = "Error! not a variable on first position";

class OperatorNode : public EquationNode {

private:
	string tag;

public:
	string getTag() { return tag; }

	OperatorNode();
	OperatorNode(string tag);
	OperatorNode(EquationNode* original, EquationNode<* parent = nullptr);
	void addChild(EquationNode* child, int index);

	bool isVariable() { return false; }
	bool isValue() { return false; }
	bool isOperator() { return true; }

	~OperatorNode();
	void operator=(OperatorNode& original);
	string toString();
	double evaluateNode();

	void assignValue(string varTag, int val);

};




