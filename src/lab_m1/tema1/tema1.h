#pragma once

#include "components/simple_scene.h"


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
    public:
        Tema1();
        ~Tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RedBandGeneration ();
        void SpotsGeneration ();
        void ContourGeneration ();
        void DiamondGeneration ();
        void HeartGeneration ();
        void StarGeneration ();
        void EnemyGeneration (float deltaTimeSeconds);
        void EnemyMovement (float deltaTimeSeconds);
        void RenferDianoond (float deltaTimeSeconds);
        void DetectCollisionEnemyDiamond (int i , float deltaTimeSeconds);
        void Shooting (float deltaTimeSeconds);
        void MovementProjectile (float deltaTimeSeconds);
        void StarApear (float deltaTimeSeconds);
        void Stars (float deltaTimeSeconds);
        void Star_Bank (float deltaTimeSeconds);
        void DetectCollisionEnemyProjectile (int i , int j, float deltaTimeSeconds);

    protected:
        float cx, cy;
        glm::mat3 modelMatrix;
        float translateX, translateY;
        float scaleX, scaleY;
        float angularStep;

        // TODO(student): If you need any other class variables, define them here.
        float rotation;

        int max_t;
        int max_s;

        int lines[3] = { 80, 250, 410 };
        std::vector<std::string> enemys = { "blue_enemy" , "purple_enemy" , "pink_enemy" , "yellow_enemy" };
        std::vector<std::string> objects = { "blue_diamond" , "purple_diamond" , "pink_diamond" , "yellow_diamond"};
        // int enemy_on_line[3] = { 999, 999, 999 };
        // int position[3] = { 1300, 1300, 1300 };
        std::vector<std::tuple<int , std::string , int , int , int>> enemy_on_line;
        // int health[3] = { 3, 3, 3 };
        std::vector<std::tuple<int, float , bool>> enemy_animation =
            { {std::make_tuple(1300, 1, false)} ,
            {std::make_tuple(1300, 1, false)} ,
            {std::make_tuple(1300, 1, false)} };
        

        int mouseX = 0;
        int mouseY = 0;

        int game_hearts = 3;

        float time_star_appear = 0;
        float enemy_rate_spawn = 0;

        bool mouseBtnPress = false;
        std::string object_to_move = "none";

        float time_shooting = 0;

        std::vector<std::vector<std::string>> diamond_place = { {"none", "none", "none"}, {"none", "none", "none"}, {"none", "none", "none"} };
        std::vector<std::vector<std::tuple<int , int , float , bool>>> diamond_animation = 
            {{std::make_tuple(420, 20 , 1 , false) , std::make_tuple(420, 185, 1, false) , std::make_tuple(420, 350, 1, false)},
            {std::make_tuple(260, 20, 1, false) , std::make_tuple(260, 185, 1, false) , std::make_tuple(260, 350, 1, false)},
            {std::make_tuple(100, 20, 1, false) , std::make_tuple(100, 185, 1, false) , std::make_tuple(100, 350, 1, false)}};
        
        std::vector<std::vector<std::tuple<int , int>>> cell_position =
            {{std::make_tuple(420, 20) , std::make_tuple(420, 185) , std::make_tuple(420, 350)},
            {std::make_tuple(260, 20) , std::make_tuple(260, 185) , std::make_tuple(260, 350)},
            {std::make_tuple(100, 20) , std::make_tuple(100, 185) , std::make_tuple(100, 350)}};

        std::vector<std::tuple<int , int , int>> stars_position;
        int star_uncolelcted = 0;
        int star_bank = 5;
        std::unordered_map<std::string , int> diamond_cost = { {"blue_diamond" , 1} , {"purple_diamond" , 2} , {"pink_diamond" , 2} , {"yellow_diamond" , 3} };

        std::vector<std::vector<int>> shooting = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };

        std::vector<std::vector<std::tuple<int , int , bool , float>>> projectilesPosition = 
            {{std::make_tuple(480, 80 , false , 0.f) , std::make_tuple(480, 245, false , 0.f) , std::make_tuple(480, 410, false , 0.f)},
            {std::make_tuple(320, 80, false , 0.f) , std::make_tuple(320, 245, false , 0.f) , std::make_tuple(320, 410, false , 0.f)},
            {std::make_tuple(160, 80, false , 0.f) , std::make_tuple(160, 245, false , 0.f) , std::make_tuple(160, 410, false , 0.f)}};

        std::unordered_map<std::string , std::string> diamond_projectile = { {"blue_diamond" , "blue_projectile"} , {"purple_diamond" , "purple_projectile"} , {"pink_diamond" , "pink_projectile"} , {"yellow_diamond" , "yellow_projectile"} };
        std::unordered_map<std::string , std::string> diamond_enemy = { {"blue_diamond" , "blue_enemy"} , {"purple_diamond" , "purple_enemy"} , {"pink_diamond" , "pink_enemy"} , {"yellow_diamond" , "yellow_enemy"} , {"none" , "none"}};
        std::unordered_map<int , std::string> enemyCode = { {0 , "blue_enemy"} , {1 , "purple_enemy"} , {2 , "pink_enemy"} , {3 , "yellow_enemy"} };
    };
}   // namespace m1
    