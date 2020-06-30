#include "pch.h"

#include "GameStateManager.h"

#include <cassert>

#include "GameState.h"
#include "Utilities.h"



GameStateManager::GameStateManager()
	: m_stateFactories()
	, m_states()
	, m_popCount(0)
	, m_nextStateName()
{
}



GameStateManager::~GameStateManager()
{
}


void GameStateManager::SetStartState(const std::string& stateName)
{
	RequestState(stateName);
}



void GameStateManager::Update(float elapsedTime)
{
	if (m_popCount > 0)
	{
		for (int i = 0; i < m_popCount; i++)
		{
			m_states.back()->Finalize();
			m_states.pop_back();
		}
		m_popCount = 0;
	}


	if (!m_nextStateName.empty())
	{
		ChangeState();
	}


	assert(m_states.size() > 0 && "There is no active state.");
	m_states.back()->Update(elapsedTime);
}



void GameStateManager::Render()
{
	assert(m_states.size() > 0 && "There is no active state.");

	for (const GameStatePtr& state : m_states)
	{
		state->Render();
	}
}



void GameStateManager::RequestState(const std::string& stateName)
{
	assert(m_stateFactories.count(stateName) == 1 && "A GameState with this name is not registered.");

	m_popCount = m_states.size();
	m_nextStateName = stateName;
}



void GameStateManager::PushState(const std::string& stateName)
{
	m_nextStateName = stateName;
}



void GameStateManager::PopState(int count)
{
	assert(count > 0 && "Count is invalid.");

	if (!m_nextStateName.empty())
	{
		m_nextStateName.clear();
		count--;
	}

	m_popCount = Clamp<int>(m_popCount + count, 0, m_states.size()-1);
}



void GameStateManager::ChangeState()
{
	m_states.push_back(m_stateFactories[m_nextStateName]());
	m_states.back()->Initialize();
	
	m_nextStateName.clear();
}
