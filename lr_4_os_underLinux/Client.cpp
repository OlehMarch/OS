#include "signal.h"
//#include "sys/type.h"
#include "sys/stat.h"
#include "unistd.h"
#include "fcntl.h"
#include "iostream"

using namespace std;



int main()
{
	int descriptor = creat("MyFile.txt", 0666);
	if (descriptor == -1)
	{
		cout << "file haven't been created!\n";
		return 0;
	}
	char* charStr = new char[255];
	cout << "Insert string:\n";
	cin >> charStr;
	int count = write(descriptor, charStr, sizeof(charStr));
	if (count == -1)
	{
		cout << "Writing operation failed!\n";
		return 0;
	}
	else if (count == 0)
	{
		cout << "Write to file nothing!\n";
		return 0;
	}
	int closingStatus = close(descriptor);
	if (closingStatus == -1)
	{
		cout << "Closing operation failed!\n";
		return 0;
	}
	signal(SIGUSR1, FunStr);
	int error = kill(getppid(), SIGUSR1);
	if (error == -1)
	{
		cout << "Killing operation failed!\n";
		return 0;
	}
}
