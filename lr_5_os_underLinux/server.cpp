#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <sys/ipc.h>

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

void infoOutput() {
	cout << "Current byte quantity in queue: " 			<< buf.msq_ds.__msg_cbytes << endl
		 << "Message quantity in queue: " 				<< buf.msq_ds.msg_qnum << endl
		 << "Max allowable byte quantity in queue: " 	<< buf.msq_ds.msg_qbytes << endl
		 << "Last sender id: " 							<< buf.msq_ds.msg_lspid << endl
		 << "Last recipient id: " 						<< buf.msq_ds.msg_lrpid << endl
		 << "Time of the last sending: " 				<< buf.msq_ds.msg_stime << endl
		 << "Time of the last receiving: " 				<< buf.msq_ds.msg_rtime << endl
		 << "Time of the last changing: " 				<< buf.msq_ds.msg_ctime << endl;
}


int main() {
	ipckey = ftok(".", 'a');
	int mq_id = msgget(ipckey, 0);

	for (int i = 0; i<5; i++) {
		int recRcv = msgrcv(mq_id, (struct msgbuf*)(&message), sizeof(message.Array), 0, 0);
		if (recRcv != -1){
			msgctl(mq_id, IPC_STAT, &(buf.msq_ds));
			messageOutput();
			infoOutput();
			break;
		}
		else{
			cout << "Error while receiving a message!" << endl;
		}
	}

	ipckey = ftok(".", 'A');
	mq_id = msgget(ipckey, IPC_CREAT | 0666);
	for (int i = 0; i < 5; ++i) {
		message.Array[i]++;
	}
	int recSnd = msgsnd(mq_id, (struct msgbuf*)(&message), sizeof(message.Array), 0);
	//int a;
	//cin >> a;
	return 0;
}
