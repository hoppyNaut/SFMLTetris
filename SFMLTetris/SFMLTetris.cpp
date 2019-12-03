#include "SFMLTetris.h"
#include"Game.h"
#include<iostream>

SFMLTetris::SFMLTetris()
{
}


SFMLTetris::~SFMLTetris()
{
}

void SFMLTetris::Initial()
{
	//数据初始化
	score = 0;
	gameOver = false;
	isEliminate = false;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (i == height - 1 || j == 0 || j == width - 1)
			{
				box_map[i][j] = 1;
			}
			else
			{
				box_map[i][j] = 0;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			box_Right[i][j] = 0;
		}
	}

	//生成第一个方块
	shape = Utility::GetRand(0, 6);
	SetShape(shape, box_shape, size_w, size_h, cur_color);

	//生成下一个方块
	next_shape = Utility::GetRand(0, 6);
	SetShape(next_shape, next_box_shape, next_size_w, next_size_h, next_color);

	//加载纹理图片
	for (int i = 0; i < SpriteTetrisNum; i++)
	{
		Texture tempTexture;
		Sprite tempSprite;
		tArray[i] = tempTexture;
		spArray[i] = tempSprite;
	}

	for (int i = 0; i < SpriteTetrisNum; i++)
	{
		if (!tArray[i].loadFromFile(textureTetrisPath[i]))
		{
			std::cout << "图片" << textureTetrisPath[i] << "没有找到" << std::endl;
		}
		spArray[i].setTexture(tArray[i]);
		spArray[i].setScale(SCALE, SCALE);
	}

	if (!tEliminate.loadFromFile(textureEliminatePath))
	{
		std::cout << "图片" << textureEliminatePath << "没有找到" << std::endl;
	}
	sf::Vector2i spriteSize(16, 16);
	spEliminate.setTexture(tEliminate);
	spEliminate.setTextureRect(sf::IntRect(0, 0, spriteSize.x, spriteSize.y));
	spEliminate.setScale(SCALE, SCALE);
	animEliminate = new Animation(spriteSize, spEliminate, 8, 0.2);


	if (!sbDrop.loadFromFile("Audios/Drop1.ogg"))
	{
		std::cout << "Drop1.ogg没有找到" << std::endl;
	}
	if (!sbAppear.loadFromFile("Audios/Appear1.ogg"))
	{
		std::cout << "Appear1.ogg没有找到" << std::endl;
	}

	soundDrop.setBuffer(sbDrop);
	soundAppear.setBuffer(sbAppear);
}

void SFMLTetris::SetDir(eDirection eDir)
{
	dir = eDir;
}


void SFMLTetris::Logic()
{
	//方块自由下落
	head_y++;
	//碰到底部处理
	if (IsAggin())
	{
		head_y--;
		for (int i = 0; i < size_h; i++)
			for (int j = 0; j < size_w; j++)
			{
				if (box_shape[i][j] == 1)
				{
					//box_map[head_y + i][head_x + j] = 1;
					box_map[head_y + i][head_x + j] = (int)cur_color;
				}
			}
		//生成下一个方块
		Score_Next();
	}

	//向左移动
	if (dir == LEFT)
	{
		dir = STOP;
		head_x--;
		if (IsAggin())
		{
			head_x++;
		}
	}

	//向右移动
	if (dir == RIGHT)
	{
		dir = STOP;
		head_x++;
		if (IsAggin())
		{
			head_x--;
		}
	}

	//变换形状
	if (dir == UP)
	{
		dir = STOP;
		Rotate();
	}

	if (dir == DOWN)
	{
		dir = STOP;
		while (!IsAggin())
		{
			head_y++;
		}
		head_y--;
		for (int i = 0; i < size_h; i++)
			for (int j = 0; j < size_w; j++)
			{
				if (box_shape[i][j] == 1)
				{
					//box_map[head_y + i][head_x + j] = 1;
					box_map[head_y + i][head_x + j] = (int)cur_color;
				}
			}
		//生成下一个方块
		Score_Next();
	}
}

