#include "Game.h"


Game::Game()
{
	window.create(sf::VideoMode(STAGE_WIDTH * GRIDSIZE * 2 + INFO_WIDTH, STAGE_HEIGHT * GRIDSIZE), L"SFML ����˹����");
}


Game::~Game()
{
}

void Game::GameLoop()
{
	do {
		Initial();
		while (window.isOpen() && !tetrisModule.gameOver)
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

		while ( tetrisModule.gameOver)
		{
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
				{
					window.close();
					tetrisModule.gameOver = false;
					gameQuit = true;
				}
				if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Y)
				{
					tetrisModule.gameOver = false;
				}
				if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::N)
				{
					tetrisModule.gameOver = false;
					gameQuit = true;
				}

				ControlMusic(failMusic, event);
			}
		}
	} while (!gameQuit);

}

void Game::Initial()
{
	srand((unsigned int)time(0));		//�������������
	
	//����ģ���ʼ��
	answerModule.Initial();
	//����˹������Ϸģ���ʼ��
	tetrisModule.Initial();

	awardCount = 0;

	scoreCount = 1;
	frameRate = 25;
	delay = 0;
	timeCounter = 0;
	soundVolume = 50;
	musicOn = true;
	//������һ�ű���
	bgIndex = Utility::GetRand(0, SpriteBgNum - 1);

	//�����ز�
	LoadMediaData();

	//���ñ�������
	bgMusic.setVolume(soundVolume);
	bgMusic.play();
	bgMusic.setLoop(true);

	//����ÿ���֡��
	window.setFramerateLimit(frameRate);

}

void Game::LoadMediaData()
{
	//��������
	if (!font.loadFromFile("Fonts/fangmeng.ttf"))
	{
		std::cout << "����fangmeng.ttfû���ҵ�" << std::endl;
	}
	text.setFont(font);


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
	if (!sbClickCorrect.loadFromFile("Audios/correct.ogg"))
	{
		std::cout << "correct.oggû���ҵ�" << std::endl;
	}
	if (!sbClickFalse.loadFromFile("Audios/false.ogg"))
	{
		std::cout << "false.oggû���ҵ�" << std::endl;
	}
	if (!sbClickBtn.loadFromFile("Audios/uiClickBtn.ogg"))
	{
		std::cout << "uiClickBt.oggû���ҵ�" << std::endl;
	}
	if (!bgMusic.openFromFile("Audios/bgMusic.ogg"))
	{
		std::cout << "bgMusic.oggû���ҵ�" << std::endl;
	}
	if (!failMusic.openFromFile("Audios/failMusic.ogg"))
	{
		std::cout << "failMusic.oggû���ҵ�" << std::endl;
	}

	soundClickCorrect.setBuffer(sbClickCorrect);
	soundClickFalse.setBuffer(sbClickFalse);
	soundClickBtn.setBuffer(sbClickBtn);

	btn_optionA = new button(1080.0f, 550.0f, 100.0f, 60.0f, font, L"A", 30, 0,0,Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 1);
	btn_optionB = new button(1310.0f, 550.0f, 100.0f, 60.0f, font, L"B", 30, 0,0,Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 2);
	btn_optionC = new button(1080.0f, 635.0f, 100.0f, 60.0f, font, L"C", 30, 0,0,Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 3);
	btn_optionD = new button(1310.0f, 635.0f, 100.0f, 60.0f, font, L"D", 30, 0,0,Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 4);

	btn_Item1 = new button(1170.0f, 100.0f, 150.0f, 80.0f, font, L"���������", 25,-50,13, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 5);
	btn_Item2 = new button(1170.0f, 300.0f, 150.0f, 80.0f, font, L"Ӣ��", 30, -25, 10, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 6);
	btn_Item3 = new button(1170.0f, 500.0f, 150.0f, 80.0f, font, L"�Խת��", 25, -50, 13, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 7);

	btn_back = new button(1170.0f, 600.0f, 130.0f, 70.0f, font, L"����", 30, -25,10, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14,  92, 109, 255), Color(0, 0, 0, 255), 8);
}

void Game::Input()
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
			tetrisModule.SetDir(UP);
		}
		//��
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
		{
			tetrisModule.SetDir(DOWN);
		}
		//��
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
		{
			tetrisModule.SetDir(LEFT);
		}
		//��
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
		{
			tetrisModule.SetDir(RIGHT);
		}
		//�˳�
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
		{
			tetrisModule.gameOver = true;
		}
		ControlMusic(bgMusic, event);
	}
}

