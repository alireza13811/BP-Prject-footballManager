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
int leagueStatus;

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
	char results[6][30];
	char name[MAX_LENGTH];
	char fixture[6][MAX_LENGTH];
	Players players[5];
} TeamDetails;

bool file_exists(const char* filename);
void getting_info(char data[MAX_LENGTH], char* item, int start, int end);
int config_file_handling(int change);
void add_team();
void add_player();
void print_col_char(char item[2*MAX_LENGTH], int characters);
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
void coach_page(Teams team);
int login();
void forget_password();
void login_page();
void files_initialize();
void submit_squad(Teams team);
void start_league_page();
void league_standing();
int compare(TeamDetails team1, TeamDetails team2);
void swap(TeamDetails* a, TeamDetails* b);
void sort(TeamDetails* target);
void league_standing(Teams team);
void set_color(int color);
void reset_color();

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
	else if(change == 31) {
		fprintf(config2, "0,");
	}
	else {
		fprintf(config2, "%s,", numbOfReadyTeams);
	}

	fprintf(config2, "%d,", leagueStatus);// updating league status

	fclose(config2);
	return number;
}

void warning(char* text) {
	set_color(2);
	printf(text);
	reset_color();
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
			warning("\nThis team name is not available!\n");
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
		else warning("Attacking power must be between 0 and 100!\n");
	} while (true);
	do
	{
		printf("Defencing power: ");
		scanf("%d", &defendingPower);
		if (defendingPower >= 0 && defendingPower <= 100) break;
		else warning("Defencing power must be between 0 and 100!\n");
	} while (true);
	do
	{
		printf("Value: ");
		scanf("%d", &value);
		if (value >= 10 && value <= 20) break;
		else warning("Value must be between 10 and 20!\n");
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

void set_color(int color) {
	if (color == 1) printf("\033[0;36m");//Cyan color
	else if (color == 2) printf("\033[0;31m");//Red color
	else printf("\033[0;32m");//Green color
}

void reset_color() {
	printf("\033[0m");
}

void print_col_char(char item[2*MAX_LENGTH], int characters,int color) {
	int length = strlen(item);
	int spaces = (characters - length) / 2;
	set_color(color);
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
	reset_color();
	printf("|");
}

void print_col_int(int item, int characters,int color) {
	int digits = 0, item2 = item;
	while (item2 > 0)
	{
		item2 /= 10;
		digits++;
	}
	if (item == 0) digits = 1;
	int length = digits;
	int spaces = (characters - length) / 2;
	set_color(color);
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
	reset_color();
	printf("|");
}

void team_info(char teamName[MAX_LENGTH]) {
	system("cls");
	Players player;

	FILE* playersFile = fopen("players.txt", "rb");
	int numberOfPlayers = 0, color = 0;
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
		print_col_char(player.name, 41,color);
		print_col_int(player.value, 11,color);
		print_col_int(player.attackingPower, 22,color);
		print_col_int(player.defencingPower, 21,color);
		printf("|");
		printf("\n");
		numberOfPlayers++;
		color = !color;
	}
	printf("======================================================================================================\n");
	fclose(playersFile);
	if (numberOfPlayers == 0) warning("This team has no players!!\n");
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
	int numberOfPlayers = 0, color = 0;
	fseek(playersFile, 0, SEEK_SET);
	printf("\n");
	printf("====================================================================================================================================================\n");
	printf("||               Player name               |             Team name            |   Attacking power    |   Defending power   |   Value   |    id    ||\n");
	while (fread(&player, sizeof(Players),1, playersFile))
	{
		int position = ftell(playersFile);
		//41 34 22 21 11 10
		printf("||-----------------------------------------|----------------------------------|----------------------|---------------------|-----------|----------||\n||");
		print_col_char(player.name, 41,color);
		print_col_char(player.teamName, 34,color);
		print_col_int(player.attackingPower, 22,color);
		print_col_int(player.defencingPower, 21,color);
		print_col_int(player.value, 11,color);
		print_col_int(player.id, 10,color);
		printf("|");
		printf("\n");
		numberOfPlayers++;
		color = !color;

	}
	fclose(playersFile);
	printf("====================================================================================================================================================\n");
	
	if (numberOfPlayers == 0) warning("There is no player!!\n");
	else printf("Press enter to continue\n");
	getchar();
	getchar();
	admin_page();
}

