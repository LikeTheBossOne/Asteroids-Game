#include "Game.h"
#include "Settings.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "GameTime.h"
#include "EntityManager.h"
#include "ResourceManager.h"
#include <thread>
#include <zmq.hpp>
#include "InputManager.h"

Game::Game(int clientNumber, int playerNumber)
{
	_shouldStartYet = false;
	_shouldEnd = false;

	_isReplaying = false;
	_isRecording = false;
	
	_clientNumber = clientNumber;
	_playerNumber = playerNumber;
	
	_entityManager = new EntityManager(this);
	_resourceManager = new ResourceManager(this);
	InputManager::getInstance();

	// Load Textures
	auto shipTexture = new sf::Texture();
	if (!shipTexture->loadFromFile("assets/images/ship.png"))
	{
		std::cout << "Failed to load ship texture" << std::endl;
		exit(EXIT_FAILURE);
	}
	auto asteroidTexture = new sf::Texture();
	if (!asteroidTexture->loadFromFile("assets/images/asteroid.png"))
	{
		std::cout << "Failed to load asteroid texture" << std::endl;
		exit(EXIT_FAILURE);
	}

	getResourceManager()->addTexture("ship", shipTexture);
	getResourceManager()->addTexture("asteroid", asteroidTexture);

	// Ship animations
	auto shipAnims = std::vector<sf::IntRect>();
	shipAnims.emplace_back(0, 0, 32, 32);
	getResourceManager()->addTextureAnimations("ship", shipAnims);
	// Asteroid animations
	auto asteroidAnims = std::vector<sf::IntRect>();
	asteroidAnims.emplace_back(0, 0, 150, 125);
	getResourceManager()->addTextureAnimations("asteroid", asteroidAnims);
	
}

Game::~Game() = default;

void Game::run(zmq::socket_t& socket, std::mutex& myPlayerLock, std::mutex& playersLock, std::mutex& entitiesLock)
{
	// Create Window
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Homework 1", sf::Style::Default, settings);

	
	// Timeline values
	int replaySpeedTime = 0;

	// Start Real Timeline
	int STEP_SIZE = 2;
	GameTime timeline(STEP_SIZE);
	int pauseResumeTime = 0;

	// Start Game Timeline
	GameTime gameTime(STEP_SIZE);
	int previousTime = gameTime.getTime();
	int deltaTime = 0;
	
	while (window.isOpen())
	{
		replaySpeedTime++;


		// HANDLE INPUT
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::LostFocus) {
				gameTime.pause();
			}
			else if (event.type == sf::Event::GainedFocus) {
				gameTime.resume();
			}
		}

		// In Game
		// Decide whether to draw or wait
		deltaTime = timeline.getTime() - previousTime;
		int governor = std::floor(1000 / (FRAMERATE * STEP_SIZE));
		while (deltaTime < governor) // 8 is for ~ 1000 / (60 * 2)
		{
			// std::this_thread::sleep_for(std::chrono::milliseconds(governor - deltaTime));
			deltaTime = timeline.getTime() - previousTime;
		}
		previousTime = timeline.getTime();
		// std::cout << 1000 / (deltaTime * STEP_SIZE * timeline.getScale()) << std::endl;


		
		// Handle ReplaySpeed
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)
			&& replaySpeedTime > 10)
		{
			std::string inputString = "REP_SPEED_DOWN";

			zmq::message_t inputMessage(inputString.data(), inputString.size());

			socket.send(inputMessage, zmq::send_flags::none);

			zmq::message_t response;
			socket.recv(response);

			continue;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)
			&& replaySpeedTime > 10)
		{
			std::string inputString = "REP_SPEED_UP";

			zmq::message_t inputMessage(inputString.data(), inputString.size());

			socket.send(inputMessage, zmq::send_flags::none);

			zmq::message_t response;
			socket.recv(response);

			continue;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)
			&& timeline.getTime() - pauseResumeTime > 500)
		{
			pauseResumeTime = timeline.getTime();
			if (gameTime.getPaused())
			{
				gameTime.resume();
			}
			else
			{
				gameTime.pause();
			}
		}
		// Send replay pressed
		if (!_isRecording && sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
		{
			std::string inputString = "START_RECORDING";

			zmq::message_t inputMessage(inputString.data(), inputString.size());

			socket.send(inputMessage, zmq::send_flags::none);

			zmq::message_t response;
			socket.recv(response);
			
			_isRecording = true;
			continue;
		}
		else if (_isRecording && sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
		{
			std::string inputString = "STOP_RECORDING";

			zmq::message_t inputMessage(inputString.data(), inputString.size());

			socket.send(inputMessage, zmq::send_flags::none);

			zmq::message_t response;
			socket.recv(response);
			
			_isRecording = false;
			_isReplaying = true;
			continue;
		}
		
		// Send input
		std::string inputString = std::to_string(_playerNumber) + "\n";
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && !gameTime.getPaused())
		{
			inputString.append("true");
		}
		else
		{
			inputString.append("false");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !gameTime.getPaused())
		{
			inputString.append(",true");
		}
		else
		{
			inputString.append(",false");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !gameTime.getPaused())
		{
			inputString.append(",true");
		}
		else
		{
			inputString.append(",false");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && !gameTime.getPaused())
		{
			inputString.append(",true");
		}
		else
		{
			inputString.append(",false");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !gameTime.getPaused())
		{
			inputString.append(",true");
		}
		else
		{
			inputString.append(",false");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !gameTime.getPaused())
		{
			inputString.append(",true");
		}
		else
		{
			inputString.append(",false");
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !gameTime.getPaused())
		{
			inputString.append(",true");
		}
		else
		{
			inputString.append(",false");
		}

		// std::cout << sf::Mouse::getPosition(window).x << "," << sf::Mouse::getPosition(window).y << std::endl;
		
		zmq::message_t inputMessage(inputString.data(), inputString.size());

		socket.send(inputMessage, zmq::send_flags::none);

		zmq::message_t response;
		socket.recv(response);
		auto responseString = std::string(static_cast<char*>(response.data()), response.size());

		// Update & Draw
		if (_shouldStartYet)
		{
			_entityManager->update(myPlayerLock);
			window.clear();
			_entityManager->draw(window, myPlayerLock, playersLock, entitiesLock);
			if (gameTime.getPaused())
			{
				sf::Text text;
				text.setPosition(0, 0);
				sf::Font font;
				font.loadFromFile("assets/fonts/OpenSans-Bold.ttf");
				text.setFont(font);
				text.setString("PAUSE");
				text.setCharacterSize(30);
				text.setFillColor(sf::Color::Red);
				text.setOutlineColor(sf::Color::Red);
				window.draw(text);
			}
			window.display();
		}
	}
	_shouldEnd = true;
	std::string inputString = std::to_string(this->getPlayerNumber()) + "\nCLOSE";
	zmq::message_t inputMessage(inputString.data(), inputString.size());

	socket.send(inputMessage, zmq::send_flags::none);

	zmq::message_t response;
	socket.recv(response);
	auto responseString = std::string(static_cast<char*>(response.data()), response.size());
	std::cout << responseString << std::endl;
	socket.close();
}
