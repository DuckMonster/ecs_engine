#pragma once
class World;

class Program
{
public:
	Program();
	~Program();

	void DoFrame();

private:
	World* m_World = nullptr;
};