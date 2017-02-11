Actor = 
{
	name = "Joe",
	class = "Player",

	bounding_box = { 0.0, 0.0, 32.0, 40.0 },
	transform = 
	{
		position = { 1.0, 2.0, 3.0 },
		rotation = { 0.0, 0.0, 0.0 },
		scale = { 1.0, 1.0, 1.0 },
	},
	

	physics_settings = 
	{
		mass = 10.0,
		drag = 0.02
	},

	render_settings = 
	{
		sprite = { 
			offset	= { 0.0, 0.0 },
					-- left, top, right, bottom
			extents	= { -1.0, -1.0, 1.0, 1.0 }
		},
		sprite_color = { 255, 255, 255, 255 }
	}
}