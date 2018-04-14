#include <iostream>
#include <vector>
#define BORDERWIDTH 5
#define BORDERHEIGHT 5
// 0 space, 1 person, 2 box, 3 brick
struct GameBoard
{
    GameBoard(int personLocation[2], int boxLocation[][2], int boxCount, int brickLocation[][2], int brickCount)
    {
        memset(status, 0, sizeof(status));
        status[personLocation[1]][personLocation[0]] = 1;
        for (int i = 0; i < boxCount; i++)
        {
            status[boxLocation[i][1]][boxLocation[i][0]] = 2;
			this->boxLocation[i][0] = boxLocation[i][0];
			this->boxLocation[i][1] = boxLocation[i][1];
        }
        for (int i = 0; i < brickCount; i++)
        {
            status[brickLocation[i][1]][brickLocation[i][0]] = 3;
			this->brickLocation[i][0] = brickLocation[i][0];
			this->brickLocation[i][1] = brickLocation[i][1];
        }
		this->boxCount = boxCount;
		this->birckCount = birckCount;
		this->personLocation[0] = personLocation[0];
		this->personLocation[1] = personLocation[1];
    }
    int status[BORDERWIDTH][BORDERHEIGHT];
	int boxLocation[BORDERWIDTH * BORDERHEIGHT][2], brickLocation[BORDERWIDTH * BORDERHEIGHT][2];
	int boxCount, birckCount;
	int personLocation[2];
};
bool operator == (const GameBoard& left, const GameBoard& right)
{
	if (left.personLocation[0] == right.personLocation[0] && left.personLocation[1] == right.personLocation[1])
	{
		for (int i = 0; i < left.boxCount; i++)
		{
			if (right.status[left.boxLocation[i][0]][left.boxLocation[i][1]] != 2)
			{
				return false;
			}
		}
	}
	else
	{
		return false;
	}
}
bool win(GameBoard *borad, int targetLocation[][2])
{
	for (int i = 0; i < borad->boxCount; i++)
	{
		if (borad->status[targetLocation[i][0]][targetLocation[i][1]] != 2)
		{
			return false;
		}
	}
	return true;
}

int main()
{
    int personLocation[2] = { 1, 1 };
	int personLocation2[2] = { 2, 2 };
    int boxLocation[][2] = { { 2, 2 }, { 3, 3 } };
    int brickLocation[][2] = 
    { 
        { 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 },
        { 0, 4 }, { 1, 4 }, { 2, 4 }, { 3, 4 },
        { 4, 4 }, { 4, 3 }, { 4, 2 }, { 4, 1 },
        { 4, 0 }, { 3, 0 }, { 2, 0 }, { 1, 0 }
    };
    int targetLocation[][2] = { { 2, 3 }, { 3, 2} };
    GameBoard game(personLocation, boxLocation, 2, brickLocation, 16);
	GameBoard game2(personLocation2, boxLocation, 2, brickLocation, 16);
	std::vector<GameBoard> p;
	p.push_back(game);
	auto i = std::find(p.begin(), p.end(), game2);
	std::cout << (i == p.end());
    return 0;
}

