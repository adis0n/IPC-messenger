#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
struct id{
	long type;
	int id;
};
struct message{
	long type;
	char text[1024];
};

struct passy{
	long type;
	char login[10];
	char haslo[10];
};

struct uzytkownik{
	char login[10];
	char haslo[10];
	int grupa[3];
	int zalogowany;
	int msgid;
	int wiadomosci;
};
struct uzytkownik uzytkownicy[9];
struct message instrukcja;
void init(){
	int dane = open("dane.txt", O_RDWR);
	char znak;
	int index;
	for (int i=0; i<9; i++){
		index=0;
		uzytkownicy[i].msgid=i+40;
		msgctl(uzytkownicy[i].msgid, IPC_RMID, NULL);
		uzytkownicy[i].grupa[0]=0;
		uzytkownicy[i].grupa[1]=0;
		uzytkownicy[i].grupa[2]=0;
		uzytkownicy[i].zalogowany=0;
		uzytkownicy[i].wiadomosci=0;
		for (int z=0; z<11; z++){
			uzytkownicy[i].login[z]='\0';
			uzytkownicy[i].haslo[z]='\0';
		}
		while(read(dane,&znak,1)>0){

			if (znak!=' '){
				uzytkownicy[i].login[index]=znak;
				index++;
				}
			if (znak==' '){
				break;
				}
			}
		index = 0;
		while(1){
			read(dane,&znak,1);
			if (znak!=' '){
				uzytkownicy[i].haslo[index]=znak;
				index++;
				}
			if (znak==' '){
				break;
				}
			}
		while(1){
			read(dane,&znak,1);
			if (znak == '0'){
				uzytkownicy[i].grupa[0]=1;
			}
			if (znak == '1'){
				uzytkownicy[i].grupa[1]=1;
			}
			if (znak == '2'){
				uzytkownicy[i].grupa[2]=1;
			}
			if (znak == '\n'){
				break;
			}
		}
	}
	}
void login(){
printf("KTOS SIE LOGUJE\n");
int IDlogin = msgget(667,0644 | IPC_CREAT);
struct passy log;
msgrcv(IDlogin,&log,sizeof(log)-sizeof(long),2,0);
struct message out;
out.type=3;
int git=0;
int ktory=0;
struct id myid;
for (int i=0; i<9; i++){
	if (strcmp(uzytkownicy[i].login,log.login) == 0 && strcmp(uzytkownicy[i].haslo,log.haslo)==0){
		if (uzytkownicy[i].zalogowany==0){
		out.text[0]='1';
		ktory=i;
		printf("Pomyslne logowanie uzytkownika %s\n",uzytkownicy[i].login);
		msgsnd(IDlogin,&out,sizeof(out)-sizeof(long),0);
		git=1;
		uzytkownicy[i].zalogowany=1;
		myid.id=uzytkownicy[i].msgid;
		myid.type=4;
		break;
		}
		if (uzytkownicy[i].zalogowany==1){
		out.text[0]='2';
		msgsnd(IDlogin,&out,sizeof(out)-sizeof(long),0);
		printf("Nieudane logowanie - uzytkownik juz byl zalogowany\n");
		break;
		}
		
	
	}
	if (i==8){
		out.text[0]='3';
		msgsnd(IDlogin,&out,sizeof(out)-sizeof(long),0);
		printf("Niepoprawne dane logowania\n");
		break;
		}
}
if (git==1){
	out.type=5;
	strcpy(out.text,uzytkownicy[ktory].login);
	printf("Przesylam dane do uzytkownika %s\n",uzytkownicy[ktory].login);
	printf("%d",myid.id);
	msgsnd(IDlogin,&out,sizeof(out)-sizeof(long),0);
	
	msgsnd(IDlogin,&myid,sizeof(myid)-sizeof(long),0);
	}
}
void logout(){
	int id=msgget(333,0664 | IPC_CREAT);
	struct message wiad;
	struct id intem;
	msgrcv(id,&intem,sizeof(intem)-sizeof(long),5,0);
	printf("%d",intem.id);
	if (uzytkownicy[intem.id].zalogowany==1){
		printf("GIT");
		uzytkownicy[intem.id].zalogowany=0;
		wiad.text[0]='1';
		}
	else{
		printf("NIE GIT");
		wiad.text[0]='0';
		}
	wiad.type=6;
	msgsnd(id,&wiad,sizeof(wiad)-sizeof(long),0);
}
	/*(for (int i=0; i<9; i++){
	if (strcmp(uzytkownicy[i].login,dane[0]) == 0 && strcmp(uzytkownicy[i].haslo,dane[1])==0){
		if (uzytkownicy[i].zalogowany==1){
		uzytkownicy[i].zalogowany==0;
		}
		if (uzytkownicy[i].zalogowany==0){
		printf("Uzytkownik nie jest zalogowany!");
		}
	}
}*/

