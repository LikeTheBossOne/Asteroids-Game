#include "Game.h"
#include "PatternPlatform.h"
#include "Settings.h"
#include "StaticPlatform.h"
#include "Pattern.h"
#include "EntityManager.h"
#include <thread>
#include "GameTime.h"
#include "Player.h"
#include "TransformComponent.h"
#include "TextureComponent.h"
#include "ColorComponent.h"
#include "SpawnZone.h"
#include <zmq.hpp>
#include "DeathZone.h"
#include "SideBoundaryComponent.h"
#include "SideBoundary.h"
#include "EventManager.h"
#include "InputManager.h"
#include "Replay.h"
#include "InputEvent.h"
#include "ReplaySystem.h"
#include "StopReplayEvent.h"
#include "ScriptManager.h"
#include <iostream>
#include "dukglue/dukglue.h"
#include "CreateAsteroidsEvent.h"

const int Game::STEP_SIZE = 2;

Game::Game()
{
	_replaySystem = new ReplaySystem();
	_entityManager = new EntityManager(this);
	InputManager::getInstance();

	
	_gameTime = new GameTime(STEP_SIZE);
	
	EventManager::getInstance()->init(_gameTime);
	ScriptManager::getInstance(); // init scriptManager
	
	_totalPlayerCount = 0;
}

Game::~Game() = default;

void Game::run(zmq::socket_t& publisher)
{
	// SETUP
	auto spawn = new SpawnZone(_entityManager->getPhysicsEngineSettings(), 300, 300);
	_entityManager->addSpawn(spawn);

	EventManager::getInstance()->raiseEvent(new CreateAsteroidsEvent(4));

	
	int previousTime = _gameTime->getTime();
	int deltaTime = 0;

	// Recording

	bool running = true;
	while (running)
	{
		deltaTime = _gameTime->getTime() - previousTime;
		
		int governor = _replaySystem->isReplaying()
			? std::floor(1000 / (FRAMERATE * STEP_SIZE * _replaySystem->getReplaySpeed()))
			: std::floor(1000 / (FRAMERATE * STEP_SIZE));
		while (deltaTime < governor) // 8 is for ~ 1000 / (60 * 2)
		{
			deltaTime = _gameTime->getTime() - previousTime;
		}
		deltaTime = _gameTime->getTime() - previousTime;
		std::cout << deltaTime << std::endl;
		previousTime = _gameTime->getTime();

		if (_replaySystem->isRecording())
		{
			auto inputs = InputManager::getInstance()->getAllInputs();
			_replaySystem->getLastReplay()->addReplayTick(ReplayTick(inputs, deltaTime));
		}
		if (_replaySystem->isReplaying())
		{
			// Check if replay complete
			if (_replaySystem->getLastReplay()->isEmpty())
			{
				EventManager::getInstance()->raiseEvent(new StopReplayEvent());
			}
			else
			{
				auto tick = _replaySystem->getLastReplay()->popReplayTick();

				deltaTime = tick.getTimestamp();
				auto inputs = tick.getInputs();
				for (auto pair : inputs)
				{
					if (pair.second != InputManager::getInstance()->getInputs(pair.first))
					{
						auto inputEvent = new InputEvent(pair.first, pair.second);
						EventManager::getInstance()->raiseEvent(inputEvent);
					}
				}
			}
		}

		// Tick
		_entityManager->update(deltaTime);
		EventManager::getInstance()->dispatchEvents();

		
		// Publish to client
		std::map<int, GameObject*> entities = _entityManager->getEntities();
		std::string entitiesString;
		for (std::pair<int, GameObject*> pPair : entities)
		{
			auto entity = pPair.second;

			if (entity->getType() == "SpawnZone" || entity->getType() == "DeathZone") continue;
			
			entitiesString += std::to_string(entity->getGUID()) + ",";
			
			entitiesString += entity->getType() + ",";

			if (entity->getType() == "SideBoundary")
			{
				entitiesString += std::to_string(entity->getTransform()->getPositionX()) + ",";
				entitiesString += std::to_string(dynamic_cast<SideBoundaryComponent*>(entity->getComponent(
					ComponentTypes::SideBoundaryComponent))->getSceneShiftWidth());
			}
			else
			{
				entitiesString += std::to_string(entity->getTransform()->getPositionX()) + ",";
				entitiesString += std::to_string(entity->getTransform()->getPositionY()) + ",";
				entitiesString += std::to_string(entity->getTransform()->getWidth()) + ",";
				entitiesString += std::to_string(entity->getTransform()->getHeight()) + ",";
				entitiesString += std::to_string(entity->getTransform()->getRotation()) + ",";

				const auto gTexture = entity->getComponent(ComponentTypes::TextureComponent);
				const auto gColor = entity->getComponent(ComponentTypes::ColorComponent);
				if (gTexture != nullptr)
				{
					auto texture = dynamic_cast<TextureComponent*>(gTexture);
					entitiesString.append("Texture,");
					entitiesString += texture->getTextureName() + ",";
					entitiesString.append(std::to_string(texture->getAnimationNumber()));
				}
				else if (gColor != nullptr)
				{
					auto color = dynamic_cast<ColorComponent*>(gColor);
					entitiesString.append("Color,");
					entitiesString += std::to_string(color->getR()) + ",";
					entitiesString += std::to_string(color->getG()) + ",";
					entitiesString += std::to_string(color->getB());
				}
				else
				{
					entitiesString.append("None");
				}
			}

			if (entity->getType() == "PlayerShip")
			{
				auto inputs = InputManager::getInstance()->getInputs(entity->getGUID());
				entitiesString += "," + std::to_string(inputs[0]) + ",";
				entitiesString += std::to_string(inputs[1]) + ",";
				entitiesString += std::to_string(inputs[2]);
			}

			entitiesString += "\n";
		}
		
		zmq::message_t entitiesMessage(entitiesString.data(), entitiesString.size());
		publisher.send(entitiesMessage, zmq::send_flags::none);
	}
}

