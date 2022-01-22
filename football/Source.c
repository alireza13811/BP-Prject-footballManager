#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define bool  _Bool
#define false 0
#define true  1
#define MAX_LENGTH 100
#define MAX_PLAYERS 800
#define MAX_TEAMS 100
#define PLAYERS_SIZE sizeof(Players)
#define TEAMS_SIZE sizeof(Teams)

int transferWindow;

typedef struct {
	int id;
	int attackingPower;
	int defencingPower;
	int value;
	char name[MAX_LENGTH];
	char teamName[MAX_LENGTH];
} Players;

typedef struct {
	int id;
	int budget;
	int numberOfPlayers;
	int trophies;
	int ready;
	char name[MAX_LENGTH];
	char email[MAX_LENGTH];
	char password[MAX_LENGTH];
	Players players[8];
}Teams;

typedef struct {
	int id;
	int gf;//goal for
	int ga;//goal against
	int gd;
	int numberOfGames;
	int won;
	int drawn;
	int lost;
	int points;
	int othersid[3];
	char name[MAX_LENGTH];
	char fixture[6][MAX_LENGTH];
} TeamDetails;

bool file_exists(const char* filename);
void getting_info(char data[MAX_LENGTH], char* item, int start, int end);
int config_file_handling(int change);
void add_team();
void add_player();
void print_col_char(char item[MAX_LENGTH], int characters);
void print_col_int(int item, int characters);
void team_info(char teamName[MAX_LENGTH]);
void show_players();
void show_teams();
void admin_page();
int number_of_objects(int number1);
void change_password(char userName[MAX_LENGTH], char password[MAX_LENGTH]);
int is_in(int playersids[PLAYERS_SIZE], int id, int length);
void change_teamsfile(Teams team);
void change_playersfile(Players player);
void buy_player(Players player, Teams team);
void buy_player_page(Teams team);
void sell_player(Players player, Teams team, int index);
void sell_player_page(Teams team);
void coach_page(char teamName[MAX_LENGTH]);
int login();
void forget_password();
void login_page();
void files_initialize();
void submit_squad(Teams team);
void start_league_page();

int main()
{
	files_initialize();
	login_page();
}

bool file_exists(const char* filename) {
	FILE* fptr = fopen(filename, "r");
	bool result = false;
	if (fptr != NULL)
	{
		result = true;
		fclose(fptr);
	}
	return result;
}

