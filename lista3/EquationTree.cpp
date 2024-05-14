#include "EquationTree.h"


EquationTree::EquationTree(){
	root = new EquationNode<T>();
	variables = {};
	guard = new ErrorGuard();
}

EquationTree::EquationTree(EquationTree& original){
	guard = new ErrorGuard();
	EquationNode<T>* pointer = original.root;
	if (pointer->isValue()) root = new ValueNode<T>(pointer);
	else if (pointer->isOperator()) root = new OperatorNode<T>(pointer);
	else if (pointer->isVariable()) root = new VariableNode<T>(pointer);
	equationString = original.equationString;
	root->findVars(&variables);
}

EquationTree::EquationTree(EquationNode<T>* root) {
	guard = new ErrorGuard();
	this->root = root;
	root->findVars(&variables);

}

EquationTree::EquationTree(string equationStr) {
	guard = new ErrorGuard();
	list<string> dividedEquation = divideStrBySpace(equationStr);
	list<string>::iterator strNodeIndex = dividedEquation.begin();
	root = createNode(*strNodeIndex);
	advance(strNodeIndex, 1);
	growTree(&root, &dividedEquation, strNodeIndex);

}


list<string> EquationTree::divideStrBySpace(string str) {
	istringstream divisor(str);
	list<string> dividedStr;
	string tmpStr;
	if (str == "") guard->raiseNoEquation();
	while (divisor >> tmpStr) dividedStr.push_back(tmpStr);
		
	return dividedStr;
}

EquationNode<T>* EquationTree::createNode(string strNode) {
	EquationNode<T>* node;
	bool isOperator = false;
	int numberOfVariables = 0;
	for (int i = 0; i < NUMBER_OF_OPERATOR_TYPES; i++) if (TAGS[i] == strNode) isOperator = true;

	if (strNode[0] > ASCII_0 && strNode[0] < ASCII_9) node = new ValueNode<T>(stoi(strNode));
	else if (isOperator) node =new OperatorNode<T>(strNode);
	else {
		node = new VariableNode<T>(strNode);
		if (!containsVar(node->toString())) variables.push_back(node);
	}
	return node;
}

bool EquationTree<T>::growTree(EquationNode<T>** startNode, list<string>* dividedEquation, list<string>::iterator i) {
	while ((**startNode).getChildrenNumber() != (**startNode).getExpectedChildrenNumber() && i!= dividedEquation->end()) {
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


EquationTree<T>::~EquationTree<T>(){
	delete root;
	delete guard;
}



void EquationTree<T>::tree2Str(EquationNode<T>* rootNode, string* str) {
	*str+= (*rootNode).toString();
	for (int i = 0; i < (*rootNode).getChildrenNumber(); i++) {
		tree2Str((*rootNode).getChildren()[i], str);
	}
}

string EquationTree<T>::toString() {
	string treeStr = "";
	tree2Str(root, &treeStr);
	return treeStr;
}

string EquationTree<T>::vars2String() {
	string vars = "";
	for (int i = 0; i < variables.size(); i++) vars+= ((VariableNode<T>*)variables.at(i))->toStringVal() + NEW_LINE;
	return vars;
}



double EquationTree::evaluate() {
	return (*root).evaluateNode();
}

void EquationTree<T>::operator=(const EquationTree<T>& originTree) {
	EquationNode<T>* pointer = originTree.root;
	if (pointer->isValue()) root = new ValueNode<T>(pointer);
	else if (pointer->isOperator()) root = new OperatorNode<T>(pointer);
	else if (pointer->isVariable()) root = new VariableNode<T>(pointer);
	equationString = originTree.equationString;
	root->findVars(&variables);
}

EquationTree<T> EquationTree<T>::operator+(EquationTree<T>& originTree) {
	EquationTree copyTree(*this);
	if (variables.size() > 0) {
		EquationNode* joint = copyTree.variables.at(0)->getParent();
		EquationNode* newBranch = nullptr;

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

EquationTree* EquationTree::operator+(EquationTree* originTree) {
	EquationTree* copyTree = new EquationTree(*this);
	if (variables.size() > 0) {
		EquationNode* joint = copyTree->variables.at(0)->getParent();
		EquationNode* newBranch = nullptr;

		if (originTree->getRoot()->isValue()) newBranch = new NumberNode(originTree->getRoot());
		else if (originTree->getRoot()->isOperator()) newBranch = new OperatorNode(originTree->getRoot());
		else if (originTree->getRoot()->isVariable()) newBranch = new VariableNode(originTree->getRoot());
		newBranch->setParent(joint);

		int index = joint->deleteChild(&copyTree->variables.at(0));
		joint->addChild(newBranch, index);
		copyTree->root->findVars(&(copyTree->variables));
	}
	return copyTree;
}


bool EquationTree::containsVar(string varTag) {
	bool ret = false;
	for (int i = 0; i < variables.size(); i++) {
		ret = ret || ((VariableNode*)variables.at(i))->getTag() == varTag;
	}
	return ret;
}

void EquationTree::assignValueToVar(string varTag, int value) {
	if (containsVar(varTag)) {
		root->assignValue(varTag, value);
	}
}