void Game::ButtonInput(sf::Event event)
{
	//������ť����
	Vector2i mousePosition(event.mouseButton.x, event.mouseButton.y);
	btn_optionA->update(mousePosition, event);
	btn_optionB->update(mousePosition, event);
	btn_optionC->update(mousePosition, event);
	btn_optionD->update(mousePosition, event);

	JudgeOptionBtnClick(btn_optionA);
	JudgeOptionBtnClick(btn_optionB);
	JudgeOptionBtnClick(btn_optionC);
	JudgeOptionBtnClick(btn_optionD);

	btn_Item1->update(mousePosition, event);
	btn_Item2->update(mousePosition, event);
	btn_Item3->update(mousePosition, event);

	JudgeChooseSubjectBtn(btn_Item1);
	JudgeChooseSubjectBtn(btn_Item2);
	JudgeChooseSubjectBtn(btn_Item3);

	btn_back->update(mousePosition, event);
	JudgeBackBtn(btn_back);
}

void Game::Logic()
{

	tetrisModule.Logic();

	if (tetrisModule.score >= scoreCount * 500)
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
}

void Game::Draw()
{
	window.clear(Color::Color(255, 0, 255, 255));		//����

	spBackground.setPosition(0, 0);
	window.draw(spBackground);

	DrawAward();
	Prompt_info(tetrisModule.width * GRIDSIZE, GRIDSIZE);

	if(answerModule.answerState == NoChooseSuject)
	{
		DrawChooseSubjectBtn();
	}
	else if (answerModule.answerState == InAnswerQuestion)
	{
		if (answerModule.curWQuestion.answer != -1)
		{
			DrawOptionBtn();
			ShowQuestion(1050, 100);
		}
		else
		{
			answerModule.answerState = OutOfQuesion;
		}
	}
	else if (answerModule.answerState == OutOfQuesion)
	{
		ShowOutOfQuestion(1080, 200);
		DrawBackBtn();
	}

	
	tetrisModule.Draw(window);

	if (tetrisModule.gameOver)
	{
		ShowGameOverInfo(tetrisModule.width / 10 * GRIDSIZE, tetrisModule.height / 3 * GRIDSIZE);
		timeCounter = 0;
	}

	if (timeCounter > 0)
	{
		ShowSpeedUpInfo(tetrisModule.width / 4 * GRIDSIZE, tetrisModule.height / 3 * GRIDSIZE);
		timeCounter -= 1;
	}
	
	window.display();
}

void Game::DrawAward()
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

void Game::Prompt_info(int x, int y)
{
	int initialX = 20, initialY = 0;
	int CharacterSize = 30;
	//���������С����ɫ�����
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(14, 92, 109, 255));
	text.setStyle(Text::Bold);

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"��	����˵����");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		�������ƶ���A");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		�������ƶ���D");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		���任��״��W");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		���������䣺S");
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
	tetrisModule.ShowNext(x + initialX + 100, y + initialY,window);

	initialY += CharacterSize;
	initialY += CharacterSize;
	initialY += CharacterSize;
	text.setPosition(x + initialX, y + initialY);
	text.setString(L"��	������" + std::to_string(tetrisModule.score));
	window.draw(text);
	initialY += CharacterSize;
}


void Game::DrawOptionBtn()
{
	btn_optionA->render(window);
	btn_optionB->render(window);
	btn_optionC->render(window);
	btn_optionD->render(window);
}

void Game::DrawChooseSubjectBtn()
{
	btn_Item1->render(window);
	btn_Item2->render(window);
	btn_Item3->render(window);
}

void Game::DrawBackBtn()
{
	btn_back->render(window);
}

