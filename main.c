#include "GCL/gcl.h"
#include "PNF/pnf.h"
#include <conio.h>

#define WORLD_WIDTH 500
#define WORLD_HEIGHT 500


int screen = 0;
int selection = 0;
int world[WORLD_WIDTH][WORLD_HEIGHT];
int zoom = 1;
WINDOW window;
CHARACTER character_clear;
PERLINFACTORY perlinFactory;

char text[5];

typedef struct
{
	int x, y;
} PLAYER;

PLAYER player;

char* IntToString(int n)
{
	for (int i = 0; i < 5; i++) {text[i] = 32;}
	if (n == 0) {text[0] = 48;}
	else
	{
		int i = 0;
		int j = 1;
		while (j <= n) {i++; j *= 10;}
		j /= 10;
		for (int k = 0; k < i; k++)
		{
			text[k] = 48+((int)(n/j));
			n -= ((int)(n/j))*j;
			j /= 10;
		}
	}
}

void InitWorld()
{
	for (int y = 0; y < WORLD_HEIGHT; y++) {for (int x = 0; x < WORLD_WIDTH; x++) {world[y][x] = ValueNoise2D(&perlinFactory, x, y)*100;}}
	player.x = WORLD_WIDTH/2;
	player.y = WORLD_HEIGHT/2;
}

int GetTileColor(int level)
{
	if (level < -60) {return NavyBack;}
	else if (level < -30) {return BlueBack;}
	else if (level < 0) {return TealBack;}
	else if (level < 20) {return YellowBack;}
	else if (level < 45) {return LimeBack;}
	else if (level < 70) {return GreenBack;}
	else if (level < 90) {return SilverBack;}
	else {return WhiteBack;}
}

void DisplayOutlines()
{
	CHARACTER character_outlines;
	character_outlines.character = 45;
	character_outlines.color = WhiteFore;
	DrawLine(&window, 1, 0, WINDOW_WIDTH-3, 0, character_outlines, 1);
	DrawLine(&window, 1, WINDOW_HEIGHT-2, WINDOW_WIDTH-3, WINDOW_HEIGHT-2, character_outlines, 1);
	character_outlines.character = 124;
	DrawLine(&window, 0, 1, 0, WINDOW_HEIGHT-3, character_outlines, 1);
	DrawLine(&window, WINDOW_WIDTH-2, 1, WINDOW_WIDTH-2, WINDOW_HEIGHT-3, character_outlines, 1);
	character_outlines.character = 43;
	SetCharacter(&window, 0, 0, character_outlines);
	SetCharacter(&window, WINDOW_WIDTH-2, 0, character_outlines);
	SetCharacter(&window, 0, WINDOW_HEIGHT-2, character_outlines);
	SetCharacter(&window, WINDOW_WIDTH-2, WINDOW_HEIGHT-2, character_outlines);
}

