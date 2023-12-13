#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <windows.h>

#define true 1

// UI 정보를 담는 구조체
typedef struct{
    char item[7][50];
    int item_atk[7];
    int stage;
    int wave;
    int ATK;
    int ATK_add;
    int total_weight;
    int stage_weight;
} UIInfo;

typedef struct itemInfo{
   int weight;
   int ATK;
   char item_name[50];
} itemInfo;

// 출력 위치 변경을 위한 함수
void GotoXY(int x, int y){
   COORD Pos;
   Pos.X = x;
   Pos.Y = y;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void ClearBuffer(){
	while(getchar()!='\n');
}

// 몬스터의 체력 저장
int monster[3][3] = {{30, 35, 40}, {35, 45, 55}, {50, 60, 65}};

// 각 스테이지별로 최적의 아이템 조합 저장(힌트 기능을 위한)
char hint[3][50];

int addATK(itemInfo* items, int n, int c, int totalATK) //(아이템 구조체 배열 포인터(index 1부터 시작할 것), 아이템 개수, 배낭 용량, 총 공격력) 
{
   int k[100][100];                      //몇 번째로 효율적인지 반환
   int i, w;
   for (i=0; i<=n; i++)           //반환값이 작을수록 효율적 
      k[i][0]=0;
   for (i=0; i<=c; i++)
      k[0][i]=0;
   
   for (i=1; i<=n; i++)
   {
      for (w=1; w<=c; w++)
      {
         if (items[i].weight>w)
         {
            k[i][w]=k[i-1][w];
         }
         else
         {
            if (k[i-1][w] > k[i-1][w-items[i].weight]+items[i].ATK)
            {
               k[i][w]=k[i-1][w];
            }
            else
            {
               k[i][w]=k[i-1][w-items[i].weight]+items[i].ATK;
            }
         }
      }
   }
   
   int tmp=0;
   int arr[30];
   int idx=0;
   
   for (i=n; i>=1; i--)
   {
      if (tmp!=k[i][c])
      {
         arr[idx++]=k[i][c];
      }
      tmp=k[i][c];
   }
   
   int order=1;
   
   for (i=0; i<idx; i++)
   {
      if (arr[i]>totalATK)
         order++;
   }
   
   return order;
    
}

void init_info(UIInfo* info){
	int i;
	int j;
    info->ATK = 0;
    info->ATK_add = 0;
    info->total_weight = 0;
    for(i=0; i<7; i++){
        info->item_atk[i] = 0;
		for(j=0; j<50; j++){
            info->item[i][j] = '\0';
        }
    }
}

void init_info_first(UIInfo* info){
	int i, j;
	info->stage = 1;
	info->wave = 1;
    info->stage_weight = 0;
    for(i=0; i<7; i++){
		for(j=0; j<50; j++){
			if(info->item[i][j] != '\0'){
				info->item[i][j] = '\0';
			}
        }
    }
}

// 입력 부분 출력을 위한 함수
void show_input(){
    // 입력 부분
    GotoXY(0, 55);
    for(int i=0; i<75; i++){
        printf("□");
    }
    GotoXY(0, 56);
}

// 게임의 기본 UI 출력을 위한 함수
void show_UI(UIInfo* info){
    int n;
    int i;
    system("cls");
    
    // 스테이지 정보, 선택한 아이템 정보, 공격력 정보가 들어갈 부분
    GotoXY(0,0);
    printf("스테이지 정보");
    GotoXY(15,0);
    printf("||  플레이어의 공격력");
    GotoXY(37,0);
    printf("||  배낭의 무게");
    GotoXY(54,0);
    printf("||  선택한 아이템 목록");
    
    GotoXY(0,1);
    printf("     %d-%d", info->stage, info->wave);
    GotoXY(15,1);
    n = info->ATK + info->ATK_add;
    printf("||  %d + %d = %d", info->ATK, info->ATK_add, n);
    GotoXY(37,1);
    printf("||     %d/%d", info->total_weight, info->stage_weight);
    GotoXY(54,1);
    printf("||  ");
    n=0;
    for(i=0; i<7; i++){
    	if(info->item[i][n] == '\0') break;
        printf("%s  ", info->item[i]);
	}
    
    GotoXY(0,3);
    for(int i=0; i<75; i++){
        printf("□");
    }
    
    // 스토리와 아이템 목록, 몬스터와의 전투 그림이 들어갈 부분
    
    //유저가 입력할 부분
    show_input();
}

void add_attack(itemInfo* items_stage1, itemInfo* items_stage2, itemInfo* items_stage3, UIInfo* info){
	int n;
    switch(info->stage){
        case 1:
            n = addATK(items_stage1, 6, 20, info->ATK);
            if(n==1) info->ATK_add = 20;
            else if(n>=2 && n <=3) info->ATK_add = 18;
            else if(n>=4 && n <=5) info->ATK_add = 16;
            else if(n>=6 && n <=8) info->ATK_add = 14;
            else if(n>=9 && n <=10) info->ATK_add = 12;
            else info->ATK_add = 10;
            break;
            
        case 2:
            n = addATK(items_stage2, 7, 30, info->ATK);
            if(n>=1 && n<=2) info->ATK_add = 20;
            else if(n>=3 && n <=4) info->ATK_add = 18;
            else if(n>=5 && n <=7) info->ATK_add = 16;
            else if(n>=8 && n <=9) info->ATK_add = 14;
            else if(n>=10 && n <=11) info->ATK_add = 12;
            else info->ATK_add = 10;
            break;
                
        case 3:
            n = addATK(items_stage3, 8, 40, info->ATK);
            if(n>=1 && n<=2) info->ATK_add = 20;
            else if(n>=3 && n <=4) info->ATK_add = 18;
            else if(n>=5 && n <=7) info->ATK_add = 16;
            else if(n>=8 && n <=10) info->ATK_add = 14;
            else if(n>=11 && n <=12) info->ATK_add = 12;
            else info->ATK_add = 10;
            break;
                
        default:
            break;
    }
}

void game_story_display(){
    char print_temp[256];
    FILE *fp;
   
    system("cls");
    
    // 스토리 부분
    fp = fopen("game_story.txt", "rt");
    if(fp == NULL){
        printf("파일 불러오기에 실패했습니다.\n");
        return;
    }
    GotoXY(0, 15);
    while(fgets(print_temp, 255, fp) != NULL){
    	printf("			");
        printf(print_temp);
    }
    fclose(fp);
    
    // 입력 부분
    show_input();
}

void game_story(){
    //게임 스토리 출력 화면
    game_story_display();
    
    //유저가 엔터키를 입력하면 게임시작
    printf("엔터키를 입력하면 게임이 시작됩니다. ");
    ClearBuffer();
}

void show_item(int stage){
	char print_temp[256];
    FILE *fp;
	
    GotoXY(0, 15);
    switch(stage){
        case 1:
            fp = fopen("stage1_weapon.txt", "rt");
            if(fp == NULL){
                printf("파일 불러오기에 실패했습니다.\n");
                return;
            }
            while(fgets(print_temp, 255, fp) != NULL){
                printf("			");
                printf(print_temp);
            }
            fclose(fp);
            break;
        case 2:
            fp = fopen("stage2_weapon.txt", "rt");
            if(fp == NULL){
                printf("파일 불러오기에 실패했습니다.\n");
                return;
            }
            while(fgets(print_temp, 255, fp) != NULL){
                printf("			");
                printf(print_temp);
            }
            fclose(fp);
            break;
        case 3:
            fp = fopen("stage3_weapon.txt", "rt");
            if(fp == NULL){
                printf("파일 불러오기에 실패했습니다.\n");
                return;
            }
            while(fgets(print_temp, 255, fp) != NULL){
                printf("			");
                printf(print_temp);
            }
            fclose(fp);
            break;
        default:
            break;
    }
    GotoXY(0, 56);
    printf("아이템 번호 입력: ");
}

void item_select(itemInfo* items_stage1, itemInfo* items_stage2, itemInfo* items_stage3, UIInfo* info){
    int n;
    int i=0;
    //스테이지에 맞게 배낭의 무게 설정
    switch(info->stage){
        case 1:
            info->stage_weight = 20;
            break;
        case 2:
            info->stage_weight = 30;
            break;
        case 3:
            info->stage_weight = 40;
            break;
        default:
            break;
    }
    
    //유저가 아이템을 입력하면 그에 따라 UI의 선택한 아이템 목록과 공격력 업데이트
    while(TRUE){
        //스테이지 정보에 대한 출력
        show_UI(info);
        
        //아이템 정보 출력
        show_item(info->stage);
    
        //선택한 아이템 목록에 추가 및 공격력 업데이트
        scanf("%d", &n);
        ClearBuffer();
        
        if(info->stage == 1){
        	if(!(n>=1 && n<=6)){
                GotoXY(0, 58);
                printf("선택할 아이템의 번호를 정확하게 다시 입력하세요");
                continue;
            }
            info->total_weight += items_stage1[n].weight;
            if(info->total_weight > info->stage_weight){
            	GotoXY(0, 58);
            	printf("더이상 배낭에 무기를 담을 수 없습니다. 2초 뒤 게임이 시작됩니다.");
            	info->total_weight -= items_stage1[n].weight;
            	Sleep(2000);
            	break;
        	}
            info->ATK += items_stage1[n].ATK;
            info->item_atk[i] = items_stage1[n].ATK;
            strcpy(info->item[i], items_stage1[n].item_name);
            i++;
		}
		else if(info->stage == 2){
			if(!(n>=1 && n<=7)){
                GotoXY(0, 58);
                printf("선택할 아이템의 번호를 정확하게 다시 입력하세요");
                continue;
            }
            info->total_weight += items_stage2[n].weight;
            if(info->total_weight > info->stage_weight){
            	GotoXY(0, 58);
            	printf("더이상 배낭에 무기를 담을 수 없습니다. 2초 뒤 게임이 시작됩니다.");
            	info->total_weight -= items_stage2[n].weight;
            	Sleep(2000);
            	break;
        	}
            info->ATK += items_stage2[n].ATK;
            info->item_atk[i] = items_stage2[n].ATK;
            strcpy(info->item[i], items_stage2[n].item_name);
            i++;
		}
		else{
			if(!(n>=1 && n<=8)){
                GotoXY(0, 58);
                printf("선택할 아이템의 번호를 정확하게 다시 입력하세요");
                continue;
            }
            info->total_weight += items_stage3[n].weight;
            if(info->total_weight > info->stage_weight){
            	GotoXY(0, 58);
            	printf("더이상 배낭에 무기를 담을 수 없습니다. 2초 뒤 게임이 시작됩니다.");
            	info->total_weight -= items_stage3[n].weight;
            	Sleep(2000);
            	break;
        	}
            info->ATK += items_stage3[n].ATK;
            info->item_atk[i] = items_stage3[n].ATK;
            strcpy(info->item[i], items_stage3[n].item_name);
            i++;
		}
    }
}

void game_play_display(){
    char print_temp[256];
    FILE *fp;
    
    // 몬스터와의 전투 출력 부분( 몬스터 체력 출력하는 부분 추가하기)
    GotoXY(0,5);
    fp = fopen("game_play.txt", "rt");
    if(fp == NULL){
        printf("파일 불러오기에 실패했습니다.\n");
        return;
    }
    while(fgets(print_temp, 255, fp) != NULL){
        // printf("         ");
        printf(print_temp);
    }
    fclose(fp);
    GotoXY(0,56);
    printf("아이템 이름 입력: ");
}

bool game_play(itemInfo* items_stage1, itemInfo* items_stage2, itemInfo* items_stage3, UIInfo* info){
    char temp[50] = {'\0'};
    int i, j, k, size, size_check;
    // 아이템을 입력한 적이 있는지 확인
    int select[7];
    int monster_hp;
    // 입력한 아이템 이름이 선택한 아이템 목록에 있는지
    bool check;
    
    for(j=0; j<7; j++){
    	select[j] = 0;
	}
    monster_hp = monster[info->stage - 1][info->wave - 1];
    
    // 선택한 아이템 목록에 있는 아이템 개수만큼 입력했는지 확인용
    size_check = 0;
    
    // 선택한 아이템 목록에 있는 아이템 개수 확인
    size = 0;
    i = 0;
    for(j=0; j<7; j++){
    	if(info->item[j][i] == '\0') break;
        else size++;
    }
    
    while(TRUE){
        k=0;
        check = false;
        show_UI(info);
        GotoXY(87, 16);
        printf("몬스터 체력: [%d]", monster_hp);
        game_play_display();
        
        gets(temp);
        for(j=0; j<7; j++){
        	if(info->item[j][i] == '\0') break;
            if(strcmp(temp, info->item[j]) == 0){
                check = true;
                break;
            }
            k++;
        }
        if(!check){
            GotoXY(0, 58);
            printf("선택한 아이템 목록에 존재하지 않는 아이템입니다. 다시 입력해주세요");
            Sleep(1000);
            continue;
        }
        
        if(select[k] == 1){
            GotoXY(0, 58);
            printf("이미 선택한 아이템입니다. 다시 입력해주세요");
            Sleep(1000);
            continue;
        }
        select[k] = 1;
        size_check++;
        
        monster_hp -= info->item_atk[k];
        GotoXY(0, 57);
        printf("몬스터를 공격하여 %d만큼 피해를 입혔습니다.", info->item_atk[k]);
        
        if(size_check >= size){
            monster_hp -= info->ATK_add;
            GotoXY(0,58);
            printf("선택한 아이템을 전부 입력하여 추가 공격력 %d만큼의 피해를 입혔습니다. ", info->ATK_add);
        }
        Sleep(1000); 
        
        if(monster_hp <= 0 && size_check <= size){
            printf("몬스터의 체력이 0이 되어 다음 몬스터와의 전투로 넘어갑니다.");
            Sleep(3000);
            return true;
        }
        else if(monster_hp > 0 && size_check >= size){
            printf("아이템을 모두 사용하였지만 몬스터의 체력이 0이 되지 않았습니다.");
            Sleep(3000);
            return false;
        }
    }
}

void game_clear_display(){
	char print_temp[256];
    FILE *fp;
   
    system("cls");
    
    // 설명 부분
    GotoXY(0, 5);
    fp = fopen("game_clear.txt", "rt");
    if(fp == NULL){
        printf("파일 불러오기에 실패했습니다.\n");
        return;
    }
    while(fgets(print_temp, 255, fp) != NULL){
        printf("         ");
        printf(print_temp);
    }
    fclose(fp);
}

void game_clear(){
	game_clear_display();
}

void game(itemInfo* items_stage1, itemInfo* items_stage2, itemInfo* items_stage3){
    // UI 정보를 담는 구조체
    UIInfo info;
    int n;
    bool clear = false;
    
    // UI 처음 초기화
	init_info_first(&info); 
    
    // 초반 스토리 화면 출력
    game_story();
    
    while(true){
        // 스테이지를 처음 시작할 때마다 info 초기화
        init_info(&info);
            
        //아이템 선택화면 출력
        item_select(items_stage1, items_stage2, items_stage3, &info);
        
        // 유저가 고른 아이템의 효율 계산해서 추가 공격력 부여
        add_attack(items_stage1, items_stage2, items_stage3, &info);
        
        //게임 플레이
        clear = game_play(items_stage1, items_stage2, items_stage3, &info);
        
        if(clear){
            //3-3까지 끝나면 끝
            if((info.stage == 3) && (info.wave == 3)) break;
        
            if(info.wave == 3){
                info.stage++;
                info.wave = 1;
            }
            else info.wave++;
        }
        else{
            GotoXY(0, 59);
            printf("스테이지를 클리어하지 못하였습니다. 클리어를 위해 힌트 기능을 제공합니다.");
            // 힌트 제공
            GotoXY(0, 60);
            printf("hint: %s", hint[info.stage-1]);
            Sleep(5000);
        }
    }
    
    //게임 클리어 화면 출력
    game_clear();
}

void game_explanation_display(){
    char print_temp[256];
    FILE *fp;
   
    system("cls");
    
    // 설명 부분
    GotoXY(0, 20);
    fp = fopen("game_explanation.txt", "rt");
    if(fp == NULL){
        printf("파일 불러오기에 실패했습니다.\n");
        return;
    }
    while(fgets(print_temp, 255, fp) != NULL){
        printf(print_temp);
    }
    fclose(fp);
    
    // 입력 부분
    show_input();
}

void game_explanation(itemInfo* items_stage1, itemInfo* items_stage2, itemInfo* items_stage3){
    //게임 설명화면 출력
    game_explanation_display();
    
    //사용자가 엔터 키 입력시 게임 시작
    GotoXY(0, 56);
    printf("게임을 시작하려면 엔터키를 입력하세요....");
    ClearBuffer();
    game(items_stage1, items_stage2, items_stage3);
}

void game_start_display(){
    char print_temp[256];
    FILE *fp;
    
    system("cls");
    
    // 게임제목 출력 부분
    GotoXY(0, 10);
    fp = fopen("game_title.txt", "rt");
    if(fp == NULL){
        printf("파일 불러오기에 실패했습니다.\n");
        return;
    }
    while(fgets(print_temp, 255, fp) != NULL){
        printf("			");
        printf(print_temp);
    }
    fclose(fp);
    
    // 게임시작, 게임설명 선택 출력 부분
    GotoXY(70, 30);
    printf("1. 게임시작");
    GotoXY(70, 32);
    printf("2. 게임설명");
    
    // 입력 부분
    show_input();
}

void game_start(itemInfo* items_stage1, itemInfo* items_stage2, itemInfo* items_stage3){
    int n;
    
    // 콘솔창 크기 및 제목 설정
    system("mode con:cols=150 lines=61");
    SetConsoleTitle("백플로우");
    
    while(true){
        // 게임 시작화면 출력
        game_start_display();
    
        //사용자 입력
        scanf("%d", &n);
        ClearBuffer();
        if(n==1) game(items_stage1, items_stage2, items_stage3);
        else if(n==2) game_explanation(items_stage1, items_stage2, items_stage3);
        else{
            printf("1 또는 2를 정확하게 다시 입력하세요.");
            Sleep(2000);
            continue;
        }
        break;
    }
}

int main()
{
	strcpy(hint[0], " *** + 독 포션 + 화염 포션 + 고통 포션");
	strcpy(hint[1], " *** + 독 포션 + 화염 포션 + 고통 포션");
	strcpy(hint[2], " *** + 책 + 화염 포션 + 고통 포션");	
	
    // 아이템 구조체 선언 및 아이템 정보 저장
    itemInfo items_stage1[7];
    itemInfo items_stage2[8];
    itemInfo items_stage3[9];
    // stage1
    itemInfo sword_1 = {14, 16, "낡은 대검"};
    itemInfo dagger_1 = {12, 15, "낡은 단검"};
    itemInfo bow_1 = {10, 12, "낡은 활"};
    itemInfo poison_1 = {4, 6, "하급 투척용 독 포션"};
    itemInfo fire_1 = {3, 5, "하급 투척용 화염 포션"};
    itemInfo pain_1 = {2, 4, "하급 투척용 고통 포션"};
    //stage2
    itemInfo sword_2 = {16, 18, "중급 대검"};
    itemInfo dagger_2 = {14, 16, "중급 단검"};
    itemInfo bow_2 = {12, 15, "중급 활"};
    itemInfo book_2 = {10, 12, "책"};
    itemInfo poison_2 = {6, 8, "중급 투척용 독 포션"};
    itemInfo fire_2 = {5, 7, "중급 투척용 화염 포션"};
    itemInfo pain_2 = {4, 6, "중급 투척용 고통 포션"};
    //stage3
    itemInfo sword_3 = {22, 27, "고급 대검"};
    itemInfo dagger_3 = {19, 24, "고급 단검"};
    itemInfo bow_3 = {17, 20, "고급 활"};
    itemInfo wand_3 = {14, 16, "고급 지팡이"};
    itemInfo book_3 = {10, 12, "책"};
    itemInfo poison_3 = {7, 9, "고급 투척용 독 포션"};
    itemInfo fire_3 = {6, 8, "고급 투척용 화염 포션"};
    itemInfo pain_3 = {5, 7, "고급 투척용 고통 포션"};
    
    items_stage1[1] = sword_1;
    items_stage1[2] = dagger_1;
    items_stage1[3] = bow_1;
    items_stage1[4] = poison_1;
    items_stage1[5] = fire_1;
    items_stage1[6] = pain_1;
    
    items_stage2[1] = sword_2;
    items_stage2[2] = dagger_2;
    items_stage2[3] = bow_2;
    items_stage2[4] = book_2;
    items_stage2[5] = poison_2;
    items_stage2[6] = fire_2;
    items_stage2[7] = pain_2;
    
    items_stage3[1] = sword_3;
    items_stage3[2] = dagger_3;
    items_stage3[3] = bow_3;
    items_stage3[4] = wand_3;
    items_stage3[5] = book_3;
    items_stage3[6] = poison_3;
    items_stage3[7] = fire_3;
    items_stage3[8] = pain_3;
    
    game_start(items_stage1, items_stage2, items_stage3);
    
    return 0;
}
