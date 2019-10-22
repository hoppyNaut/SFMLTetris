#include "SFMLTetris.h"
#include<iostream>

sf::RenderWindow window(sf::VideoMode(STAGE_WIDTH * GRIDSIZE + INFO_WIDTH, STAGE_HEIGHT * GRIDSIZE), L"SFML 俄罗斯方块");

SFMLTetris::SFMLTetris()
{
}


SFMLTetris::~SFMLTetris()
{
}

int SFMLTetris::GetRand(int min, int max)
{
	return (min + rand() % (max - min + 1));
}

void SFMLTetris::Initial()
{
	
	//数据初始化
	score = 0;
	scoreCount = 1;
	frameRate = 30;
	delay = 0;
	timeCounter = 0;
	gameOver = false;
	gameQuit = false;
	soundVolume = 50;
	musicOn = true;

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

	window.setFramerateLimit(frameRate);	//设置每秒的帧数

	
	//加载字体
	if (!font.loadFromFile("Fonts/fangmeng.ttf"))
	{
		std::cout << "字体fangmeng.ttf没有找到" << std::endl;
	}
	text.setFont(font);

	//加载纹理图片
	for (int i = 0; i < SpriteNum; i++)
	{
		Texture tempTexture;
		Sprite tempSprite;
		tArray[i] = tempTexture;
		spArray[i] = tempSprite;
	}

	for (int i = 0; i < SpriteNum; i++)
	{
		if (!tArray[i].loadFromFile(texturePath[i]))
		{
			std::cout << "图片" <<  texturePath[i] <<"没有找到" << std::endl;
		}
		spArray[i].setTexture(tArray[i]);
		spArray[i].setScale(SCALE, SCALE);
	}

	//加载音乐
	if (!sbDrop.loadFromFile("Audios/Drop1.ogg"))
	{
		std::cout << "Drop1.ogg没有找到" << std::endl;
	}
	if (!sbAppear.loadFromFile("Audios/Appear1.ogg"))
	{
		std::cout << "Appear1.ogg没有找到" << std::endl;
	}
	if (!bgMusic.openFromFile("Audios/bgMusic.ogg"))
	{
		std::cout << "bgMusic.ogg没有找到" << std::endl;
	}
	if (!failMusic.openFromFile("Audios/failMusic.ogg"))
	{
		std::cout << "failMusic.ogg没有找到" << std::endl;
	}

	soundDrop.setBuffer(sbDrop);
	soundAppear.setBuffer(sbAppear);

	bgMusic.setVolume(soundVolume);
	bgMusic.play();
	bgMusic.setLoop(true);

	srand((unsigned int)time(0));		//生成随机数种子

	//生成第一个方块
	shape = GetRand(0, 6);
	SetShape(shape, box_shape, size_w, size_h,cur_color);

	//生成下一个方块
	next_shape = GetRand(0, 6);
	SetShape(next_shape, next_box_shape, next_size_w, next_size_h,next_color);
}