void SFMLTetris::Draw(sf::RenderWindow & window)
{

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			if (box_map[j][i] > 0)
			{
				if (box_map[j][i] == 1)
				{
					//绘制右侧游戏框
					Sprite curSprite = spArray[box_map[j][i] - 1];
					curSprite.setPosition(i * GRIDSIZE + GRIDSIZE * STAGE_WIDTH + INFO_WIDTH, j * GRIDSIZE);
					window.draw(curSprite);
				}

				Sprite curSprite = spArray[box_map[j][i] - 1];
				curSprite.setPosition(i * GRIDSIZE, j * GRIDSIZE);
				window.draw(curSprite);
			}

		}

	for (int i = 0; i < size_h; i++)
		for (int j = 0; j < size_w; j++)
		{
			if (box_shape[i][j] == 1)
			{
				Sprite curSprite = spArray[cur_color - 1];
				curSprite.setPosition((j + head_x) * GRIDSIZE, (i + head_y) * GRIDSIZE);
				window.draw(curSprite);
			}
		}

	if (isEliminate)
	{
		if (eliminateRowStack.size() > 0)
		{
			rowToEliminate = eliminateRowStack.top();
			eliminateRowStack.pop();

		}
		for (int j = 1; j < width - 1; j++)
		{
			if (!animEliminate->PlayAnim(j * GRIDSIZE, rowToEliminate * GRIDSIZE, window))
			{
				isEliminate = false;
			}
		}
	}

}

void SFMLTetris::ShowNext(int x, int y, sf::RenderWindow & window)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (next_box_shape[i][j] == 1)
			{
				Sprite nextSprite = spArray[next_color - 1];
				nextSprite.setPosition(x + j * GRIDSIZE, y + i * GRIDSIZE);
				window.draw(nextSprite);
			}
		}
}


void SFMLTetris::SetShape(int & cshape, int shape[][4], int & size_w, int & size_h,eColor &color)
{
	//初始化当前形状数组
	int i, j;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			shape[i][j] = 0;
		}
	}

	//设置7中初始形状以及他们的size
	switch (cshape)
	{
		//I形
	case 0:
		size_h = 1;
		size_w = 4;
		shape[0][0] = 1;
		shape[0][1] = 1;
		shape[0][2] = 1;
		shape[0][3] = 1;
		color = RED;
		break;
		//L形
	case 1:
		size_h = 2;
		size_w = 3;
		shape[0][0] = 1;
		shape[1][0] = 1;
		shape[1][1] = 1;
		shape[1][2] = 1;
		color = GREEN;
		break;
		//J形
	case 2:
		size_h = 2;
		size_w = 3;
		shape[0][2] = 1;
		shape[1][0] = 1;
		shape[1][1] = 1;
		shape[1][2] = 1;
		color = BLUE;
		break;
		//Z形
	case 3:
		size_h = 2;
		size_w = 3;
		shape[0][1] = 1;
		shape[0][2] = 1;
		shape[1][0] = 1;
		shape[1][1] = 1;
		color = DARKBLUE;
		break;
		//S形
	case 4:
		size_h = 2;
		size_w = 3;
		shape[0][0] = 1;
		shape[0][1] = 1;
		shape[1][1] = 1;
		shape[1][2] = 1;
		color = ORANGE;
		break;
		//O形
	case 5:
		size_h = 2;
		size_w = 2;
		shape[0][0] = 1;
		shape[0][1] = 1;
		shape[1][0] = 1;
		shape[1][1] = 1;
		color = PURPLE;
		break;
		//T形
	case 6:
		size_h = 2;
		size_w = 3;
		shape[0][1] = 1;
		shape[1][0] = 1;
		shape[1][1] = 1;
		shape[1][2] = 1;
		color = YELLOW;
		break;
	}


	//设置方块的初始位置
	head_x = width / 2;
	head_y = 0;

	//越界
	if (IsAggin())
	{
		gameOver = true;
	}
}

void SFMLTetris::Judge()
{
	int i, j, line = 0;
	bool full;
	for (i = 1; i < STAGE_HEIGHT - 1; i++)
	{
		full = true;
		for (j = 1; j < STAGE_WIDTH - 1; j++)
		{
			if (box_map[i][j] == 0)
			{
				full = false;
			}
		}
		if (full)
		{
			line++;
			//TODO
			isEliminate = true;
			eliminateRowStack.push(i);
			animEliminate->Reset();
			score += 50;
			for (j = 1; j < STAGE_WIDTH - 1; j++)
			{
				box_map[i][j] = 0;
			}
		}
	}

	if (line > 0)
	{
		soundAppear.play();
		//从底部向上搜索空行
		for (i = STAGE_HEIGHT - 2; i >= 2; i--)
		{
			int s = i;
			//如果当前行为空行
			if (Exsqr(i))
			{
				//往上搜索下一个非空行
				while (s > 1 && Exsqr(--s));
				for (j = 1; j < STAGE_WIDTH - 1; j++)
				{
					box_map[i][j] = box_map[s][j];
					box_map[s][j] = 0;
				}
			}
		}
	}
	else
	{
		soundDrop.play();
	}
	
}

