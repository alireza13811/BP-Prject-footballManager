#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGHT 100

typedef struct{
    int id;
    char name[MAX_LENGHT];
    char teamName[MAX_LENGHT];
    int attackingPower;
    int defencingPower;
    int value;
} Players;

typedef struct{
    int id;
    char name[MAX_LENGHT];
    char email[MAX_LENGHT];
    char password[MAX_LENGHT];
    int budget;
    int numberOfPlayers;
    int trophies;
    Players players[8];
} Teams;

#define PLAYERS_SIZE sizeof(Players)
//#define sizeof(Teams) sizeof(Teams)

void getting_info(char data[MAX_LENGHT],char* item,int start,int end) {
    int counter = 1,startposition;
    char inFileTeamName[MAX_LENGHT]="";
    for (int i = 0; i < MAX_LENGHT; i++) {
        if (data[i] == ',' && i!=0) {
            if (start == 0) {
                strncpy(item, data, i);
                break;
            }
            if (counter == start) {
                startposition = i + 1;
            }
            else if (counter == end) {
                strncpy(item, data + startposition, i - startposition);
                break;
            }
            counter++;
        } 
    }
}

int config_file_handling(int change) {
    //change-> 0:adding team, 1:adding player
    char teamId[4]="", playerId[4]="", line[10];
    int id=0;
    FILE* config = fopen("config.txt", "r");
    fgets(line, 10, config);
    getting_info(line, teamId,0,1);
    getting_info(line, playerId, 1, 2);
    fclose(config);

    FILE* config2 = fopen("config.txt", "w");
    if (change == 0) {       
        sscanf(teamId, "%d", &id);
        if (id == 100) {
            printf("You cannot add more teams!!\n");
            fclose(config2);
            return 0;
        }  
        id++;
        fprintf(config2, "%d,", id);
        
    }
    else {        
        fprintf(config2, "%s,", teamId); 
    }

    if (change == 1) {
        sscanf(playerId, "%d", &id);
        if (id == 800) {
            printf("You cannot add more players!!\n");
            fclose(config2);
            return 0;
        }
        id++;
        fprintf(config2, "%d,", id);
        
    }
    else {
        fprintf(config2, "%s,", playerId);
    }
    fclose(config2);
    return id;
}

void add_team() {
    system("cls");
    char email[MAX_LENGHT], teamName[MAX_LENGHT], inFileEmail[MAX_LENGHT]="";
    Teams team1;
    int isUnique = 1;
    do
    {   
        
        isUnique = 1;
        printf("Coach email:");
        scanf("%s", email);
        printf("Team name:");
        scanf("%s", teamName);

        FILE* fptr = fopen("teams.txt", "r");
        while (fread(&team1,sizeof(Teams), 1, fptr)) {
            if (strcmp(team1.name, teamName) == 0) {
                isUnique = 0;
            }
        }
        fclose(fptr);

        if (isUnique == 0) {
            printf("\nTeam name has already been got!\n");
        }

    } while (isUnique==0);

    int id = config_file_handling(0);
    Teams team;
    if (id) {
        FILE* teams = fopen("teams.txt", "a");
        id--;
        team.id = id;
        team.budget = 100;
        team.numberOfPlayers = 0;
        team.trophies = 0;
        strcpy(team.name, teamName);
        strcpy(team.email, email);
        strcpy(team.password, email);
        fwrite(&team, sizeof(Teams), 1, teams);
        fclose(teams);
    }
}

int add_player() {
    system("cls");
    char playerName[MAX_LENGHT];
    int attackingPower, defendingPower, value;
    printf("Name: ");
    scanf("%s", playerName);
    printf("Attacking power: ");
    scanf("%d", &attackingPower);
    printf("Defencing power: ");
    scanf("%d", &defendingPower);
    printf("Value: ");
    scanf("%d", &value);
    
    int id = config_file_handling(1);
    if (id == 0) {
        return 0;
    }
    Players player;
    FILE* playersFile = fopen("players.txt", "a");
    id--;
    player.id = id;
    strcpy(player.name, playerName);
    strcpy(player.teamName, "Free agent");
    player.value = value;
    player.attackingPower = attackingPower;
    player.defencingPower = defendingPower;
    fwrite(&player, PLAYERS_SIZE, 1, playersFile);
    fclose(playersFile);

    
}

