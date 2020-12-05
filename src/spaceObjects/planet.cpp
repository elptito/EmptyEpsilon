#include "planet.h"
#include <SFML/OpenGL.hpp>
#include "main.h"
#include "pathPlanner.h"

#include "scriptInterface.h"

static Mesh* planet_mesh[16];

class PlanetMeshGenerator
{
public:
    std::vector<MeshVertex> vertices;
    int max_iterations;

    PlanetMeshGenerator(int iterations)
    {
        max_iterations = iterations;

        createFace(0, sf::Vector3f(0, 0, 1), sf::Vector3f(0, 1, 0), sf::Vector3f(1, 0, 0), sf::Vector2f(0, 0), sf::Vector2f(0, 0.5), sf::Vector2f(0.25, 0.5));
        createFace(0, sf::Vector3f(0, 0, 1), sf::Vector3f(1, 0, 0), sf::Vector3f(0,-1, 0), sf::Vector2f(0.25, 0), sf::Vector2f(0.25, 0.5), sf::Vector2f(0.5, 0.5));
        createFace(0, sf::Vector3f(0, 0, 1), sf::Vector3f(0,-1, 0), sf::Vector3f(-1, 0, 0), sf::Vector2f(0.5, 0), sf::Vector2f(0.5, 0.5), sf::Vector2f(0.75, 0.5));
        createFace(0, sf::Vector3f(0, 0, 1), sf::Vector3f(-1, 0, 0), sf::Vector3f(0, 1, 0), sf::Vector2f(0.75, 0), sf::Vector2f(0.75, 0.5), sf::Vector2f(1.0, 0.5));

        createFace(0, sf::Vector3f(0, 0,-1), sf::Vector3f(1, 0, 0), sf::Vector3f(0, 1, 0), sf::Vector2f(0, 1.0), sf::Vector2f(0.25, 0.5), sf::Vector2f(0.0, 0.5));
        createFace(0, sf::Vector3f(0, 0,-1), sf::Vector3f(0,-1, 0), sf::Vector3f(1, 0, 0), sf::Vector2f(0.25, 1.0), sf::Vector2f(0.5, 0.5), sf::Vector2f(0.25, 0.5));
        createFace(0, sf::Vector3f(0, 0,-1), sf::Vector3f(-1, 0, 0), sf::Vector3f(0,-1, 0), sf::Vector2f(0.5, 1.0), sf::Vector2f(0.75, 0.5), sf::Vector2f(0.5, 0.5));
        createFace(0, sf::Vector3f(0, 0,-1), sf::Vector3f(0,1, 0), sf::Vector3f(-1, 0, 0), sf::Vector2f(0.75, 1.0), sf::Vector2f(1.0, 0.5), sf::Vector2f(0.75, 0.5));

        for(unsigned int n=0; n<vertices.size(); n++)
        {
            float u = sf::vector2ToAngle(sf::Vector2f(vertices[n].position[1], vertices[n].position[0])) / 360.0f;
            if (u < 0.0f)
                u = 1.0 + u;
            if (std::abs(u - vertices[n].uv[0]) > 0.5)
                u += 1.0f;
            vertices[n].uv[0] = u;
            vertices[n].uv[1] = 0.5 + sf::vector2ToAngle(sf::Vector2f(sf::length(sf::Vector2f(vertices[n].position[0], vertices[n].position[1])), vertices[n].position[2])) / 180.0f;
        }
    }

    void createFace(int iteration, sf::Vector3f v0, sf::Vector3f v1, sf::Vector3f v2, sf::Vector2f uv0, sf::Vector2f uv1, sf::Vector2f uv2)
    {
        if (iteration < max_iterations)
        {
            sf::Vector3f v01 = v0 + v1;
            sf::Vector3f v12 = v1 + v2;
            sf::Vector3f v02 = v0 + v2;
            sf::Vector2f uv01 = (uv0 + uv1) / 2.0f;
            sf::Vector2f uv12 = (uv1 + uv2) / 2.0f;
            sf::Vector2f uv02 = (uv0 + uv2) / 2.0f;
            v01 /= sf::length(v01);
            v12 /= sf::length(v12);
            v02 /= sf::length(v02);
            createFace(iteration + 1, v0, v01, v02, uv0, uv01, uv02);
            createFace(iteration + 1, v01, v1, v12, uv01, uv1, uv12);
            createFace(iteration + 1, v01, v12, v02, uv01, uv12, uv02);
            createFace(iteration + 1, v2, v02, v12, uv2, uv02, uv12);
        }else{
            vertices.emplace_back();
            vertices.back().position[0] = v0.x;
            vertices.back().position[1] = v0.y;
            vertices.back().position[2] = v0.z;
            vertices.back().normal[0] = v0.x;
            vertices.back().normal[1] = v0.y;
            vertices.back().normal[2] = v0.z;
            vertices.back().uv[0] = uv0.x;
            vertices.back().uv[1] = uv0.y;

            vertices.emplace_back();
            vertices.back().position[0] = v1.x;
            vertices.back().position[1] = v1.y;
            vertices.back().position[2] = v1.z;
            vertices.back().normal[0] = v1.x;
            vertices.back().normal[1] = v1.y;
            vertices.back().normal[2] = v1.z;
            vertices.back().uv[0] = uv1.x;
            vertices.back().uv[1] = uv1.y;

            vertices.emplace_back();
            vertices.back().position[0] = v2.x;
            vertices.back().position[1] = v2.y;
            vertices.back().position[2] = v2.z;
            vertices.back().normal[0] = v2.x;
            vertices.back().normal[1] = v2.y;
            vertices.back().normal[2] = v2.z;
            vertices.back().uv[0] = uv2.x;
            vertices.back().uv[1] = uv2.y;
        }
    }
};

