#pragma once
#include "EquationNode.h"


const string DEFAULT_VAR = "x";
const string EQUALS = "= "
;
class VariableNode: public EquationNode{

private:
	string tag;
	int value;

public:

	int getValue() { return value; }
	string getTag() { return tag; }
	bool isVariable() { return true; }
	bool isValue() { return false; }
	bool isOperator() { return false; }


	bool setValue(int val) {
		if (val >= 0) {
			value = val;
			return true;
		}
		else return false;
	}
	void setTag(string tag) { this->tag = tag; }

	VariableNode();
	VariableNode(string tag);
	//VariableNode(VariableNode& original, EquationNode* parent = nullptr);
	VariableNode(EquationNode* original, EquationNode* parent = nullptr);

	~VariableNode();
	void operator=(VariableNode& original);

	string toStringVal();
	string toString();
	double evaluateNode();

	void assignValue(string varTag, int val);
	void addChild(EquationNode* child, int index);
};