void show_teams() {
	system("cls");
	Teams team;
	FILE* teamsFile = fopen("teams.txt", "rb");
	char  teamChoice[MAX_LENGTH], teamsData[100][MAX_LENGTH];
	int choice, index = 0, color = 0;

	printf("\n");
	printf("==================================================================================\n");
	printf("||               Team name               |   trophies   |   Number of players   ||\n");

	while (fread(&team, sizeof(team), 1, teamsFile)) {
		int position = ftell(teamsFile);
		strcpy(teamsData[index], team.name);

		printf("||---------------------------------------|--------------|-----------------------||\n||");
		print_col_char(team.name, 39,color);
		print_col_int(team.trophies, 14,color);
		print_col_int(team.numberOfPlayers, 23,color);
		printf("|");
		printf("\n");
		index++;
		color = !color;
	}
	printf("==================================================================================\n");
	fclose(teamsFile);
	if (index == 0) {
		warning("There isn't any team!!\nPress enter to continue");
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
			if (correct == 0) warning("Team name is incorrect\n");
			
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
	else printf("5)Star week %d-th games\n", leagueStatus);
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
					warning("There aren't enough ready teams!\n");
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
	int index = 0;
	while (fread(&team,TEAMS_SIZE,1,teamsFile))
	{
		if (!is_in(selectedIds, team.id, 4))continue;
		strcpy(teamdetail.name, team.name);
		teamdetail.id = team.id;
		all_teams[index] = teamdetail;
		index++;
	}
	fclose(teamsFile);
	
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

	FILE* leagueFile = fopen("league.txt", "wb");
	for (int i = 0; i < 4; i++) {
		fwrite(&all_teams[i], sizeof(TeamDetails), 1, leagueFile);
	}
	fclose(leagueFile);
	leagueStatus = 1;
	transferWindow = 0;
	config_file_handling(4);
	config_file_handling(31);
}

void start_league_page() {
	system("cls");
	Teams team;
	FILE* teamsFile = fopen("teams.txt", "rb");
	char  teamChoice[MAX_LENGTH];
	int choice, index = 0, color = 0, teamsData[100];

	printf("\n");
	printf("=============================================================================================\n");
	printf("||               Team name               |   trophies   |   Number of players   |    id    ||\n");

	while (fread(&team, sizeof(team), 1, teamsFile)) {
		if (!team.ready) continue;
		teamsData[index] = team.id;

		printf("||---------------------------------------|--------------|-----------------------|----------||\n||");
		print_col_char(team.name, 39,color);
		print_col_int(team.trophies, 14,color);
		print_col_int(team.numberOfPlayers, 23,color);
		print_col_int(team.id, 10,color);
		printf("|");
		printf("\n");
		index++;
		color = !color;
	}
	printf("=============================================================================================\n");
	fclose(teamsFile);

	int selectedTeams[4], j = 0;
	do
	{
		char text[10];
		if (j == 0)strcpy(text, "first");
		else if (j == 1)strcpy(text, "second");
		else if (j == 2)strcpy(text, "third");
		else strcpy(text, "fourth");

		printf("Enter id of the %s team you want to choose: ", text);
		scanf("%d", &choice);
		if (!is_in(teamsData,choice,index)) {
			warning("This id is not available!\n");
		}
		else if (is_in(selectedTeams, choice, j))
			warning("You have chosen this team before!\n");
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
	char data[20], teamNumber[10] = "";
	fgets(data, 20, config);
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
	int index = 0, * playersids = (int*)malloc(numbofPlayers * sizeof(int)), color = 0;
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
			print_col_char(player.name, 41,color);
			print_col_int(player.attackingPower, 22,color);
			print_col_int(player.defencingPower, 21,color);
			print_col_int(player.value, 11,color);
			print_col_int(player.id, 10,color);
			printf("|");
			printf("\n");
			playersids[index] = player.id;
			index++;
			color = !color;
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
					warning("Your budget is not enough! ");
					break;
				}
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			warning("Player id is not available!\n");
		}
	} while (flag == 0);
	if (choice != -1)buy_player(player, team);
	coach_page(team);
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
	int color = 0;
	for (int i = 20; i > 9; i--) {
		for (int j = 0; j < team.numberOfPlayers; j++) {
			if (team.players[j].value != i) continue;
			printf("||-----------------------------------------|----------------------|---------------------|-----------|----------||\n||");
			print_col_char(team.players[j].name, 41,color);
			print_col_int(team.players[j].attackingPower, 22,color);
			print_col_int(team.players[j].defencingPower, 21,color);
			print_col_int(team.players[j].value, 11,color);
			print_col_int(team.players[j].id, 10,color);
			printf("|");
			printf("\n");
			color = !color;
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
		if (flag == 0) warning("This id is not available\n");
	} while (flag == 0);
	if (choice != -1)sell_player(team.players[index], team, index);
	coach_page(team);
}

