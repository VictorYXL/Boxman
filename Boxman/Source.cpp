#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <math.h>
#define BORDERWIDTH 7
#define BORDERHEIGHT 7
#define BOXCOUNT 3
// 0 space, 1 person, 2 box, 3 brick
struct GameBoard
{
    GameBoard(int personLocation[2], int boxLocation[][2], int brickLocation[][2], int brickCount, GameBoard *parent, char dir)
    {
        memset(status, 0, sizeof(status));
        status[personLocation[0]][personLocation[1]] = 1;
        for (int i = 0; i < BOXCOUNT; i++)
        {
            status[boxLocation[i][0]][boxLocation[i][1]] = 2;
			this->boxLocation[i][0] = boxLocation[i][0];
			this->boxLocation[i][1] = boxLocation[i][1];
        }
        for (int i = 0; i < brickCount; i++)
        {
            status[brickLocation[i][0]][brickLocation[i][1]] = 3;
        }
		this->personLocation[0] = personLocation[0];
		this->personLocation[1] = personLocation[1];
		this->parent = parent;
		this->dir = dir;
    }
    int status[BORDERWIDTH][BORDERHEIGHT];
	int boxLocation[BOXCOUNT][2];
	int personLocation[2];
	GameBoard *parent;
	char dir;
};

bool Win(GameBoard *board, int targetLocation[][2])
{
	for (int i = 0; i < BOXCOUNT; i++)
	{
		if (board->status[targetLocation[i][0]][targetLocation[i][1]] != 2)
		{
			return false;
		}
	}
	return true;
}
bool InTarget(int x, int y, int targetLocation[][2])
{
	for (int i = 0; i < BOXCOUNT; i++)
	{
		if (x == targetLocation[i][0] && y == targetLocation[i][1])
			return true;
	}
	return false;
}
bool Dead(GameBoard *board, int targetLocation[][2])
{
	int x, y, t;
	for (int i = 0; i < BOXCOUNT; i++)
	{
		x = board->boxLocation[i][0];
		y = board->boxLocation[i][1];
		if (InTarget(x, y, targetLocation))
			continue;
		if ((x == 0 || board->status[x - 1][y] == 3) && (y == 0 || board->status[x][y - 1] == 3))
			return true;
		if ((x == 0 || board->status[x - 1][y] == 3) && (y == BORDERHEIGHT || board->status[x][y + 1] == 3))
			return true;
		if ((x == BORDERWIDTH || board->status[x + 1][y] == 3) && (y == 0 || board->status[x][y - 1] == 3))
			return true;
		if ((x == BORDERWIDTH || board->status[x + 1][y] == 3) && (y == BORDERHEIGHT || board->status[x][y + 1] == 3))
			return true;
	}
	return false;
}
GameBoard* Move(GameBoard *cur, int dx, int dy, int brickLocation[][2], int brickCount, char dir)
{
	int x = cur->personLocation[0], y = cur->personLocation[1];
	int x1 = cur->personLocation[0] + dx, y1 = cur->personLocation[1] + dy;
	int x2 = cur->personLocation[0] + dx * 2, y2 = cur->personLocation[1] + dy * 2;
	int personLocation[2] = { x, y };
	GameBoard *next;
	if (x1 >= 0 && x1 < BORDERWIDTH && y1 >= 0 && y1 < BORDERHEIGHT)
	{
		switch (cur->status[x1][y1])
		{
		case 3:
			return NULL;
		case 2:
			personLocation[0] = x1;
			personLocation[1] = y1;
			if (x2 >= 0 && x2 < BORDERWIDTH && y2 >= 0 && y2 < BORDERHEIGHT)
				if (cur->status[x2][y2] == 0)
				{
					for (int i = 0; i < BOXCOUNT; i++)
					{
						if (cur->boxLocation[i][0] == x1 && cur->boxLocation[i][1] == y1)
						{
							cur->boxLocation[i][0] = x2;
							cur->boxLocation[i][1] = y2;
							next = new GameBoard(personLocation, cur->boxLocation, brickLocation, brickCount, cur, dir);
							cur->boxLocation[i][0] = x1;
							cur->boxLocation[i][1] = y1;
							return next;
						}
					}
				}
			break;
		case 0:
			personLocation[0] = x1;
			personLocation[1] = y1;
			next = new GameBoard(personLocation, cur->boxLocation, brickLocation, brickCount, cur, dir);
			return next;
		default:
			return NULL;
		}
	}
	return NULL;
}

long int BoardToInt(GameBoard *board)
{
	long int target = 0;
	std::vector<int> points;
	for (int i = 0; i < BOXCOUNT; i++)
	{
		points.push_back(board->boxLocation[i][1] * BORDERHEIGHT + board->boxLocation[i][0]);
	}
	std::sort(points.begin(), points.end());
	target = board->personLocation[1] * BORDERHEIGHT + board->personLocation[0];
	for (int i = 0; i < BOXCOUNT; i++)
	{
		target *= BORDERHEIGHT * BORDERWIDTH;
		target += points[i];
	}
	return target;
}

void Go(GameBoard *board, int brickLocation[][2], int brickCount, int targetLocation[][2], unsigned char *visit)
{
	std::queue<GameBoard *> queue;
	GameBoard *cur, *next;
	int dir[4][2] = { { 1, 0 },{ -1, 0 },{ 0, 1 },{ 0, -1 } };
	char dirC[4] = { 'r', 'l', 'u', 'd' };
	
	queue.push(board);
	visit[BoardToInt(board)] = 1;
	while (!queue.empty())
	{
		cur = queue.front();
		queue.pop();
		for (int i = 0; i < 4; i++)
		{
			next = Move(cur, dir[i][0], dir[i][1], brickLocation, brickCount, dirC[i]);
			if (next != NULL)
			{
				long int num = BoardToInt(next);
				if (visit[num] == 0)
				{
					if (Win(next, targetLocation))
					{
						while (next != NULL)
						{
							std::cout << next->dir;
							next = next->parent;
						}
						return;
					}
					if (!Dead(next, targetLocation))
					{
						queue.push(next);
						visit[num] = 1;
					}
				}
			}
		}
	}
}
int main()
{
	int personLocation[2] = { 5, 2 };
	int boxLocation[][2] = { { 2, 2 }, { 3, 2 }, { 4, 3 } };
	int brickLocation[][2] =
	{
		{ 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 },
		{ 1, 5 }, { 2, 5 }, { 3, 5 }, { 4, 5 },
		{ 4, 4 }, { 5, 4 }, { 6, 4 },
		{ 6, 3 }, { 6, 2 }, { 6, 1 },
		{ 6, 0 }, { 5, 0 }, { 4, 0 }, { 3, 0 }, { 2, 0 }, { 1, 0 },
		{ 3, 1 }
	};
	int targetLocation[][2] = { { 1, 1 },{ 1, 2 },{ 1, 4 } };
	unsigned char *visit = (unsigned char *)malloc(pow(BORDERHEIGHT * BORDERWIDTH, BOXCOUNT + 1) * sizeof(unsigned char));
	memset(visit, 0, pow(BORDERHEIGHT * BORDERWIDTH, BOXCOUNT + 1) * sizeof(unsigned char));
	
    GameBoard *game = new GameBoard(personLocation, boxLocation, brickLocation, 23, NULL, 'o');
	Go(game, brickLocation, 23, targetLocation, visit);
    return 0;
}

