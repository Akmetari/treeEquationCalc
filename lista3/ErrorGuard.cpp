#include "ErrorGuard.h"

ErrorGuard::ErrorGuard() {
	tooManyParams=false;
	tooLittleParams = false;
	noEquation = false;
	internalError = false;
	newErrorsOccured = false;
}

void ErrorGuard::clearFlags() {
	tooManyParams = false;
	tooLittleParams = false;
	noEquation = false;
	internalError = false;

	newErrorsOccured = false;
}


string ErrorGuard::generateErrorList() {
	int numberOfErrors= tooManyParams+ tooLittleParams + noEquation + internalError;
	string errorList = to_string(numberOfErrors)+ ERRORS_OCURED;
	if (tooManyParams) errorList += NEW_LINE+MANY_PARAMS_MESSAGE;
	if (tooLittleParams) errorList += NEW_LINE + LITTLE_PARAMS_MESSAGE;
	if (noEquation) errorList += NEW_LINE + NO_EQUATION_MESSAGE;
	if (internalError) errorList += NEW_LINE + INTERNAL_ERROR_MESSAGE;

	return errorList;
}