void print_col_char(char item[MAX_LENGHT],int characters) {
    int length = strlen(item);
    int spaces = (characters - length)/2;
    if (spaces <= 2) {
        printf("%s", item);
    }
    else {
        if ((characters - length) % 2 != 0) {
            printf(" ");
        }
        for (int i = 0; i < spaces; i++) {
            printf(" ");
        }
        
        printf("%s", item);
        for (int i = 0; i < spaces; i++) {
            printf(" ");
        }  
    }
    printf("|");
}

void print_col_int(int item, int characters) {
    int digits = 0,item2=item;
    while (item2>0)
    {
        item2 /= 10;
        digits++;
    }
    if (item == 0) digits = 1;
    int length = digits;
    int spaces = (characters - length) / 2;
    if (spaces <= 2) {
        printf("%d", item);
    }
    else {
        if ((characters - length) % 2 != 0) {
            printf(" ");
        }
        for (int i = 0; i < spaces; i++) {
            printf(" ");
        }

        printf("%d", item);
        for (int i = 0; i < spaces; i++) {
            printf(" ");
        }
    }
    printf("|");
}

void team_info(char teamName[MAX_LENGHT]) {
    system("cls");
    Players player;
    FILE* playersFile = fopen("players.txt", "r");
    int numberOfPlayers = 0;
    printf("\n%s players\n", teamName);
    printf("======================================================================================================\n");
    printf("||               Player name               |   Value   |   Attacking power    |   Defencing power   ||\n");
    //26 characters
    while (fread(&player,PLAYERS_SIZE,1,playersFile))
    {
        if (strcmp(player.teamName, teamName) != 0) {
            continue;
        }

        printf("||-----------------------------------------|-----------|----------------------|---------------------||\n||");
        print_col_char(player.name, 41);
        print_col_int(player.value, 11);
        print_col_int(player.attackingPower, 22);
        print_col_int(player.defencingPower, 21);
        printf("|");
        printf("\n");
        numberOfPlayers++;
    }
    printf("======================================================================================================\n");
    fclose(playersFile);
    if (numberOfPlayers == 0) {
        printf("This team has no players!!\n");
    }
    printf("Press enter to continue\n");
    getchar();
    getchar();
}

void show_players() {
    system("cls");
    Players player;
    FILE* playersFile = fopen("players.txt", "r");
    int numberOfPlayers = 0;
    printf("\n");
    printf("====================================================================================================================================================\n");
    printf("||               Player name               |             Team name            |   Attacking power    |   Defending power   |   Value   |    id    ||\n");
    while (fread(&player,PLAYERS_SIZE,1,playersFile))
    {
        
        //41 34 22 21 11 10
        printf("||-----------------------------------------|----------------------------------|----------------------|---------------------|-----------|----------||\n||");
        print_col_char(player.name, 41);
        print_col_char(player.teamName, 34);
        print_col_int(player.attackingPower, 22);
        print_col_int(player.defencingPower, 21);
        print_col_int(player.value, 11);
        print_col_int(player.id, 10);
        printf("|");
        printf("\n");
        numberOfPlayers++;

    }
    fclose(playersFile);
    printf("====================================================================================================================================================\n");
    if (numberOfPlayers == 0) {
        printf("There is no player!!\n");
    }
    else {
        printf("Press enter to continue\n");
    }
    getchar();
    getchar();
}

void show_teams() {
    system("cls");
    Teams team;
    FILE* teamsFile = fopen("teams.txt", "r");
    char  teamChoice[MAX_LENGHT],teamsData[100][MAX_LENGHT];
    int choice, index = 0;

    printf("\n");
    printf("==================================================================================\n");
    printf("||               Team name               |   trophies   |   Number of players   ||\n");

    while (fread(&team,sizeof(Teams),1,teamsFile)) {
        
        strcpy(teamsData[index], team.name);

        printf("||---------------------------------------|--------------|-----------------------||\n||");
        print_col_char(team.name, 39);
        print_col_int(team.trophies, 14);
        print_col_int(team.numberOfPlayers, 23);
        printf("|");
        printf("\n");
        index++;
    }
    printf("==================================================================================\n");
    fclose(teamsFile);
    if (index == 0) {
        printf("There isn't any team!!\nPress enter to continue");
        getchar();
        getchar();
    }
    else {
        int correct = 0;
        do
        {
            printf("\nFor more info enter the team name: ");
            scanf("%s", teamChoice);
            for (int i = 0; i < index; i++) {
                if (strcmp(teamsData[i], teamChoice) == 0) {
                    correct = 1;
                }
            }
            if (correct == 0) {
                printf("Team name is incorrect\n");
            }
        } while (correct == 0);
        team_info(teamChoice);
    }
}

