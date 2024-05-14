EquationNode<T>::EquationNode<T>(int childrenNum) {
	childrenNumber = 0;
	expectedChildrenNumber = childrenNum;
	children = new EquationNode*[expectedChildrenNumber];
	parent = nullptr;
}

EquationNode::EquationNode(EquationNode& original) {
	childrenNumber=original.childrenNumber;
	expectedChildrenNumber = original.expectedChildrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode * [expectedChildrenNumber];
	for (int i = 0; i < childrenNumber; i++) {
		EquationNode* pointer = original.children[i];
		if(pointer->isValue()) children[i] = new ValueNode<T>(pointer, this);
		if(pointer->isOperator()) children[i] = new OperatorNode(pointer, this);
		if(pointer->isVariable()) children[i] = new VariableNode(pointer, this);
	}

	parent = original.parent;

}

EquationNode::EquationNode(EquationNode& original, EquationNode* par) {
	childrenNumber = original.childrenNumber;
	expectedChildrenNumber = original.expectedChildrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode * [expectedChildrenNumber];
	for (int i = 0; i < childrenNumber; i++) {
		EquationNode* pointer = original.children[i];
		if (pointer->isValue()) children[i] = new ValueNode<T>(pointer, this);
		if (pointer->isOperator()) children[i] = new OperatorNode(pointer, this);
		if (pointer->isVariable()) children[i] = new VariableNode(pointer, this);
	}
	parent = par;

}

EquationNode:: ~EquationNode() {
	for (int i = 0; i < childrenNumber; i++) delete (children[i]);
	delete children;
}

void  EquationNode:: operator=(const EquationNode& original) {
	childrenNumber = original.childrenNumber;
	expectedChildrenNumber = original.expectedChildrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode*[expectedChildrenNumber];
	for (int i = 0; i < childrenNumber; i++) {
		EquationNode* pointer = original.children[i];
		if (pointer->isValue()) children[i] = new ValueNode<T>(pointer, this);
		if (pointer->isOperator()) children[i] = new OperatorNode(pointer, this);
		if (pointer->isVariable()) children[i] = new VariableNode(pointer, this);
	}
	parent = original.parent;
}


void EquationNode::addChild(EquationNode * child, int index) {
	if (index < 0) {
		child->parent = this;
		children[childrenNumber] = child;
		childrenNumber++;
	}
	else if (children[index] == nullptr) {
		child->parent = this;
		children[index] = child;
		childrenNumber++;
	}
}

int EquationNode::deleteChild(EquationNode** child) {
	int number = -1;
	for (int i = 0; i < childrenNumber; i++) {
		if (children[i] == *child) {
			delete children[i];
			children[i] = nullptr;
			number = i;
			childrenNumber--;
			return number;
		}
	}
	return number;
}

string EquationNode::toString() {
	return "";
}

double EquationNode::evaluateNode() { return 0; }

bool EquationNode::isVariable() { return false; }
bool EquationNode::isValue() { return false; }
bool EquationNode::isOperator() { return false; }

void EquationNode::assignValue(string varTag, int val) {}

void EquationNode::findVars(vector <EquationNode*>* varVector) {
	if (isVariable()) varVector->push_back(this);
	for (int i = 0; i < childrenNumber; i++) {
		children[i]->findVars(varVector);
		
	}
}



// OPERATOR NODE  ********************************************************************************



OperatorNode<T>::OperatorNode<T>() {
	tag = DEFAULT_TAG;
}

OperatorNode<T>::OperatorNode<T>(EquationNode<T>* original, EquationNode<T>* parent) {
	OperatorNode<T>* pointer = dynamic_cast<OperatorNode<T>*>  (original);
	tag = pointer->tag;

	childrenNumber = pointer->childrenNumber;
	expectedChildrenNumber = pointer->expectedChildrenNumber;
	if (children != nullptr) delete[] children;
	children = new EquationNode<T> * [expectedChildrenNumber];
	for (int i = 0; i < childrenNumber; i++) {
		EquationNode<T>* childrenPtr = original->getChildren()[i];
		if (childrenPtr->isValue()) children[i] = new ValueNode<T>(childrenPtr, this);
		if (childrenPtr->isOperator()) children[i] = new OperatorNode<T>(childrenPtr, this);
		if (childrenPtr->isVariable()) children[i] = new VariableNode<T>(childrenPtr, this);
	}
	this->parent = parent;
}


OperatorNode::OperatorNode(string tag) {
	this->tag = tag;

	for (int i = 0; i < NUMBER_OF_OPERATOR_TYPES; i++) {
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
	children = new EquationNode * [childrenNumber];
	for (int i = 0; i < childrenNumber; i++) children[i] = original.children[i];
	parent = original.parent;
}

string OperatorNode::toString() {
	return tag + " ";
}

double OperatorNode::evaluateNode() {
	double eval = 0;
	if (tag == TAGS[0]) for (int i = 0; i < childrenNumber; i++) eval += (*children[i]).evaluateNode();
	else if (tag == TAGS[2]) eval = (*children[0]).evaluateNode() * (*children[1]).evaluateNode();
	else if (tag == TAGS[1]) eval = (*children[0]).evaluateNode() - (*children[1]).evaluateNode();
	else if (tag == TAGS[3]) {
		if ((*children[1]).evaluateNode() == 0) return 0;
		else eval = (*children[0]).evaluateNode() / (*children[1]).evaluateNode();
	}
	else if (tag == TAGS[4]) eval = sin((*children[0]).evaluateNode());
	else if (tag == TAGS[5]) eval = cos((*children[0]).evaluateNode());
	else if (tag == TAGS[6]) for (int i = 0; i < childrenNumber; i++) eval += (*children[i]).evaluateNode();
	return eval;
}

void OperatorNode::assignValue(string varTag, int val) {

	for (int i = 0; i < childrenNumber; i++) children[i]->assignValue(varTag, val);
}

void OperatorNode::addChild(EquationNode* child, int index) { // ulepszyc modyfikacje z poprzednigo zadania

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

// VARIABLE NODE **************************************************************************



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
	children = new EquationNode * [childrenNumber];
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