/// A planet.
REGISTER_SCRIPT_SUBCLASS(Planet, SpaceObject)
{
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setPlanetAtmosphereColor);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setPlanetAtmosphereTexture);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setPlanetSurfaceTexture);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setPlanetIcon);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, getPlanetSurfaceTexture);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setPlanetCloudTexture);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, getPlanetRadius);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setPlanetRadius);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, getCollisionSize);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setPlanetCloudRadius);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setPlanetAtmosphereRadius);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setDistanceFromMovementPlane);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setAxialRotationTime);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setAxialRotation);
    REGISTER_SCRIPT_CLASS_FUNCTION(Planet, setOrbit);
}

REGISTER_MULTIPLAYER_CLASS(Planet, "Planet");
Planet::Planet()
: SpaceObject(5000, "Planet")
{
    planet_size = 5000;
    cloud_size = 5200;
    planet_texture = "planets/2k_mercury.jpg";
    planet_icon = "planets/icons/Storm-Planet.png";
    cloud_texture = "";
    atmosphere_texture = "";
    atmosphere_size = 0;
    atmosphere_color = sf::Color(0, 0, 0);
    atmosphere_size = 0;
    distance_from_movement_plane = 0;
    axial_rotation_time = random(100.0,400.0);
    rotation_axis = random(0.0,360.0);
    orbit_target_id = -1;
    orbit_time = 0.0f;
    orbit_distance = 0.0f;

    callsign = "PL" + string(getMultiplayerId());
    setScanningParameters(3,1);

    collision_size = -2.0f;

    setRadarSignatureInfo(0.5, 0, 0.3);

    addInfos(0,"Rotation",string(irandom(5,45))+ " H.LO");
	addInfos(1,"Revolution",string(irandom(50,5000))+ " J.LO");
	addInfos(2,"Axe de rotation",string(irandom(1,360))+"deg" + string(irandom(1,60))+"'" + string(irandom(1,60))+"''");
	addInfos(3,"Taille",string(irandom(50,500) * 100) + " km");
	if (random(0.0,1.0) < 0.1)
        addInfos(4,"Type","Gazeuse");
    else
        addInfos(4,"Type","Tellurique");
	addInfos(5,"Age",string(irandom(5,100)*100)+ " M3 A.LO");

	if (infos_value[4] == "Tellurique" && random(0.0,1.0) < 0.3)
        addInfos(6,"Atmosphere","oui");
    else
        addInfos(6,"Atmosphere","non");
	addInfos(7,"Pression",string(random(0.1,4),1)+" Pa");
	addInfos(8,"Gravite",string(random(2.0,20.0),3)+" m/s2");
	addInfos(9,"ressource principale","");

    registerMemberReplication(&planet_size);
    registerMemberReplication(&cloud_size);
    registerMemberReplication(&atmosphere_size);
    registerMemberReplication(&planet_texture);
    registerMemberReplication(&planet_icon);
    registerMemberReplication(&cloud_texture);
    registerMemberReplication(&atmosphere_texture);
    registerMemberReplication(&atmosphere_color);
    registerMemberReplication(&atmosphere_size);
    registerMemberReplication(&distance_from_movement_plane);
    registerMemberReplication(&axial_rotation_time);
    registerMemberReplication(&rotation_axis);
    registerMemberReplication(&orbit_target_id);
    registerMemberReplication(&orbit_time);
    registerMemberReplication(&orbit_distance);
}

void Planet::setPlanetAtmosphereColor(float r, float g, float b)
{
    atmosphere_color.r = r * 255;
    atmosphere_color.g = g * 255;
    atmosphere_color.b = b * 255;
}

void Planet::setPlanetAtmosphereTexture(string texture_name)
{
    atmosphere_texture = texture_name;
}

