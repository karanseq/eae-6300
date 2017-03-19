ActorList =
{
	{
		name = "Ramp",
		type = "Monster",

		bounding_box = { 0.0, 0.0, 192.0, 82.0 },
		transform = 
		{
			position = { 0.0, 0.0, 0.0 },
			rotation = { 0.0, 0.0, 30.0 },
			scale = { 1.0, 1.0, 1.0 }
		},
	

		physics_settings = 
		{
			mass = 50.0,
			drag = 0.02,
			collide = true
		},

		render_settings = 
		{
			sprite_name = "Data\\cjtrapm01.dds"
		}
	},
	{
		name = "Inky",
		type = "Monster",

		bounding_box = { 0.0, 0.0, 22.0, 20.0 },
		transform = 
		{
			position = { -200.0, -200.0, 0.0 },
		},
	

		physics_settings = 
		{
			mass = 50.0,
			drag = 0.02,
			collide = true
		},

		render_settings = 
		{
			sprite_name = "Data\\enemy_01.dds"
		}
	},
	{
		name = "Binky",
		type = "Monster",

		bounding_box = { 0.0, 0.0, 22.0, 20.0 },
		transform = 
		{
			position = { -200.0, 200.0, 0.0 },
		},
	

		physics_settings = 
		{
			mass = 50.0,
			drag = 0.02,
			collide = true
		},

		render_settings = 
		{
			sprite_name = "Data\\enemy_01.dds"
		}
	},
	{
		name = "Pinky",
		type = "Monster",

		bounding_box = { 0.0, 0.0, 22.0, 20.0 },
		transform = 
		{
			position = { 200.0, -200.0, 0.0 },
		},
	

		physics_settings = 
		{
			mass = 50.0,
			drag = 0.02,
			collide = true
		},

		render_settings = 
		{
			sprite_name = "Data\\enemy_01.dds"
		}
	},
	{
		name = "Clyde",
		type = "Monster",

		bounding_box = { 0.0, 0.0, 22.0, 20.0 },
		transform = 
		{
			position = { 200.0, 200.0, 0.0 },
		},
	

		physics_settings = 
		{
			mass = 50.0,
			drag = 0.02,
			collide = true
		},

		render_settings = 
		{
			sprite_name = "Data\\enemy_01.dds"
		}
	}
}