//==========================
//�������ƣ�ͨѶ¼ 
//�����ˣ�LinJ��Jie 
//�汾�ţ�1.0
//����ʱ�䣺2020.9.2
//�ϴ��޸�ʱ�䣺2020.9.5 18:49
//������������4
//==========================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void addCard(char**** cards);
void editCard(char*** cards);
void deleteCard(char*** cards);
void searchCards(char*** cards);
void showCards(char*** cards, int b, int e);
void saveCards(char*** cards, int mode, int n);
bool checkEmail(char*** cards,char* Email, int m);
void printTitle();
void exitSystem();
#define sendError(msg) {printf("[ERROR] %s\n", msg);exit(1);}
char notes[10][30] = {"�����������*", "������ƴ����д����LH��*", "ְ��", "��λ*", "�̶��绰", "�ֻ�", "E-mail*", "QQ", "΢�ź�", "��ע"};

char* id;
int CardsNum = 0;	// ����card���� 

int main() {
	char*** cards;
	char buf[200];
	FILE *fp = fopen("Cards.dat", "r");
	
	if ((cards = (char***) malloc(sizeof(char**))) == NULL) {
			sendError("��̬����洢�ռ�ʧ��");
		}
	cards[0] = NULL;
	if (fp == NULL) {
		/* Cards.dat�����ڣ����ȡid���½�  */
		printTitle();
		printf("�״����������������ע��ţ�ѧ�ţ���\n");
		
		scanf("%s", buf);
		if(buf == NULL) return 0;
		printf("\n���ڼ������Ժ�...\n\n");
		if ((id = (char*) malloc(strlen(buf))) == NULL) {
			sendError("��̬����洢�ռ�ʧ��");
		}
		strcpy(id, buf);
		
		// д��id��CardsNum 
		FILE *fp = fopen("Cards.dat", "w");
		fprintf(fp, "%s %d", id, CardsNum);
		
		// ���� 
		Sleep(500);
		system("cls");
	} else {
		/*  Cards���ڣ���ȡid��cards��Ϣ  */ 
		
		// ��ȡid 
		fscanf(fp, "%s %d", buf, &CardsNum);
		if ((id = (char*) malloc(2 * strlen(buf))) == NULL) {
			sendError("��̬����洢�ռ�ʧ��");
		}
		strcpy(id, buf);
		
		if ((cards = (char***) realloc(cards, (CardsNum + 1) * sizeof(char**))) == NULL) {
			sendError("��̬����洢�ռ�ʧ��");
		}
		// ��ȡcards
		for(int i = 0; i < CardsNum; i++) {
			// ������Ƭ�ռ�
			if ((cards[i] = (char**) malloc(10 * sizeof(char*))) == NULL) {
				sendError("��̬����洢�ռ�ʧ��");
			}
			
			//������Ƭ�ڸ��ֶοռ� 
			for (int j = 0; j < 10; j++) {
				fscanf(fp, "%s", buf);
				if ((cards[i][j] = (char*) malloc(sizeof(buf))) == NULL) {
					sendError("��̬����洢�ռ�ʧ��");
				}
				strcpy(cards[i][j], buf);
			}
		}
		cards[CardsNum] = NULL;	
	}
	
	fclose(fp);
		
	// ����ѡ��
	while(1) {
		printTitle();
		printf("��ӭ%s��������%d����Ƭ��\n\n", id, CardsNum);
		
		printf("������Ž��빦��ģ�飺\n\n");
		printf("[1] �����ϵ��  [2] �༭��ϵ��  [3] ɾ����ϵ��\n\n"
			"[4] ������ϵ��  [0] �˳�ϵͳ\n\n" 
		);
		
		scanf("%s", buf);
		switch(buf[0]) {
			case '0': exitSystem(); break;
			case '1': addCard(&cards); break;
			case '2': editCard(cards); break;
			case '3': deleteCard(cards); break;
			case '4': searchCards(cards); break;
			default: system("cls"); printf("������������������\n\n");
		}
	} 
}