void SFMLTetris::Draw()
{
	window.clear(Color::Color(255, 0, 255, 255));		//清屏

	Prompt_info(width * GRIDSIZE, GRIDSIZE);

	for(int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
		{
			//if (box_map[j][i] == 1)
			//{
			//	spWhite.setPosition(i * GRIDSIZE, j * GRIDSIZE);
			//	window.draw(spWhite);
			//}
			//else if (box_map[j][i] == 2)
			//{
			//	spRed.setPosition(i * GRIDSIZE, j * GRIDSIZE);
			//	window.draw(spRed);
			//}
			//else if (box_map[j][i] == 3)
			//{
			//	spGreen.setPosition(i * GRIDSIZE, j * GRIDSIZE);
			//	window.draw(spGreen);
			//}
			if (box_map[j][i] > 0)
			{
				Sprite curSprite = spArray[box_map[j][i] - 1];
				curSprite.setPosition(i * GRIDSIZE, j * GRIDSIZE);
				window.draw(curSprite);
			}

		}

	for(int i = 0;i < size_h; i++)
		for (int j = 0; j < size_w; j++)
		{
			if (box_shape[i][j] == 1)
			{
				/*switch (cur_color)
				{
				case WHITE:
					spWhite.setPosition((j + head_x) * GRIDSIZE, (i + head_y) * GRIDSIZE);
					window.draw(spWhite);
					break;
				case RED:
					spRed.setPosition((j + head_x) * GRIDSIZE, (i + head_y) * GRIDSIZE);
					window.draw(spRed);
					break;
				case GREEN:
					spGreen.setPosition((j + head_x) * GRIDSIZE, (i + head_y) * GRIDSIZE);
					window.draw(spGreen);
					break;
				default:
					break;
				}*/
				Sprite curSprite = spArray[cur_color - 1];
				curSprite.setPosition((j + head_x) * GRIDSIZE, (i + head_y) * GRIDSIZE);
				window.draw(curSprite);
			}
		}

	if (gameOver)
	{
		ShowGameOverInfo(width / 10 * GRIDSIZE, height / 3 * GRIDSIZE);
		timeCounter = 0;
	}

	if (timeCounter > 0)
	{
		ShowSpeedUpInfo(width / 4 * GRIDSIZE, height / 3 * GRIDSIZE);
		timeCounter -= 1;
	}

	window.display();
}

void SFMLTetris::Input()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		//上
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up)
		{
			dir = UP;
		}
		//下
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
		{
			dir = DOWN;
		}
		//左
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
		{
			dir = LEFT;
		}
		//右
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
		{
			dir = RIGHT;
		}
		//退出
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
		{
			gameOver = true;
		}

		ControlMusic(bgMusic, event);
		////提高\降低音量
		//if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Add)
		//{
		//	soundVolume += 5;
		//	bgMusic.setVolume(soundVolume);
		//}
		//if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Subtract)
		//{
		//	soundVolume -= 5;
		//	bgMusic.setVolume(soundVolume);
		//}
		////开启\关闭音乐
		//if (event.type == sf::Event::EventType::KeyReleased && (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Multiply))
		//{
		//	if (musicOn)
		//	{
		//		bgMusic.stop();
		//		musicOn = false;
		//	}
		//	else
		//	{
		//		bgMusic.play();
		//		musicOn = true;
		//	}
		//}
	}

	//if (Keyboard::isKeyPressed(Keyboard::Left))
	//{
	//	dir = LEFT;
	//}

	//if (Keyboard::isKeyPressed(Keyboard::Right))
	//{
	//	dir = RIGHT;
	//}

	//if (Keyboard::isKeyPressed(Keyboard::Up))
	//{
	//	dir = UP;
	//}

	//if (Keyboard::isKeyPressed(Keyboard::Down))
	//{
	//	dir = DOWN;
	//}

	//if (Keyboard::isKeyPressed(Keyboard::X))
	//{
	//	gameOver = true;
	//	gameQuit = true;
	//}
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

void SFMLTetris::Prompt_info(int x, int y)
{
	int initialX = 20, initialY = 0;
	int CharacterSize = 30;
	//设置字体大小、颜色、风格
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"■	操作说明：");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□向左移动：←");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□向右移动：→");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□变换形状：↑");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□快速下落：↓");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□提高/降低音量：+/-");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□开启/关闭音乐：Enter/*");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□退出游戏：X");
	window.draw(text);
	initialY += CharacterSize;
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"■	下一个方块：");
	window.draw(text);
	initialY += CharacterSize;

	initialY += CharacterSize;
	initialY += CharacterSize;
	ShowNext(x + initialX + 100, y + initialY);

	initialY += CharacterSize;
	initialY += CharacterSize;
	initialY += CharacterSize;
	text.setPosition(x + initialX, y + initialY);
	text.setString(L"■	分数：" + std::to_string(score) );
	window.draw(text);
	initialY += CharacterSize;

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

	//将方块映射到数组中
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		box_map[head_x + j][head_y + i] = shape[j][i];
	//	}
	//}

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

	if (score >= scoreCount * 100 )
	{
		scoreCount++;
		if (frameRate < 50)
		{
			frameRate += 5;
			//1s后消除加速提示
			timeCounter = frameRate;
			
			window.setFramerateLimit(frameRate);
		}
	}

	//将下一个方块的信息赋给当前方块
	SetShape(next_shape, box_shape, size_w, size_h,cur_color);
	//cur_color = next_color;

	//重新生成下一个方块
	//next_color = (eColor)GetRand(1, 3);
	next_shape = GetRand(0, 6); 
	SetShape(next_shape, next_box_shape, next_size_w, next_size_h,next_color);
}

