#pragma once
#include "EquationNode.h"

class NumberNode: public EquationNode{
private: 
	double value;

public:

	double getValue() { return value; }

	NumberNode();
	NumberNode(double val);
	//NumberNode(NumberNode& original, EquationNode* parent= nullptr);
	NumberNode(EquationNode* original, EquationNode* parent=nullptr);
	bool isVariable() { return false; }
	bool isValue() { return true; }
	bool isOperator() { return false; }

	~NumberNode();
	void operator=(NumberNode& original);
	string toString();
	double evaluateNode();
	void assignValue(string varTag, int val);

	void addChild(EquationNode* child, int index);
};

