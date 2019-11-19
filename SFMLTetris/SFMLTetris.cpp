#include "SFMLTetris.h"
#include<iostream>

sf::RenderWindow window(sf::VideoMode(STAGE_WIDTH * GRIDSIZE * 2 + INFO_WIDTH, STAGE_HEIGHT * GRIDSIZE), L"SFML ����˹����");

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
	srand((unsigned int)time(0));		//�������������
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


	//���ݳ�ʼ��
	score = 0;
	scoreCount = 1;
	frameRate = 25;
	delay = 0;
	timeCounter = 0;
	gameOver = false;
	gameQuit = false;
	soundVolume = 50;
	musicOn = true;
	//������һ�ű���
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

	//���ɵ�һ������
	shape = GetRand(0, 6);
	SetShape(shape, box_shape, size_w, size_h,cur_color);

	//������һ������
	next_shape = GetRand(0, 6);
	SetShape(next_shape, next_box_shape, next_size_w, next_size_h,next_color);

	//�����ز�
	LoadMediaData();

	//���ñ�������
	bgMusic.setVolume(soundVolume);
	bgMusic.play();
	bgMusic.setLoop(true);

	//����ÿ���֡��
	window.setFramerateLimit(frameRate);	
}

void SFMLTetris::LoadMediaData()
{

	//��������
	if (!font.loadFromFile("Fonts/fangmeng.ttf"))
	{
		std::cout << "����fangmeng.ttfû���ҵ�" << std::endl;
	}
	text.setFont(font);

	//��������ͼƬ
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
			std::cout << "ͼƬ" << textureTetrisPath[i] << "û���ҵ�" << std::endl;
		}
		spArray[i].setTexture(tArray[i]);
		spArray[i].setScale(SCALE, SCALE);
	}

	//���ر�������ͼƬ
	if (!tBackground.loadFromFile(textureBgPath[bgIndex]))
	{
		std::cout << "ͼƬ" + textureBgPath[bgIndex] + "û���ҵ�" << std::endl;
	}
	spBackground.setTexture(tBackground);
	spBackground.setScale(BGSCALEX, BGSCALEY);

	//���ؽ�������ͼƬ
	if (!tAward.loadFromFile(textureAwardPath))
	{
		std::cout << "ͼƬ" + textureAwardPath + "û���ҵ�" << endl;
	}
	spAward.setTexture(tAward);
	spAward.setScale(0.4, 0.4);

	if (!tAwardGray.loadFromFile(textureAwardGrayPath))
	{
		std::cout << "ͼƬ" + textureAwardGrayPath + "û���ҵ�" << endl;
	}
	spAwardGray.setTexture(tAwardGray);
	spAwardGray.setScale(0.4, 0.4);

	//��������
	if (!sbDrop.loadFromFile("Audios/Drop1.ogg"))
	{
		std::cout << "Drop1.oggû���ҵ�" << std::endl;
	}
	if (!sbAppear.loadFromFile("Audios/Appear1.ogg"))
	{
		std::cout << "Appear1.oggû���ҵ�" << std::endl;
	}
	if (!sbClickCorrect.loadFromFile("Audios/correct.ogg"))
	{
		std::cout << "correct.oggû���ҵ�" << std::endl;
	}
	if (!sbClickFalse.loadFromFile("Audios/false.ogg"))
	{
		std::cout << "false.oggû���ҵ�" << std::endl;
	}
	if (!bgMusic.openFromFile("Audios/bgMusic.ogg"))
	{
		std::cout << "bgMusic.oggû���ҵ�" << std::endl;
	}
	if (!failMusic.openFromFile("Audios/failMusic.ogg"))
	{
		std::cout << "failMusic.oggû���ҵ�" << std::endl;
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
	window.clear(Color::Color(255, 0, 255, 255));		//����

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
					//�����Ҳ���Ϸ��
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

		//��
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::W)
		{
			dir = UP;
		}
		//��
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
		{
			dir = DOWN;
		}
		//��
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
		{
			dir = LEFT;
		}
		//��
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
		{
			dir = RIGHT;
		}
		//�˳�
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
		{
			gameOver = true;
		}
		ControlMusic(bgMusic, event);
	}
}