void SFMLTetris::ShowNext(int x,int y)
{
	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			if (next_box_shape[i][j] == 1)
			{
				/*switch (next_color)
				{
				case WHITE:
					spWhite.setPosition(x + j * GRIDSIZE, y + i * GRIDSIZE);
					window.draw(spWhite);
					break;
				case RED:
					spRed.setPosition(x + j * GRIDSIZE, y + i * GRIDSIZE);
					window.draw(spRed);
					break;
				case GREEN:
					spGreen.setPosition(x + j * GRIDSIZE, y + i * GRIDSIZE);
					window.draw(spGreen);
					break;
				default:
					break;
				}*/
				Sprite nextSprite = spArray[next_color - 1];
				nextSprite.setPosition(x + j * GRIDSIZE, y + i * GRIDSIZE);
				window.draw(nextSprite);
			}
		}
}

void SFMLTetris::ShowGameOverInfo(int x, int y)
{
	int initialX = 20, initialY = 0;
	int characterSize = 48;
	text.setCharacterSize(characterSize);
	text.setFillColor(Color(255, 0, 0, 255));
	text.setStyle(Text::Bold);


	text.setPosition(x + initialX, y + initialY);
	text.setString(L"	游戏结束！！");
	window.draw(text);
	initialY += characterSize;
	text.setPosition(x + initialX, y + initialY);
	text.setString(L"	Y重新开始/N退出");
	window.draw(text);
}

void SFMLTetris::ShowSpeedUpInfo(int x, int y)
{
	int characterSize = 30;
	text.setCharacterSize(characterSize);
	text.setFillColor(Color(255, 0, 0, 255));
	text.setStyle(Text::Bold);


	text.setPosition(x, y);
	text.setString(L"	Speed Up！！");
	window.draw(text);

}

void SFMLTetris::ControlMusic(Music &music,sf::Event event)
{
	//提高\降低音量
	if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Add)
	{
		soundVolume += 10;
		music.setVolume(soundVolume);
	}
	if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Subtract)
	{
		soundVolume -= 10;
		music.setVolume(soundVolume);
	}
	//开启\关闭音乐
	if (event.type == sf::Event::EventType::KeyReleased && (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Multiply))
	{
		if (musicOn)
		{
			music.stop();
			musicOn = false;
		}
		else
		{
			music.play();
			musicOn = true;
		}
	}
}

void SFMLTetris::GameLoop()
{
	do {
		Initial();
		while (window.isOpen() && !gameOver)
		{
			Input();
			delay++;
			if (delay % 5 == 0)
			{
				delay = 0;
				Logic();
			}
			
			Draw();
		}

		bgMusic.stop();
		failMusic.setVolume(soundVolume);
		failMusic.play();
		failMusic.setLoop(true);
		
		while (gameOver)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
					gameOver = false;
					gameQuit = true;
				}
				if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Y)
				{
					gameOver = false;
				}
				if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::N)
				{
					gameOver = false;
					gameQuit = true;
				}

				ControlMusic(failMusic, event);

				////提高\降低音量
				//if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Add)
				//{
				//	soundVolume += 5;
				//	failMusic.setVolume(soundVolume);
				//}
				//if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Subtract)
				//{
				//	soundVolume -= 5;
				//	failMusic.setVolume(soundVolume);
				//}
				////开启\关闭音乐
				//if (event.type == sf::Event::EventType::KeyReleased && (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Multiply))
				//{
				//	if (musicOn)
				//	{
				//		failMusic.stop();
				//		musicOn = false;
				//	}
				//	else
				//	{
				//		failMusic.play();
				//		musicOn = true;
				//	}
				//}
			}
		}
	} while (!gameQuit);
}
