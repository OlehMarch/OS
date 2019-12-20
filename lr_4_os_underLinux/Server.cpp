#include "signal.h"
//#include "sys/type.h"
#include "sys/wait.h"
#include "unistd.h"
#include "fcntl.h"
#include "iostream"

using namespace std;

char* charStr = new char[255];

void FunStr(int signum)
{
	if (signum == SIGUSR1)
	{
		cout << "Received SIGUSR1!\n";
		
		int descriptor = open("MyFile.txt", O_RDWR);
		if (descriptor == -1)
		{
			cout << "Opening operation failed!\n";
			return;
		}
		int count = read(descriptor, charStr, sizeof(charStr));
		if (count == -1)
		{
			cout << "Reading operation failed!\n";
			return;
		}
		int closingStatus = close(descriptor);
		if (closingStatus == -1)
		{
			cout << "Closing operation failed!\n";
			return;
		}
	}
	
}

int main(int argc, char **argv)
{
	char** str = new char*[1];
	str[0] = new char[255];
	int status;
	
	str[0] = charStr;
	int processStatus = fork();
	if (processStatus == 0)
	{
		execvp(*str, str);
	}
	else if (processStatus > 0)
	{
		waitpid(processStatus, &status, WUNTRACED | WCONTINUED);
	}
	else if (processStatus < 0)
	{
		cout << "Creating process failed!\n";
	}
	cout << "output:\n";
	cout << str[0];
	int a;
	cin >> a; 
}