void Planet::setPlanetSurfaceTexture(string texture_name)
{
    planet_texture = texture_name;
}

string Planet::getPlanetSurfaceTexture()
{
    return planet_texture;
}

void Planet::setPlanetIcon(string texture_name)
{
    planet_icon = texture_name;
}

void Planet::setPlanetCloudTexture(string texture_name)
{
    cloud_texture = texture_name;
}

float Planet::getPlanetRadius()
{
    return planet_size;
}

float Planet::getCollisionSize()
{
    return collision_size;
}

void Planet::setPlanetRadius(float size)
{
    setRadius(size);
    this->planet_size = size;
    this->cloud_size = size * 1.05;
    this->atmosphere_size = size * 1.2;
}

void Planet::setPlanetCloudRadius(float size)
{
    cloud_size = size;
}

void Planet::setPlanetAtmosphereRadius(float size)
{
    atmosphere_size = size;
}

void Planet::setDistanceFromMovementPlane(float distance_from_movement_plane)
{
    this->distance_from_movement_plane = distance_from_movement_plane;
}

void Planet::setAxialRotationTime(float time)
{
    axial_rotation_time = time;
}

void Planet::setAxialRotation(float axis)
{
    rotation_axis = axis;
}

void Planet::setOrbit(P<SpaceObject> target, float orbit_time)
{
    if (!target)
        return;
    this->orbit_target_id = target->getMultiplayerId();
    this->orbit_distance = sf::length(getPosition() - target->getPosition());
    this->orbit_time = orbit_time;
}

void Planet::update(float delta)
{
    if (collision_size == -2.0f)
    {
        updateCollisionSize();
        if (collision_size > 0.0)
            PathPlannerManager::getInstance()->addAvoidObject(this, collision_size);
    }

    if (orbit_distance > 0.0f)
    {
        P<SpaceObject> orbit_target;
        if (game_server)
            orbit_target = game_server->getObjectById(orbit_target_id);
        else
            orbit_target = game_client->getObjectById(orbit_target_id);
        if (orbit_target)
        {
            float angle = sf::vector2ToAngle(getPosition() - orbit_target->getPosition());
            angle += delta / orbit_time * 360.0f;
            setPosition(orbit_target->getPosition() + sf::vector2FromAngle(angle) * orbit_distance);
        }
    }

    if (axial_rotation_time != 0.0f)
        setRotation(getRotation() + delta / axial_rotation_time * 360.0f);
}

#if FEATURE_3D_RENDERING
void Planet::draw3D()
{
    float distance = sf::length(camera_position - sf::Vector3f(getPosition().x, getPosition().y, distance_from_movement_plane));

    //view_scale ~= about the size the planet is on the screen.
//    float view_scale = planet_size / distance;
    float view_scale = getRadius() / distance;
    int level_of_detail = 4;
    if (view_scale < 0.01)
        level_of_detail = 2;
    if (view_scale < 0.1)
        level_of_detail = 3;
//    level_of_detail = 4;

//    if (planet_texture != "" && planet_size > 0)
    if (planet_texture != "" && getRadius() > 0)
    {
        glTranslatef(0, 0, distance_from_movement_plane);
        glRotatef(rotation_axis, 0.0, 1.0, 0.0);
//        glScalef(planet_size, planet_size, planet_size);
        glScalef(getRadius(), getRadius(), getRadius());
        glColor3f(1, 1, 1);

        if (!planet_mesh[level_of_detail])
        {
            PlanetMeshGenerator planet_mesh_generator(level_of_detail);
            planet_mesh[level_of_detail] = new Mesh(planet_mesh_generator.vertices);
        }
        sf::Shader* shader = ShaderManager::getShader("planetShader");
        shader->setUniform("baseMap", *textureManager.getTexture(planet_texture));
        shader->setUniform("atmosphereColor", (sf::Glsl::Vec4)atmosphere_color);
        sf::Shader::bind(shader);
        planet_mesh[level_of_detail]->render();
    }
}

