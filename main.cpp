
#include "String.h"
#include <string>

int main() {
	String a = "tttttttttt";
	String b = "tttttttttt";
	//a += "test";
	std::cout << (a + b).length() << std::endl;

	return 0;
}