void SFMLTetris::ButtonInput(sf::Event event)
{
	//������ť����
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
	//������������
	head_y++;
	//�����ײ�����
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
		//������һ������
		Score_Next();
	}

	//�����ƶ�
	if (dir == LEFT)
	{
		dir = STOP;
		head_x--;
		if (IsAggin())
		{
			head_x++;
		}
	}

	//�����ƶ�
	if (dir == RIGHT)
	{
		dir = STOP;
		head_x++;
		if (IsAggin())
		{
			head_x--;
		}
	}

	//�任��״
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
		//������һ������
		Score_Next();
	}
}



void SFMLTetris::Prompt_info(int x, int y)
{
	int initialX = 20, initialY = 0;
	int CharacterSize = 30;
	//���������С����ɫ�����
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"��	����˵����");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		�������ƶ�����");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		�������ƶ�����");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		���任��״����");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		���������䣺��");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		�����/����������+/-");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		������/�ر����֣�Enter/*");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		���˳���Ϸ��X");
	window.draw(text);
	initialY += CharacterSize;
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"��	��һ�����飺");
	window.draw(text);
	initialY += CharacterSize;

	initialY += CharacterSize;
	initialY += CharacterSize;
	ShowNext(x + initialX + 100, y + initialY);

	initialY += CharacterSize;
	initialY += CharacterSize;
	initialY += CharacterSize;
	text.setPosition(x + initialX, y + initialY);
	text.setString(L"��	������" + std::to_string(score) );
	window.draw(text);
	initialY += CharacterSize;

}

