#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <cstdlib>
#include <sstream>
using namespace sf;
	//Function declartion for branches
	void updateBranches(int seed);
	
	const int NUM_BRANCHES=6;
	//define an array named branches of Sprite Type
	Sprite branches[NUM_BRANCHES];
	
	enum class side {LEFT,RIGHT,NONE};
	side branchPositions[NUM_BRANCHES];
	
int main(){
	//Define window resolution and title
	VideoMode vm(1966,768);
	RenderWindow window(vm,"Timber!!");
	//Create a float box inside the window to set the pic to be seen as fullscreen
	View view(FloatRect(0,0,1920,1080));
	window.setView(view);
	//add background 
	Texture textureBackground;
	textureBackground.loadFromFile("Graphics/win.png");
	
	Sprite spriteBackground;
	spriteBackground.setTexture(textureBackground);
	spriteBackground.setPosition(0,0);
	
	//add tree
	Texture textureTree;
	textureTree.loadFromFile("Graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810,0);
	
	//add tree2
	Texture textureTree2;
	textureTree2.loadFromFile("Graphics/tree2.png");
	
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	spriteTree2.setPosition(1500,-200);
	
	//add bee
	Texture textureBee;
	textureBee.loadFromFile("Graphics/bee9.png");
	
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0,800);
	bool beeActive=false;
	float beeSpeed=0.0f;
	
	//add sun
	Texture textureSun;
	textureSun.loadFromFile("Graphics/sun.png");
	
	Sprite spriteSun;
	spriteSun.setTexture(textureSun);
	spriteSun.setPosition(1600,100);
	//bool beeActive=false;
	//float beeSpeed=0.0f;
	
	//add clouds(1,2,3)
	Texture textureCloud;
	textureCloud.loadFromFile("Graphics/cld.png");
	
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	spriteCloud1.setPosition(0,0);
	spriteCloud2.setPosition(0,150);
	spriteCloud3.setPosition(0,300);
	bool cloud1Active=false;
	float cloud1Speed=0.0f;
	bool cloud2Active=false;
	float cloud2Speed=0.0f;
	bool cloud3Active=false;
	float cloud3Speed=0.0f;
	
	Clock clock;	//To control Time
	//Time Bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight=80;	
	timeBar.setSize(Vector2f(timeBarStartWidth,timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920/2)-timeBarStartWidth/2,980);
	
	Time gameTime;
	float timeRemaining=6.0f;
	float timeBarWidthPerSecond=timeBarStartWidth/timeRemaining;
	//Track whether game is running or not
	bool paused = true;
	
	//Draw Text
	int score=0;
	Text messageText;
	Text scoreText;
	
	//Loading Font
	Font font;
	font.loadFromFile("Font/Cookie.ttf");
	
	//Set the loaded font to message
	messageText.setFont(font);
	scoreText.setFont(font);
	
	//Assign the text to be displayed
	messageText.setString("Press Enter to Start the game");
	scoreText.setString("Score = 0");
	
	//Assign the size of the font
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	
	//Choose the Color of text
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	
	//Positioning of the text
	FloatRect textRect= messageText.getLocalBounds();
	
	messageText.setOrigin(textRect.left +textRect.width/2.0f, textRect.top+textRect.height/2.0f);
	
	messageText.setPosition(1920/2.0f,1080/2.0f);
	
	scoreText.setPosition(20,20);
	//Add 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("Graphics/branch.png");
	
	//set texture for each branch
	for (int i=0;i<NUM_BRANCHES;i++)
		{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000,-2000);
		// Set the origin of the sprite
		branches[i].setOrigin(220,20);
		}
	//Set Player texture
	Texture texturePlayer;
	texturePlayer.loadFromFile("Graphics/image.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580,720);
	
	// The player starts on the left
	side playerSide = side::LEFT;

	//Set RIP texture
	Texture textureRIP;
	textureRIP.loadFromFile("Graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600,860);
	
	//Set the Axe
	Texture textureAxe;
	textureAxe.loadFromFile("Graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700,830);
	
	// Line the axe up with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//Set the Log
	Texture textureLog;
	textureLog.loadFromFile("Graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);
	// Some other useful log related variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;
	
	//Control player input
	bool acceptInput = false;
	
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("Sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);
	
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("Sound/death.wav");
	Sound death;
	death.setBuffer(chopBuffer);
	
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("Sound/out_of_time.wav");
	Sound oot;
	oot.setBuffer(ootBuffer);
	
	//Main Loop
	while (window.isOpen())
	{
	Event event;
	while (window.pollEvent(event))
	{
		if (event.type == Event::KeyReleased && !paused)
		{
			// Listen for key presses again
			acceptInput = true;
			// hide the axe
	spriteAxe.setPosition(2000,spriteAxe.getPosition().y);
		}
	}
	
	Event event1;
	while(window.pollEvent(event1))
	{
		if(event1.type==event1.Closed)
		{
			window.close();
		}
	}
	
	//keyboard press check
	if (Keyboard::isKeyPressed(Keyboard::Escape))
			
	{
		window.close();
	}	
	if (Keyboard::isKeyPressed(Keyboard::Enter))
	{
		paused= false;
		// Reset the time and the score
		score = 0;
		timeRemaining = 6;
		//Make all the branches disaapear at first
		for (int i=1;i<NUM_BRANCHES;i++)
		{
			branchPositions[i] = side::NONE;
		}
		//Make the Gravestone Disappear
		spriteRIP.setPosition(675,2000);
		spritePlayer.setPosition(580,720);
		acceptInput=true;
	}
	
	//To accept Player input
	if(acceptInput)
	{
		//For the right button press
		if (Keyboard::isKeyPressed(Keyboard::Right))
		{
			playerSide=side::RIGHT;
			score++;
			//Set the time remaining 
			timeRemaining += (2/score)+.15;
			spriteAxe.setPosition(AXE_POSITION_RIGHT,spriteAxe.getPosition().y);
			spritePlayer.setPosition(1200,700);
			updateBranches(score);
			
			//Set the log flying to the left
			spriteLog.setPosition(810,720);
			logSpeedX = -5000;
			logActive = true;
			
			acceptInput = false;
			chop.play();
		}
		//Accept for left input
		if (Keyboard::isKeyPressed(Keyboard::Left))
		{
			playerSide=side::LEFT;
			score++;
			//Set the time remaining 
			timeRemaining += (2/score)+.15;
			spriteAxe.setPosition(AXE_POSITION_LEFT,spriteAxe.getPosition().y);
			spritePlayer.setPosition(580,700);
			//update branches
			updateBranches(score);
			
			//Set the log flying to the left
			spriteLog.setPosition(810,720);
			logSpeedX = -5000;
			logActive = true;
			
			acceptInput = false;
			chop.play();
		}
	}
	if (!paused)
	{	
	Time dt=clock.restart(); //To measure Time
	//Subtract from the amount of time remaining
	timeRemaining -= dt.asSeconds();
	//Set size of timebar
	timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining,timeBarHeight));
	
	
	if(timeRemaining<=0.0f)
	{
		paused=true; //Pausing the game
		//Show msg to player
		messageText.setString("Out Of Time!!");
		//Reposition the text based on new size
		FloatRect textRect=messageText.getLocalBounds();
		messageText.setOrigin(textRect.left+textRect.width/2.0f,textRect.top+textRect.height/2.0f);
		
		messageText.setPosition(1920/2.0f,1080/2.0f);
		oot.play();
	}
	// BEE MOVEMENT
	if (!beeActive)
	{
		srand((int)time(0)*10);
		beeSpeed = (rand()%200)+200;
		
		//Height
		srand((int)time(0)*10);
		float height =(rand() %500)+500;
		spriteBee.setPosition(1850,height);
		
		beeActive=true;
	}
	
	else	//If bee is alrady moving
	{
		spriteBee.setPosition( spriteBee.getPosition().x - (beeSpeed*dt.asSeconds()),spriteBee.getPosition().y);
		
		//Check if the bee has reached the end of screen
		if(spriteBee.getPosition().x< -100)
		{
			beeActive=false;
		}
	}	
	
	
	//For Cloud 1
	if (!cloud1Active)
	{
		srand((int)time(0)*10);
		cloud1Speed = (rand() % 200);
		
		//Height
		srand((int)time(0)*10);
		float height = (rand() % 150);
		spriteCloud1.setPosition(-200,height);
		cloud1Active=true;
	}
	
	else
	{
	// here + is used instead of - so that the cloud moves from left to right
	spriteCloud1.setPosition(spriteCloud1.getPosition().x + (cloud1Speed*dt.asSeconds()),spriteCloud1.getPosition().y);
		if(spriteCloud1.getPosition().x > 1920)
		{
			cloud1Active= false;
		}
		 
	}
	
	//For Cloud 2
	if (!cloud2Active)
	{
		srand((int)time(0)*20);
		cloud2Speed = (rand() % 200);
		
		//Height
		srand((int)time(0)*20);
		float height = (rand() % 300);
		spriteCloud2.setPosition(-200,height);
		cloud2Active=true;
	}
	
	else
	{
	// here + is used instead of - so that the cloud moves from left to right
	spriteCloud2.setPosition(spriteCloud2.getPosition().x + (cloud2Speed*dt.asSeconds()),spriteCloud2.getPosition().y);
		if(spriteCloud2.getPosition().x > 1920)
		{
			cloud2Active= false;
		}
		 
	}
	
	//For Cloud 3
	if (!cloud3Active)
	{
		srand((int)time(0)*30);
		cloud3Speed = (rand() % 200);
		
		//Height
		srand((int)time(0)*30);
		float height = (rand() % 450);
		spriteCloud3.setPosition(-200,height);
		cloud3Active=true;
	}
	
	else
	{
	// here + is used instead of - so that the cloud moves from left to right
	spriteCloud3.setPosition(spriteCloud3.getPosition().x + (cloud3Speed*dt.asSeconds()),spriteCloud3.getPosition().y);
		if(spriteCloud3.getPosition().x > 1920)
		{
			cloud3Active= false;
		}
		 
	}
	// Update the score text
	std::stringstream ss;
	ss<< "Score = " << score;
	scoreText.setString(ss.str());
	//score++;
	//Update the Branches
	for (int i=0;i<NUM_BRANCHES;i++)
	{
		float height =i*150;
		if(branchPositions[i] == side::LEFT)
		{
			//Move the branch to th eleft side
			branches[i].setPosition(610,height);
			//Flip it cuz the graphic we have is facing right
			branches[i].setRotation(180);
		}
		
		else if(branchPositions[i] == side::RIGHT)
		{
			//Move the sprite to the right side
			branches[i].setPosition(1330,height);
			//No flip required
			branches[i].setRotation(0);
		}
		
		else
		{
			//Hide the branch
			branches[i].setPosition(3000,height);
		}
	}
	// Handle a flying log
	if (logActive)
	{
		spriteLog.setPosition(spriteLog.getPosition().x +(logSpeedX * dt.asSeconds()),spriteLog.getPosition().y +
(logSpeedY * dt.asSeconds()));
		// Has the log reached the right hand edge?
		if (spriteLog.getPosition().x < -100 ||spriteLog.getPosition().x > 2000)
		{
		// Set it up ready to be a whole new log next frame
		logActive = false;
		spriteLog.setPosition(810, 720);
		}
	}
	//Has the player been squished
	if(branchPositions[5] == playerSide)
	{
		//dead
		paused = true;
		acceptInput = false;
		//draw gravestone
		spriteRIP.setPosition(525,760);
		//hide Player
		spritePlayer.setPosition(2000,660);
		//Change text of message
		messageText.setString("SQUISHED!!");
		
		//Set position on screen
		FloatRect textRect = messageText.getLocalBounds();
		messageText.setOrigin(textRect.left+textRect.width /2.0f, textRect.top + textRect.height/2.0f);
		
		messageText.setPosition(1920/2.0f, 1080/2.0f);
		spriteAxe.setPosition(2000,spriteAxe.getPosition().y);
		death.play();
		}
	}
	
	// Draw Scene	
		window.clear();
		window.draw(spriteBackground);
		window.draw(spriteSun);
		window.draw(spriteCloud1);
		for (int i=0;i<NUM_BRANCHES;i++)
		{
			window.draw(branches[i]);
		}
		window.draw(spriteTree);
		window.draw(spriteTree2);
		window.draw(spriteBee);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		window.draw(spritePlayer);
		window.draw(spriteAxe);
		window.draw(spriteRIP);
		window.draw(spriteLog);
		window.draw(scoreText);
		window.draw(timeBar);
		if (paused)
		{
			window.draw(messageText);
		}
		window.display();
	
	}
	return 0;
}	
	// Function definition
	void updateBranches(int seed)
	{
	// Move all the branches down one place
	for (int j = NUM_BRANCHES-1; j > 0; j--) 
	{
		branchPositions[j] = branchPositions[j - 1];
	}
	
	//Spawn new branch at position 0
	//LEFT RIGHT OR NONE
	srand((int)time(0)+seed);
	int r = (rand() % 5);
	switch (r) 
	{
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}