/*  �½���Ƭ  */

void addCard(char**** cards) {
	system("cls");
	
	int i = 0;
	char buf[200];
	char*** cards1;
	cards1 = *cards; // ָ��cards��άָ��ĵ�ַ
	
	// ָ��Ѱ�ҽ��ղ�������λ�� 
  while (cards1[i] != NULL) {
  	i++;
	}
  
  if((cards1 = (char***) realloc(cards1, (i + 2) * sizeof(char**))) == NULL) {
		sendError("��̬����洢�ռ�ʧ��");
	}
	if((cards1[i] = (char**) malloc(10 * sizeof(char*))) == NULL) {
		sendError("��̬����洢�ռ�ʧ��");
	}
	
	printf("\nPS: * ��ʾ�����Ϊ��\n"); 
	fflush(stdin);	// ��������� 
	for(int j = 0; j < 10; j++) {
		printf("\n[%d/10] ��������Ҫ��ӵ�%s:\n", j + 1, notes[j]);
		memset(buf, 0, sizeof(buf));
		gets(buf);
		if(buf[0] == 0)
	  {
	  	if(j == 0 || j == 1 || j == 3 || j == 6) {
	  		j--;
	  		printf("[ERROR] ����Ϊ�������������!\n");
	  		continue;
			}
			strcpy(buf, "-1");
		} else if(j == 6 && !checkEmail(cards1, buf, -1)) {
			j--;
			printf("[ERROR] �������ѱ�����ռ�ã���һ����!\n");
			continue;
		}
		if((cards1[i][j] = (char*) malloc(sizeof(buf))) == NULL) {
			sendError("��̬����洢�ռ�ʧ��");
		}
		strcpy(cards1[i][j], buf);
	}
	cards1[i + 1] = NULL;
	*cards = cards1;

	saveCards(*cards, 1, -1);	// ������cards.dat 
	printf("\n[Success] ����ɹ�\n"
				"�����������....\n"
	);
	gets(buf);
	system("cls");
}

/*  �޸���Ƭ  */

void editCard(char*** cards) {
	system("cls");
	
	int i = 0,n,m;
	char buf[200];
	char **cards2;
	
	while(1) {
		showCards(cards, 0, CardsNum);
		
		printf("\n������\n"
					 "[1] ɾ��    [0] ������ҳ\n\n");
		fflush(stdin);	// ���������
		gets(buf);
		
		if(buf[0] == '0') {
			break;
		} else if(buf[0] == '1') {
			printf("\n��������Ҫ�޸ĵ���Ƭ��ţ�");
			scanf("%d",&n);
			
			if((cards2 = (char**) malloc(10 * sizeof(char*))) == NULL) {
				sendError("��̬����洢�ռ�ʧ��");
			}
			
			printf("\n[ע��] �������޸� %s ������\n\n", cards[n][0]);
			printf(
						"-----------------------------------------------\n"
						"PS:\n"
						"�� * ��ʾ������޸ĳɿգ�\n"
						"�� �������޸�ĳ��������ֱ�Ӱ�ENTER����\n"
						"�� �������ĳ������������'-1'\n"
						"-----------------------------------------------\n\n"); 
			fflush(stdin);	// ��������� 
			for(int j = 0; j < 10; j++){
				printf("[%d/10] �޸�%s:  [ԭ����Ϊ %s]\n",j + 1, notes[j], cards[n][j]);
				memset(buf, 0, sizeof(buf));
				gets(buf);
				if(buf[0] != 0)
				{
					if(j == 6 && !checkEmail(cards, buf, n)) {
						j--;
						printf("[ERROR] �������ѱ�����ռ�ã���һ����!\n\n");
						continue;
					}
					if((cards2[j] = (char*) malloc(sizeof(buf))) == NULL) {
						sendError("��̬����洢�ռ�ʧ��");
					}
					strcpy(cards2[j],buf);
				} else {
					if((cards2[j] = (char*) malloc(sizeof(cards[n][j]))) == NULL) {
						sendError("��̬����洢�ռ�ʧ��");
					}
					strcpy(cards2[j], cards[n][j]);
				}
			}
			printf("\n%s�����ݼ����޸�Ϊ\n\n", cards[n][0]);
			printf("| ���|  ����  | ��д | ְ��|   ����   |  �̶��绰  |     �ֻ�    |       ����     |     QQ    |     ΢��  | ��ע |\n"
						 "-------------------------------------------------------------------------------------------------------------------\n");
			printf("| [%d] |%8s|%6s|%5s|%10s|%12s|%13s|%16s|%11s|%11s|%6s|\n", i, cards2[0], cards2[1], cards2[2], cards2[3], cards2[4], cards2[5], cards2[6], cards2[7], cards2[8], cards2[9]);
			
			
			printf("\n[ϵͳ����] �˲����޷��������Ƿ������\n"
						 "����������'1'�������޸�������'0'\n\n");
			scanf("%d",&m);
			if(m == 1){
			    cards[n]=cards2;
			    saveCards(cards, 1, n);
			    printf("\n[Success] �����ɹ�\n");
			} else {
				printf("\n������ȡ��\n");
			}
			
			printf("�����������...\n"	
			);
			fflush(stdin);	// ���������
			gets(buf);
			
			system("cls");
		} else {
			system("cls");
			
			printf("������������������...\n\n");
		}
	}
	
	system("cls");
}