void SFMLTetris::Rotate()
{
	int temp[4][4] = { 0 };		//临时数组
	int temp_piece[4][4] = { 0 };
	int tmp_size_w, tmp_size_h;

	tmp_size_w = size_w;
	tmp_size_h = size_h;

	//备份旋转前数组，便于还原
	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			temp_piece[i][j] = box_shape[i][j];
		}

	//关于斜对角线对称
	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			temp[j][i] = box_shape[i][j];
		}
	//调整size
	int t = size_h;
	size_h = size_w;
	size_w = t;
	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			box_shape[i][j] = 0;
		}
	//左右对称
	for(int i = 0; i < size_h; i++)
		for (int j = 0; j < size_w; j++)
		{
			box_shape[i][size_w - 1 - j] = temp[i][j];
		}

	//如果旋转后不符合条件，恢复原图形
	if (IsAggin())
	{
		for(int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
			{
				box_shape[i][j] = temp_piece[i][j];
			}
		size_h = tmp_size_h;
		size_w = tmp_size_w;
	}
}

bool SFMLTetris::IsAggin()
{
	for(int i = 0; i < size_h; i++)
		for (int j = 0; j < size_w; j++)
		{
			if (box_shape[i][j] == 1)
			{
				if (head_y + i > height - 2)
					return true;
				if (head_x + j > width - 2 || head_x + j - 1 < 0)
					return true;
				if (box_map[head_y + i][head_x + j] != 0)
					return true;
			}
		}
	return false;
}

bool SFMLTetris::Exsqr(int row)
{
	for (int i = 1; i < STAGE_WIDTH - 1; i++)
	{
		if (box_map[row][i] != 0)
		{
			return false;
		}
	}
	return true;
}

void SFMLTetris::Score_Next()
{
	score += 10;
	Judge();

	//将下一个方块的信息赋给当前方块
	SetShape(next_shape, box_shape, size_w, size_h,cur_color);
	//cur_color = next_color;

	//重新生成下一个方块
	next_shape = Utility::GetRand(0, 6); 
	SetShape(next_shape, next_box_shape, next_size_w, next_size_h,next_color);
}

void SFMLTetris::EliminateRandRow()
{
	//不为空的行数
	int rowCount = 0;
	for (int i = STAGE_HEIGHT - 2; i >= 2; i--)
	{
		//如果当前行为非空行
		if (!Exsqr(i))
		{
			rowCount++;
		}
		//如果是空行就跳出循环
		else
		{
			break;
		}
	}
	if (rowCount <= 0)
	{
		return;
	}
	//生成随机数
	int randRow = Utility::GetRand(0, rowCount - 1);
	for (int j = 1; j < STAGE_WIDTH - 1; j++)
	{
		box_map[STAGE_HEIGHT - 2 - randRow][j] = 0;
	}
	//TODO
	isEliminate = true;
	eliminateRowStack.push(STAGE_HEIGHT - 2 - randRow);
	animEliminate->Reset();
	score += 50;
	for (int i = STAGE_HEIGHT - 2; i >= 2; i--)
	{
		int s = i;
		//如果当前行为空行
		if (Exsqr(i))
		{
			//往上搜索下一个非空行
			while (s > 1 && Exsqr(--s));
			for (int j = 1; j < STAGE_WIDTH - 1; j++)
			{
				box_map[i][j] = box_map[s][j];
				box_map[s][j] = 0;
			}
		}
	}
}

void SFMLTetris::EliminateLongestRow()
{

	int index = STAGE_WIDTH - 2;
	int maxCount = 0;
	int count = 0;
	for(int i = STAGE_HEIGHT - 2; i >= 2; i--)
	{
		if (!Exsqr(i))
		{
			for (int j = 1; j < STAGE_WIDTH - 1; j++)
			{
				if (box_map[i][j] != 0)
				{
					count++;
				}
			}
			if (count > maxCount)
			{
				maxCount = count;
				index = i;
			}
			count = 0;
		}
		else {
			break;
		}
	}
	for (int j = 1; j < STAGE_WIDTH - 1; j++)
	{
		box_map[index][j] = 0;
	}
	//TODO
	isEliminate = true;
	eliminateRowStack.push(index);
	animEliminate->Reset();
	score += 50;
	for (int i = STAGE_HEIGHT - 2; i >= 2; i--)
	{
		int s = i;
		//如果当前行为空行
		if (Exsqr(i))
		{
			//往上搜索下一个非空行
			while (s > 1 && Exsqr(--s));
			for (int j = 1; j < STAGE_WIDTH - 1; j++)
			{
				box_map[i][j] = box_map[s][j];
				box_map[s][j] = 0;
			}
		}
	}
}
