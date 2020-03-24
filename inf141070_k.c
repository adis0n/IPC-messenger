#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <stdio_ext.h>
int dalej = 1;
int ja;
char jaa;
struct wiad{
	long type;
	int od;
	int dla;
	char text[1024];
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
struct id{
	long type;
	int id;
};
struct message w;
int logowanie(){
	int ID = msgget(666,0644 | IPC_CREAT);
	struct message wiad;
	wiad.text[0]='1';
	wiad.type=1;
	msgsnd(ID,&wiad,sizeof(wiad)-sizeof(long),0);
	struct passy log;
	printf("PODAJ LOGIN\n\n");
	scanf("%s",log.login);
	printf("PODAJ HASLO\n\n");
	scanf("%s",log.haslo);
	log.type=2;
	int ID2 = msgget(667,0644 | IPC_CREAT);
	msgsnd(ID2,&log,sizeof(log)-sizeof(long),0);
	msgrcv(ID2,&wiad,sizeof(wiad)-sizeof(long),3,0);
	if (wiad.text[0]=='1'){
		printf("Pomyslne logowanie!\n\n\n");
		msgrcv(ID2,&wiad,sizeof(wiad)-sizeof(long),5,0);
		printf("Witaj, %s!\n",wiad.text);
		struct id myid;
		msgrcv(ID2,&myid,sizeof(myid)-sizeof(long),4,0);
		ja=myid.id-40;
		int mojeID=msgget(myid.id, 0644 | IPC_CREAT);
		int zalogowany=1;
		return mojeID;
	}
	if (wiad.text[0]=='2'){
		printf("Ten uzytkownik jest juz zalogowany!\n\n\n");
		return -1;
	}
	if (wiad.text[0]=='3'){
		printf("Dane niepoprawne!\n\n\n");
		return -1;
	}

	}
void show(){
	int id=msgget(21,0644 | IPC_CREAT);
	int instrukcja=msgget(666,0644 | IPC_CREAT);
	struct message instrukcjaa;
	instrukcjaa.type=1;
	instrukcjaa.text[0]='2';
	msgsnd(instrukcja,&instrukcjaa,sizeof(instrukcjaa)-sizeof(long),0);
	struct message wiad;
	msgrcv(id,&wiad,sizeof(wiad)-sizeof(long),6,0);
	printf("ZALOGOWANI UZYTKOWNICY\n\n");
	printf("%s\n",wiad.text);
	}
/*void wiad(){
	printf("Do kogo?\n1.adis0n\n2.radziu\n3.kacper\n4.marcin\n5.janko\n6.login\n7.super\n8.marek\n9.haslo\n\n\n");
	struct message mess;
	struct id intem;
	int instrukcja=msgget(666,0644 | IPC_CREAT);
	mess.type=1;
	mess.text[0]='5';
	msgsnd(instrukcja,&mess,sizeof(mess)-sizeof(long),0);
	intem.id=ja;
	intem.type=3;
	int id=msgget(668,0644 | IPC_CREAT);
	msgsnd(id,&intem,sizeof(intem)-sizeof(long),0);
	scanf("%d",&intem.id);
	intem.id=intem.id-1;
	int dokogo=intem.id+40;
	int kolejka=msgget(dokogo,0644 | IPC_CREAT);
	mess.type=1;
	msgsnd(id,&intem,sizeof(intem)-sizeof(long),0);
	printf("Wiadomosc:\n");
	scanf("%s",mess.text);
	mess.type=2;
	msgsnd(id,&wiad,sizeof(wiad)-sizeof(long),0);
	printf("WIADOMOSC WYSLANA!\n\n");
	
}*/
void wiad(){
	printf("Do kogo?\n1.adis0n\n2.radziu\n3.kacper\n4.marcin\n5.janko\n6.login\n7.super\n8.marek\n9.haslo\n\n\n");
	int dokogo=0;
	scanf("%d",&dokogo);
	int idd=dokogo+39;
	int kolejka=msgget(idd,0664 | IPC_CREAT);
	struct wiad w;
	w.od=ja;
	w.dla=dokogo;
	printf("Wiadomosc:\n");
	char pomoc[1024];
	char jaaa[1];
	sprintf(jaaa,"%d",ja);
	
	strcpy(w.text,"Uzytkownik o id ");
	pomoc[0]=ja;
	strncat(w.text,jaaa,1);
	strncat(w.text," napisal\n",10);
	scanf("%s",pomoc);
	strncat(w.text,pomoc,1000);
	w.type=6;
	msgsnd(kolejka,&w,sizeof(w)-sizeof(long),0);
}
	
void odbierz(){
	int kolejka=msgget(ja+40,0664 | IPC_CREAT);
	struct wiad w;
	msgrcv(kolejka,&w,sizeof(w)-sizeof(long),6,IPC_NOWAIT);
	printf("%s\n\n",w.text);
}
	


	

void groups(){
	int ID = msgget(666,0664 | IPC_CREAT);
	struct message wiad;
	wiad.type=1;
	wiad.text[0]='3';
	int kolejka=msgget(601,0664 | IPC_CREAT);
	msgsnd(ID,&wiad,sizeof(wiad)-sizeof(long),0);
	msgrcv(kolejka,&wiad,sizeof(wiad)-sizeof(long),9,0);
	printf("%s\n\n",wiad.text);
	}
void logout(){
	int instrukcja=msgget(666,0644 | IPC_CREAT);
	struct message instrukcjaa;
	instrukcjaa.type=1;
	instrukcjaa.text[0]='4';
	msgsnd(instrukcja,&instrukcjaa,sizeof(instrukcjaa)-sizeof(long),0);
	int id = msgget(333,0644 | IPC_CREAT);
	struct id intem;
	intem.type=5;
	intem.id=ja;
	msgsnd(id,&intem,sizeof(intem)-sizeof(long),0);
	msgrcv(id,&instrukcjaa,sizeof(instrukcja)-sizeof(long),6,0);
	printf("wylogowano pomyslnie!\n\n");
}
void joingroup(){
	int instrukcja=msgget(666,0664 | IPC_CREAT);
	struct message wiad;
	wiad.type=1;
	wiad.text[0]='7';
	msgsnd(instrukcja,&wiad,sizeof(wiad)-sizeof(long),0);
	int kolejka=msgget(222,0664 | IPC_CREAT);
	wiad.text[0]=ja;
	printf("\n\nDO KTOREJ GRUPY CHCESZ DOLACZYC?\n\n");
	int pomoc;
	scanf("%d",&pomoc);
	wiad.text[1]=pomoc;
	msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);


