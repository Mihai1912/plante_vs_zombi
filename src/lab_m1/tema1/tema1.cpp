#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/obj2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, 0, 0);
    float squareSide = 100;

    // TODO(student): Compute coordinates of a square's center, and store
    // then in the `cx` and `cy` class variables (see the header). Use
    // `corner` and `squareSide`. These two class variables will be used
    // in the `Update()` function. Think about it, why do you need them?

    cx = corner.x + squareSide/2;
    cy = corner.y + squareSide/2;

    // Initialize tx and ty (the translation steps)
    translateX = 0;
    translateY = 0;

    // Initialize sx and sy (the scale factors)
    scaleX = 1;
    scaleY = 1;

    // Initialize angularStep
    angularStep = 0;
    rotation = 0;

    max_t = 0;
    max_s = 0;

    Mesh* square1 = obj2D::CreateSquare("square1", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square1);

    Mesh* square2 = obj2D::CreateSquare("square2", corner, squareSide, glm::vec3(51/255.f, 102/255.f, 0) , true);
    AddMeshToList(square2);

    Mesh* square3 = obj2D::CreateSquare("square3", corner, squareSide, glm::vec3(1, 1, 1));
    AddMeshToList(square3);

    Mesh* heart1 = obj2D::CreateHeart("heart1", corner, 50, glm::vec3(1, 0, 0), true);
    AddMeshToList(heart1);

    Mesh* blue_diamond = obj2D::CreateDiamond("blue_diamond", corner, 30, glm::vec3(62/255.f, 146/255.f, 1), true);
    AddMeshToList(blue_diamond);

    Mesh* purple_diamond = obj2D::CreateDiamond("purple_diamond", corner, 30, glm::vec3((128/255.f), 0, 128/255.f), true);
    AddMeshToList(purple_diamond);
    
    Mesh* pink_diamond = obj2D::CreateDiamond("pink_diamond", corner, 30, glm::vec3(1, 2/255.f, 244/255.f), true);
    AddMeshToList(pink_diamond);

    Mesh* yellow_diamond = obj2D::CreateDiamond("yellow_diamond", corner, 30, glm::vec3(1, 1, 0), true);
    AddMeshToList(yellow_diamond);
    
    Mesh* star = obj2D::CreateStar("star", corner, 15, glm::vec3(192/255.f, 192/255.f, 192/255.f), true);
    AddMeshToList(star);

    Mesh* star1 = obj2D::CreateStar("star1", corner, 15, glm::vec3(0, 1, 0), true);
    AddMeshToList(star1);

    Mesh* blue_enemy = obj2D::CreateHexagon("blue_enemy", corner, 40, glm::vec3(62/255.f, 146/255.f, 1), glm::vec3(36/255.f , 88/255.f, 156/255.f), true);
    AddMeshToList(blue_enemy);

    Mesh* purple_enemy = obj2D::CreateHexagon("purple_enemy", corner, 40, glm::vec3((128/255.f), 0, 128/255.f), glm::vec3(74/255.f, 20/255.f, 140/255.f), true);
    AddMeshToList(purple_enemy);

    Mesh* pink_enemy = obj2D::CreateHexagon("pink_enemy", corner, 40, glm::vec3(1, 2/255.f, 244/255.f), glm::vec3(255/255.f, 105/255.f, 180/255.f), true);
    AddMeshToList(pink_enemy);

    Mesh* yellow_enemy = obj2D::CreateHexagon("yellow_enemy", corner, 40, glm::vec3(1, 1, 0), glm::vec3(255/255.f, 215/255.f, 0), true);
    AddMeshToList(yellow_enemy);

    Mesh* blue_projectile = obj2D::CreateStar("blue_projectile", corner, 15, glm::vec3(62/255.f, 146/255.f, 1), true);
    AddMeshToList(blue_projectile);

    Mesh* purple_projectile = obj2D::CreateStar("purple_projectile", corner, 15, glm::vec3((128/255.f), 0, 128/255.f), true);
    AddMeshToList(purple_projectile);

    Mesh* pink_projectile = obj2D::CreateStar("pink_projectile", corner, 15, glm::vec3(1, 2/255.f, 244/255.f), true);
    AddMeshToList(pink_projectile);

    Mesh* yellow_projectile = obj2D::CreateStar("yellow_projectile", corner, 15, glm::vec3(1, 1, 0), true);
    AddMeshToList(yellow_projectile);
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.128f, 0.128f, 0.128f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::Update(float deltaTimeSeconds)
{
    RedBandGeneration();
    SpotsGeneration();
    ContourGeneration();
    DiamondGeneration();
    HeartGeneration();
    StarGeneration();

    EnemyGeneration(deltaTimeSeconds);
    EnemyMovement(deltaTimeSeconds);
    
    if (mouseBtnPress)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(mouseX, 720 - mouseY);
        RenderMesh2D(meshes[object_to_move], shaders["VertexColor"], modelMatrix);
    }

    RenferDianoond(deltaTimeSeconds);
    Shooting(deltaTimeSeconds);
    MovementProjectile(deltaTimeSeconds);

    if (time_star_appear >= 8000)
    {
        StarApear(deltaTimeSeconds);
        time_star_appear = 0;
    } else
    {
        time_star_appear += deltaTimeSeconds * 1000;
    }

    if (star_uncolelcted != 0) 
    {
        Stars(deltaTimeSeconds);
    }

    Star_Bank(deltaTimeSeconds);
    time_shooting += deltaTimeSeconds * 100;
    cout << time_shooting << endl;
}


