Actor =
{
	name = "Blinky",
	type = "Enemy",

	bounding_box = { 0.0, 0.0, 40.0, 35.0 },
	transform = 
	{
		position = { 0.0, 0.0, 0.0 },
	},

	physics_settings = 
	{
		type = "kinematic",
		mass = 50.0,
		drag = 0.1,
		collide = true,
        collision_filter = 1
	},

	render_settings = 
	{
		sprite_name = "Data\\Sprites\\Enemy_02.dds"
	}
}