void admin_page() {
    
    int choice;
    do
    {
        system("cls");
        printf("ADMIN PAGE\n");
        printf("1)Add team\n");
        printf("2)Add player\n");
        printf("3)Show teams\n");
        printf("4)Show players\n");

        scanf("%d", &choice);
        if (choice == 1) {
            add_team();
        }
        else if (choice == 2) {
            add_player();
        } else if (choice == 3) {
            show_teams();
        }
        else if (choice == 4) {
            show_players();
        }
    } while (1);
    
    
}

void change_password(char userName[MAX_LENGHT], char password[MAX_LENGHT]) {
    int i = 0;
    Teams team,allTeams[100];
    FILE* teamsFile = fopen("teams.txt", "r");
    while (fread(&team,sizeof(Teams),1,teamsFile))
    {
        if (strcmp(team.name, userName) == 0) {
            *(team).password = password;
        }
        allTeams[i] = team;
        i++;
    }
    fclose(teamsFile);
    
    FILE* teamsFile2 = fopen("teams.txt", "w");
    for (int j =0 ; j < i;j++) {
        fwrite(&allTeams[j], sizeof(Teams), 1, teamsFile2);
    }
    fclose(teamsFile2);
}

void coach_page(char teamName[MAX_LENGHT]) {
    system("cls");
    int choice;
    Teams team;
    FILE* teamsFile = fopen("teams.txt", "r");
    
    while (fread(&team,sizeof(Teams),1,teamsFile))
    {
        if (strcmp(team.name, teamName) == 0) {
            break;
        }
    }
    fclose(teamsFile);
    do
    {
        printf("\nTeam budget: %d\n", team.budget);
        printf("\n1)Buy a player\n");
        printf("2)Sell a player\n");
        printf("3)Select squad(Submit squad)\n");
        printf("4)League Standing\n");
        printf("5)Fixtures\n");
        printf("6)Upcoming Opponent\n");
        printf("7)Change Password\n");
        printf("8)Upcoming Opponent\n");
        scanf("%d", &choice);
        if (choice == 1) {

        }
    } while (1);
    

}

int login() {
    system("cls");
    Teams team;
    char teamname[MAX_LENGHT], password[MAX_LENGHT];
    do
    {
        printf("Team name:");
        scanf("%s", teamname);
        printf("Password:");
        scanf("%s", password);
        if (strcmp(teamname, "Admin") == 0 && strcmp(password, "Admin") == 0) {
            admin_page();
            continue;
        }
        else {
            FILE* teamsFile = fopen("teams.txt", "r");
            while (fread(&team,sizeof(Teams),1,teamsFile)) {
                if (strcmp(team.name, teamname) == 0 && strcmp(team.password,password)==0) {                          
                    coach_page(teamname);
                    break;
                }
            }
            fclose(teamsFile);
        }
        printf("Team Name is incorrect!!\n");
    } while (1);
}

void forget_password() {
    system("cls");
    Teams team;
    int flag = 0;
    char email[MAX_LENGHT], teamName[MAX_LENGHT];
    do
    {
        
        printf("Team name: ");
        scanf("%s", teamName);
        printf("Email: ");
        scanf("%s", email);
        FILE* teamsFile = fopen("teams.txt", "r");
        char password[MAX_LENGHT];
        while (fread(&team,sizeof(Teams),1,teamsFile))
        {
            if (strcmp(team.password, teamName) == 0 && strcmp(team.password, email) == 0) {
                flag = 1;
                fclose(teamsFile);
                printf("Enter new password: ");
                scanf("%s", password);
                change_password(teamName,password);
            }
        }
        if (flag == 0) {
            printf("Team name or email is incorrect!!\n");
            fclose(teamsFile);
        }
    } while (flag==0);
    
}

void login_page() {
    int choice;
    system("cls");
    printf("1)Login\n2)Forget password\n");
    do
    {
        scanf("%d", &choice);
        if (choice == 1) {
            login();
        }
        else if (choice == 2) {
            forget_password();
        }
    } while (choice != 1 && choice != 2);

}

void files_initilize() {
    FILE* teams = fopen("teams.txt", "a");
    fclose(teams);
    FILE* players = fopen("players.txt", "a");
    fclose(players);
    FILE* config = fopen("config.txt", "a");
    fseek(config, 0, SEEK_END);
    int position = ftell(config);
    fclose(config);
    if (position == 0) {
        FILE* config2 = fopen("config.txt", "w");
        fprintf(config2, "0,0,");//first value:nomber of teams, second value:number of players
        fclose(config2);
    }
}

int main()
{
    files_initilize();
    login_page();
    
}