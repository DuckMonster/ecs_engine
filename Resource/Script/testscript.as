float timer = 1.0;

void OnBehaviour(Entity& entity, float delta)
{
	TransformComponent@ transform = entity.Transform;

	timer -= delta;
	transform.Position.x = sin(Time::Elapsed() * 1.5);
	transform.Position.y = 1.0;
	transform.Position.z = cos(Time::Elapsed() * 1.5);
}