#include "Game.h"


Game::Game()
{
	window.create(sf::VideoMode(STAGE_WIDTH * GRIDSIZE * 2 + INFO_WIDTH, STAGE_HEIGHT * GRIDSIZE), L"SFML 俄罗斯方块");
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
		//更换音乐
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
	srand((unsigned int)time(0));		//生成随机数种子
	
	//答题模块初始化
	answerModule.Initial();
	//俄罗斯方块游戏模块初始化
	tetrisModule.Initial();

	awardCount = 0;

	scoreCount = 1;
	frameRate = 25;
	delay = 0;
	timeCounter = 0;
	soundVolume = 50;
	musicOn = true;
	//随机获得一张背景
	bgIndex = Utility::GetRand(0, SpriteBgNum - 1);

	//加载素材
	LoadMediaData();

	//设置背景音乐
	bgMusic.setVolume(soundVolume);
	bgMusic.play();
	bgMusic.setLoop(true);

	//设置每秒的帧数
	window.setFramerateLimit(frameRate);

}

void Game::LoadMediaData()
{
	//加载字体
	if (!font.loadFromFile("Fonts/fangmeng.ttf"))
	{
		std::cout << "字体fangmeng.ttf没有找到" << std::endl;
	}
	text.setFont(font);


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
	if (!sbClickCorrect.loadFromFile("Audios/correct.ogg"))
	{
		std::cout << "correct.ogg没有找到" << std::endl;
	}
	if (!sbClickFalse.loadFromFile("Audios/false.ogg"))
	{
		std::cout << "false.ogg没有找到" << std::endl;
	}
	if (!sbClickBtn.loadFromFile("Audios/uiClickBtn.ogg"))
	{
		std::cout << "uiClickBt.ogg没有找到" << std::endl;
	}
	if (!bgMusic.openFromFile("Audios/bgMusic.ogg"))
	{
		std::cout << "bgMusic.ogg没有找到" << std::endl;
	}
	if (!failMusic.openFromFile("Audios/failMusic.ogg"))
	{
		std::cout << "failMusic.ogg没有找到" << std::endl;
	}

	soundClickCorrect.setBuffer(sbClickCorrect);
	soundClickFalse.setBuffer(sbClickFalse);
	soundClickBtn.setBuffer(sbClickBtn);

	btn_optionA = new button(1080.0f, 550.0f, 100.0f, 60.0f, font, L"A", 30, 0,0,Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 1);
	btn_optionB = new button(1310.0f, 550.0f, 100.0f, 60.0f, font, L"B", 30, 0,0,Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 2);
	btn_optionC = new button(1080.0f, 635.0f, 100.0f, 60.0f, font, L"C", 30, 0,0,Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 3);
	btn_optionD = new button(1310.0f, 635.0f, 100.0f, 60.0f, font, L"D", 30, 0,0,Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 4);

	btn_Item1 = new button(1170.0f, 100.0f, 150.0f, 80.0f, font, L"计算机二级", 25,-50,13, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 5);
	btn_Item2 = new button(1170.0f, 300.0f, 150.0f, 80.0f, font, L"英语", 30, -25, 10, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 6);
	btn_Item3 = new button(1170.0f, 500.0f, 150.0f, 80.0f, font, L"脑筋急转弯", 25, -50, 13, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14, 92, 109, 255), Color(0, 0, 0, 255), 7);

	btn_back = new button(1170.0f, 600.0f, 130.0f, 70.0f, font, L"返回", 30, -25,10, Color(196, 196, 196, 255), Color(119, 119, 119, 255), Color(14,  92, 109, 255), Color(0, 0, 0, 255), 8);
}

void Game::Input()
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
			tetrisModule.SetDir(UP);
		}
		//下
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
		{
			tetrisModule.SetDir(DOWN);
		}
		//左
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
		{
			tetrisModule.SetDir(LEFT);
		}
		//右
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
		{
			tetrisModule.SetDir(RIGHT);
		}
		//退出
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::X)
		{
			tetrisModule.gameOver = true;
		}
		ControlMusic(bgMusic, event);
	}
}

void Game::ButtonInput(sf::Event event)
{
	//监听按钮按下
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
			//1s后消除加速提示
			timeCounter = frameRate;

			window.setFramerateLimit(frameRate);
		}
	}
}

void Game::Draw()
{
	window.clear(Color::Color(255, 0, 255, 255));		//清屏

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
	//设置字体大小、颜色、风格
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(14, 92, 109, 255));
	text.setStyle(Text::Bold);

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"■	操作说明：");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□向左移动：A");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□向右移动：D");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□变换形状：W");
	window.draw(text);
	initialY += CharacterSize;

	text.setPosition(x + initialX, y + initialY);
	text.setString(L"		□快速下落：S");
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
	tetrisModule.ShowNext(x + initialX + 100, y + initialY,window);

	initialY += CharacterSize;
	initialY += CharacterSize;
	initialY += CharacterSize;
	text.setPosition(x + initialX, y + initialY);
	text.setString(L"■	分数：" + std::to_string(tetrisModule.score));
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
	//设置字体大小、颜色、风格
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(14, 92, 109, 255));
	text.setStyle(Text::Bold);

	//按照分隔符分隔宽字符串，进行换行
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
	//设置字体大小、颜色、风格
	text.setCharacterSize(CharacterSize);
	text.setFillColor(Color(14, 92, 109, 255));
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

void Game::ShowGameOverInfo(int x, int y)
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

void Game::ShowSpeedUpInfo(int x, int y)
{
	int characterSize = 30;
	text.setCharacterSize(characterSize);
	text.setFillColor(Color(255, 0, 0, 255));
	text.setStyle(Text::Bold);


	text.setPosition(x, y);
	text.setString(L"	Speed Up！！");
	window.draw(text);
}


void Game::ControlMusic(Music & music, sf::Event event)
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