	wiad.type =7;
	msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);
	msgrcv(kolejka,&wiad,sizeof(wiad)-sizeof(long),8,0);
	if (wiad.text[0]!=-1){
		printf("DOLACZONO POMYSLNIE!!!\n\n");
	}
	else{
		printf("NALEZYSZ JUZ DO TEJ GRUPY \n");
	}
}
void leavegroup(){
	int instrukcja=msgget(666,0664 | IPC_CREAT);
	struct message wiad;
	wiad.type=1;
	wiad.text[0]='8';
	msgsnd(instrukcja,&wiad,sizeof(wiad)-sizeof(long),0);
	int kolejka=msgget(223,0664 | IPC_CREAT);
	wiad.text[0]=ja;
	printf("\n\nKTORA GRUPE CHCESZ OPUSCIC?\n\n");
	int pomoc;
	scanf("%d",&pomoc);
	wiad.text[1]=pomoc;
	msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);


	wiad.type =7;
	msgsnd(kolejka,&wiad,sizeof(wiad)-sizeof(long),0);
	msgrcv(kolejka,&wiad,sizeof(wiad)-sizeof(long),8,0);
	if (wiad.text[0]!=-1){
		printf("OPUSZCZONO POMYSLNIE!!!\n\n");
	}
	else{
		printf("NIE NALEZYSZ DO TEJ GRUPY \n");
	}
}
void grouptouser(){
	int instrukcja=msgget(666,0664 | IPC_CREAT);
	struct message wiad;
	wiad.type=1;
	wiad.text[0]='9';
	msgsnd(instrukcja,&wiad,sizeof(wiad)-sizeof(long),0);
	printf("Z KTOREJ GRUPY ODCZYTAC KONWERSACJE? (0-2)\n\n");
	int pomoc;
	scanf("%d",&pomoc);
	wiad.text[0]=pomoc;
	wiad.text[1]=ja;
	int id=msgget(1337,0664 | IPC_CREAT);
	wiad.type=4;
	msgsnd(id,&wiad,sizeof(wiad)-sizeof(long),0);	
	msgrcv(id,&wiad,sizeof(wiad)-sizeof(long),3,0);
	if (wiad.text[0] != -1){
	printf("ODCZYT KONWERSACJI:\n\n");
	printf("\n%s\n",wiad.text);
	}
	else{
		printf("NIE NALEZYSZ DO TEJ GRUPY!\n\n");
}
}

