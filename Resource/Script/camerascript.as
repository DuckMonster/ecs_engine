void OnBehaviour(Entity@ entity, float delta)
{
	TransformComponent@ transform = entity.Transform;

	// Spin around
	vec3 position(
		sin(Time::Elapsed() * 0.1) * 5.0,
		6.0,
		cos(Time::Elapsed() * 0.1) * 5.0
	);
	
	transform.Position = position;
	transform.LookAt(vec3(0.0, 3.0, 0.0), vec3(0.0, 1.0, 0.0));
}