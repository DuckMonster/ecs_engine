float timer = 1.0;

void OnBehaviour(Entity& entity, float delta)
{
	timer -= delta;

	if (timer < 0.0)
	{
		Print(entity.GetName() + " says hello!");
		timer = 1.0;
	}

	//Print("OnBehaviour for entity '" + entity.GetName() + "'");
	//Print("TotalElapsed is ( " + Time::Elapsed() + " )");
	//Print("Hello!");
}