void DisplayMenu()
{
	ClearWindow(&window, character_clear);
	DrawTextAt(&window, (WINDOW_WIDTH-38)/2, (WINDOW_HEIGHT-13)/2, "Creation de carte avec bruit de Perlin", NavyFore);
	CHARACTER character;
	character.color = WhiteFore;
	IntToString(perlinFactory.numOctaves);
	DrawTextAt(&window, (WINDOW_WIDTH-25)/2, (WINDOW_HEIGHT-13)/2+3, "< Nombre d'octaves : ", WhiteFore);
	int i = 0;
	while (text[i] != 32) {character.character = text[i]; SetCharacter(&window, (WINDOW_WIDTH-25)/2+21+i, (WINDOW_HEIGHT-13)/2+3, character); i++;}
	DrawTextAt(&window, (WINDOW_WIDTH-25)/2+21+i, (WINDOW_HEIGHT-13)/2+3, " >", WhiteFore);
	if (selection == 0) {character.character = 91; SetCharacter(&window, (WINDOW_WIDTH-25)/2-1, (WINDOW_HEIGHT-13)/2+3, character); character.character = 93; SetCharacter(&window, (WINDOW_WIDTH-25)/2+23+i, (WINDOW_HEIGHT-13)/2+3, character);}
	DrawTextAt(&window, (WINDOW_WIDTH-58)/2, (WINDOW_HEIGHT-13)/2+4, "(Plus ce nombre est petit, plus il y aura de points d'eau)", WhiteFore);
	IntToString(perlinFactory.persistence);
	DrawTextAt(&window, (WINDOW_WIDTH-26)/2, (WINDOW_HEIGHT-13)/2+6, "< Persistence : ", WhiteFore);
	i = 0;
	while (text[i] != 32) {character.character = text[i]; SetCharacter(&window, (WINDOW_WIDTH-26)/2+16+i, (WINDOW_HEIGHT-13)/2+6, character); i++;}
	DrawTextAt(&window, (WINDOW_WIDTH-26)/2+16+i, (WINDOW_HEIGHT-13)/2+6, " (/10) >", WhiteFore);
	if (selection == 1) {character.character = 91; SetCharacter(&window, (WINDOW_WIDTH-26)/2-1, (WINDOW_HEIGHT-13)/2+6, character); character.character = 93; SetCharacter(&window, (WINDOW_WIDTH-26)/2+24+i, (WINDOW_HEIGHT-13)/2+6, character);}
	DrawTextAt(&window, (WINDOW_WIDTH-56)/2, (WINDOW_HEIGHT-13)/2+7, "(Plus ce nombre est grand, plus le relief sera prononce)", WhiteFore);
	IntToString(perlinFactory.primeIndex);
	DrawTextAt(&window, (WINDOW_WIDTH-13)/2, (WINDOW_HEIGHT-13)/2+9, "< Seed : ", WhiteFore);
	i = 0;
	while (text[i] != 32) {character.character = text[i]; SetCharacter(&window, (WINDOW_WIDTH-13)/2+9+i, (WINDOW_HEIGHT-13)/2+9, character); i++;}
	DrawTextAt(&window, (WINDOW_WIDTH-13)/2+9+i, (WINDOW_HEIGHT-13)/2+9, " >", WhiteFore);
	if (selection == 2) {character.character = 91; SetCharacter(&window, (WINDOW_WIDTH-13)/2-1, (WINDOW_HEIGHT-13)/2+9, character); character.character = 93; SetCharacter(&window, (WINDOW_WIDTH-13)/2+11+i, (WINDOW_HEIGHT-13)/2+9, character);}
	DrawTextAt(&window, (WINDOW_WIDTH-33)/2, (WINDOW_HEIGHT-13)/2+10, "(Ce nombre fait changer la carte)", WhiteFore);
	DrawTextAt(&window, (WINDOW_WIDTH-7)/2, (WINDOW_HEIGHT-13)/2+12, "Generer", RedFore);
	character.color = RedFore;
	if (selection == 3) {character.character = 91; SetCharacter(&window, (WINDOW_WIDTH-7)/2-1, (WINDOW_HEIGHT-13)/2+12, character); character.character = 93; SetCharacter(&window, (WINDOW_WIDTH+7)/2, (WINDOW_HEIGHT-13)/2+12, character);}
	DisplayOutlines();
	CHARACTER character_delimiter;
	character_delimiter.character = 32;
	character_delimiter.color = 0;
	DrawRectangle(&window, 1, 1, 50, 3, 1, character_delimiter, 0, character_delimiter);
	character_delimiter.color = WhiteFore;
	character_delimiter.character = 45;
	DrawLine(&window, 1, 4, 50, 4, character_delimiter, 1);
	character_delimiter.character = 124;
	DrawLine(&window, 51, 1, 51, 3, character_delimiter, 1);
	character_delimiter.character = 43;
	SetCharacter(&window, 51, 0, character_delimiter);
	SetCharacter(&window, 0, 4, character_delimiter);
	SetCharacter(&window, 51, 4, character_delimiter);
	DrawTextAt(&window, 2, 1, "Fleches du haut/bas : se deplacer dans les menus", WhiteFore);
	DrawTextAt(&window, 2, 2, "Fleches de gauche/droite : changer les valeurs", WhiteFore);
	DrawTextAt(&window, 2, 3, "Entree : valider", WhiteFore);
	DisplayWindow(&window);
}

