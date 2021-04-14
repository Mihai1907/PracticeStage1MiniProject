#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define nr_teams 16

typedef struct team
{
	char* name;
	int g;
	int p;
};

typedef struct match
{
	char* host;
	char* guest;
	int g_host;
	int g_guest;
};

void create_teams(team* teams, FILE* file)
{
	if (file == NULL)
		perror("Unable to open the file!");
	char s[128];
	int i = 0;
	while(fgets(s, sizeof(s), file))
	{
		s[strlen(s) - 1] = '\0';
		char* new_s = (char*)malloc(sizeof(s)); 
		strcpy(new_s, s);
		teams[i].name = new_s;
		teams[i].g = 0;
		teams[i].p = 0;
		i++;
	}
	free(s);
}

int search(team* teams, const char* s)
{
	for(int i = 0; i < nr_teams; i++)
		if (!strcmp(teams[i].name, s))
			return i;
	return -1;
}

void display_classment(team* teams)
{
	for (int i = 0; i < nr_teams; i++)
		if (teams[i].p != 1)
			printf("%d. %s %d points\n", i + 1, teams[i].name, teams[i].p);
		else
			printf("%d. %s %d point\n", i + 1, teams[i].name, teams[i].p);
	printf("\n");
}

void matches(team* teams, FILE* file)
{
	FILE* games = fopen("games.txt", "w+");
	if (file == NULL)
		perror("Unable to open the file!");
	char s[128], q[128];
	int x = 0, n;
	match m;
	m.host = (char*)malloc(128 * sizeof(char));
	m.guest = (char*)malloc(128 * sizeof(char));
	int pos_h, pos_g;
	while(fgets(s, sizeof(s), file))
	{
		s[strlen(s) - 1] = '\0';
		switch (x % 4)
		{
			case 0:
				strcpy(m.host, s);
				pos_h = search(teams, s);
				break;
			case 1:
				strcpy(m.guest, s);
				pos_g = search(teams, s);
				break;
			case 2:
				m.g_host = atoi(s);
				break;
			case 3:
				m.g_guest = atoi(s);
				teams[pos_h].g += m.g_host - m.g_guest;
				teams[pos_g].g += m.g_guest - m.g_host; 
				if (m.g_host > m.g_guest)
					teams[pos_h].p += 3;
				else
					if (m.g_host < m.g_guest)
						teams[pos_g].p += 3;
					else
					{
						teams[pos_h].p++;
						teams[pos_g].p++;
					}
				fprintf(games, "%s %d - %d %s\n", m.host, m.g_host, m.g_guest, m.guest);
				n = sprintf(q, "%s %d - %d %s\n", m.host, m.g_host, m.g_guest, m.guest);
				q[strlen(q) - 1] = '\0';
				break;
		}
		x++;
	}
	fclose(games);
}

void swap(int &a, int &b)
{
    a += b;
    b = a - b;
    a -= b;
}

void swapp(team &a, team &b)
{
	team aux;
	aux.name = (char*)malloc(128 * sizeof(char));
	swap(a.p, b.p);
	swap(a.g, b.g);
	strcpy(aux.name, a.name);
	strcpy(a.name, b.name);
	strcpy(b.name, aux.name);
}

void sort_teams(team* teams)
{
	for (int i = 0; i < nr_teams - 1; i++)
		for (int j = 0; j < nr_teams - 1 - i; j++)
			if (teams[j].p < teams[j + 1].p)
			{
				swapp(teams[j], teams[j + 1]);
			}
			else
			{
				if (teams[j].p == teams[j + 1].p)
				{
					if (teams[j].g < teams[j + 1].g)
					{
						swapp(teams[j], teams[j + 1]);
					}
				}
			}
}

void display_winner(team* teams)
{
	printf("Winner name: %s\n", teams[0].name);
	printf("Points: %d\n", teams[0].p);
	printf("Goals: %d\n\n", teams[0].g);
}

void display_team(team* teams, const char* s)
{
	int pos = search(teams, s);
	if (pos >= 0)
	{
		printf("Team name: %s\n", teams[pos].name);
		printf("Points: %d\n", teams[pos].p);
		printf("Goals: %d\n", teams[pos].g);
		printf("Position: %d\n", pos + 1);
	}
	else
		printf("%s is not from this championship!\n\n", s);
}

void commands()
{
	printf("Type 1 to display the classment\n");
	printf("Type 2 to display the winner\n");
	printf("Type 3 to display a specific team\n");
	printf("Type 4 to display the games\n");
	printf("Type 5 to display list of commands\n");
	printf("Type 0 to end the program\n\n");
}

void display_games()
{
	FILE* games = fopen("games.txt", "r");
	if (games == NULL)
		perror("Unable to open the file!");
	char s[128];
	while(fgets(s, sizeof(s), games))
		printf("%s", s);
	fclose(games);
	printf("\n");
}

int main()
{
	team* teams = (team*)calloc(nr_teams, sizeof(team));
	FILE* file = fopen("teams.txt", "r");
	char s[128];
	int command = 1;
	create_teams(teams, file);
	fclose(file);
	file = fopen("stages.txt", "r");
	matches(teams, file);
	sort_teams(teams);
	fclose(file);
	commands();
	while(command)
	{
		printf("Select your option: ");
		scanf("%d", &command);
		switch (command)
		{
			case 0:
				printf("Thanks for using my app!");
				break;
			case 1:
				display_classment(teams);
				break;
			case 2:
				display_winner(teams);
				break;
			case 3:
				printf("Name: ");
				getchar();
				scanf("%[^\n]", s);
				display_team(teams, s);
				break;
			case 4:
				display_games();
				break;
			case 5:
				commands();
				break;
			default:
				printf("%d is not a valid option!\n", command);
				break;
		}
	}
	free(file);
	free(teams);
	return 0;
}
