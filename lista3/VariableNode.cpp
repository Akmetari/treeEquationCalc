#include "VariableNode.h"


VariableNode::VariableNode() {
	tag = DEFAULT_VAR;
	value = 0;
	expectedChildrenNumber = 0;
}

VariableNode::VariableNode(string tag) {
	this->tag = tag;
	value = 0;
	expectedChildrenNumber = 0;
}

VariableNode::VariableNode(EquationNode* original, EquationNode* parent) {
	VariableNode* pointer = dynamic_cast<VariableNode*>  (original);
	tag = pointer->tag;
	value = pointer->value;

	childrenNumber = pointer->childrenNumber;
	expectedChildrenNumber = pointer->expectedChildrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode * [expectedChildrenNumber];
	for (int i = 0; i < childrenNumber; i++) {
		EquationNode* childrenPtr = original->getChildren()[i];
		if (childrenPtr->isValue()) children[i] = new ValueNode<T>(childrenPtr, this);
		else if (childrenPtr->isOperator()) children[i] = new OperatorNode(childrenPtr, this);
		else if (childrenPtr->isVariable()) children[i] = new VariableNode(childrenPtr, this);
	}
	this->parent = parent;
}
VariableNode::~VariableNode() {}

void VariableNode::operator=(VariableNode& original) {
	tag = original.tag;
	value = original.value;
	expectedChildrenNumber = original.expectedChildrenNumber;
	childrenNumber = original.childrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode*[childrenNumber];
	for (int i = 0; i < childrenNumber; i++) children[i] = original.children[i];
	parent = original.parent;
}

string VariableNode::toStringVal() {
	return tag + EQUALS + to_string(value);
}

string VariableNode::toString() {
	return tag;
}

double VariableNode::evaluateNode() {
	return value;
}

void VariableNode::assignValue(string varTag, int val) {
	if (tag == varTag) value = val;
}

void VariableNode::addChild(EquationNode* child, int index) {
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