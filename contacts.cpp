//==========================
//程序名称：通讯录 
//制作人：LinJ，Jie 
//版本号：1.0
//创建时间：2020.9.2
//上次修改时间：2020.9.5 18:49
//总制作天数：4
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
char notes[10][30] = {"姓名（如李华）*", "姓名的拼音缩写（如LH）*", "职务", "单位*", "固定电话", "手机", "E-mail*", "QQ", "微信号", "备注"};

char* id;
int CardsNum = 0;	// 返回card数量 

int main() {
	char*** cards;
	char buf[200];
	FILE *fp = fopen("Cards.dat", "r");
	
	if ((cards = (char***) malloc(sizeof(char**))) == NULL) {
			sendError("动态申请存储空间失败");
		}
	cards[0] = NULL;
	if (fp == NULL) {
		/* Cards.dat不存在，则读取id并新建  */
		printTitle();
		printf("首次运行软件，请输入注册号（学号）：\n");
		
		scanf("%s", buf);
		if(buf == NULL) return 0;
		printf("\n正在加载请稍后...\n\n");
		if ((id = (char*) malloc(strlen(buf))) == NULL) {
			sendError("动态申请存储空间失败");
		}
		strcpy(id, buf);
		
		// 写入id与CardsNum 
		FILE *fp = fopen("Cards.dat", "w");
		fprintf(fp, "%s %d", id, CardsNum);
		
		// 清屏 
		Sleep(500);
		system("cls");
	} else {
		/*  Cards存在，读取id与cards信息  */ 
		
		// 读取id 
		fscanf(fp, "%s %d", buf, &CardsNum);
		if ((id = (char*) malloc(2 * strlen(buf))) == NULL) {
			sendError("动态申请存储空间失败");
		}
		strcpy(id, buf);
		
		if ((cards = (char***) realloc(cards, (CardsNum + 1) * sizeof(char**))) == NULL) {
			sendError("动态申请存储空间失败");
		}
		// 读取cards
		for(int i = 0; i < CardsNum; i++) {
			// 申请名片空间
			if ((cards[i] = (char**) malloc(10 * sizeof(char*))) == NULL) {
				sendError("动态申请存储空间失败");
			}
			
			//申请名片内各字段空间 
			for (int j = 0; j < 10; j++) {
				fscanf(fp, "%s", buf);
				if ((cards[i][j] = (char*) malloc(sizeof(buf))) == NULL) {
					sendError("动态申请存储空间失败");
				}
				strcpy(cards[i][j], buf);
			}
		}
		cards[CardsNum] = NULL;	
	}
	
	fclose(fp);
		
	// 功能选择
	while(1) {
		printTitle();
		printf("欢迎%s，您共有%d张名片！\n\n", id, CardsNum);
		
		printf("输入序号进入功能模块：\n\n");
		printf("[1] 添加联系人  [2] 编辑联系人  [3] 删除联系人\n\n"
			"[4] 搜索联系人  [0] 退出系统\n\n" 
		);
		
		scanf("%s", buf);
		switch(buf[0]) {
			case '0': exitSystem(); break;
			case '1': addCard(&cards); break;
			case '2': editCard(cards); break;
			case '3': deleteCard(cards); break;
			case '4': searchCards(cards); break;
			default: system("cls"); printf("输入有误，请重新输入\n\n");
		}
	} 
}

/*  新建名片  */

void addCard(char**** cards) {
	system("cls");
	
	int i = 0;
	char buf[200];
	char*** cards1;
	cards1 = *cards; // 指向cards三维指针的地址
	
	// 指针寻找接收插入数据位置 
  while (cards1[i] != NULL) {
  	i++;
	}
  
  if((cards1 = (char***) realloc(cards1, (i + 2) * sizeof(char**))) == NULL) {
		sendError("动态申请存储空间失败");
	}
	if((cards1[i] = (char**) malloc(10 * sizeof(char*))) == NULL) {
		sendError("动态申请存储空间失败");
	}
	
	printf("\nPS: * 表示该项不能为空\n"); 
	fflush(stdin);	// 清空输入流 
	for(int j = 0; j < 10; j++) {
		printf("\n[%d/10] 请输入需要添加的%s:\n", j + 1, notes[j]);
		memset(buf, 0, sizeof(buf));
		gets(buf);
		if(buf[0] == 0)
	  {
	  	if(j == 0 || j == 1 || j == 3 || j == 6) {
	  		j--;
	  		printf("[ERROR] 此项为必填，请重新输入!\n");
	  		continue;
			}
			strcpy(buf, "-1");
		} else if(j == 6 && !checkEmail(cards1, buf, -1)) {
			j--;
			printf("[ERROR] 该邮箱已被他人占用，换一个吧!\n");
			continue;
		}
		if((cards1[i][j] = (char*) malloc(sizeof(buf))) == NULL) {
			sendError("动态申请存储空间失败");
		}
		strcpy(cards1[i][j], buf);
	}
	cards1[i + 1] = NULL;
	*cards = cards1;

	saveCards(*cards, 1, -1);	// 保存至cards.dat 
	printf("\n[Success] 保存成功\n"
				"按任意键继续....\n"
	);
	gets(buf);
	system("cls");
}