void Tema1::FrameEnd()
{
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    if (mouseBtnPress)
    {
        Tema1::mouseX = mouseX;
        Tema1::mouseY = mouseY;
    }
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
        if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
            Tema1::mouseBtnPress = true;
            if (mouseX >= 20 && mouseX <= 140 && mouseY <= 720 - 570 && mouseY >= 720 - 690)
            {
                object_to_move = "blue_diamond";
                Tema1::mouseX = 60;
                Tema1::mouseY = 720 - 630;
            } else if (mouseX >= 180 && mouseX <= 300 && mouseY <= 720 - 570 && mouseY >= 720 - 690)
            {
                object_to_move = "purple_diamond";
                Tema1::mouseX = 220;
                Tema1::mouseY = 720 - 630;
            } else if (mouseX >= 340 && mouseX <= 460 && mouseY <= 720 - 570 && mouseY >= 720 - 690)
            {
                object_to_move = "pink_diamond";
                Tema1::mouseX = 380;
                Tema1::mouseY = 720 - 630;
            } else if (mouseX >= 500 && mouseX <= 620 && mouseY <= 720 - 570 && mouseY >= 720 - 690)
            {
                object_to_move = "yellow_diamond";
                Tema1::mouseX = 540;
                Tema1::mouseY = 720 - 630;
            } else
            {
                object_to_move = "none";
            }
            for (int i = 0 ; i < star_uncolelcted ; i++)
            {
                if (mouseX >= get<0>(stars_position[i]) - 10 && mouseX <= get<0>(stars_position[i]) + 10 && mouseY <= 720 - get<2>(stars_position[i]) + 10 && mouseY >= 720 - get<2>(stars_position[i]) - 10)
                {
                    star_uncolelcted--;
                    star_bank++;
                    vector<tuple<int , int , int>>::iterator it;
                    it = stars_position.begin() + i;
                    stars_position.erase(it);
                }
            }
        }
        if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_RIGHT) ) {
            if (mouseX >= 100 && mouseX <= 220 && mouseY <= 720 - 20 && mouseY >= 720 - 140) 
            {
                if (diamond_place[2][0] != "none")
                {
                    diamond_place[2][0] = "none";
                }
            } else if (mouseX >= 100 && mouseX <= 220 && mouseY <= 720 - 185 && mouseY >= 720 - 310)
            {
                if (diamond_place[2][1] != "none")
                {
                    diamond_place[2][1] = "none";
                }
            } else if (mouseX >= 100 && mouseX <= 220 && mouseY <= 720 - 350 && mouseY >= 720 - 470)
            {
                if (diamond_place[2][2] != "none")
                {
                    diamond_place[2][2] = "none";
                }
            } else if (mouseX >= 260 && mouseX <= 380 && mouseY <= 720 - 20 && mouseY >= 720 - 140)
            {
                if (diamond_place[1][0] != "none")
                {
                    diamond_place[1][0] = "none";
                }
            } else if (mouseX >= 260 && mouseX <= 380 && mouseY <= 720 - 185 && mouseY >= 720 - 310)
            {
                if (diamond_place[1][1] != "none")
                {
                    diamond_place[1][1] = "none";
                }
            } else if (mouseX >= 260 && mouseX <= 380 && mouseY <= 720 - 350 && mouseY >= 720 - 470)
            {
                if (diamond_place[1][2] != "none")
                {
                    diamond_place[1][2] = "none";
                }
            } else if (mouseX >= 420 && mouseX <= 540 && mouseY <= 720 - 20 && mouseY >= 720 - 140)
            {
                if (diamond_place[0][0] != "none")
                {
                    diamond_place[0][0] = "none";
                }
            } else if (mouseX >= 420 && mouseX <= 540 && mouseY <= 720 - 185 && mouseY >= 720 - 310)
            {
                if (diamond_place[0][1] != "none")
                {
                    diamond_place[0][1] = "none";
                }
            } else if (mouseX >= 420 && mouseX <= 540 && mouseY <= 720 - 350 && mouseY >= 720 - 470)
            {
                if (diamond_place[0][2] != "none")
                {
                    diamond_place[0][2] = "none";
                }
            }
        }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
    if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
        Tema1::mouseBtnPress = false;
        if (mouseX >= 100 && mouseX <= 220 && mouseY <= 720 - 20 && mouseY >= 720 - 140) 
        {
            if (diamond_cost[object_to_move] <= star_bank)
            {
                star_bank -= diamond_cost[object_to_move];
                diamond_place[2][0] = object_to_move;
            }
        } else if (mouseX >= 100 && mouseX <= 220 && mouseY <= 720 - 185 && mouseY >= 720 - 310)
        {
            if (diamond_cost[object_to_move] <= star_bank)
            {
                star_bank -= diamond_cost[object_to_move];
                diamond_place[2][1] = object_to_move;
            }
        } else if (mouseX >= 100 && mouseX <= 220 && mouseY <= 720 - 350 && mouseY >= 720 - 470)
        {
            if (diamond_cost[object_to_move] <= star_bank)
            {
                star_bank -= diamond_cost[object_to_move];
                diamond_place[2][2] = object_to_move;
            }
        } else if (mouseX >= 260 && mouseX <= 380 && mouseY <= 720 - 20 && mouseY >= 720 - 140)
        {
            if (diamond_cost[object_to_move] <= star_bank)
            {
                star_bank -= diamond_cost[object_to_move];
                diamond_place[1][0] = object_to_move;
            }
        } else if (mouseX >= 260 && mouseX <= 380 && mouseY <= 720 - 185 && mouseY >= 720 - 310)
        {
            if (diamond_cost[object_to_move] <= star_bank)
            {
                star_bank -= diamond_cost[object_to_move];
                diamond_place[1][1] = object_to_move;
            }
        } else if (mouseX >= 260 && mouseX <= 380 && mouseY <= 720 - 350 && mouseY >= 720 - 470)
        {
            if (diamond_cost[object_to_move] <= star_bank)
            {
                star_bank -= diamond_cost[object_to_move];
                diamond_place[1][2] = object_to_move;
            }
        } else if (mouseX >= 420 && mouseX <= 540 && mouseY <= 720 - 20 && mouseY >= 720 - 140)
        {
            if (diamond_cost[object_to_move] <= star_bank)
            {
                star_bank -= diamond_cost[object_to_move];
                diamond_place[0][0] = object_to_move;
            }
        } else if (mouseX >= 420 && mouseX <= 540 && mouseY <= 720 - 185 && mouseY >= 720 - 310)
        {
            if (diamond_cost[object_to_move] <= star_bank)
            {
                star_bank -= diamond_cost[object_to_move];
                diamond_place[0][1] = object_to_move;
            }
        } else if (mouseX >= 420 && mouseX <= 540 && mouseY <= 720 - 350 && mouseY >= 720 - 470)
        {
            if (diamond_cost[object_to_move] <= star_bank)
            {
                star_bank -= diamond_cost[object_to_move];
                diamond_place[0][2] = object_to_move;
            }
        }
    }
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