void SFMLTetris::SetShape(int & cshape, int shape[][4], int & size_w, int & size_h,eColor &color)
{
	//��ʼ����ǰ��״����
	int i, j;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			shape[i][j] = 0;
		}
	}

	//����7�г�ʼ��״�Լ����ǵ�size
	switch (cshape)
	{
		//I��
	case 0:
		size_h = 1;
		size_w = 4;
		shape[0][0] = 1;
		shape[0][1] = 1;
		shape[0][2] = 1;
		shape[0][3] = 1;
		color = RED;
		break;
		//L��
	case 1:
		size_h = 2;
		size_w = 3;
		shape[0][0] = 1;
		shape[1][0] = 1;
		shape[1][1] = 1;
		shape[1][2] = 1;
		color = GREEN;
		break;
		//J��
	case 2:
		size_h = 2;
		size_w = 3;
		shape[0][2] = 1;
		shape[1][0] = 1;
		shape[1][1] = 1;
		shape[1][2] = 1;
		color = BLUE;
		break;
		//Z��
	case 3:
		size_h = 2;
		size_w = 3;
		shape[0][1] = 1;
		shape[0][2] = 1;
		shape[1][0] = 1;
		shape[1][1] = 1;
		color = DARKBLUE;
		break;
		//S��
	case 4:
		size_h = 2;
		size_w = 3;
		shape[0][0] = 1;
		shape[0][1] = 1;
		shape[1][1] = 1;
		shape[1][2] = 1;
		color = ORANGE;
		break;
		//O��
	case 5:
		size_h = 2;
		size_w = 2;
		shape[0][0] = 1;
		shape[0][1] = 1;
		shape[1][0] = 1;
		shape[1][1] = 1;
		color = PURPLE;
		break;
		//T��
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


	//���÷���ĳ�ʼλ��
	head_x = width / 2;
	head_y = 0;

	//Խ��
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
		//�ӵײ�������������
		for (i = STAGE_HEIGHT - 2; i >= 2; i--)
		{
			int s = i;
			//�����ǰ��Ϊ����
			if (Exsqr(i))
			{
				//����������һ���ǿ���
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
	int temp[4][4] = { 0 };		//��ʱ����
	int temp_piece[4][4] = { 0 };
	int tmp_size_w, tmp_size_h;

	tmp_size_w = size_w;
	tmp_size_h = size_h;

	//������תǰ���飬���ڻ�ԭ
	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			temp_piece[i][j] = box_shape[i][j];
		}

	//����б�Խ��߶Գ�
	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			temp[j][i] = box_shape[i][j];
		}
	//����size
	int t = size_h;
	size_h = size_w;
	size_w = t;
	for(int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			box_shape[i][j] = 0;
		}
	//���ҶԳ�
	for(int i = 0; i < size_h; i++)
		for (int j = 0; j < size_w; j++)
		{
			box_shape[i][size_w - 1 - j] = temp[i][j];
		}

	//�����ת�󲻷����������ָ�ԭͼ��
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
			//1s������������ʾ
			timeCounter = frameRate;
			
			window.setFramerateLimit(frameRate);
		}
	}

	//����һ���������Ϣ������ǰ����
	SetShape(next_shape, box_shape, size_w, size_h,cur_color);
	//cur_color = next_color;

	//����������һ������
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
	//��Ϊ�յ�����
	int rowCount = 0;
	for (int i = STAGE_HEIGHT - 2; i >= 2; i--)
	{
		//�����ǰ��Ϊ�ǿ���
		if (!Exsqr(i))
		{
			rowCount++;
		}
		//����ǿ��о�����ѭ��
		else
		{
			break;
		}
	}
	if (rowCount <= 0)
	{
		return;
	}
	//���������
	int randRow = GetRand(0, rowCount - 1);
	for (int j = 1; j < STAGE_WIDTH - 1; j++)
	{
		box_map[STAGE_HEIGHT - 2 - randRow][j] = 0;
	}
	for (int i = STAGE_HEIGHT - 2; i >= 2; i--)
	{
		int s = i;
		//�����ǰ��Ϊ����
		if (Exsqr(i))
		{
			//����������һ���ǿ���
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
		//�����ǰ��Ϊ����
		if (Exsqr(i))
		{
			//����������һ���ǿ���
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
	text.setString(L"	��Ϸ��������");
	window.draw(text);
	initialY += characterSize;
	text.setPosition(x + initialX, y + initialY);
	text.setString(L"	Y���¿�ʼ/N�˳�");
	window.draw(text);
}

void SFMLTetris::ShowSpeedUpInfo(int x, int y)
{
	int characterSize = 30;
	text.setCharacterSize(characterSize);
	text.setFillColor(Color(255, 0, 0, 255));
	text.setStyle(Text::Bold);


	text.setPosition(x, y);
	text.setString(L"	Speed Up����");
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
	//���\��������
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
	//����\�ر�����
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
		cout << "Failed to load file��No root element" << endl;
		//�����ڴ�
		doc.Clear();
		return false;
	}

	//�����ӽڵ�
	for (TiXmlElement * elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		Question question;
		//TODO
		WQuestion wquestion;
		//��ȡԪ����
		string elemName = elem->Value();
		string attr;
		//��ȡԪ������ֵ
		attr = elem->Attribute("ID");
		int index = atoi(attr.c_str());
		//�ٴα��������Խ��
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
	//Ԥת�����õ�����ռ�Ĵ�С;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//ת��
	MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//������'\0'
	wszString[wcsLen] = '\0';

	string unicodeString = Wchar_tToString(wszString);

	delete[] wszString;
	wszString = NULL;

	return unicodeString;
}

string SFMLTetris::Wchar_tToString(wchar_t * wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
	char *psText; // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
	string szDst = psText;// std::string��ֵ
	delete[]psText;// psText�����
	return szDst;
}

wstring SFMLTetris::Utf8ToUnicodeW(const char * szU8)
{
	//Ԥת�����õ�����ռ�Ĵ�С;
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);

	//����ռ�Ҫ��'\0'�����ռ䣬MultiByteToWideChar�����'\0'�ռ�
	wchar_t* wszString = new wchar_t[wcsLen + 1];

	//ת��
	MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);

	//������'\0'
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
	//TODO ������ʾ
}

void SFMLTetris::ShowQuestion(int x, int y)
{
	int initialX = 20, initialY = 0;
	int CharacterSize = 30;
	//���������С����ɫ�����
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);

	//���շָ����ָ����ַ��������л���
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
	//���������С����ɫ�����
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(255, 255, 255, 255));
	text.setStyle(Text::Bold);

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"       ��ϲ��!");
	window.draw(text);

	initialY += CharacterSize + CharacterSize / 2;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"�Ѿ�����������Ŀ����");
	window.draw(text);

	initialY += CharacterSize + CharacterSize / 2;
	
	text.setPosition(x + initialX, y + initialY);
	text.setString(L"�������������......");
	window.draw(text);

	initialY += CharacterSize + CharacterSize / 2;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"     �����ڴ�");
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
		//��������
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

				////���\��������
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
				////����\�ر�����
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