void showlogged(){
	int ID2 = msgget(21, 0644 | IPC_CREAT);
	struct message wiad1;
	wiad1.type=4;
	strcpy(wiad1.text," ");
	for (int i=0; i<9; i++){
		if (uzytkownicy[i].zalogowany==1){

			strncat(wiad1.text, uzytkownicy[i].login, 10);
			strncat(wiad1.text,", ",2);
		}
	}
	wiad1.type=6;
	msgsnd(ID2,&wiad1,sizeof(wiad1)-sizeof(long),0);
}

void joingroup(){
	int kolejka=msgget(222,0664 | IPC_CREAT);
	struct message wiad;
	msgrcv(kolejka,&wiad,sizeof(wiad)-sizeof(long),7,0);
	int user=wiad.text[0];
	int numer=wiad.text[1];
	if (uzytkownicy[user].grupa[numer]==0){
		uzytkownicy[user].grupa[numer]=1;
		int dane = open("dane.txt", O_RDWR);
		int a,aa,aaa;
		char znak;
		a=uzytkownicy[user].grupa[0];
		aa=uzytkownicy[user].grupa[1];
		aaa=uzytkownicy[user].grupa[2];
		int linia=0;
		while (linia!=user){
			read(dane,&znak,1);
			if (znak=='\n'){
				linia++;
				}
			}
		int spacje = 0;
		while(read(dane,&znak,1)>0){

			if (znak == '\0'){
				lseek(dane,-1,SEEK_CUR);
			}
			if (znak == ' '){
				spacje++;
				}
			if (spacje == 2){
				break;
				}
		}
	/*	if (a==1){

			write(dane,"0",1);
		}
		if (aa==1){
			write(dane,"1",1);
		}
		if (aaa==1){
			write(dane,"2",1);
		}*/
		wiad.text[0]=9;
		wiad.type=8;
		msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);
		close(dane);
		
}
		else
		{
		wiad.type=8;
		wiad.text[0]=-1;
msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);
		}
}

void leavegroup(){
	int kolejka=msgget(223,0664 | IPC_CREAT);
	struct message wiad;
	msgrcv(kolejka,&wiad,sizeof(wiad)-sizeof(long),7,0);
	int user=wiad.text[0];
	int numer=wiad.text[1];
	if (uzytkownicy[user].grupa[numer]==1){
		uzytkownicy[user].grupa[numer]=0;
		int dane = open("dane.txt", O_RDWR);
		int a,aa,aaa;
		char znak;
		a=uzytkownicy[user].grupa[0];
		aa=uzytkownicy[user].grupa[1];
		aaa=uzytkownicy[user].grupa[2];
		int linia=0;
		while (linia!=user){
			read(dane,&znak,1);
			if (znak=='\n'){
				linia++;
				}
			}
		int spacje = 0;
		while(read(dane,&znak,1)>0){

			if (znak == '\0'){
				lseek(dane,-1,SEEK_CUR);
			}
			if (znak == ' '){
				spacje++;
				}
			if (spacje == 2){
				break;
				}
		}
	/*	write(dane,"   ",3);
		lseek(dane,-3,SEEK_CUR);
		if (a==1){

			write(dane,"0",1);
		}
		if (aa==1){
			write(dane,"1",1);
		}
		if (aaa==1){
			write(dane,"2",1);
		}

		lseek(dane,0,SEEK_SET);
		while(read(dane,&znak,1)>0){
			if (znak != '\0'){
			lseek(dane,-1,SEEK_CUR);
			write(dane,&znak,1);
			lseek(dane,-1,SEEK_CUR);
			}
		}*/
		close(dane);
	wiad.text[0]=9;
		wiad.type=8;
		msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);
		
}
		else
		{
		wiad.type=8;
		wiad.text[0]=-1;
		msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);
		}
}