void Planet::draw3DTransparent()
{
    float distance = sf::length(camera_position - sf::Vector3f(getPosition().x, getPosition().y, distance_from_movement_plane));

    //view_scale ~= about the size the planet is on the screen.
//    float view_scale = planet_size / distance;
    float view_scale = getRadius() / distance;
    int level_of_detail = 4;
    if (view_scale < 0.01)
        level_of_detail = 2;
    if (view_scale < 0.1)
        level_of_detail = 3;
    level_of_detail = 4;

    glTranslatef(0, 0, distance_from_movement_plane);
    if (cloud_texture != "" && cloud_size > 0)
    {
        glPushMatrix();
        glScalef(cloud_size, cloud_size, cloud_size);
        glRotatef(engine->getElapsedTime() * 1.0f, 0, 0, 1);
        glColor3f(1, 1, 1);

        if (!planet_mesh[level_of_detail])
        {
            PlanetMeshGenerator planet_mesh_generator(level_of_detail);
            planet_mesh[level_of_detail] = new Mesh(planet_mesh_generator.vertices);
        }
        sf::Shader* shader = ShaderManager::getShader("planetShader");
        shader->setUniform("baseMap", *textureManager.getTexture(cloud_texture));
        shader->setUniform("atmosphereColor", (sf::Glsl::Vec4)sf::Color(0,0,0));
        sf::Shader::bind(shader);
        planet_mesh[level_of_detail]->render();
        glPopMatrix();
    }
    if (atmosphere_texture != "" && atmosphere_size > 0)
    {
        ShaderManager::getShader("billboardShader")->setUniform("textureMap", *textureManager.getTexture(atmosphere_texture));
        sf::Shader::bind(ShaderManager::getShader("billboardShader"));
        glColor4f(atmosphere_color.r / 255.0f, atmosphere_color.g / 255.0f, atmosphere_color.b / 255.0f, atmosphere_size * 2.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);
        glTexCoord2f(1, 0);
        glVertex3f(0, 0, 0);
        glTexCoord2f(1, 1);
        glVertex3f(0, 0, 0);
        glTexCoord2f(0, 1);
        glVertex3f(0, 0, 0);
        glEnd();
    }
}
#endif

void Planet::drawOnRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, float rotation, bool long_range)
{
    if (long_range){
        sf::Sprite object_sprite;
        textureManager.setTexture(object_sprite, planet_icon);
        object_sprite.setRotation(getRotation());
        object_sprite.setPosition(position);
        if (collision_size < 0)
        {
            float size = getRadius() * scale * 0.5 / object_sprite.getTextureRect().width * 3.0;
            object_sprite.setScale(size, size);
        }
        else
        {
            float size = getRadius() * scale / object_sprite.getTextureRect().width * 3.0;
            object_sprite.setScale(size, size);
        }
        object_sprite.setColor(sf::Color(255, 255, 255));
        window.draw(object_sprite, sf::BlendAdd);
    } else if (collision_size > 0)
    {
        sf::Sprite object_sprite;
        textureManager.setTexture(object_sprite, planet_icon);
        object_sprite.setRotation(getRotation());
        object_sprite.setPosition(position);
//        float size = getRadius() * collision_size * scale / object_sprite.getTextureRect().width * 3.0;
        float size = getRadius() * scale / object_sprite.getTextureRect().width * 3.0;
        object_sprite.setScale(size, size);
//        object_sprite.setColor(sf::Color(255, 255, 255));
        window.draw(object_sprite, sf::BlendAdd);
    }
}

void Planet::drawOnGMRadar(sf::RenderTarget& window, sf::Vector2f position, float scale, float rotation, bool long_range)
{
    sf::CircleShape radar_radius(planet_size * scale);
    radar_radius.setOrigin(planet_size * scale, planet_size * scale);
    radar_radius.setPosition(position);
    radar_radius.setFillColor(sf::Color::Transparent);
    radar_radius.setOutlineColor(sf::Color(255, 255, 255, 128));
    radar_radius.setOutlineThickness(3);
    window.draw(radar_radius);
}

void Planet::collide(Collisionable* target, float collision_force)
{
    if (collision_size > 0)
    {
        //Something hit this planet...
    }
}

void Planet::updateCollisionSize()
{
    setRadius(planet_size);
    if (std::abs(distance_from_movement_plane) >= planet_size)
    {
        collision_size = -1.0;
    }else{
        collision_size = sqrt((planet_size * planet_size) - (distance_from_movement_plane * distance_from_movement_plane)) * 1.1f;
        setCollisionRadius(collision_size);
        setCollisionPhysics(true, true);
    }
}

string Planet::getExportLine()
{
    string ret="Planet():setPosition(" + string(getPosition().x, 0) + ", " + string(getPosition().y, 0) + "):setPlanetRadius(" + string(getPlanetRadius(), 0) + ")";
    if (atmosphere_color.r != 0 || atmosphere_color.g != 0 || atmosphere_color.b != 0)
    {
        ret += ":setPlanetAtmosphereColor(" + string(atmosphere_color.r/255.0f) + "," + string(atmosphere_color.g/255.0f) + "," + string(atmosphere_color.b/255.0f) + ")";
    }
    if (distance_from_movement_plane!=0)
    {
        ret += ":setDistanceFromMovementPlane("  + string(distance_from_movement_plane) + ")";
    }
    //TODO setPlanetAtmosphereTexture
    //TODO setPlanetSurfaceTexture
    //TODO setPlanetCloudTexture
    //TODO setPlanetCloudRadius
    //TODO setAxialRotationTime
    //TODO setOrbit
    return ret;
}
