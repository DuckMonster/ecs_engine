float timer = 1.0;

void OnBehaviour(Entity& entity, float delta)
{
	TransformComponent@ transform = entity.Transform;

	timer -= delta;
	transform.Position.x = 0.0;

	vec3 deltaVec;
	deltaVec.x = 0.5;
	deltaVec.y = 0.1;
	deltaVec.z = 2.0;
	transform.Position = deltaVec;

	if (timer < 0.0)
	{
		Print(entity.GetName() + " says hello!");

		Print("Transform.Position = " + transform.Position.ToString());
		timer = 1.0;
	}

	//Print("OnBehaviour for entity '" + entity.GetName() + "'");
	//Print("TotalElapsed is ( " + Time::Elapsed() + " )");
	//Print("Hello!");
}