void DisplayWorld()
{
	CHARACTER tile;
	tile.character = 32;
	for (int y = 0; y < WINDOW_HEIGHT-1; y++)
	{
		for (int x = 0; x < WINDOW_WIDTH/2-1; x++)
		{
			int n = 0;
			int total = 0;
			for (int y2 = 0; y2 < zoom; y2++) {for (int x2 = 0; x2 < zoom; x2++) {if (0 <= player.x+(x-WINDOW_WIDTH/4+1)*zoom+x2 && player.x+(x-WINDOW_WIDTH/4+1)*zoom+x2 < WORLD_WIDTH && 0 <= player.y+(y-WINDOW_HEIGHT/2+1)*zoom+y2 && player.y+(y-WINDOW_HEIGHT/2+1)*zoom+y2 < WORLD_HEIGHT) {n++; total += world[player.y+(y-WINDOW_HEIGHT/2+1)*zoom+y2][player.x+(x-WINDOW_WIDTH/4+1)*zoom+x2];}}}
			if (n == 0) {tile.color = 0;}
			else {tile.color = GetTileColor(total/n);}
			SetCharacter(&window, x*2, y, tile);
			SetCharacter(&window, x*2+1, y, tile);
		}
	}
	DisplayOutlines();
	CHARACTER character_delimiter;
	character_delimiter.character = 32;
	character_delimiter.color = 0;
	DrawRectangle(&window, 1, 1, 38, 4, 1, character_delimiter, 0, character_delimiter);
	character_delimiter.color = WhiteFore;
	character_delimiter.character = 45;
	DrawLine(&window, 1, 5, 38, 5, character_delimiter, 1);
	character_delimiter.character = 124;
	DrawLine(&window, 39, 1, 39, 4, character_delimiter, 1);
	character_delimiter.character = 43;
	SetCharacter(&window, 39, 0, character_delimiter);
	SetCharacter(&window, 0, 5, character_delimiter);
	SetCharacter(&window, 39, 5, character_delimiter);
	DrawTextAt(&window, 2, 1, "Fleches directionelles : se deplacer", WhiteFore);
	DrawTextAt(&window, 2, 2, "+/- : changer le zoom", WhiteFore);
	DrawTextAt(&window, 2, 3, "Echap : revenir au menu", WhiteFore);
	DrawTextAt(&window, 2, 4, "x =", WhiteFore);
	IntToString(player.x);
	int i = 0;
	while (text[i] != 32) {character_delimiter.character = text[i]; SetCharacter(&window, 6+i, 4, character_delimiter); i++;}
	DrawTextAt(&window, 7+i, 4, "; y =", WhiteFore);
	IntToString(player.y);
	int j = 0;
	while (text[j] != 32) {character_delimiter.character = text[j]; SetCharacter(&window, 13+i+j, 4, character_delimiter); j++;}
	DrawTextAt(&window, 14+i+j, 4, "; zoom = 1:", WhiteFore);
	IntToString(zoom);
	int k = 0;
	while (text[k] != 32) {character_delimiter.character = text[k]; SetCharacter(&window, 25+i+j+k, 4, character_delimiter); k++;}
	character_delimiter.character = 32;
	DrawRectangle(&window, 1, 6, 24, 13, 1, character_delimiter, 0, character_delimiter);
	character_delimiter.character = 45;
	DrawLine(&window, 1, 14, 25, 14, character_delimiter, 1);
	character_delimiter.character = 124;
	DrawLine(&window, 25, 6, 25, 13, character_delimiter, 1);
	character_delimiter.character = 43;
	SetCharacter(&window, 25, 5, character_delimiter);
	SetCharacter(&window, 0, 14, character_delimiter);
	SetCharacter(&window, 25, 14, character_delimiter);
	DrawTextAt(&window, 2, 6, "  ", NavyBack);
	DrawTextAt(&window, 5, 6, ": Eau tres profonde", WhiteFore);
	DrawTextAt(&window, 2, 7, "  ", BlueBack);
	DrawTextAt(&window, 5, 7, ": Eau profonde", WhiteFore);
	DrawTextAt(&window, 2, 8, "  ", TealBack);
	DrawTextAt(&window, 5, 8, ": Eau peu profonde", WhiteFore);
	DrawTextAt(&window, 2, 9, "  ", YellowBack);
	DrawTextAt(&window, 5, 9, ": Sable", WhiteFore);
	DrawTextAt(&window, 2, 10, "  ", LimeBack);
	DrawTextAt(&window, 5, 10, ": Herbe", WhiteFore);
	DrawTextAt(&window, 2, 11, "  ", GreenBack);
	DrawTextAt(&window, 5, 11, ": Foret", WhiteFore);
	DrawTextAt(&window, 2, 12, "  ", SilverBack);
	DrawTextAt(&window, 5, 12, ": Montagne", WhiteFore);
	DrawTextAt(&window, 2, 13, "  ", WhiteBack);
	DrawTextAt(&window, 5, 13, ": Montagne enneigee", WhiteFore);
	character_delimiter.character = 32;
	DrawRectangle(&window, 1, WINDOW_HEIGHT-6, 24, WINDOW_HEIGHT-3, 1, character_delimiter, 0, character_delimiter);
	character_delimiter.character = 45;
	DrawLine(&window, 1, WINDOW_HEIGHT-7, 24, WINDOW_HEIGHT-7, character_delimiter, 1);
	character_delimiter.character = 124;
	DrawLine(&window, 25, WINDOW_HEIGHT-6, 25, WINDOW_HEIGHT-3, character_delimiter, 1);
	character_delimiter.character = 43;
	SetCharacter(&window, 25, WINDOW_HEIGHT-7, character_delimiter);
	SetCharacter(&window, 0, WINDOW_HEIGHT-7, character_delimiter);
	SetCharacter(&window, 25, WINDOW_HEIGHT-2, character_delimiter);
	DrawTextAt(&window, 2, WINDOW_HEIGHT-6, "Dimensions :", WhiteFore);
	IntToString(WORLD_WIDTH);
	i = 0;
	while (text[i] != 32) {character_delimiter.character = text[i]; SetCharacter(&window, 15+i, WINDOW_HEIGHT-6, character_delimiter); i++;}
	character_delimiter.character = 120;
	SetCharacter(&window, 15+i, WINDOW_HEIGHT-6, character_delimiter);
	IntToString(WORLD_HEIGHT);
	j = 0;
	while (text[j] != 32) {character_delimiter.character = text[j]; SetCharacter(&window, 16+i+j, WINDOW_HEIGHT-6, character_delimiter); j++;}
	DrawTextAt(&window, 2, WINDOW_HEIGHT-5, "Nombre d'octaves :", WhiteFore);
	IntToString(perlinFactory.numOctaves);
	i = 0;
	while (text[i] != 32) {character_delimiter.character = text[i]; SetCharacter(&window, 21+i, WINDOW_HEIGHT-5, character_delimiter); i++;}
	DrawTextAt(&window, 2, WINDOW_HEIGHT-4, "Persistance (/10) :", WhiteFore);
	IntToString(perlinFactory.persistence*10);
	i = 0;
	while (text[i] != 32) {character_delimiter.character = text[i]; SetCharacter(&window, 22+i, WINDOW_HEIGHT-4, character_delimiter); i++;}
	DrawTextAt(&window, 2, WINDOW_HEIGHT-3, "Seed :", WhiteFore);
	IntToString(perlinFactory.persistence*10);
	i = 0;
	while (text[i] != 32) {character_delimiter.character = text[i]; SetCharacter(&window, 9+i, WINDOW_HEIGHT-3, character_delimiter); i++;}
	DisplayWindow(&window);
}