void submit_squad(Teams team) {
	team.ready = 1;
	config_file_handling(3);
	printf("Your squad submitted!\n");
	change_teamsfile(team);
}

int compare(TeamDetails team1,TeamDetails team2) {
	if (team1.points != team2.points) 
		return team1.points > team2.points;
	
	if (team1.gd != team2.gd)
		return team1.gd > team2.gd;
	
	if (team1.ga != team2.ga)
		return team1.ga > team2.ga;
	
	return team1.gf >= team2.gf;
}

void swap(TeamDetails* a, TeamDetails* b) {
	TeamDetails temp = *a;
	*a = *b;
	*b = temp;
}

void sort(TeamDetails *target) {
	int repeated = 0;
	do
	{
		repeated = 0;
		for (int i = 0; i < 3; i++) {
			if (!compare(target[i],target[i+1])) {
				swap(&target[i],&target[i+1]);
				repeated++;
			}
		}
	} while (repeated);
}

void league_standing(Teams team) {
	TeamDetails all_teams[4];
	int index = 0, color = 0;
	FILE* league = fopen("league.txt", "r");
	while (fread(&all_teams[index],sizeof(TeamDetails),1,league))
	{
		index++;
	}
	fclose(league);
	sort(all_teams);
	//Team Name, Played, Won, Drawn, Lost, GF, GA, GD,Points
	system("cls");
	printf("\n");
	printf("============================================================================================================================================\n");
	printf("||               Team name               |   Played    |   Won   |   Drawn   |    Lost    |    GA    |    GF    |    GD    |    Points    ||\n");
	for (int i = 0; i < 4; i++) {
		printf("||---------------------------------------|-------------|---------|-----------|------------|----------|----------|----------|--------------||\n||");
		print_col_char(all_teams[i].name, 39, color);
		print_col_int(all_teams[i].numberOfGames, 13, color);
		print_col_int(all_teams[i].won, 9, color);
		print_col_int(all_teams[i].drawn, 11, color);
		print_col_int(all_teams[i].lost, 12, color);
		print_col_int(all_teams[i].ga, 10, color);
		print_col_int(all_teams[i].gf, 10, color);
		print_col_int(all_teams[i].gd, 10, color);
		print_col_int(all_teams[i].points, 14, color);
		printf("|");
		printf("\n");
		color = !color;
	}
	printf("============================================================================================================================================\n");
	printf("Press enter to continue!\n");
	getchar();
	getchar();
	coach_page(team);
}

