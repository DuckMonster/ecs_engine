float timer = 1.0;

void OnBehaviour(Entity@ entity, float delta)
{
	TransformComponent@ transform = entity.Transform;
	transform.Position = vec3(
		sin(Time::Elapsed()) * 5.0,
		5.0,
		cos(Time::Elapsed()) * 5.0
	);
	transform.LookAt(vec3(), vec3(0.0, 1.0, 0.0));

	timer -= delta;
}