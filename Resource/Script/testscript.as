void CameraFunction(int a)
{
	Print("Hello From AngelScript! CameraFunction was called with a = " + a);
}

void OnBehaviour(Entity& entity, float delta)
{
	Print("OnBehaviour for entity '" + entity.GetName() + "'");
	Print("TotalElapsed is ( " + Time::Elapsed() + " )");
}