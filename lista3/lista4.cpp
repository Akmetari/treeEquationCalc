#include <iostream>
#include "TreeCalc.h"

int main(){

	// narzucic klase interfejsu, ktora rozpoczyna program od zapytania jaki typ drzew będziemy wpisywać, następnie go uruchsmia
	//do interfejsu drzewa dodac opcje, ktora wraca z int drzew typu T do interfaceu pytającego o typ interfaceu
	// lista interfaceów rożnych typów i możliwosc przełączania się miedzy nimi

	TreeCalc<int>::run();
}