void Game::ShowQuestion(int x, int y)
{
	int initialX = 20, initialY = 0;
	int CharacterSize = 30;
	//���������С����ɫ�����
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(14, 92, 109, 255));
	text.setStyle(Text::Bold);

	//���շָ����ָ����ַ��������л���
	std::vector<wstring> ws_result = ws_spilt(answerModule.wquestionArray[answerModule.questionIndex].quesTxt, L"[|]");
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

	ws_result = ws_spilt(answerModule.wquestionArray[answerModule.questionIndex].optionA, L"[|]");

	for (int i = 0; i < ws_result.size(); i++)
	{
		text.setPosition(x + initialX, y + initialY);
		text.setString(ws_result[i]);
		window.draw(text);
		initialY += CharacterSize;
	}

	initialY += CharacterSize / 2;

	ws_result = ws_spilt(answerModule.wquestionArray[answerModule.questionIndex].optionB, L"[|]");

	for (int i = 0; i < ws_result.size(); i++)
	{
		text.setPosition(x + initialX, y + initialY);
		text.setString(ws_result[i]);
		window.draw(text);
		initialY += CharacterSize;
	}

	initialY += CharacterSize / 2;

	ws_result = ws_spilt(answerModule.wquestionArray[answerModule.questionIndex].optionC, L"[|]");

	for (int i = 0; i < ws_result.size(); i++)
	{
		text.setPosition(x + initialX, y + initialY);
		text.setString(ws_result[i]);
		window.draw(text);
		initialY += CharacterSize;
	}

	initialY += CharacterSize / 2;

	ws_result = ws_spilt(answerModule.wquestionArray[answerModule.questionIndex].optionD, L"[|]");

	for (int i = 0; i < ws_result.size(); i++)
	{
		text.setPosition(x + initialX, y + initialY);
		text.setString(ws_result[i]);
		window.draw(text);
		initialY += CharacterSize;
	}

}

void Game::ShowOutOfQuestion(int x, int y)
{
	int initialX = 20, initialY = 0;
	int CharacterSize = 30;
	//���������С����ɫ�����
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(14, 92, 109, 255));
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

void Game::ShowGameOverInfo(int x, int y)
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

void Game::ShowSpeedUpInfo(int x, int y)
{
	int characterSize = 30;
	text.setCharacterSize(characterSize);
	text.setFillColor(Color(255, 0, 0, 255));
	text.setStyle(Text::Bold);


	text.setPosition(x, y);
	text.setString(L"	Speed Up����");
	window.draw(text);
}


void Game::ControlMusic(Music & music, sf::Event event)
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

void Game::JudgeOptionBtnClick(button * btn)
{
	if (!(answerModule.answerState == InAnswerQuestion))
	{
		return;
	}

	if (btn->isClick)
	{
		btn->isClick = false;
		cout << "Button " << btn->Getid() << " Clicked" << endl;

		if (btn->Getid() == answerModule.curAnswer)
		{
			soundClickCorrect.play();
			answerModule.NextQuestion();
			cout << "Corret" << endl;
			if (awardCount < 3)
			{
				awardCount++;
				if (awardCount == 3)
				{
					awardCount = 0;
					tetrisModule.soundAppear.play();
					tetrisModule.EliminateRandRow();
				}
			}
		}
		else
		{
			soundClickFalse.play();
			answerModule.NextQuestion();
			cout << "False" << endl;
			if (awardCount > 0)
			{
				awardCount--;
			}
			else
			{
				if (tetrisModule.score >= 20)
				{
					tetrisModule.score -= 20;
				}
			}
		}
	}
}

void Game::JudgeChooseSubjectBtn(button * btn)
{
	if (!(answerModule.answerState == NoChooseSuject))
	{
		return;
	}

	if (btn->isClick)
	{
		btn->isClick = false;
		cout << "Button " << btn->Getid() << " Clicked" << endl;
		soundClickBtn.play();
		answerModule.answerState = InAnswerQuestion;

		switch (btn->Getid())
		{
		case 5:
			answerModule.LoadXmlFile("datastructurequestions.xml");
			answerModule.Initial();
			break;
		case 6:
			answerModule.LoadXmlFile("englishquestions.xml");
			answerModule.Initial();
			break;
		case 7:
			answerModule.LoadXmlFile("braintwisterquestions.xml");
			answerModule.Initial();
			break;
		default:
			break;
		}
	
	}
}

void Game::JudgeBackBtn(button * btn)
{
	if (!(answerModule.answerState == OutOfQuesion))
	{
		return;
	}

	if (btn->isClick)
	{
		soundClickBtn.play();
		btn->isClick = false;
		cout << "Button " << btn->Getid() << " Clicked" << endl;
		answerModule.answerState = NoChooseSuject;
	}
}

std::vector<std::wstring> Game::ws_spilt(const std::wstring & in, const std::wstring & delim)
{
	std::wregex re{ delim };
	return std::vector<std::wstring>{
		std::wsregex_token_iterator(in.begin(), in.end(), re, -1),
			std::wsregex_token_iterator()
	};
}