/*  修改名片  */

void editCard(char*** cards) {
	system("cls");
	
	int i = 0,n,m;
	char buf[200];
	char **cards2;
	
	while(1) {
		showCards(cards, 0, CardsNum);
		
		printf("\n操作：\n"
					 "[1] 删除    [0] 返回主页\n\n");
		fflush(stdin);	// 清空输入流
		gets(buf);
		
		if(buf[0] == '0') {
			break;
		} else if(buf[0] == '1') {
			printf("\n请输入需要修改的名片编号：");
			scanf("%d",&n);
			
			if((cards2 = (char**) malloc(10 * sizeof(char*))) == NULL) {
				sendError("动态申请存储空间失败");
			}
			
			printf("\n[注意] 您正在修改 %s 的数据\n\n", cards[n][0]);
			printf(
						"-----------------------------------------------\n"
						"PS:\n"
						"① * 表示该项不能修改成空；\n"
						"② 若不想修改某项数据则直接按ENTER跳过\n"
						"③ 若想清空某项数据则输入'-1'\n"
						"-----------------------------------------------\n\n"); 
			fflush(stdin);	// 清空输入流 
			for(int j = 0; j < 10; j++){
				printf("[%d/10] 修改%s:  [原数据为 %s]\n",j + 1, notes[j], cards[n][j]);
				memset(buf, 0, sizeof(buf));
				gets(buf);
				if(buf[0] != 0)
				{
					if(j == 6 && !checkEmail(cards, buf, n)) {
						j--;
						printf("[ERROR] 该邮箱已被他人占用，换一个吧!\n\n");
						continue;
					}
					if((cards2[j] = (char*) malloc(sizeof(buf))) == NULL) {
						sendError("动态申请存储空间失败");
					}
					strcpy(cards2[j],buf);
				} else {
					if((cards2[j] = (char*) malloc(sizeof(cards[n][j]))) == NULL) {
						sendError("动态申请存储空间失败");
					}
					strcpy(cards2[j], cards[n][j]);
				}
			}
			printf("\n%s的数据即将修改为\n\n", cards[n][0]);
			printf("| 序号|  姓名  | 缩写 | 职务|   部门   |  固定电话  |     手机    |       邮箱     |     QQ    |     微信  | 备注 |\n"
						 "-------------------------------------------------------------------------------------------------------------------\n");
			printf("| [%d] |%8s|%6s|%5s|%10s|%12s|%13s|%16s|%11s|%11s|%6s|\n", i, cards2[0], cards2[1], cards2[2], cards2[3], cards2[4], cards2[5], cards2[6], cards2[7], cards2[8], cards2[9]);
			
			
			printf("\n[系统提醒] 此操作无法撤销，是否继续？\n"
						 "继续请输入'1'，放弃修改请输入'0'\n\n");
			scanf("%d",&m);
			if(m == 1){
			    cards[n]=cards2;
			    saveCards(cards, 1, n);
			    printf("\n[Success] 操作成功\n");
			} else {
				printf("\n操作已取消\n");
			}
			
			printf("按任意键继续...\n"	
			);
			fflush(stdin);	// 清空输入流
			gets(buf);
			
			system("cls");
		} else {
			system("cls");
			
			printf("输入有误，请重新输入...\n\n");
		}
	}
	
	system("cls");
}

/*  删除名片  */

void deleteCard(char*** cards) {
	system("cls");
	
	int i = 0, m, n;
	char buf[20];
	
	while(1) {
		showCards(cards, 0, CardsNum);
		
		printf("\n操作：\n"
					 "[1] 删除    [0] 返回主页\n\n");
		fflush(stdin);	// 清空输入流
		gets(buf);
		
		if(buf[0] == '0') {
			break;
		} else if(buf[0] == '1') {
			char **cards3;
			
			printf("\n请输入需要删除的名片编号：");
			scanf("%d",&n);
			cards3 = cards[n];
			
			printf("\n[系统提醒] 此操作将永远删除%s，是否继续？\n", cards3[0]);
			printf("           继续请输入'1'，放弃删除请输入'0'\n\n");
			
			scanf("%d",&m);
			if(m == 1) {
				strcpy(cards3[0],"-1");
				cards[n] = cards3;
				saveCards(cards, 2, -1);
				printf("[Success] 操作成功");
			} else {
				printf("已取消操作...\n");
			}
			printf("按任意键继续...");
			fflush(stdin);	// 清空输入流
			gets(buf);
			
			system("cls");	
		} else {
			system("cls");
			printf("输入有误，请重新输入...\n\n");
		}	
	}
	system("cls");	
}

