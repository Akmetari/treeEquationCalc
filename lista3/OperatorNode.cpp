#include "OperatorNode.h"


OperatorNode::OperatorNode() {
	tag = DEFAULT_TAG;
}

OperatorNode::OperatorNode(EquationNode* original, EquationNode* parent) {
	OperatorNode* pointer = dynamic_cast<OperatorNode*>  (original);
	tag = pointer->tag;

	childrenNumber = pointer->childrenNumber;
	expectedChildrenNumber = pointer->expectedChildrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode * [expectedChildrenNumber];
	for (int i = 0; i < childrenNumber; i++) {
		EquationNode* childrenPtr =original->getChildren()[i];
		if (childrenPtr->isValue()) children[i] = new NumberNode(childrenPtr, this);
		if (childrenPtr->isOperator()) children[i] = new OperatorNode(childrenPtr, this);
		if (childrenPtr->isVariable()) children[i] = new VariableNode(childrenPtr, this);
	}
	this->parent = parent;
}


OperatorNode::OperatorNode(string tag) {
	this->tag = tag;

	for(int i= 0; i< NUMBER_OF_OPERATOR_TYPES; i++){
		if (tag == TAGS[i]) expectedChildrenNumber = NUMBER_OF_PARAMS_FOR_TAG[i];
	}

	childrenNumber = 0;
	if (children == nullptr) delete[] children;
	children = new EquationNode * [expectedChildrenNumber];
	parent = nullptr;

}

OperatorNode::~OperatorNode() {}

void OperatorNode::operator=(OperatorNode& original) {
	tag = original.tag;
	expectedChildrenNumber = original.expectedChildrenNumber;
	childrenNumber = original.childrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode*[childrenNumber];
	for (int i = 0; i < childrenNumber; i++) children[i] = original.children[i];
	parent = original.parent;
}

string OperatorNode:: toString(){
	return tag + " ";
}

double OperatorNode::evaluateNode() {
	double eval = 0;
	if (tag == TAGS[0]) for (int i = 0; i < childrenNumber; i++) eval += (*children[i]).evaluateNode();
	else if (tag == TAGS[2]) eval = (*children[0]).evaluateNode() * (*children[1]).evaluateNode();
	else if (tag == TAGS[1]) eval = (*children[0]).evaluateNode() - (*children[1]).evaluateNode();
	else if (tag == TAGS[3]) {
		if ((*children[1]).evaluateNode() == 0) return 0;
		else eval= (*children[0]).evaluateNode() / (*children[1]).evaluateNode();
	}
	else if (tag == TAGS[4]) eval = sin((*children[0]).evaluateNode());
	else if (tag == TAGS[5]) eval = cos((*children[0]).evaluateNode());
	else if (tag == TAGS[6]) for (int i = 0; i < childrenNumber; i++) eval += (*children[i]).evaluateNode();
	return eval;
}

void OperatorNode::assignValue(string varTag, int val) {

	for (int i = 0; i < childrenNumber; i++) children[i]->assignValue(varTag, val);
}

void OperatorNode::addChild(EquationNode * child, int index) {
	
	if (this->tag == TAGS[6]) {
		if (childrenNumber == 0 && !child->isVariable()) {
			cout << NOT_A_VARIABLE;

			if (index < 0) {
				children[childrenNumber] = new VariableNode();
				children[childrenNumber]->setParent(this);

				childrenNumber++;
			}
			else if (children[index] == nullptr) {
				children[index] = new VariableNode();
				children[index]->setParent(this);
				childrenNumber++;
			}
		}
		else if (childrenNumber == 1 && !child->isOperator()) {
			cout << NOT_AN_OPERATOR;
			
			if (index < 0) {
				children[childrenNumber] = new OperatorNode();
				children[childrenNumber]->setParent(this);
				childrenNumber++;
			}
			else if (children[index] == nullptr) {
				children[index] = new OperatorNode();
				children[index]->setParent(this);
				childrenNumber++;
			}
		}
		else {

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
	}
	else {

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
}
