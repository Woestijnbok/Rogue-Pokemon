#pragma once

template<typename... Arguments>
class State
{
public:
	/*
	* @brief Does a check returns a possible non null state.
	* When the state is not null the state machine needs to switch states.
	* 
	* @return Returns possible new state.
	*/
	virtual State* Check(Arguments... arguments);
	virtual void Update();
	virtual void Enter();
	virtual void Exit();

protected:
	explicit State() = default;
	virtual ~State() = default;

	State(const State&) = delete;
	State(State&&) noexcept = delete;
	State& operator= (const State&) = delete;
	State& operator= (const State&&) noexcept = delete;

private:

};

template<typename... Arguments>
State<Arguments...>* State<Arguments...>::Check(Arguments... /*arguments*/)
{
	return nullptr;
}

template<typename... Arguments>
void State<Arguments...>::Update()
{

}

template<typename ...Arguments>
void State<Arguments...>::Enter()
{

}

template<typename ...Arguments>
void State<Arguments...>::Exit()
{

}