void showgroups(){
	struct message wiad;
	strcpy(wiad.text,"");
	int id=msgget(601,0664 | IPC_CREAT);
	for (int j=0; j<3; j++){
		strncat(wiad.text,"\n",1);
	for (int i=0; i<9; i++){
		if (uzytkownicy[i].grupa[j]==1){
			strncat(wiad.text,uzytkownicy[i].login,10);
			strncat(wiad.text," ",1);
		}
	}
}
	wiad.type=9;
	msgsnd(id,&wiad,sizeof(wiad)-sizeof(long),0);

}



void grouptouser(){
	int plik;
	char str[1024];
	strcpy(str," ");
	struct message wiad;
	int id=msgget(1337,0664 | IPC_CREAT);
	msgrcv(id,&wiad,sizeof(wiad)-sizeof(long),4,0);
	int grupa=wiad.text[0];
	int user=wiad.text[1];
	int git=0;
	if (grupa==0 && uzytkownicy[user].grupa[0]==1){
		plik=open("0.txt", O_RDONLY);
		git=1;
		}
	if (grupa==1 && uzytkownicy[user].grupa[1]==1){
		plik=open("1.txt", O_RDONLY);
		git=1;
		}
	if (grupa==2 && uzytkownicy[user].grupa[2]==1){
		plik=open("2.txt", O_RDONLY);
		git=1;
	}
	char znak;
	if (git==1){
	while(read(plik,&znak,1)>0){
		strncat(str,&znak,1);
	}
	strcpy(wiad.text,str);
	wiad.type=3;
	msgsnd(id,&wiad,sizeof(wiad)-sizeof(long),0);
	}
	else{
		wiad.text[0]=-1;
		wiad.type=3;
		msgsnd(id,&wiad,sizeof(wiad)-sizeof(long),0);
}
}
void usertogroup(){
	int plik;
	int kolejka=msgget(420,0664 | IPC_CREAT);
	struct message wiad;
	msgrcv(kolejka,&wiad,sizeof(wiad)-sizeof(long),3,0);
	int user=wiad.text[0];
	int grupa=wiad.text[1];
	msgrcv(kolejka,&wiad,sizeof(wiad)-sizeof(long),4,0);
	if (uzytkownicy[user].grupa[grupa] == 0){
		printf("Nie nalezy do tej grupy");
		wiad.text[0]=-1;
		wiad.type=2;
		msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);
		return;
	}
	if (grupa==0){
		plik=open("0.txt", O_WRONLY);
		}
	if (grupa==1){
		plik=open("1.txt", O_WRONLY);
		}
	if (grupa==2){
		plik=open("2.txt", O_WRONLY);
	}
	lseek(plik,0,SEEK_END);
	char userr[10];
	strcpy(userr,uzytkownicy[user].login);
	write(plik,userr, 10);
	write(plik,": ",2);
	int i=0;
	char msg[1024];
	strcpy(msg,wiad.text);
	while(msg[i] != '\n' && msg[i] !='\0')
	{
	write(plik,&msg[i],1);
	i++;
	}
	write(plik,"\n",1);
	close(plik);
	wiad.text[0]=1;
	wiad.type=2;
	msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);

}
void showmygroups(int user){
	printf("%d%d%d",uzytkownicy[user].grupa[0],uzytkownicy[user].grupa[1],uzytkownicy[user].grupa[2]);
}
void przekazwiadomosc(){
	int id=msgget(1001,0664 | IPC_CREAT);
	struct message wiad;
	msgrcv(id,&wiad,sizeof(wiad)-sizeof(long),1,0);
	uzytkownicy[wiad.text[1]].wiadomosci++;
	}