/*  ɾ����Ƭ  */

void deleteCard(char*** cards) {
	system("cls");
	
	int i = 0, m, n;
	char buf[20];
	
	while(1) {
		showCards(cards, 0, CardsNum);
		
		printf("\n������\n"
					 "[1] ɾ��    [0] ������ҳ\n\n");
		fflush(stdin);	// ���������
		gets(buf);
		
		if(buf[0] == '0') {
			break;
		} else if(buf[0] == '1') {
			char **cards3;
			
			printf("\n��������Ҫɾ������Ƭ��ţ�");
			scanf("%d",&n);
			cards3 = cards[n];
			
			printf("\n[ϵͳ����] �˲�������Զɾ��%s���Ƿ������\n", cards3[0]);
			printf("           ����������'1'������ɾ��������'0'\n\n");
			
			scanf("%d",&m);
			if(m == 1) {
				strcpy(cards3[0],"-1");
				cards[n] = cards3;
				saveCards(cards, 2, -1);
				printf("[Success] �����ɹ�");
			} else {
				printf("��ȡ������...\n");
			}
			printf("�����������...");
			fflush(stdin);	// ���������
			gets(buf);
			
			system("cls");	
		} else {
			system("cls");
			printf("������������������...\n\n");
		}	
	}
	system("cls");	
}

/*  ������Ƭ  */

void searchCards(char*** cards) {
	system("cls");
	
	int i = 0, j = CardsNum;
	char buf[20];
	while(1) {
		showCards(cards, i, j);

		printf("\n������\n"
					 "[1] �鿴ȫ��    [2] ģ������    [0] ������ҳ\n\n");
		fflush(stdin);	// ���������
		gets(buf);
		
		if(buf[0] == '0') {
			break;
		} else if(buf[0] == '1'){
			i = 0;
			j = CardsNum;
		} else if(buf[0] == '2') {
			printf("�����������ؼ���(����ƴ����д����LH)\n");
			scanf("%s", buf);
			int b = 0;
			while(b < CardsNum && strncmp(cards[b][1], buf, strlen(buf))) {
				b++;
			}
			i = b;
			while(b < CardsNum && strncmp(cards[b][1], buf, strlen(buf)) == 0) {
				b++;
			}
			j = b;
		}
		
		printf("\n[Success] ��ѯ�ɹ�\n"
					"�����������...\n"	
		);

		system("cls");
	}
	system("cls");
	
}

/*  ��ʾ��Ƭ  */

