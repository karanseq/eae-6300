Actor = 
{
    name = "Enemy_01_Bullet",
    type = "Bullet",

    bounding_box = { 0.0, 0.0, 11.0, 21.0 },
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