void przekazwiadomosc2(){
	struct message wiad;
	
	int id2=msgget(1002,0664 | IPC_CREAT);	
	msgrcv(id2,&wiad,sizeof(wiad)-sizeof(long),7,0);
	wiad.text[0]=uzytkownicy[wiad.text[0]].wiadomosci;
	wiad.type=2;
	msgsnd(id2,&wiad,sizeof(wiad)-sizeof(long),0);
}
	/*struct message wiad;
	struct id intem;
	int kolejka = msgget(668,0644 | IPC_CREAT);
	msgrcv(kolejka,&intem,sizeof(wiad)-sizeof(long),3,0);
	int odkogo=intem.id;
	msgrcv(kolejka,&intem,sizeof(wiad)-sizeof(long),1,0);
	int dokogo=intem.id;
	msgrcv(kolejka,&wiad,sizeof(wiad)-sizeof(long),2,0);
	printf("test");
	int kolejka2=msgget(uzytkownicy[dokogo].msgid,0644 | IPC_CREAT);
	char s[1024];
	strcpy(s,uzytkownicy[odkogo].login);
	strncpy(s,": ",2);
	strncpy(s,wiad.text,1000);
	wiad.type=7;
	msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);
*/

			
void getinstruction(){
	int ID = msgget(666,0644 | IPC_CREAT);
	if (msgrcv(ID,&instrukcja, sizeof(instrukcja)-sizeof(long),1,0)>0){
}
	if (instrukcja.text[0] == '1'){
		login();
	}
	if (instrukcja.text[0]=='2'){
		showlogged();
	}
	if (instrukcja.text[0]=='3'){
		showgroups();
	}
	if (instrukcja.text[0]=='4'){
		logout();
	}
	if (instrukcja.text[0]=='5'){
		przekazwiadomosc();
	}
	if (instrukcja.text[0]=='6'){
		przekazwiadomosc2();
	}
	if (instrukcja.text[0]=='7'){
		joingroup();
	}
	if (instrukcja.text[0]=='8'){
		leavegroup();
	}
	if (instrukcja.text[0]=='9'){
		grouptouser();
	}
	if (instrukcja.text[0]=='0'){
		usertogroup();
	}
}

	
void funkcja(){
	int a =msgget(667,0);
	msgctl(a,IPC_RMID,NULL);
	a =msgget(666,0);
	msgctl(a,IPC_RMID,NULL);
	a =msgget(333,0);
	msgctl(a,IPC_RMID,NULL);
	a =msgget(668,0);
	msgctl(a,IPC_RMID,NULL);
	a =msgget(601,0);
	msgctl(a,IPC_RMID,NULL);
	a =msgget(21,0);
	msgctl(a,IPC_RMID,NULL);
}

int main(){
	int IDloginn=msgget(667, 0644 | IPC_CREAT);
	funkcja();
	msgctl(IDloginn, IPC_RMID, NULL);
	init();
	for (int i=0;i<9;i++){
	printf("%s\t",uzytkownicy[i].login);
	printf("%s\t",uzytkownicy[i].haslo);
	printf("%d\t",uzytkownicy[i].grupa[0]);
	printf("%d\t",uzytkownicy[i].grupa[1]);
	printf("%d\n",uzytkownicy[i].grupa[2]);
	} 
	while(1){
	getinstruction();
	}
	
	return 0;
	}

//ipcrm -a