void showCards(char*** cards, int b, int e) {
	int i = 0;
	
	printf("\n��ǰ��ѯ��%d����¼\n\n", e-b);
	printf("| ���|  ����  | ��д | ְ��|   ����   |  �̶��绰  |     �ֻ�    |       ����     |     QQ    |     ΢��  | ��ע |\n"
				 "-------------------------------------------------------------------------------------------------------------------\n");
	for(int i = b; i < e; i++) {
		printf("| [%d] |%8s|%6s|%5s|%10s|%12s|%13s|%16s|%11s|%11s|%6s|\n", i, cards[i][0], cards[i][1], cards[i][2], cards[i][3], cards[i][4], cards[i][5], cards[i][6], cards[i][7], cards[i][8], cards[i][9]);
	}
	
}

/*  ������cards.dat  */

void saveCards(char*** cards, int mode, int n) {
	/*  mode ��ʾ����ģʽ

			0 ��ʾ�����浽�ļ�
			1 �����ļ�ǰ�Ƚ��������������������card
			2 �����ļ�ǰ��ɾ������Ϊ-1���û�������ɾ��
			
			n �����޸�cardʱ���������� 
	*/
	int i = 0, j = n == -1? CardsNum : n;
	if(mode == 1) {
		// ����Ѱ��������cardӦ���������λ�� 
		while(i < CardsNum && strcmp(cards[i][1], cards[j][1]) <= 0) {
			i++;
		}
		
		// ������ԭλ��ִ����λ���� 
		
		// i ΪӦ�������λ�ã�jΪ���Ƶ�λ�� 
		if(i < j) {
			char** card;
			card = cards[j];
			while(i < j) {
				cards[j] = cards[j-1];
				j--;
			}
			cards[j] = card;
		} else if(i > j) {
			char ** card;
			card = cards[j];
			while(i > (j+1)) {
				cards[j] = cards[j+1];
				j++;
			}
			cards[j] = card;
		}
		
		if(n == -1) CardsNum += 1;
		
	} else if (mode == 2) {
		// ��ָ��������ɾ������ 
		while(strcmp(cards[i][0], "-1") != 0) {
			i++;
		}
		
		// ���������һ����ִ����λ���� 
		while(i < j) {
			cards[i] = cards[i + 1];
			i++;
		} 
		
		// ���·���cards�Ŀռ䣬��ɾ�����һλcard 
		if ((cards = (char***) realloc(cards, (CardsNum + 1) * sizeof(char**))) == NULL) {
			sendError("���·���洢�ռ�ʧ��");
		}
		cards[i] = NULL;
		
		CardsNum -= 1;
	} 
	
	// ������д��Cards.dat 
	FILE *fp = fopen("Cards.dat", "w");
	
	fprintf(fp, "%s %d", id, CardsNum); // д��ID��CardsNum 
	// ѭ��д��card
	for(int i = 0; i < CardsNum; i++) {
		fprintf(fp, "\n%s %s %s %s %s %s %s %s %s %s", cards[i][0], cards[i][1], cards[i][2], cards[i][3], cards[i][4], cards[i][5], cards[i][6], cards[i][7], cards[i][8], cards[i][9]);
	}
	
	fclose(fp);
}

/* ���Email�Ƿ��ظ� */
bool checkEmail(char*** cards,char* Email, int m){
	/* m�������ڼ���޸�������Ϣʱ������������ͬ�������� */
  int i;
	for(i = 0; i < CardsNum; i++){
		if(strcmp(cards[i][6], Email) == 0 && i != m) {
			break;	// ��鵽�ظ�����,ȥ��
		}
	}
	if(i != CardsNum) {
		return false;	
	}
	return true; 
}

/*  ��ӡ����  */
 
void printTitle() {
	printf(" ==================================================\n");
	printf("�U                                                 �U\n");
	printf("�U               ͨѶ¼�������                    �U\n");
	printf("�U                                                 �U\n");
	printf(" ==================================================\n\n");
}

/*  �˳�ϵͳ  */

void exitSystem() {
	system("cls");
	printTitle(); 
	printf("\n �ټ�(��_�� )...\n");
	exit(0);
}
