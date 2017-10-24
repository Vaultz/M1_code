#include "library.h"

int compare(int userNumber, int randomN) {
	if (userNumber < randomN) 
		return -1;
	else if (userNumber > randomN) 
		return 1;
	else 
		return 0;
}
