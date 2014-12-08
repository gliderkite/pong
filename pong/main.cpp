#include "Pong.h"
using namespace pong;

#include <iostream>
using namespace std;


int main()
{
	try
	{
		Pong pong;
		pong.run();
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		cin.get();
	}
	
	return 0;
}