void select_squad(Teams team) {
	system("cls");
	printf("\n");
	printf("=================================================================================================================\n");
	printf("||               Player name               |   Attacking power    |   Defencing power   |   Value   |    id    ||\n");
	int color = 0, all_ids[8], index = 0;
	for (int i = 20; i > 9; i--) {
		for (int j = 0; j < team.numberOfPlayers; j++) {
			if (team.players[j].value != i) continue;
			printf("||-----------------------------------------|----------------------|---------------------|-----------|----------||\n||");
			print_col_char(team.players[j].name, 41, color);
			print_col_int(team.players[j].attackingPower, 22, color);
			print_col_int(team.players[j].defencingPower, 21, color);
			print_col_int(team.players[j].value, 11, color);
			print_col_int(team.players[j].id, 10, color);
			printf("|");
			printf("\n");
			color = !color;
			all_ids[index] = team.players[j].id;
			index++;
		}
	}
	printf("=================================================================================================================\n");
	
	int choice, j = 0, selectedIds[5];
	do
	{
		char text[10];
		if (j == 0)strcpy(text, "first");
		else if (j == 1)strcpy(text, "second");
		else if (j == 2)strcpy(text, "third");
		else if (j == 3)strcpy(text, "fourth");
		else strcpy(text, "fifth");

		printf("Enter id of the %s player you want to choose: ", text);
		scanf("%d", &choice);
		if (!is_in(all_ids, choice, 8))warning("This id is not available!\n");
		else if (is_in(selectedIds, choice, j)) warning("You have chosen this team before!\n");
		else {
			printf("Id %d selected!\n", choice);
			selectedIds[j] = choice;
			j++;
		}
		if (j == 5)break;
	} while (true);

	TeamDetails teamDetail;
	FILE* league = fopen("league.txt", "rb");
	FILE* temp = fopen("temp.txt", "wb");
	index = 0;
	while (fread(&teamDetail,sizeof(TeamDetails),1,league))
	{
		if (teamDetail.id == team.id) {
			for (int i = 0; i < 8; i++) {
				if (is_in(selectedIds, team.players[i].id, 5)) {
					teamDetail.players[index] = team.players[i];
					index++;
				}
			}
		}
		fwrite(&teamDetail, sizeof(TeamDetails), 1, temp);
	}
	fclose(league);
	fclose(temp);

	FILE* league2 = fopen("league.txt", "wb");
	FILE* temp2 = fopen("temp.txt", "rb");
	for (int i = 0; i < 4; i++) {
		fread(&teamDetail, sizeof(TeamDetails), 1, temp2);
		fwrite(&teamDetail, sizeof(TeamDetails), 1, league2);
	}
	fclose(league2);
	fclose(temp2);
	config_file_handling(3);
	coach_page(team);
}

void team_fixture(Teams team) {
	TeamDetails teamDetail;
	FILE* league = fopen("league.txt", "rb");
	while (fread(&teamDetail,sizeof(TeamDetails),1,league))
	{
		if (teamDetail.id == team.id)break;
	}
	fclose(league);
	system("cls");
	printf("\n");
	printf("||===================================================================================||\n");
	printf("||                                       Game                                        ||\n");
	for (int i = 0; i < teamDetail.numberOfGames; i++) {
		printf("||------------------------------------------------------------------------------------------||\n||");
		char temp[MAX_LENGTH];
		strcpy(temp, teamDetail.name);
		strcat(temp, " VS ");
		strcat(temp, teamDetail.fixture[i]);
		print_col_char(temp, 45, 0);
		printf("|");
		printf("\n");
		printf("||");
		print_col_char(teamDetail.results[i], 45, 0);
		printf("|");
		printf("\n");
	}

	for (int i = 0; i < 6 - teamDetail.numberOfGames; i++) {
		printf("||------------------------------------------------------------------------------------------||\n||");
		char temp[MAX_LENGTH];
		strcpy(temp, teamDetail.name);
		strcat(temp, " VS ");
		strcat(temp, teamDetail.fixture[i + teamDetail.numberOfGames]);
		print_col_char(temp, 45, 1);
		printf("|");
		printf("\n");
	}
	printf("||===================================================================================||\n");
	printf("Press enter to continue!\n");
	getchar();
	getchar();
	coach_page(team);
}