void getting_info(char data[MAX_LENGTH], char* item, int start, int end) {
	int counter = 1, startposition;
	char inFileTeamName[MAX_LENGTH] = "";
	for (int i = 0; i < MAX_LENGTH; i++) {
		if (data[i] == ',' && i != 0) {
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
	char teamId[4] = "", playerId[4] = "", line[10], numbOfReadyTeams[3] = "";
	int number = 0;
	FILE* config = fopen("config.txt", "r");
	fgets(line, 10, config);
	getting_info(line, teamId, 0, 1);
	getting_info(line, playerId, 1, 2);
	getting_info(line, numbOfReadyTeams, 3, 4);
	fclose(config);

	FILE* config2 = fopen("config.txt", "w");
	if (change == 0) {//changing team id
		sscanf(teamId, "%d", &number);
		if (number == 100) {
			printf("You cannot add more teams!!\n");
			fclose(config2);
			return 0;
		}
		number++;
		fprintf(config2, "%d,", number);

	}
	else {
		fprintf(config2, "%s,", teamId);
	}

	if (change == 1) {//changing player id
		sscanf(playerId, "%d", &number);
		if (number == 800) {
			printf("You cannot add more players!!\n");
			fclose(config2);
			return 0;
		}
		number++;
		fprintf(config2, "%d,", number);

	}
	else {
		fprintf(config2, "%s,", playerId);
	}

	fprintf(config2, "%d,", transferWindow);// updating transfer window status

	if (change == 3) {//updating number of ready teams
		sscanf(numbOfReadyTeams, "%d", &number);
		number++;
		fprintf(config2, "%d,", number);
	}
	else {
		fprintf(config2, "%s,", numbOfReadyTeams);
	}

	fclose(config2);
	return number;
}

void add_team() {
	system("cls");
	char email[MAX_LENGTH], teamName[MAX_LENGTH], inFileEmail[MAX_LENGTH] = "";
	Teams team1;
	int isUnique = 1;
	do
	{

		isUnique = 1;
		printf("Coach email:");
		scanf("%s", email);
		printf("Team name:");
		scanf("%s", teamName);

		FILE* fptr = fopen("teams.txt", "rb");
		while (fread(&team1, sizeof(Teams), 1, fptr)) {
			if (strcmp(team1.name, teamName) == 0) {
				isUnique = 0;
			}
		}
		fclose(fptr);

		if (isUnique == 0) {
			printf("\nTeam name has already been got!\n");
		}

	} while (isUnique == 0);

	int id = config_file_handling(0);
	Teams team;
	if (id) {
		FILE* teams = fopen("teams.txt", "ab");
		id--;
		team.id = id;
		team.budget = 100;
		team.numberOfPlayers = 0;
		team.trophies = 0;
		team.ready = 0;
		strcpy(team.name, teamName);
		strcpy(team.email, email);
		strcpy(team.password, email);
		fwrite(&team, sizeof(Teams), 1, teams);
		fclose(teams);
	}
	admin_page();
}

void add_player() {
	system("cls");
	char playerName[MAX_LENGTH];

	int attackingPower, defendingPower, value;
	printf("Name: ");
	scanf("%s", playerName);
	do
	{
		printf("Attacking power: ");
		scanf("%d", &attackingPower);
		if (attackingPower >= 0 && attackingPower <= 100) break;
		else printf("Attacking power must be between 0 and 100!\n");
	} while (true);
	do
	{
		printf("Defencing power: ");
		scanf("%d", &defendingPower);
		if (defendingPower >= 0 && defendingPower <= 100) break;
		else printf("Defencing power must be between 0 and 100!\n");
	} while (true);
	do
	{
		printf("Value: ");
		scanf("%d", &value);
		if (value >= 10 && value <= 20) break;
		else printf("Value must be between 10 and 20!\n");
	} while (true);
	
	int id = config_file_handling(1);
	if (id == 0) {
		return 0;
	}
	Players player;
	for (int i = 0; i < MAX_LENGTH; i++) {
		player.teamName[i] = 'x';
		player.name[i] = 'x';
	}
	FILE* playersFile = fopen("players.txt", "ab");
	id--;
	player.id = id;
	strcpy(player.name, playerName);
	strcpy(player.teamName, "Free agent");
	player.value = value;
	player.attackingPower = attackingPower;
	player.defencingPower = defendingPower;
	fwrite(&player, sizeof(Players), 1, playersFile);
	fclose(playersFile);

	admin_page();
}

void print_col_char(char item[MAX_LENGTH], int characters) {
	int length = strlen(item);
	int spaces = (characters - length) / 2;
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
	int digits = 0, item2 = item;
	while (item2 > 0)
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

void team_info(char teamName[MAX_LENGTH]) {
	system("cls");
	Players player;

	FILE* playersFile = fopen("players.txt", "rb");
	int numberOfPlayers = 0;
	printf("\n%s players\n", teamName);
	printf("======================================================================================================\n");
	printf("||               Player name               |   Value   |   Attacking power    |   Defencing power   ||\n");
	//26 characters
	while (fread(&player, PLAYERS_SIZE, 1, playersFile))
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
	for (int i = 0; i < MAX_LENGTH; i++) {
		player.teamName[i] = NULL;
		player.name[i] = NULL;
	}
	FILE* playersFile = fopen("players.txt", "rb");
	int numberOfPlayers = 0;
	fseek(playersFile, 0, SEEK_SET);
	printf("\n");
	printf("====================================================================================================================================================\n");
	printf("||               Player name               |             Team name            |   Attacking power    |   Defending power   |   Value   |    id    ||\n");
	while (fread(&player, sizeof(Players),1, playersFile))
	{
		int position = ftell(playersFile);
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
	admin_page();
}

void show_teams() {
	system("cls");
	Teams team;
	FILE* teamsFile = fopen("teams.txt", "rb");
	char  teamChoice[MAX_LENGTH], teamsData[100][MAX_LENGTH];
	int choice, index = 0;

	printf("\n");
	printf("==================================================================================\n");
	printf("||               Team name               |   trophies   |   Number of players   ||\n");

	while (fread(&team, sizeof(team), 1, teamsFile)) {
		int position = ftell(teamsFile);
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
	admin_page();
}

void admin_page() {

	int choice;
	system("cls");
	printf("ADMIN PAGE\n");
	printf("1)Add team\n");
	printf("2)Add player\n");
	printf("3)Show teams\n");
	printf("4)Show players\n");
	if (transferWindow)printf("5)Start League\n");
	printf("6)Back\n");
	int flag = 0;
	do
	{
		flag = 0;
		printf("\nEnter your choice: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:add_team(); break;
		case 2:add_player(); break;
		case 3:show_teams(); break;
		case 4:show_players(); break;
		case 5:
			if (transferWindow) {
				int status = number_of_objects(4);
				if (status < 4) {
					printf("There aren't enough ready teams!\n");
					flag = 1;
					continue;
				}
				start_league_page();
			}
				
		default:
			login_page();
			break;
		}
	} while (flag);


}

void start_league(int selectedIds[4]) {
	TeamDetails teamdetail,all_teams[4];
	teamdetail.ga = 0;
	teamdetail.gf = 0;
	teamdetail.gd = 0;
	teamdetail.lost = 0;
	teamdetail.points = 0;
	teamdetail.won = 0;
	teamdetail.numberOfGames = 0;
	teamdetail.drawn = 0;
	for (int i = 0; i < 3; i++) {
		teamdetail.othersid[i] = -1;
	}
	Teams team;
	FILE* teamsFile = fopen("teams.txt", "rb");
	//FILE* leagueFile = fopen("league.txt", "wb");
	int index = 0;
	while (fread(&team,TEAMS_SIZE,1,teamsFile))
	{
		if (!is_in(selectedIds, team.id, 4))continue;
		strcpy(teamdetail.name, team.name);
		teamdetail.id = team.id;
		//fwrite(&teamdetail, sizeof(teamdetail), 1, leagueFile);
		all_teams[index] = teamdetail;
		index++;
	}
	fclose(teamsFile);
	//fclose(leagueFile);
	srand(time(NULL));
	int id1, id2, id3, id4;
	for (int i = 0; i < 3; i++) {
		int numbers[4] = { 0,1,2,3 };
		do
		{
			id1 = rand() % 4;
			id2 = rand() % 4;
			if (id1 != id2 && !is_in(all_teams[id1].othersid, all_teams[id2].id, 3))break;
		} while (true);
		numbers[id1] = -1;
		numbers[id2] = -1;

		for (int j = 0; j < 4; j++) {
			if (numbers[j] != -1){
				id3 = numbers[j];
				numbers[j] = -1;
				break;
			}
		}
		for (int j = 0; j < 4; j++) {
			if (numbers[j] != -1) {
				id4 = numbers[j];
				numbers[j] = -1;
				break;
			}
		}

		all_teams[id1].othersid[i] = all_teams[id2].id;
		all_teams[id2].othersid[i] = all_teams[id1].id;
		all_teams[id3].othersid[i] = all_teams[id4].id;
		all_teams[id4].othersid[i] = all_teams[id3].id;
		strcpy(all_teams[id1].fixture[i], all_teams[id2].name);
		strcpy(all_teams[id2].fixture[i], all_teams[id1].name);
		strcpy(all_teams[id3].fixture[i], all_teams[id4].name);
		strcpy(all_teams[id4].fixture[i], all_teams[id3].name);
	}
	for (int i = 0; i < 3; i++) {
		strcpy(all_teams[0].fixture[i + 3], all_teams[0].fixture[i]);
		strcpy(all_teams[1].fixture[i + 3], all_teams[1].fixture[i]);
		strcpy(all_teams[2].fixture[i + 3], all_teams[2].fixture[i]);
		strcpy(all_teams[3].fixture[i + 3], all_teams[3].fixture[i]);
	}
	all_teams;

}

void start_league_page() {
	system("cls");
	Teams team;
	FILE* teamsFile = fopen("teams.txt", "rb");
	char  teamChoice[MAX_LENGTH], teamsData[100];
	int choice, index = 0;

	printf("\n");
	printf("=============================================================================================\n");
	printf("||               Team name               |   trophies   |   Number of players   |    id    ||\n");

	while (fread(&team, sizeof(team), 1, teamsFile)) {
		if (!team.ready) continue;
		teamsData[index] = team.id;

		printf("||---------------------------------------|--------------|-----------------------|----------||\n||");
		print_col_char(team.name, 39);
		print_col_int(team.trophies, 14);
		print_col_int(team.numberOfPlayers, 23);
		print_col_int(team.id, 10);
		printf("|");
		printf("\n");
		index++;
	}
	printf("=============================================================================================\n");
	fclose(teamsFile);

	int flag = 0, selectedTeams[4], j = 0;
	do
	{
		flag = 0;
		printf("Enter %s id of the team you want to choose: ", j == 0 ? "" : "the next");
		scanf("%d", &choice);
		for (int i = 0; i < index; i++) {
			if (teamsData[i] == choice) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			printf("This id is not available!\n");
		}
		else if (is_in(selectedTeams, choice, j))
			printf("You have chosen this team before!\n");
		else {
			printf("Id %d selected!\n", choice);
			selectedTeams[j] = choice;
			j++;
		}
		if (j == 4)break;
	} while (true);
	start_league(selectedTeams);
}

int number_of_objects(int number1) {
	FILE* config = fopen("config.txt", "r");
	char data[10], teamNumber[10] = "";
	fgets(data, 10, config);
	getting_info(data, teamNumber, number1 - 1, number1);
	int number;
	sscanf(teamNumber, "%d", &number);
	fclose(config);
	return number;
}

void change_password(char teamName[MAX_LENGTH], char password[MAX_LENGTH]) {
	Teams team;
	FILE* teamsFile = fopen("teams.txt", "rb");
	while (fread(&team, sizeof(Teams), 1, teamsFile))
	{
		if (strcmp(team.name, teamName) == 0) {
			strcpy(team.password, password);
			break;
		}
	}
	change_teamsfile(team);
}

int is_in(int playersids[PLAYERS_SIZE], int id, int length) {
	for (int i = 0; i < length; i++) {
		if (playersids[i] == id) return 1;
	}
	return 0;
}

void change_teamsfile(Teams team) {

	FILE* teamsFile = fopen("teams.txt", "rb");
	FILE* temp = fopen("temp.txt", "wb");
	int number = number_of_objects(1);
	Teams fileTeam;
	while (fread(&fileTeam, TEAMS_SIZE, 1, teamsFile))
	{
		fwrite(&fileTeam, TEAMS_SIZE, 1, temp);
	}
	fclose(teamsFile);
	fclose(temp);

	FILE* temp2 = fopen("temp.txt", "rb");
	FILE* teamsFile2 = fopen("teams.txt", "wb");
	for (int j = 0; j < number; j++) {
		fread(&fileTeam, TEAMS_SIZE, 1, temp2);
		if (strcmp(fileTeam.name, team.name) == 0) fwrite(&team, TEAMS_SIZE, 1, teamsFile2);
		else fwrite(&fileTeam, TEAMS_SIZE, 1, teamsFile2);
	}
	fclose(teamsFile2);
	fclose(temp2);
}

void change_playersfile(Players player) {
	FILE* playersFile = fopen("players.txt", "rb");
	FILE* temp = fopen("temp.txt", "wb");
	int number2 = number_of_objects(2);//number2: number of players
	Players filePlayer;
	while (fread(&filePlayer, PLAYERS_SIZE, 1, playersFile))
	{
		fwrite(&filePlayer, PLAYERS_SIZE, 1, temp);
	}
	fclose(playersFile);
	fclose(temp);

	FILE* temp2 = fopen("temp.txt", "rb");
	FILE* playersFile2 = fopen("players.txt", "wb");
	for (int j = 0; j < number2; j++) {
		fread(&filePlayer, PLAYERS_SIZE, 1, temp2);
		if (filePlayer.id == player.id) fwrite(&player, PLAYERS_SIZE, 1, playersFile2);
		else fwrite(&filePlayer, PLAYERS_SIZE, 1, playersFile2);
	}
	fclose(playersFile2);
	fclose(temp2);
}

void buy_player(Players player, Teams team) {
	strcpy(player.teamName, team.name);
	team.players[team.numberOfPlayers] = player;
	team.numberOfPlayers++;
	team.budget -= player.value;

	change_teamsfile(team);
	change_playersfile(player);
}

void buy_player_page(Teams team) {
	system("cls");
	FILE* playersFile = fopen("players.txt", "rb");
	Players player;
	int numbofPlayers = number_of_objects(2);
	int index = 0, * playersids = (int*)malloc(numbofPlayers * sizeof(int));
	printf("\n");
	printf("=================================================================================================================\n");
	printf("||               Player name               |   Attacking power    |   Defencing power   |   Value   |    id    ||\n");
	for (int i = 20; i > 9; i--) {
		fseek(playersFile, 0, SEEK_SET);
		while (fread(&player, PLAYERS_SIZE, 1, playersFile))
		{
			if (strcmp(player.teamName, "Free agent") != 0 && !is_in(playersids, player.id, index)) continue;
			if (i != player.value) continue;

			printf("||-----------------------------------------|----------------------|---------------------|-----------|----------||\n||");
			print_col_char(player.name, 41);
			print_col_int(player.attackingPower, 22);
			print_col_int(player.defencingPower, 21);
			print_col_int(player.value, 11);
			print_col_int(player.id, 10);
			printf("|");
			printf("\n");
			playersids[index] = player.id;
			index++;
		}
	}
	printf("=================================================================================================================\n");
	fseek(playersFile, 0, SEEK_SET);
	int choice, flag = 0;
	do
	{
		printf("Please enter the player id or -1 to go back: ");
		scanf("%d", &choice);
		if (choice == -1)break;
		fseek(playersFile, 0, SEEK_SET);
		while (fread(&player, PLAYERS_SIZE, 1, playersFile))
		{
			if (is_in(playersids, player.id, index) && choice == player.id) {
				if (team.budget - player.value < 0) {
					printf("Your budget is not enough! ");
					break;
				}
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			printf("Player id is not available!\n");
		}
	} while (flag == 0);
	if (choice != -1)buy_player(player, team);
	coach_page(team.name);
}

void sell_player(Players player, Teams team, int index) {
	team.players[index] = team.players[team.numberOfPlayers - 1];
	strcpy(player.teamName, "Free agent");
	team.numberOfPlayers--;
	team.budget += player.value;

	change_playersfile(player);
	change_teamsfile(team);
}

void sell_player_page(Teams team) {
	system("cls");
	printf("\n");
	printf("=================================================================================================================\n");
	printf("||               Player name               |   Attacking power    |   Defencing power   |   Value   |    id    ||\n");
	for (int i = 20; i > 9; i--) {
		for (int j = 0; j < team.numberOfPlayers; j++) {
			if (team.players[j].value != i) continue;
			printf("||-----------------------------------------|----------------------|---------------------|-----------|----------||\n||");
			print_col_char(team.players[j].name, 41);
			print_col_int(team.players[j].attackingPower, 22);
			print_col_int(team.players[j].defencingPower, 21);
			print_col_int(team.players[j].value, 11);
			print_col_int(team.players[j].id, 10);
			printf("|");
			printf("\n");
		}
	}
	printf("=================================================================================================================\n");

	int choice, flag = 0, index = 0;
	do
	{
		printf("Please enter the player id or enter -1 to go back: ");
		scanf("%d", &choice);
		if (choice == -1)break;
		for (int i = 0; i < team.numberOfPlayers; i++) {
			if (team.players[i].id == choice) {
				flag = 1;
				index = i;
				break;
			}
		}
		if (flag == 0) printf("This id is not available\n");
	} while (flag == 0);
	if (choice != -1)sell_player(team.players[index], team, index);
	coach_page(team.name);
}

void submit_squad(Teams team) {
	team.ready = 1;
	config_file_handling(3);
	printf("Your squad submitted!\n");
	change_teamsfile(team);
}

void coach_page(char teamName[MAX_LENGTH]) {
	system("cls");
	int choice;
	Teams team;
	FILE* teamsFile = fopen("teams.txt", "rb");

	while (fread(&team, sizeof(Teams), 1, teamsFile))
	{
		if (strcmp(team.name, teamName) == 0) {
			break;
		}
	}
	fclose(teamsFile);
	printf("Team budget: %d    Number of players: %d\n", team.budget, team.numberOfPlayers);
	printf("1)Buy a player\n");
	printf("2)Sell a player\n");
	if (transferWindow) printf("3)Submit squad\n");
	else printf("3)Select squad\n");
	printf("4)League Standing\n");
	printf("5)Fixtures\n");
	printf("6)Upcoming Opponent\n");
	printf("7)Change Password\n");
	printf("8)Upcoming Opponent\n");
	printf("9)Back\n");
	int flag = 1;
	do
	{
		printf("\nEnter your choice: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			if (!transferWindow) {
				printf("Transfer window is closed!\n");
			}
			else if (team.ready) {
				printf("You have submitted your squad!\n");
			}
			else if (team.numberOfPlayers == 8) {
				printf("You cannot buy more players!\n");
			}
			else {
				buy_player_page(team);
			}break;
		case 2:
			if (!transferWindow) {
				printf("Transfer window is closed!\n");
			}
			else if (team.ready) {
				printf("You have submitted your squad!\n");
			}
			else if (team.numberOfPlayers == 0) {
				printf("You don't have any players to sell!\n");
			}
			else {
				sell_player_page(team);
			}break;
		case 3:
			if (transferWindow) {
				if (team.numberOfPlayers != 8) printf("You don't have enough players to submit your squad!\n");
				else if(team.ready) printf("You have submitted your squad!\n");
				else { 
					submit_squad(team);
					team.ready = 1;
				}
			}
			else {
				//select_squad();
			}
			break;
		default:
			login_page();
			flag = 0;
			break;
		}
	} while (flag);


}

int login() {
	system("cls");
	Teams team;
	char teamname[MAX_LENGTH], password[MAX_LENGTH];
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
			FILE* teamsFile = fopen("teams.txt", "rb");
			while (fread(&team, sizeof(Teams), 1, teamsFile)) {
				if (strcmp(team.name, teamname) == 0 && strcmp(team.password, password) == 0) {
					coach_page(teamname);
					break;
				}
			}
			fclose(teamsFile);
		}
		printf("Team Name or password is incorrect!!\n");
	} while (1);
}

void forget_password() {
	system("cls");
	Teams team;
	int flag = 0;
	char email[MAX_LENGTH], teamName[MAX_LENGTH];
	do
	{

		printf("Team name: ");
		scanf("%s", teamName);
		printf("Email: ");
		scanf("%s", email);
		FILE* teamsFile = fopen("teams.txt", "rb");
		char password[MAX_LENGTH];
		while (fread(&team, sizeof(Teams), 1, teamsFile))
		{
			if (strcmp(team.name, teamName) == 0 && strcmp(team.email, email) == 0) {
				flag = 1;
				fclose(teamsFile);
				printf("Enter new password: ");
				scanf("%s", password);
				change_password(teamName, password);
			}
		}
		if (flag == 0) {
			printf("Team name or email is incorrect!!\n");
			fclose(teamsFile);
		}
	} while (flag == 0);

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

void files_initialize() {
	if (!file_exists("teams.txt")) {
		FILE* teams = fopen("teams.txt", "w");
		fclose(teams);
	}

	if (!file_exists("players.txt")) {
		FILE* players = fopen("players.txt", "w");
		fclose(players);
	}

	if (!file_exists("config.txt")) {
		FILE* config = fopen("config.txt", "w");
		fprintf(config, "0,0,1,0,");//nomber of teams, number of players, transferWindow, numberOfReadyTeams
		fclose(config);
	}
	else {
		transferWindow = number_of_objects(3);
	}
}

