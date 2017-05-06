Actor = 
{
    name = "Enemy_03_Bullet",
    type = "Bullet",

    bounding_box = { 0.0, 0.0, 9.0, 14.0 },
    transform = 
    {
        position = { 0.0, 0.0, 0.0 },
    },

    physics_settings = 
    {
        type = "dynamic",
        mass = 10.0,
        drag = 0.0,
        collide = true,
        collision_filter = 0
    },

    render_settings = 
    {
        sprite_name = "Data\\Sprites\\Bullet.dds"
    }
}

