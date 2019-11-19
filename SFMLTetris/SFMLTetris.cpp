#include "SFMLTetris.h"
#include<iostream>

sf::RenderWindow window(sf::VideoMode(STAGE_WIDTH * GRIDSIZE * 2 + INFO_WIDTH, STAGE_HEIGHT * GRIDSIZE), L"SFML 俄罗斯方块");

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
	srand((unsigned int)time(0));		//生成随机数种子
	LoadXmlFile();

	questionIndex = 0;
	curQuestion = questionArray[questionIndex];
	curAnswer = curQuestion.answer;
	awardCount = 0;

	//printf("%s", questionArray[i].quesTxt);
	cout << curQuestion.quesTxt << endl;
	cout << curQuestion.optionA << endl;
	cout << curQuestion.optionB << endl;
	cout << curQuestion.optionC << endl;
	cout << curQuestion.optionD << endl;
	cout << curQuestion.answer << endl;


	//数据初始化
	score = 0;
	scoreCount = 1;
	frameRate = 25;
	delay = 0;
	timeCounter = 0;
	gameOver = false;
	gameQuit = false;
	soundVolume = 50;
	musicOn = true;
	//随机获得一张背景
	bgIndex = GetRand(0, SpriteBgNum - 1);

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
	shape = GetRand(0, 6);
	SetShape(shape, box_shape, size_w, size_h,cur_color);

	//生成下一个方块
	next_shape = GetRand(0, 6);
	SetShape(next_shape, next_box_shape, next_size_w, next_size_h,next_color);

	//加载素材
	LoadMediaData();

	//设置背景音乐
	bgMusic.setVolume(soundVolume);
	bgMusic.play();
	bgMusic.setLoop(true);

	//设置每秒的帧数
	window.setFramerateLimit(frameRate);	
}

void SFMLTetris::LoadMediaData()
{

	//加载字体
	if (!font.loadFromFile("Fonts/fangmeng.ttf"))
	{
		std::cout << "字体fangmeng.ttf没有找到" << std::endl;
	}
	text.setFont(font);

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

	//加载背景纹理图片
	if (!tBackground.loadFromFile(textureBgPath[bgIndex]))
	{
		std::cout << "图片" + textureBgPath[bgIndex] + "没有找到" << std::endl;
	}
	spBackground.setTexture(tBackground);
	spBackground.setScale(BGSCALEX, BGSCALEY);

	//加载奖励纹理图片
	if (!tAward.loadFromFile(textureAwardPath))
	{
		std::cout << "图片" + textureAwardPath + "没有找到" << endl;
	}
	spAward.setTexture(tAward);
	spAward.setScale(0.4, 0.4);

	if (!tAwardGray.loadFromFile(textureAwardGrayPath))
	{
		std::cout << "图片" + textureAwardGrayPath + "没有找到" << endl;
	}
	spAwardGray.setTexture(tAwardGray);
	spAwardGray.setScale(0.4, 0.4);

	//加载音乐
	if (!sbDrop.loadFromFile("Audios/Drop1.ogg"))
	{
		std::cout << "Drop1.ogg没有找到" << std::endl;
	}
	if (!sbAppear.loadFromFile("Audios/Appear1.ogg"))
	{
		std::cout << "Appear1.ogg没有找到" << std::endl;
	}
	if (!sbClickCorrect.loadFromFile("Audios/correct.ogg"))
	{
		std::cout << "correct.ogg没有找到" << std::endl;
	}
	if (!sbClickFalse.loadFromFile("Audios/false.ogg"))
	{
		std::cout << "false.ogg没有找到" << std::endl;
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
	soundClickCorrect.setBuffer(sbClickCorrect);
	soundClickFalse.setBuffer(sbClickFalse);

	btn_optionA = new button(1080.0f, 550.0f, 100.0f, 60.0f, font, "A", 30, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(0, 0, 0, 255), Color(0, 0, 0, 255), 1);
	btn_optionB = new button(1310.0f, 550.0f, 100.0f, 60.0f, font, "B", 30, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(0, 0, 0, 255), Color(0, 0, 0, 255), 2);
	btn_optionC = new button(1080.0f, 635.0f, 100.0f, 60.0f, font, "C", 30, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(0, 0, 0, 255), Color(0, 0, 0, 255), 3);
	btn_optionD = new button(1310.0f, 635.0f, 100.0f, 60.0f, font, "D", 30, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(0, 0, 0, 255), Color(0, 0, 0, 255), 4);
}

void SFMLTetris::Draw()
{
	window.clear(Color::Color(255, 0, 255, 255));		//清屏

	spBackground.setPosition(0, 0);
	window.draw(spBackground);

	DrawAward();
	Prompt_info(width * GRIDSIZE, GRIDSIZE);

	if (curWQuestion.answer != -1)
	{
		DrawBtn();
		ShowQuestion(1050, 100);
	}
	else
	{
		ShowOutOfQuestion(1080, 200);
	}
	

	for(int i = 0; i < width; i++)
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

	for(int i = 0;i < size_h; i++)
		for (int j = 0; j < size_w; j++)
		{
			if (box_shape[i][j] == 1)
			{
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

		ButtonInput(event);

		//上
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::W)
		{
			dir = UP;
		}
		//下
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
		{
			dir = DOWN;
		}
		//左
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
		{
			dir = LEFT;
		}
		//右
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
		{
			dir = RIGHT;
		}
		//退出
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
		{
			gameOver = true;
		}
		ControlMusic(bgMusic, event);
	}
}

void SFMLTetris::ButtonInput(sf::Event event)
{
	//监听按钮按下
	Vector2i mousePosition(event.mouseButton.x, event.mouseButton.y);
	btn_optionA->update(mousePosition,event);
	btn_optionB->update(mousePosition,event);
	btn_optionC->update(mousePosition, event);
	btn_optionD->update(mousePosition, event);

	JudgeBtnClick(btn_optionA);
	JudgeBtnClick(btn_optionB);
	JudgeBtnClick(btn_optionC);
	JudgeBtnClick(btn_optionD);
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

	if (score >= scoreCount * 500 )
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
				Sprite nextSprite = spArray[next_color - 1];
				nextSprite.setPosition(x + j * GRIDSIZE, y + i * GRIDSIZE);
				window.draw(nextSprite);
			}
		}
}