void upcoming_opponent(Teams team) {
	TeamDetails teamDetail;
	FILE* league = fopen("league.txt", "rb");
	while (fread(&teamDetail, sizeof(TeamDetails), 1, league))
	{
		if (teamDetail.id == team.id)break;
	}

	char teamName[MAX_LENGTH];
	strcpy(teamName, teamDetail.fixture[teamDetail.numberOfGames]);
	fseek(league, 0, SEEK_SET);
	while (fread(&teamDetail,sizeof(TeamDetails),1,league))
	{
		if (strcmp(teamName, teamDetail.name) == 0)break;
	}
	fclose(league);

	system("cls");
	printf("\n");
	printf("=================================================================================================================\n");
	printf("||               Player name               |   Attacking power    |   Defencing power   ||\n");
	int color = 0, sum = 0;;
	for (int i = 0; i < 5; i++) {
		printf("||-----------------------------------------|----------------------|---------------------||\n||");
		print_col_char(teamDetail.players[i].name, 41, color);
		print_col_int(teamDetail.players[i].attackingPower, 22, color);
		print_col_int(teamDetail.players[i].defencingPower, 21, color);
		printf("|");
		printf("\n");
		color = !color;
		sum += teamDetail.players[i].attackingPower + teamDetail.players[i].defencingPower;
	}
	printf("=================================================================================================================\n");
	printf("Total Power: %d\n", sum);
	printf("Press enter to continue!\n");
	getchar();
	getchar();
	coach_page(team);
}

void coach_page(Teams team) {
	system("cls");
	int choice;

	printf("Team budget: %d    Number of players: %d\n", team.budget, team.numberOfPlayers);
	printf("1)Buy a player\n");
	printf("2)Sell a player\n");
	if (transferWindow) printf("3)Submit squad\n");
	else printf("3)Select squad\n");
	printf("4)League Standing\n");
	printf("5)Fixtures\n");
	printf("6)Upcoming Opponent\n");
	printf("7)Change Password\n");
	printf("8)Exit\n");
	int flag = 1;
	do
	{
		printf("\nEnter your choice: ");
		scanf("%d", &choice);
		switch (choice)
		{
		case 1:
			if (!transferWindow) {
				warning("Transfer window is closed!\n");
			}
			else if (team.ready) {
				warning("You have submitted your squad!\n");
			}
			else if (team.numberOfPlayers == 8) {
				warning("You cannot buy more players!\n");
			}
			else {
				buy_player_page(team);
				flag = 0;
			}break;
		case 2:
			if (!transferWindow) {
				warning("Transfer window is closed!\n");
			}
			else if (team.ready) {
				warning("You have submitted your squad!\n");
			}
			else if (team.numberOfPlayers == 0) {
				warning("You don't have any players to sell!\n");
			}
			else {
				sell_player_page(team);
				flag = 0;
			}break;
		case 3:
			if (transferWindow) {
				if (team.numberOfPlayers != 8) warning("You don't have enough players to submit your squad!\n");
				else if(team.ready) warning("You have submitted your squad!\n");
				else { 
					submit_squad(team);
					team.ready = 1;
					flag = 0;
				}
			}
			else {
				if (!team.ready)warning("You are not in the league!\n");
				else {
					select_squad(team);
					flag = 0;
				}
			}
			break;
		case 4:
			if (!leagueStatus) warning("League has not started yet!\n");
			else if (!team.ready) warning("You are not in the league!\n");
			else {
				league_standing(team);
				flag = 0;
			}
			break;
		case 5:
			if (!leagueStatus) warning("League has not started yet!\n");
			else if (!team.ready) warning("You are not in the league!\n");
			else {
				team_fixture(team);
				flag = 0;
			}
			break;
		case 6:
			if (!leagueStatus) warning("League has not started yet!\n");
			else if (!team.ready) warning("You are not in the league!\n");
			else {
				upcoming_opponent(team);
				flag = 0;
			}
			break;
		case 7:
			forget_password();
			flag = 0;
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
					coach_page(team);
					break;
				}
			}
			fclose(teamsFile);
		}
		warning("Team Name or password is incorrect!!\n");
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
			warning("Team name or email is incorrect!!\n");
			fclose(teamsFile);
		}
	} while (flag == 0);
	login_page();
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
			break;
		}
		else if (choice == 2) {
			forget_password();
			break;
		}
		if (choice != 1 && choice != 2)warning("Please enter 1 or 2\n");
	} while (true);

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
		fprintf(config, "0,0,1,0,0,");//nomber of teams, number of players, transferWindow, numberOfReadyTeams, league status
		fclose(config);
	}
	else {
		transferWindow = number_of_objects(3);
		leagueStatus = number_of_objects(5);
	}
}

