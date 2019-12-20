#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <iostream>

using namespace std;

struct Message{
	long type;
	int Array[5];
};
struct Buf {
	struct msqid_ds msq_ds;
};

int ipckey = 0;
Message message;
Buf buf;

void messageOutput() {
	cout << "Type number: " << message.type << endl
		 << "Message:" << endl;
	for (int i = 0; i< 5; i++) {
		cout << message.Array[i] << ", ";
	}
	cout << "; " << endl;
}

void enterData() {
	cout << "Enter number of type: ";
	cin >> message.type;
	cout << endl << "Enter 5 numbers:" << endl;
	for (int i = 0; i < 5; i++) {
		cout << "array[" << i << "] = ";
		cin >> message.Array[i];
	}
}

int main(int argc, char** argv) {
	ipckey = ftok(".", 'a');
	int mq_id = msgget(ipckey, IPC_CREAT | 0666);
	enterData();
	int recSnd = msgsnd(mq_id, (struct msgbuf*)(&message), sizeof(message.Array), 0);

	cout << "Receiving reply...\nPress any key...";
	int a; cin >> a;
	
	ipckey = ftok(".", 'A');
	mq_id = msgget(ipckey, 0);
	for (int i = 0; i<5; i++) {
		int recRcv = msgrcv(mq_id, (struct msgbuf*)(&message), sizeof(message.Array), 0, 0);
		if (recRcv != -1){
			msgctl(mq_id, IPC_STAT, &(buf.msq_ds));
			messageOutput();
			break;
		}
		else{
			cout << "Error while receiving a message!" << endl;
		}
	}

	return 0;
}