void SFMLTetris::EliminateRandRow()
{
	soundAppear.play();
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
	int randRow = GetRand(0, rowCount - 1);
	for (int j = 1; j < STAGE_WIDTH - 1; j++)
	{
		box_map[STAGE_HEIGHT - 2 - randRow][j] = 0;
	}
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
	soundAppear.play();
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

void SFMLTetris::DrawBtn()
{
	btn_optionA->render(window);
	btn_optionB->render(window);
	btn_optionC->render(window);
	btn_optionD->render(window);
}

void SFMLTetris::DrawAward()
{
	for (int i = 0; i < 3; i++)
	{
		if (i < awardCount)
		{
			spAward.setPosition(STAGE_WIDTH * GRIDSIZE + 50 + 150 * i, STAGE_HEIGHT * GRIDSIZE - 200);
			window.draw(spAward);
		}
		else
		{
			spAwardGray.setPosition(STAGE_WIDTH * GRIDSIZE + 50 + 150 * i, STAGE_HEIGHT * GRIDSIZE - 200);
			window.draw(spAwardGray);
		}
	}
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

bool SFMLTetris::LoadXmlFile()
{
	TiXmlDocument doc;
	if (!doc.LoadFile("questions.xml"))
	{
		cout << doc.ErrorDesc() << endl;
		return false;
	}

	TiXmlElement * root = doc.FirstChildElement();
	if (root == NULL)
	{
		cout << "Failed to load file：No root element" << endl;
		//清理内存
		doc.Clear();
		return false;
	}

	//遍历子节点
	for (TiXmlElement * elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		Question question;
		//TODO
		WQuestion wquestion;
		//获取元素名
		string elemName = elem->Value();
		string attr;
		//获取元素属性值
		attr = elem->Attribute("ID");
		int index = atoi(attr.c_str());
		//再次遍历子属性结点
		for (TiXmlElement *node = elem->FirstChildElement(); node != NULL; node = node->NextSiblingElement())
		{
			string nodeName = node->Value();
			TiXmlNode* node2 = node->FirstChild();
			//cout << node->GetText() << endl;
			if (nodeName.compare("question") == 0)
			{
				question.quesTxt = Utf8ToUnicode(node2->ToText()->Value());
				//TODO
				wquestion.quesTxt = Utf8ToUnicodeW(node2->ToText()->Value());
			}
			else if (nodeName.compare("optionA") == 0)
			{
				question.optionA = Utf8ToUnicode( node2->ToText()->Value());
				//TODO
				wquestion.optionA = Utf8ToUnicodeW(node2->ToText()->Value());
			}
			else if (nodeName.compare("optionB") == 0)
			{
				question.optionB = Utf8ToUnicode(node2->ToText()->Value());
				//TODO
				wquestion.optionB = Utf8ToUnicodeW(node2->ToText()->Value());
			}
			else if (nodeName.compare("optionC") == 0)
			{
				question.optionC = Utf8ToUnicode(node2->ToText()->Value());
				//TODO
				wquestion.optionC = Utf8ToUnicodeW(node2->ToText()->Value());
			}
			else if (nodeName.compare("optionD") == 0)
			{
				question.optionD = Utf8ToUnicode(node2->ToText()->Value());
				//TODO
				wquestion.optionD = Utf8ToUnicodeW(node2->ToText()->Value());
			}
			else if (nodeName.compare("answer") == 0)
			{
				question.answer = atoi(node2->ToText()->Value());
				//TODO
				wquestion.answer = atoi(node2->ToText()->Value());
			};
		}
		questionArray[index] = question;
		wquestionArray[index] = wquestion;
	}

	return true;
}

string SFMLTetris::Utf8ToUnicode(const char * szU8)
{
	//预转换，得到所需空间的大小;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//转换
	MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//最后加上'\0'
	wszString[wcsLen] = '\0';

	string unicodeString = Wchar_tToString(wszString);

	delete[] wszString;
	wszString = NULL;

	return unicodeString;
}

string SFMLTetris::Wchar_tToString(wchar_t * wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
	char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
	string szDst = psText;// std::string赋值
	delete[]psText;// psText的清除
	return szDst;
}

wstring SFMLTetris::Utf8ToUnicodeW(const char * szU8)
{
	//预转换，得到所需空间的大小;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//转换
	MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//最后加上'\0'
	wszString[wcsLen] = '\0';

	wstring unicodeString(wszString);

	delete[] wszString;
	wszString = NULL;

	return unicodeString;
}

void SFMLTetris::NextQuestion()
{
	questionIndex++;
	curQuestion = questionArray[questionIndex];
	curWQuestion = wquestionArray[questionIndex];
	curAnswer = curQuestion.answer;

	cout << curQuestion.quesTxt << endl;
	cout << curQuestion.optionA << endl;
	cout << curQuestion.optionB << endl;
	cout << curQuestion.optionC << endl;
	cout << curQuestion.optionD << endl;
	cout << curQuestion.answer << endl;
	//TODO 更换显示
}

void SFMLTetris::ShowQuestion(int x, int y)
{
	int initialX = 20, initialY = 0;
	int CharacterSize = 30;
	//设置字体大小、颜色、风格
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);

	//按照分隔符分隔宽字符串，进行换行
	std::vector<wstring> ws_result = ws_spilt(wquestionArray[questionIndex].quesTxt, L"[|]");
	//std::copy(ws_result.begin(), ws_result.end(), std::ostream_iterator<std::wstring, std::wstring::value_type>(std::wcout, L"\n"));
	//cout << ws_result.size() << endl;

	for (int i = 0; i < ws_result.size(); i++)
	{
		text.setPosition(x + initialX, y + initialY);
		text.setString(ws_result[i]);
		window.draw(text);
		initialY += CharacterSize;
	}

	initialY += CharacterSize / 2;

	ws_result = ws_spilt(wquestionArray[questionIndex].optionA, L"[|]");

	for (int i = 0; i < ws_result.size(); i++)
	{
		text.setPosition(x + initialX, y + initialY);
		text.setString(ws_result[i]);
		window.draw(text);
		initialY += CharacterSize;
	}

	initialY += CharacterSize / 2;

	ws_result = ws_spilt(wquestionArray[questionIndex].optionB, L"[|]");

	for (int i = 0; i < ws_result.size(); i++)
	{
		text.setPosition(x + initialX, y + initialY);
		text.setString(ws_result[i]);
		window.draw(text);
		initialY += CharacterSize;
	}

	initialY += CharacterSize / 2;

	ws_result = ws_spilt(wquestionArray[questionIndex].optionC, L"[|]");

	for (int i = 0; i < ws_result.size(); i++)
	{
		text.setPosition(x + initialX, y + initialY);
		text.setString(ws_result[i]);
		window.draw(text);
		initialY += CharacterSize;
	}

	initialY += CharacterSize / 2;

	ws_result = ws_spilt(wquestionArray[questionIndex].optionD, L"[|]");

	for (int i = 0; i < ws_result.size(); i++)
	{
		text.setPosition(x + initialX, y + initialY);
		text.setString(ws_result[i]);
		window.draw(text);
		initialY += CharacterSize;
	}

}


void SFMLTetris::ShowOutOfQuestion(int x, int y)
{
	int initialX = 20, initialY = 0;
	int CharacterSize = 30;
	//设置字体大小、颜色、风格
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"       恭喜你!");
	window.draw(text);

	initialY += CharacterSize + CharacterSize / 2;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"已经答完所有题目！！");
	window.draw(text);

	initialY += CharacterSize + CharacterSize / 2;
	
	text.setPosition(x + initialX, y + initialY);
	text.setString(L"题库正在扩充中......");
	window.draw(text);

	initialY += CharacterSize + CharacterSize / 2;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"     敬请期待");
	window.draw(text);

}

std::vector<std::wstring> SFMLTetris::ws_spilt(const std::wstring & in, const std::wstring & delim)
{
	std::wregex re{ delim };
	return std::vector<std::wstring>{
		std::wsregex_token_iterator(in.begin(), in.end(), re, -1),
		std::wsregex_token_iterator()
	};
}

void SFMLTetris::JudgeBtnClick(button * btn)
{
	if (btn->isClick)
	{
		btn->isClick = false;
		cout << "Button " << btn->Getid() << " Clicked" << endl;

		if (btn->Getid() == curAnswer)
		{
			soundClickCorrect.play();
			NextQuestion();
			cout << "Corret" << endl;
			if (awardCount < 3)
			{
				awardCount++;
				if (awardCount == 3)
				{
					awardCount = 0;
					EliminateRandRow();
				}
			}
		}
		else
		{
			soundClickFalse.play();
			NextQuestion();
			cout << "False" << endl;
			if (awardCount > 0)
			{
				awardCount--;
			}
			else
			{
				if (score >= 20)
				{
					score -= 20;
				}
			}
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
		//更换音乐
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
