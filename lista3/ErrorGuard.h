#pragma once
#include <string>

using namespace std;
const string MANY_PARAMS_MESSAGE = "Found too many parameters for an operator. Ignored the excess.\n";
const string LITTLE_PARAMS_MESSAGE = "Not enough parameters for an operator. Added default parameters.\n";
const string NO_EQUATION_MESSAGE = "No equation.";
const string INTERNAL_ERROR_MESSAGE = "Internal error. Please inform the developer. \n";
const string ERRORS_OCURED = " occured: ";
const string NEW_LINE = "\n";

class ErrorGuard
{
private:
	bool tooManyParams;
	bool tooLittleParams;
	bool noEquation;
	bool internalError;

	bool newErrorsOccured;

public:

	ErrorGuard();

	bool newErrors() {
		bool ret = newErrorsOccured;
		newErrorsOccured = false;
		return ret;
	}

	void raiseTooManyParams() { tooManyParams = true; newErrorsOccured = true; }
	void lowerTooManyParams() { tooManyParams = false; }

	void raiseTooLittleParams() { tooLittleParams= true; newErrorsOccured = true;}
	void lowerTooLittleParams() { tooLittleParams= false; }

	void raiseNoEquation() { noEquation = true; newErrorsOccured = true;}
	void lowerNoEquation() { noEquation = false; }

	void raiseInternalError() { internalError = true; newErrorsOccured = true;}
	void lowerInternalError() { internalError = false; }

	void clearFlags();

	string generateErrorList();
	
	
};

