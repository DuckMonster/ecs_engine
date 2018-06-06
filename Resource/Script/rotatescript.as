void OnBehaviour(Entity@ entity, float delta)
{
	TransformComponent@ transform = entity.Transform;
	vec3 pos = transform.Position;
	transform.LookAt(vec3(sin(Time::Elapsed() * 0.2), pos.y, cos(Time::Elapsed() * 0.2)), vec3(0.0, 1.0, 0.0));
}