void usertogroup(){
	int instrukcja=msgget(666,0664 | IPC_CREAT);
	struct message wiad;
	wiad.type=1;
	wiad.text[0]='0';	
	msgsnd(instrukcja,&wiad,sizeof(wiad)-sizeof(long),0);
	int id=msgget(420,0664 | IPC_CREAT);
	wiad.type=3;
	wiad.text[0]=ja;
	printf("\n\nDO JAKIEJ GRUPY???\n\n");
	int pomoc;
	scanf("%d",&pomoc);
	wiad.text[1]=pomoc;
	msgsnd(id,&wiad,sizeof(wiad)-sizeof(long),0);
	printf("\n\nWIADOMOSC:\n\n");
	char dd[1024];
	scanf("%s",dd);
	strcpy(wiad.text,dd);
	wiad.type=4;
	msgsnd(id,&wiad,sizeof(wiad)-sizeof(long),0);
	msgrcv(id,&wiad,sizeof(wiad)-sizeof(long),2,0);
	if (wiad.text[0]==-1){
		printf("NIE NALEZYSZ DO TEJ GRUPY\n\n");
	}
	else{
		printf("WIADOMOSC WYSLANA\n\n");
	}

}
int main(){
	printf("\t\t\tmessenger by adis0n\n\n\n");
	int opcja;
	int a = 0;
	int zalogowany=0;
	while(dalej==1){
		w.text[0]=='0';
		printf("WYBIERZ OPCJE\n1:Logowanie\n2:Pokaz liste zalogowanych\n3.Pokaz grupy i ich czlonkow\n4.Wyloguj\n5.Wyslij wiadomosc do uzytkownika\n6.Odbierz wiadomosc prywatna\n7.Dolacz do grupy\n8.Opusc grupe\n9.Wyswietl konwersacje grupowa\n0.Napisz do grupy\n\n");
		scanf("%d",&opcja);
		if (opcja==1 && zalogowany==0){
			a= logowanie();
			if (a!=-1 && a!=0){
				zalogowany=1;
			}
		}
		else if (opcja==1 && zalogowany==1){
			printf("JESTES JUZ ZALOGOWANY\n\n");
		}

		else if (opcja==2){
			show();
		}
		else if (opcja==3){
			groups();
		}
		else if (opcja==4 && zalogowany==1){
			logout();
			zalogowany=0;
		}
		else if (opcja==4 && zalogowany==0){
			printf("NIE JESTES ZALOGOWANY!!!\n\n");
		}
		else if (opcja==5 && zalogowany==0){
			printf("NIE JESTES ZALOGOWANY!!!\n\n");
		}
		else if (opcja==5 && zalogowany==1){
			wiad();
		}
		else if (opcja==6 && zalogowany==1){
			odbierz();
		}
		else if (opcja==6 && zalogowany==0){
			odbierz();
		}
		else if (opcja==7 && zalogowany==1){
			joingroup();
		}
		else if (opcja==8 && zalogowany==1){
			leavegroup();
		}
		else if (opcja==9 && zalogowany==1){
			grouptouser();
		}
		else if (opcja==0 && zalogowany==1){
			usertogroup();
		}
		else
		{
		printf("PODANO NIEPOPRAWNA WARTOSC LUB TA OPERACJA WYMAGA ZALOGOWANIA\n\n");
		}

			
}}