/*  搜索名片  */

void searchCards(char*** cards) {
	system("cls");
	
	int i = 0, j = CardsNum;
	char buf[20];
	while(1) {
		showCards(cards, i, j);

		printf("\n操作：\n"
					 "[1] 查看全部    [2] 模糊搜索    [0] 返回主页\n\n");
		fflush(stdin);	// 清空输入流
		gets(buf);
		
		if(buf[0] == '0') {
			break;
		} else if(buf[0] == '1'){
			i = 0;
			j = CardsNum;
		} else if(buf[0] == '2') {
			printf("请输入搜索关键词(姓名拼音缩写，如LH)\n");
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
		
		printf("\n[Success] 查询成功\n"
					"按任意键继续...\n"	
		);

		system("cls");
	}
	system("cls");
	
}

/*  显示名片  */

void showCards(char*** cards, int b, int e) {
	int i = 0;
	
	printf("\n当前查询到%d条记录\n\n", e-b);
	printf("| 序号|  姓名  | 缩写 | 职务|   部门   |  固定电话  |     手机    |       邮箱     |     QQ    |     微信  | 备注 |\n"
				 "-------------------------------------------------------------------------------------------------------------------\n");
	for(int i = b; i < e; i++) {
		printf("| [%d] |%8s|%6s|%5s|%10s|%12s|%13s|%16s|%11s|%11s|%6s|\n", i, cards[i][0], cards[i][1], cards[i][2], cards[i][3], cards[i][4], cards[i][5], cards[i][6], cards[i][7], cards[i][8], cards[i][9]);
	}
	
}

/*  保存至cards.dat  */

void saveCards(char*** cards, int mode, int n) {
	/*  mode 表示保存模式

			0 表示仅保存到文件
			1 保存文件前先进行排序操作，用于新增card
			2 保存文件前先删除姓名为-1的用户，用于删除
			
			n 用于修改card时的重新排序 
	*/
	int i = 0, j = n == -1? CardsNum : n;
	if(mode == 1) {
		// 遍历寻找新增的card应当被插入的位置 
		while(i < CardsNum && strcmp(cards[i][1], cards[j][1]) <= 0) {
			i++;
		}
		
		// 若不是原位，执行移位操作 
		
		// i 为应当插入的位置，j为被移的位置 
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
		// 将指针移至被删除的项 
		while(strcmp(cards[i][0], "-1") != 0) {
			i++;
		}
		
		// 若不是最后一个，执行移位操作 
		while(i < j) {
			cards[i] = cards[i + 1];
			i++;
		} 
		
		// 重新分配cards的空间，即删除最后一位card 
		if ((cards = (char***) realloc(cards, (CardsNum + 1) * sizeof(char**))) == NULL) {
			sendError("重新分配存储空间失败");
		}
		cards[i] = NULL;
		
		CardsNum -= 1;
	} 
	
	// 将数据写入Cards.dat 
	FILE *fp = fopen("Cards.dat", "w");
	
	fprintf(fp, "%s %d", id, CardsNum); // 写入ID与CardsNum 
	// 循环写入card
	for(int i = 0; i < CardsNum; i++) {
		fprintf(fp, "\n%s %s %s %s %s %s %s %s %s %s", cards[i][0], cards[i][1], cards[i][2], cards[i][3], cards[i][4], cards[i][5], cards[i][6], cards[i][7], cards[i][8], cards[i][9]);
	}
	
	fclose(fp);
}

/* 检查Email是否重复 */
bool checkEmail(char*** cards,char* Email, int m){
	/* m参数用于检查修改邮箱信息时，因与自身相同产生问题 */
  int i;
	for(i = 0; i < CardsNum; i++){
		if(strcmp(cards[i][6], Email) == 0 && i != m) {
			break;	// 检查到重复邮箱,去重
		}
	}
	if(i != CardsNum) {
		return false;	
	}
	return true; 
}

/*  打印程序  */
 
void printTitle() {
	printf(" ==================================================\n");
	printf("║                                                 ║\n");
	printf("║               通讯录管理软件                    ║\n");
	printf("║                                                 ║\n");
	printf(" ==================================================\n\n");
}

/*  退出系统  */

void exitSystem() {
	system("cls");
	printTitle(); 
	printf("\n 再见(￣_￣ )...\n");
	exit(0);
}