void Tema1::RedBandGeneration()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(20, 20);
    modelMatrix *= transform2D::Scale(0.5f , 4.5f);
    RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}

void Tema1::SpotsGeneration()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(100, 20);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(100, 185);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(100, 350);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(260, 20);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(260, 185);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(260, 350);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(420, 20);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(420, 185);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(420, 350);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square2"], shaders["VertexColor"], modelMatrix);
}

void Tema1::ContourGeneration()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(20, 570);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(180, 570);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(340, 570);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(500, 570);
    modelMatrix *= transform2D::Scale(1.2f , 1.2f);
    RenderMesh2D(meshes["square3"], shaders["VertexColor"], modelMatrix);
}

void Tema1::DiamondGeneration()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(60, 630);
    RenderMesh2D(meshes["blue_diamond"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(220, 630);
    RenderMesh2D(meshes["purple_diamond"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(380, 630);
    RenderMesh2D(meshes["pink_diamond"], shaders["VertexColor"], modelMatrix);
    
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(540, 630);
    RenderMesh2D(meshes["yellow_diamond"], shaders["VertexColor"], modelMatrix);
}

void Tema1::HeartGeneration()
{
    if (game_hearts == 0)
    {
        exit(0);
    }
    for (int i = 0 ; i < game_hearts ; i++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(800 + i * 150, 570);
        modelMatrix *= transform2D::Scale(1.2f , 1.2f);
        RenderMesh2D(meshes["heart1"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::StarGeneration()
{
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(80, 550);
    modelMatrix *= transform2D::Rotate((M_PI/180)*90);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(225, 550);
    modelMatrix *= transform2D::Rotate((M_PI/180)*90);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(255, 550);
    modelMatrix *= transform2D::Rotate((M_PI/180)*90);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(385, 550);
    modelMatrix *= transform2D::Rotate((M_PI/180)*90);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(415, 550);
    modelMatrix *= transform2D::Rotate((M_PI/180)*90);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(530, 550);
    modelMatrix *= transform2D::Rotate((M_PI/180)*90);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(560, 550);
    modelMatrix *= transform2D::Rotate((M_PI/180)*90);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);

    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(590, 550);
    modelMatrix *= transform2D::Rotate((M_PI/180)*90);
    RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
}

void Tema1::EnemyGeneration(float deltaTimeSeconds)
{
    // if (rand()%73 == 0)
    // {
    //     int i = rand()%3;
    //     int e = rand()%4;
    //     if (enemy_on_line[i] == 999)
    //     {
    //         enemy_on_line[i] = e;
    //     }
    // }
    float rate = rand() % 1000;
    if (rate > 500 && rate < 505)
    {
        int i = rand()%3; // linia
        int e = rand()%4; // tip inamic
        if (i == 0)
        {
            enemy_on_line.push_back(make_tuple(i , enemys[e] , 1300 , 80 , 3));
        } else if (i == 1)
        {
            enemy_on_line.push_back(make_tuple(i , enemys[e] , 1300 , 245 , 3));
        } else if (i == 2)
        {
            enemy_on_line.push_back(make_tuple(i , enemys[e] , 1300 , 410 , 3));
        }
    }
}

void Tema1::EnemyMovement(float deltaTimeSeconds)
{
    if (!enemy_on_line.empty())
    {
        for (int i = 0 ; i < enemy_on_line.size() ; i++)
        {
            if (get<4>(enemy_on_line[i]) > 0)
            {
                if (get<2>(enemy_on_line[i]) > 50)
                {
                    get<2>(enemy_on_line[i]) -= 40 * deltaTimeSeconds;
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(get<2>(enemy_on_line[i]), get<3>(enemy_on_line[i]));
                    RenderMesh2D(meshes[get<1>(enemy_on_line[i])], shaders["VertexColor"], modelMatrix);
                    DetectCollisionEnemyDiamond(i , deltaTimeSeconds);
                } else
                {
                    vector<tuple<int , string , int , int , int>>::iterator it;
                    it = enemy_on_line.begin() + i;
                    enemy_on_line.erase(it);
                    game_hearts--;
                }
            }
            else
            {
                vector<tuple<int , string , int , int , int>>::iterator it;
                it = enemy_on_line.begin() + i;
                enemy_on_line.erase(it);
            }
        }
    }
}

void Tema1::RenferDianoond(float deltaTimeSeconds)
{
    for (int i = 0 ; i < 3 ; i++)
    {
        for (int j = 0 ; j < 3 ; j++)
        {
            if (diamond_place[i][j] != "none")
            {
                if (get<3>(diamond_animation[i][j]))
                {
                    if (get<2>(diamond_animation[i][j]) > 0 && get<3>(diamond_animation[i][j]))
                    {
                        get<2>(diamond_animation[i][j]) -= deltaTimeSeconds * 2;
                        modelMatrix = glm::mat3(1);
                        modelMatrix *= transform2D::Translate(get<0>(cell_position[i][j]) + 50, get<1>(cell_position[i][j])+60);
                        modelMatrix *= transform2D::Scale(get<2>(diamond_animation[i][j]), get<2>(diamond_animation[i][j]));
                        RenderMesh2D(meshes[diamond_place[i][j]], shaders["VertexColor"], modelMatrix);
                    } else
                    {
                        get<3>(diamond_animation[i][j]) = false;
                        get<2>(diamond_animation[i][j]) = 1;
                        diamond_place[i][j] = "none";
                    }
                } else
                {
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(get<0>(cell_position[i][j]) + 50, get<1>(cell_position[i][j])+60);
                    RenderMesh2D(meshes[diamond_place[i][j]], shaders["VertexColor"], modelMatrix);
                }
            }
        }
    }
}

void Tema1::DetectCollisionEnemyDiamond(int idx , float deltaTimeSeconds)
{
    for (int i = 0; i < 3 ; i++)
    {
        for (int j = 0; j < 3 ; j++)
        {
            if (get<0>(enemy_on_line[idx]) == i)
            {
                if (get<2>(enemy_on_line[idx]) <= get<0>(cell_position[j][i]) + 50 && get<2>(enemy_on_line[idx]) >= get<0>(cell_position[j][i]) - 50)
                {
                    if (get<3>(enemy_on_line[idx]) <= get<1>(cell_position[j][i]) + 60 && get<3>(enemy_on_line[idx]) >= get<1>(cell_position[j][i]) - 60)
                    {
                        if (diamond_place[j][i] != "none")
                        {
                            get<3>(diamond_animation[j][i]) = true;
                            RenferDianoond(deltaTimeSeconds);
                            get<2>(projectilesPosition[i][j]) = false;
                            get<0>(projectilesPosition[i][j]) = get<0>(cell_position[j][i]) + 60;
                            get<3>(projectilesPosition[i][j]) = 0;
                        }
                    }
                }
            }
        }
    }
}

void Tema1::Shooting(float deltaTimeSeconds)
{
    // in diamond_enemy j este linia si i coloana ???????
    if (time_shooting > 100)
    {
        for (int i = 0 ; i < 3 ; i++ )
        {
            for (int j = 0 ; j < 3 ; j++)
            {
                for (int k = 0 ; k < enemy_on_line.size() ; k ++ )
                {
                    if (j == get<0>(enemy_on_line[k]))
                    {
                        if (diamond_enemy[diamond_place[i][j]] == get<1>(enemy_on_line[k]))
                        {
                            get<2>(projectilesPosition[i][j]) = true;
                        }
                    }
                }
            }
        }
        time_shooting = 0;
    }
}

void Tema1::MovementProjectile(float deltaTimeSeconds)
{
    for (int i = 0 ; i < 3 ; i++ )
    {
        for (int j = 0 ; j < 3 ; j++)
        {
            if (get<2>(projectilesPosition[i][j]))
            {
                if (get<0>(projectilesPosition[i][j]) < 1300)
                {
                    get<0>(projectilesPosition[i][j]) += 400 * deltaTimeSeconds;
                    get<3>(projectilesPosition[i][j]) += 3 * deltaTimeSeconds;
                    modelMatrix = glm::mat3(1);
                    modelMatrix *= transform2D::Translate(get<0>(projectilesPosition[i][j]), get<1>(projectilesPosition[i][j]));
                    modelMatrix *= transform2D::Scale(2,2);
                    modelMatrix *= transform2D::Rotate(get<3>(projectilesPosition[i][j]));
                    RenderMesh2D(meshes[diamond_projectile[diamond_place[i][j]]], shaders["VertexColor"], modelMatrix);
                } else
                {
                    get<2>(projectilesPosition[i][j]) = false;
                    get<0>(projectilesPosition[i][j]) = get<0>(cell_position[j][i]) + 60;
                    get<3>(projectilesPosition[i][j]) = 0;
                }
                DetectCollisionEnemyProjectile(i , j, deltaTimeSeconds);
            }
        }
    }
}

void Tema1::StarApear(float deltaTimeSeconds)
{
    for (int i = 0 ; i < 3 ; i++)
    {
        int x = rand()%700 + 550;
        int y = rand()%500;
        int y1 = 720;
        stars_position.push_back(std::make_tuple(x , y , y1));
    }
    star_uncolelcted += 3;
}

void Tema1::Stars(float deltaTimeSeconds)
{
    for (int i = 0 ; i < stars_position.size() ; i++)
    {
        if (get<1>(stars_position[i]) > get<2>(stars_position[i]))
        {
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(get<0>(stars_position[i]), get<1>(stars_position[i]));
            modelMatrix *= transform2D::Rotate((M_PI/180)*90);
            modelMatrix *= transform2D::Scale(1.5, 1.5);
            RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);
        } else
        {
            get<2>(stars_position[i]) -= deltaTimeSeconds * 100;
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(get<0>(stars_position[i]), get<2>(stars_position[i]));
            modelMatrix *= transform2D::Rotate((M_PI/180)*90);
            modelMatrix *= transform2D::Scale(1.5, 1.5);
            RenderMesh2D(meshes["star1"], shaders["VertexColor"], modelMatrix);
        }
    }
}

void Tema1::Star_Bank(float deltaTimeSeconds)
{
    for (int i = 0 ; i < star_bank ; i ++)
    {
        modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(750 + i * 30, 550);
        modelMatrix *= transform2D::Rotate((M_PI/180)*90);
        RenderMesh2D(meshes["star"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::DetectCollisionEnemyProjectile(int i , int j , float deltaTimeSeconds)
{
    for (int k = 0 ; k < enemy_on_line.size() ; k ++)
    {
        if (get<0>(enemy_on_line[k]) == j)
        {
            if (get<1>(enemy_on_line[k]) == diamond_enemy[diamond_place[i][j]])
            {
                if (get<2>(enemy_on_line[k]) <= get<0>(projectilesPosition[i][j]))
                {
                    get<4>(enemy_on_line[k])--;
                    get<2>(projectilesPosition[i][j]) = false;
                    get<0>(projectilesPosition[i][j]) = get<0>(cell_position[i][j]) + 60;
                    get<3>(projectilesPosition[i][j]) = 0;
                    break;
                }
            }
        }
    }
}

