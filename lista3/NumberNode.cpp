#include "NumberNode.h"
#include "VariableNode.h"
#include "OperatorNode.h"

NumberNode::NumberNode() {
	value = 1;
	expectedChildrenNumber = 0;
}

NumberNode::NumberNode(double val) {
	value = val;
	expectedChildrenNumber = 0;
	childrenNumber = 0;
	if (children != nullptr) delete[] children;
	children = new EquationNode*[expectedChildrenNumber];
	parent = nullptr;
}

NumberNode::NumberNode(EquationNode* original, EquationNode* parent) {
	NumberNode* pointer = dynamic_cast<NumberNode*>  (original);
	value = pointer->value;
	childrenNumber = pointer->childrenNumber;
	expectedChildrenNumber = pointer->expectedChildrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode * [expectedChildrenNumber];
	for (int i = 0; i < childrenNumber; i++) {
		EquationNode* childrenPtr = original->getChildren()[i];
		if (childrenPtr->isNumber()) children[i] = new NumberNode(childrenPtr, this);
		else if (childrenPtr->isOperator()) children[i] = new OperatorNode(childrenPtr, this);
		else if (childrenPtr->isVariable()) children[i] = new VariableNode(childrenPtr, this);
	}
	this->parent = original->getParent();
}


NumberNode::~NumberNode() {}

void NumberNode::operator=(NumberNode& original) {
	value = original.value;
	childrenNumber = original.childrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode*[childrenNumber];
	for (int i = 0; i < childrenNumber; i++) children[i] = original.children[i];
	parent = original.parent;
}

string NumberNode::toString() {
	return to_string(value) + " ";
}

double NumberNode::evaluateNode() {
	return value;
}

void NumberNode::assignValue(string varTag, int val) {}

void NumberNode::addChild(EquationNode* child, int index) {
	if (index < 0) {
		child->setParent(this);
		children[childrenNumber] = child;
		childrenNumber++;
	}
	else if (children[index] == nullptr) {
		child->setParent(this);
		children[index] = child;
		childrenNumber++;
	}
}