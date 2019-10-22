#include "SFMLTetris.h"
#include<iostream>

sf::RenderWindow window(sf::VideoMode(STAGE_WIDTH * GRIDSIZE + INFO_WIDTH, STAGE_HEIGHT * GRIDSIZE), L"SFML ����˹����");

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
	
	//���ݳ�ʼ��
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

	window.setFramerateLimit(frameRate);	//����ÿ���֡��

	
	//��������
	if (!font.loadFromFile("Fonts/fangmeng.ttf"))
	{
		std::cout << "����fangmeng.ttfû���ҵ�" << std::endl;
	}
	text.setFont(font);

	//��������ͼƬ
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
			std::cout << "ͼƬ" <<  texturePath[i] <<"û���ҵ�" << std::endl;
		}
		spArray[i].setTexture(tArray[i]);
		spArray[i].setScale(SCALE, SCALE);
	}

	//��������
	if (!sbDrop.loadFromFile("Audios/Drop1.ogg"))
	{
		std::cout << "Drop1.oggû���ҵ�" << std::endl;
	}
	if (!sbAppear.loadFromFile("Audios/Appear1.ogg"))
	{
		std::cout << "Appear1.oggû���ҵ�" << std::endl;
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

	bgMusic.setVolume(soundVolume);
	bgMusic.play();
	bgMusic.setLoop(true);

	srand((unsigned int)time(0));		//�������������

	//���ɵ�һ������
	shape = GetRand(0, 6);
	SetShape(shape, box_shape, size_w, size_h,cur_color);

	//������һ������
	next_shape = GetRand(0, 6);
	SetShape(next_shape, next_box_shape, next_size_w, next_size_h,next_color);
}

void SFMLTetris::Draw()
{
	window.clear(Color::Color(255, 0, 255, 255));		//����

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
		//��
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Up)
		{
			dir = UP;
		}
		//��
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
		{
			dir = DOWN;
		}
		//��
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
		{
			dir = LEFT;
		}
		//��
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
		{
			dir = RIGHT;
		}
		//�˳�
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
		{
			gameOver = true;
		}

		ControlMusic(bgMusic, event);
		////���\��������
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
		////����\�ر�����
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

	//������ӳ�䵽������
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		box_map[head_x + j][head_y + i] = shape[j][i];
	//	}
	//}

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

	if (score >= scoreCount * 100 )
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
