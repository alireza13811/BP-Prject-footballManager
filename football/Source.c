#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGHT 100

char* getting_info(char data[MAX_LENGHT],char* item,int start,int end) {
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
    int isUnique = 1;
    do
    {   
        
        isUnique = 1;
        printf("Coach email:");
        scanf("%s", email);
        printf("Team name:");
        scanf("%s", teamName);

        FILE* fptr = fopen("accounts.txt", "r");
        char data[MAX_LENGHT];
        while (fgets(data, MAX_LENGHT, fptr) != NULL) {
            char inFileTeamName[MAX_LENGHT] = "";
            getting_info(data,inFileTeamName,1,2);
            if (strcmp(inFileTeamName, teamName) == 0) {
                isUnique = 0;
            }
        }
        fclose(fptr);

        if (isUnique == 0) {
            printf("\nTeam name has already been got!\n");
        }

    } while (isUnique==0);

    int id = config_file_handling(0);
    if (id) {
        FILE* fptr2 = fopen("accounts.txt", "a");
        id--;
        fprintf(fptr2, "%s,%s,%s,%d,\n", email, teamName, email, id);//password,teamname,email,id
        fclose(fptr2);

        FILE* teams = fopen("teams.txt","a");
        fprintf(teams, "0,0,100,%s,\n", teamName);//number of players,number of trophies,budget,name
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
    FILE* players = fopen("players.txt", "a");
    id--;
    fprintf(players, "none,%d,%d,%d,%d,%s,\n", id, attackingPower, defendingPower, value, playerName);
    fclose(players);

    
}

void print_col(char item[MAX_LENGHT],int characters) {
    int length = strlen(item);
    int spaces = (characters - length)/2;
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
    printf("|");
}

void team_info(char teamName[MAX_LENGHT]) {
    system("cls");
    FILE* players = fopen("players.txt", "r");
    char data[MAX_LENGHT];

    printf("\n%s players\n", teamName);
    printf("======================================================================================================\n");
    printf("||               Player name               |   Value   |   Attacking power    |   Defending power   ||\n");
    //26 characters
    while (fgets(data,MAX_LENGHT,players))
    {
        char playerTeam[MAX_LENGHT]="";
        getting_info(data, playerTeam, 0, 1);
        if (strcmp(playerTeam, teamName) != 0) {
            continue;
        }
        char attackingPower[4]="", defendingPower[4]="", value[3]="", name[MAX_LENGHT]="";
        getting_info(data, attackingPower, 2, 3);
        getting_info(data, defendingPower, 3, 4);
        getting_info(data, value, 4, 5);
        getting_info(data, name, 5, 6);

        printf("||-----------------------------------------|-----------|----------------------|---------------------||\n||");
        print_col(name, 41);    
        print_col(value, 11);
        print_col(attackingPower, 22);
        print_col(defendingPower, 21);
        printf("|");
        printf("\n");
        //printf("    %s     |   %s   |   %s    |   %s   \n",name,value,attackingPower,defendingPower);
    }
    printf("======================================================================================================\n");
    scanf("%d");
}

void show_teams() {
    system("cls");
    FILE* teams = fopen("teams.txt", "r");
    char data[MAX_LENGHT], teamChoice[MAX_LENGHT],teamsData[100][MAX_LENGHT];
    int choice, index = 0;

    printf("\n");
    printf("==================================================================================\n");
    printf("||               Team name               |   trophies   |   Number of players   ||\n");

    while (fgets(data, MAX_LENGHT, teams)) {
        char trophies[2] = "", teamName[MAX_LENGHT] = "", playersNumbers[2] = "",teamChoice[MAX_LENGHT];
        getting_info(data, trophies, 1, 2);
        getting_info(data, teamName, 3, 4);
        strcpy(teamsData[index], teamName);
        getting_info(data, playersNumbers, 0, 1);

        printf("||--------------------------------------|--------------|-----------------------||\n||");
        print_col(teamName, 38);
        print_col(trophies, 14);
        print_col(playersNumbers, 23);
        printf("  |  ");
        
        index++;
    }
    printf("==================================================================================\n");
    fclose(teams);
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
    } while (correct==0);
    team_info(teamChoice);
    
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
    } while (1);
    
    
}

void change_password(char userName[MAX_LENGHT], char password[MAX_LENGHT]) {
    char data[MAX_LENGHT][MAX_LENGHT],data2[MAX_LENGHT],*dataptr;
    int i = 0;
    FILE* accounts = fopen("accounts.txt", "r");
    while (fgets(data2,MAX_LENGHT,accounts)!=NULL)
    {
        strcpy(data[i], data2);
        i++;
    }
    fclose(accounts);
    
    FILE* accounts2 = fopen("accounts.txt", "w");
    for (int j =0 ; j < i;j++) {
        char fileTeamName[MAX_LENGHT]="";
        getting_info(data[j], fileTeamName, 1, 2);
        if (strcmp(fileTeamName, userName) == 0) {
            dataptr = strchr(data[j], ',');
            fprintf(accounts2, "%s%s", password, dataptr);
        }
        else {
            fprintf(accounts2, "%s", data[j]);
        }
    }
    fclose(accounts2);
}

void coach_page(char teamName[MAX_LENGHT]) {
    system("cls");
    int choice;
    printf("1)Buy a player\n");
    printf("2)Sell a player\n");
    printf("3)Select squad(Submit squad)\n");
    printf("4)League Standing\n");
    printf("5)Fixtures\n");
    printf("6)Upcoming Opponent\n");
    printf("7)Change Password\n");
    printf("8)Upcoming Opponent\n");
    scanf("%d", &choice);

}

int login() {
    system("cls");
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
            FILE* accounts = fopen("accounts.txt", "r");
            char data[MAX_LENGHT];
            while (fgets(data, MAX_LENGHT, accounts) != NULL) {
                char userName[MAX_LENGHT] = "", userPassword[MAX_LENGHT] = "";
                getting_info(data,userPassword,0,1);
                getting_info(data, userName,1,2);
                if (strcmp(userName, teamname) == 0 && strcmp(userPassword,password)==0) {
                    coach_page(teamname);
                }
            }
        }
        printf("Team Name is incorrect!!\n");
    } while (1);
}

void forget_password() {
    system("cls");
    int flag = 0;
    char email[MAX_LENGHT], teamName[MAX_LENGHT];
    do
    {
        
        printf("Team name: ");
        scanf("%s", teamName);
        printf("Email: ");
        scanf("%s", email);
        FILE* accounts = fopen("accounts.txt", "r");
        char data[MAX_LENGHT], password[MAX_LENGHT];
        while (fgets(data, MAX_LENGHT, accounts) != NULL)
        {
            char userEmail[MAX_LENGHT]="", userTeamName[MAX_LENGHT]="";
            getting_info(data, userTeamName, 1, 2);
            getting_info(data, userEmail, 2, 3);
            if (strcmp(userTeamName, teamName) == 0 && strcmp(userEmail, email) == 0) {
                flag = 1;
                fclose(accounts);
                printf("Enter new password: ");
                scanf("%s", password);
                change_password(teamName,password);
            }
        }
        if (flag == 0) {
            printf("Team name or email is incorrect!!\n");
            fclose(accounts);
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
    FILE* accounts = fopen("accounts.txt", "a");
    fclose(accounts);
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