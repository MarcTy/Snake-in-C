#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <math.h>
#include <unistd.h>

//Compilation command with gcc: gcc snake.c -o snake -lncurses
//add '-Wall -g' for error handling. 

//Window dimensions.
#define DIMX 12
#define DIMY 12

//Higher value will lead to slower frames.
#define TIME_BETWEEN_FRAMES 80

int game_over = 0;
int score = 0;

int posx, posy, fposx, fposy, dir;
char *area[DIMX * DIMY];

//Function declarations.
void keyput(void);
void draw_elements(void);
void determine_start(void);
void move_snake(void);
void calc_fruit_pos(void);
void free_tail(void);
void add_tail(void);
void update_tail(void);
void end_logic(void);
int is_a_tail(int pX, int pY);
//void iter_tail(void);


typedef struct Node
{
	int d;
	int px;
	int py;
	struct Node *f;
} node;

node *head = NULL;
node **head_ptr = &head;

int lnlinkedlist(node **Head);

int main(void)
{
	dir = 0;
	initscr();
	timeout(TIME_BETWEEN_FRAMES);
	determine_start();
	fposx = posx;
	fposy = posy;
	calc_fruit_pos();
	while (game_over == 0)
	{
		if (score == ((ceil(DIMX/2)) * (ceil(DIMY/2))) - 1)
		{
			game_over = 1;
		}
		
		if ((posx == fposx) && (posy == fposy))
		{
			calc_fruit_pos();
			add_tail();
		}
		refresh();
		draw_elements();
		printw("\nPress q to exit\nScore: %d\n", score);
		move_snake();
		keyput();
		end_logic();
	}
	free_tail();
	endwin();
	printf("\n\nGAME OVER!\n\n");
	return 1;
}

void end_logic()
{
	if (is_a_tail(posx, posy) == 1)
	{
		game_over = 1;
	}
	else if ((posx >= DIMX || posy >= DIMY-1) || (posx <= 0 || posy <= 0))
	{
		game_over = 1;
	}
	return;
}

void update_tail()
{
	//The previous position equals the subsequent position.
	node *lead_node = *head_ptr;
	if (lead_node == NULL)
	{
		return;
	}
	int tx, ty, td;
	int c = 0;
	int ax = lead_node->px;
	int ay = lead_node->py;
	int tdir = lead_node->d;
	while (lead_node != NULL)
	{
		if (c == 0)
		{
			lead_node->px = posx;
			lead_node->py = posy;
			lead_node->d = dir;
			c = c+1;
		}
		else
		{
			tx = ax;
			ty = ay;
			td = tdir;
			ax = lead_node->px;
			ay = lead_node->py;
			tdir = lead_node->d;
			lead_node->px = tx;
			lead_node->py = ty;
			lead_node->d = td;
		}
		lead_node = lead_node->f;
	}
}

//Frees linked list.
void free_tail()
{
	node *lead_node = *head_ptr;
	while (lead_node != NULL)
	{
		node *current_node = lead_node;
		lead_node = lead_node->f;
		free(current_node);
	}
}

	//void iter_tail()
	//{
	//	node *lead_node = *head_ptr;
	//	while (lead_node != NULL)
	//	{
	//		int x = lead_node->px;
	//		int y = lead_node->py;
	//		lead_node = lead_node->f;
	//	}
	//}

void add_tail()
{
	score = score + 1;
	node *new = (node *)malloc(sizeof(node));
	node *search_node = *head_ptr;
	if (new != NULL)
	{  	//If tail has zero length.
		if (search_node == NULL)
		{
			if (dir == 0)
			{
				new->py = posy + 1;
				new->px = posx;
			}
			else if (dir == 1)
			{
				new->py = posy;
				new->px = posx - 1;
			}
			else if (dir == 2)
			{
				new->py = posy - 1;
				new->px = posx;
			}
			else
			{
				new->py = posy;
				new->px = posx + 1;
			}
			new->d = dir;
			*head_ptr = new;
		}
		else
		{
			//If tail length is more than one.
			while ((search_node->f) != NULL)
			{
				search_node = search_node->f;
			}

			if (search_node->d == 0)
			{
				new->py = (search_node->py) + 1;
				new->px = search_node->px;
			}
			else if (search_node->d == 1)
			{
				new->py = search_node->py;
				new->px = (search_node->px) - 1;
			}
			else if (search_node->d == 2)
			{
				new->py = (search_node->py) - 1;
				new->px = search_node->px;
			}
			else
			{
				new->py = search_node->py;
				new->px = (search_node->px) + 1;
			}
			new->f = NULL;
			new->d = search_node->d;

			search_node->f = new;

		}
	}
	else
	{
		return;
	}
}

void determine_start(void)
{
	posx = DIMX / 2;
	posy = DIMY / 2;
}

void calc_fruit_pos(void)
{	
	while (1)
	{
		srand(time(NULL));
		int fx = rand() % DIMX;
		int fy = rand() % DIMY;
		if (fx > abs(fx - DIMX))
		{
			fposx = fx - 1;
		}
		else
		{
			fposx = fx + 1;
		}
		if (fy > abs(fy - DIMY))
		{
			fposy = fy - 1;
		}
		else
		{
			fposy = fy + 1;
		}
		//Check if random pos. is tail or random pos. is position of snake.
		if (((is_a_tail(fx, fy)) == 0) && ((posx != fposx) && (posy != fposy)))
		{
			return;
		}
	}
}

void draw_elements(void)
{
	clear();
	for (int i = 0; i < DIMX; i++)
	{
		for (int j = 0; j < DIMY; j++)
		{
			int is_tail = is_a_tail(i, j);
			if (i == 0 || j == 0 || i == DIMX - 1 || j == DIMY - 1)
			{
				printw("-");
			}
			else if (((i == posx) && (j == posy)))
			{
				printw("0");
			}
			else if (is_tail == 1)
			{
				printw("O");
			}
			else if ((i == fposx) && (j == fposy))
			{
				printw("*");
			}
			else
			{
				printw(".");
			}
		}
		printw("\n");
	}
}

// time complexity warning
int is_a_tail(int pX, int pY)
{
	node *lead_node = *head_ptr;
	if (lead_node == NULL)
	{
		return 0;
	}
	while ((lead_node) != NULL)
	{
		if ((lead_node->px == pX) && (lead_node->py == pY))
		{
			return 1;
		}
		lead_node = lead_node->f;
	}
	return 0;
}

	//int lnlinkedlist(node **Head)
	//{
	//	node *lead_node = *Head;
	//	int counter = 0;
	//	while (lead_node)
	//	{
	//		counter = counter + 1;
	//		lead_node = lead_node->f;
	//	}
	//	return counter;
	//}

void move_snake(void)
{
	update_tail();
	switch (dir)
	{
	case (0):
		posy = posy - 1;
		break;
	case (1):
		posx = posx + 1;
		break;
	case (2):
		posy = posy + 1;
		break;
	case (3):
		posx = posx - 1;
		break;
	}
}

void keyput(void)
{
	int c = getch();
	if (c != ERR)
	{
		switch (c)
		{
		case ('a'):
			if (dir != 2)
			{
				dir = 0;
			}
			break;
		case ('s'):
			if (dir != 3)
			{
				dir = 1;
			}
			break;
		case ('d'):
			if (dir != 0)
			{
				dir = 2;
			}
			break;
		case ('w'):
			if (dir != 1)
			{
				dir = 3;
			}
			break;
		case ('q'):
			game_over = 1;
			break;
		}
	}
}

