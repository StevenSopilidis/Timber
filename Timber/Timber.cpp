#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

using namespace sf;

void updateBranches(int seed);

const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

//where is the player or the branch
enum class side { LEFT,RIGHT,NONE};
side branchPosition[NUM_BRANCHES];


int main()
{
	// Create a video mode object
	VideoMode vm(1920, 1080);

	// Create and open a window for the game
	RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

	//create the tree sprite
	// Create a texture to hold a graphic on the GPU
	Texture textureBackground;
	// Load a graphic into the texture
	textureBackground.loadFromFile("graphics/background.png");
	// Create a sprite
	Sprite spriteBackground;
	// Attach the texture to the sprite
	spriteBackground.setTexture(textureBackground);
	// Set the spriteBackground to cover the screen
	spriteBackground.setPosition(0, 0);

	//create the tree texture
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(820, 0);

	//create the bee texture
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	//is the bee currently moving
	bool beeActive{ false };
	//How fast can the bee fly (speed in pixels per seconds)
	float beeSpeed{ 0.0f };

	//create the cload Texture
	Texture cloudTexture;
	cloudTexture.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(cloudTexture);
	spriteCloud2.setTexture(cloudTexture);
	spriteCloud3.setTexture(cloudTexture);
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 200);
	spriteCloud3.setPosition(0, 450);
	//Are the clouds currently on the sreen??
	bool cloud1Active{ false };
	bool cloud2Active{ false };
	bool cloud3Active{ false };
	//how fast is each cload moving
	float cloud1Speed{ 0.0f };
	float cloud2Speed{ 0.0f };
	float cloud3Speed{ 0.0f };

	//var to controll time (Utility class that measures the elapsed time)
	Clock clock;
	//Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400.f;
	float timeBarHeight = 80.f;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Red);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	//for keeping track of the time the play has
	Time gameTimeTotal;
	float timeRemaining = 6.0f;
	//how much the timebar needs to shrink per second
	float  timeBarWitdthPerSeconds = timeBarStartWidth / timeRemaining;

	//track wether the game is running
	bool paused = true;

	int score = 0;
	//Draw some text
	Text messageText;
	Text scoreText;
	/// load the font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");
	//set the font to the Text objects
	messageText.setFont(font);
	scoreText.setFont(font);
	//assing the message to the Text objects
	messageText.setString("Press Enter to Start");
	scoreText.setString("Score = 0");
	//set the font size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);
	//set the color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);

	//Position the text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(
		textRect.left + textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f
	);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	//Prepare the 6 branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		//set the sprites origin to dead center
		//We can spin it around without changing its position
		branches[i].setOrigin(220, 20);
	}

	//update the branches
	for (int i = 0; i < 6; i++)
	{
		updateBranches(i);
	}

	while (window.isOpen())
	{

		//	/*
		//	****************************************
		//	Handle the players input
		//	****************************************
		//	*/

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		//	//if Enter was pressed, remove the pause and start the game
		if (Keyboard::isKeyPressed(Keyboard::Enter))
		{
			paused = false;
			//reset the score and time
			timeRemaining = 6.f;
			score = 0;
		}

		//	/*
		//	****************************************
		//	Update the scene
		//	****************************************
		//	*/
		//	//clock.restarts() restaruts the clock
		//	//returns the time elapsed since the last clock restart
		//	//dt -> ammount of time that has elapsed since we last time updated the scene and
		//	//restarted the clock

		//	//if the game is not paused start the game
		if (!paused)
		{
			Time dt = clock.restart();

			//Subtract from the ammount of time remainging
			timeRemaining -= dt.asSeconds();
			//resize up the timeBar
			timeBar.setSize(
				Vector2f(timeBarWitdthPerSeconds * timeRemaining, timeBarHeight)
			);
			//if time run out
			if (timeRemaining <= 0.0f)
			{
				//pause the game
				paused = true;
				//change the message show to the player
				messageText.setString("Out of time!!!!");

				//reposition the text based on its new size
				FloatRect textRext = messageText.getLocalBounds();
				messageText.setOrigin(
					textRect.left + textRect.width / 2.0f,
					textRect.top + textRect.top / 2.0f
				);
				messageText.setPosition(
					1920 / 2.0f,
					1080 / 2.0f
				);
			}

			//set the bee
			if (!beeActive)
			{
				//how fast is the bee
				//seed the rand
				//time(0) -> current time
				//bee speed (rand int between 200-399)

				// How fast is the bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				// How high is the bee
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				//move the bee
				//dt.asSeconds() -> fraction of 1 that represents how long the previous frame of animation took in seconds
				spriteBee.setPosition(
					spriteBee.getPosition().x - beeSpeed * dt.asSeconds(),
					spriteBee.getPosition().y
				);
				//if be has reached the left-hand edge of the screen?     
				if (spriteBee.getPosition().x < -100)
				{
					// Set it up ready to be a whole new bee next frame         
					beeActive = false;
				}
					if (!cloud1Active)
					{
						srand(time(0) * 10);
						cloud1Speed = (rand() % 200);

						//			//how hight is the cloud
						srand(time(0) * 10);
						float height = (rand() % 150);
						spriteCloud1.setPosition(-200, height);
						cloud1Active = true;
					}
					else
					{
						spriteCloud1.setPosition(
							spriteCloud1.getPosition().x + (cloud1Speed * dt.asSeconds()),
							spriteCloud1.getPosition().y
						);
						//if the cloud has reached the right hand side of the screen
						if (spriteCloud1.getPosition().x > 1920)
						{
							//set it to be a new cloud next frame
							cloud1Active = false;
						}
					}
					//second cloud
					if (!cloud2Active)
					{
						srand(time(0) * 20);
						cloud2Speed = (rand() % 200);

						srand(time(0) * 20);
						float height = (rand() % 450) - 150;
						spriteCloud2.setPosition(-200, height);
						cloud2Active = true;
					}
					else {
						spriteCloud2.setPosition(
							spriteCloud2.getPosition().x + (cloud2Speed * dt.asSeconds()),
							spriteCloud2.getPosition().y
						);

						if (spriteCloud2.getPosition().x > 1920)
						{
							cloud2Active = false;
						}
					}
					//third cloud
					if (!cloud3Active)
					{
						srand(time(0) * 30);
						cloud3Speed = (rand() % 200);

						srand(time(0) * 20);
						float height = (rand() % 450) - 150;
						spriteCloud3.setPosition(-200, height);
						cloud3Active = true;
					}
					else {
						spriteCloud3.setPosition(
							spriteCloud3.getPosition().x + (cloud3Speed * dt.asSeconds()),
							spriteCloud3.getPosition().y
						);

						if (spriteCloud3.getPosition().x > 1920)
						{
							cloud3Active = false;
						}
					}
					//Update the score text
					std::stringstream ss;
					//save as Score = score ( for concating 2 or more strings)
					ss << "Score = " << score;
					scoreText.setString(ss.str());
					for (int i = 0; i < NUM_BRANCHES; i++)
					{
						float height = i * 150;
						if (branchPosition[i] == side::LEFT)
						{
							//move the sprite to the left side
							branches[i].setPosition(610, height);

							//Flip the sprite around the other way
							branches[i].setRotation(180);
						}
						else if (branchPosition[i] == side::RIGHT)
						{
							//move the sprite to the right side
							branches[i].setPosition(1330, height);
							//Set the sprite rotation to normal
							branches[i].setRotation(0);
						}
						else {
							//hide the branch
							branches[i].setPosition(3000, height);
						}
					}

			}
		}
		//	
		//	/*
		//	****************************************
		//	Draw the scene
		//	****************************************
		//	*/
			// Clear everything from the last frame
		window.clear();

		//	// Draw our game scene here
		window.draw(spriteBackground);
		//draw the clouds
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		//draw the branches
		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}
		//draw the tree
		window.draw(spriteTree);
		//draw the bee
		window.draw(spriteBee);
		//draw the score
		window.draw(scoreText);
		//draw the timebar
		window.draw(timeBar);
		//if game is paused draw the message Text
		if (paused)
		{
			window.draw(messageText);
		}

		// Show everything we just drew
		window.display();
	}
	return 0;
}


//update branches function definition
void updateBranches(int seed)
{
	//Move all the branches on place
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPosition[j] = branchPosition[j - 1];
		//Spawn a new branch at position 0
		srand((int)time(0) + seed);
		int r = (rand() % 5);
		switch (r)
		{
		case 0:
			branchPosition[0] = side::LEFT;
			break;
		case 1:
			branchPosition[0] = side::RIGHT;
			break;
		default:
			branchPosition[0] = side::NONE;
			break;
		}

	}
}