int main()
{
	character_clear.character = 32;
	character_clear.color = 0;
	window = NewWindow();
	perlinFactory = NewPerlinFactory();
	perlinFactory.numX = WORLD_WIDTH;
	perlinFactory.numY = WORLD_HEIGHT;
	perlinFactory.numOctaves = 7;
	perlinFactory.persistence = 5;
	perlinFactory.primeIndex = 0;
	DisplayMenu();
	while (1)
	{
		if (kbhit())
		{
			int key = getch();
			if (screen)
			{
				if (key == 72) {player.y -= zoom;}
				else if (key == 80) {player.y += zoom;}
				else if (key == 75) {player.x -= zoom;}
				else if (key == 77) {player.x += zoom;}
				else if (key == 43 && zoom > 1) {zoom--;}
				else if (key == 45 && zoom < 10) {zoom++;}
				if (player.x < 0) {player.x = 0;}
				else if (player.x >= WORLD_WIDTH) {player.x = WORLD_WIDTH-1;}
				if (player.y < 0) {player.y = 0;}
				else if (player.y >= WORLD_HEIGHT) {player.y = WORLD_HEIGHT-1;}
				if (key == 27) {screen = 0; perlinFactory.persistence = (int)(perlinFactory.persistence*10); DisplayMenu();}
				else {DisplayWorld();}
			}
			else
			{
				if (key == 72) {selection--;}
				else if (key == 80) {selection++;}
				else if (key == 75)
				{
					if (selection == 0) {perlinFactory.numOctaves--;}
					else if (selection == 1) {perlinFactory.persistence--;}
					else if (selection == 2) {perlinFactory.primeIndex--;}
				}
				else if (key == 77)
				{
					if (selection == 0) {perlinFactory.numOctaves++;}
					else if (selection == 1) {perlinFactory.persistence++;}
					else if (selection == 2) {perlinFactory.primeIndex++;}
				}
				if (selection < 0) {selection = 3;}
				else if (selection > 3) {selection = 0;}
				else if (perlinFactory.numOctaves < 1) {perlinFactory.numOctaves = 10;}
				else if (perlinFactory.numOctaves > 10) {perlinFactory.numOctaves = 1;}
				else if (perlinFactory.persistence < 0) {perlinFactory.persistence = 10;}
				else if (perlinFactory.persistence > 10) {perlinFactory.persistence = 0;}
				else if (perlinFactory.primeIndex < 0) {perlinFactory.primeIndex = perlinFactory.maxPrimeIndex;}
				else if (perlinFactory.primeIndex > perlinFactory.maxPrimeIndex) {perlinFactory.primeIndex = 0;}
				if (key == 13 && selection == 3) {screen = 1; perlinFactory.persistence /= 10; InitWorld(); DisplayWorld();}
				else {DisplayMenu();}
			}
		}
	}
}