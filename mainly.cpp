#include "raylib.h"
#include <vector>
#include <cmath>
#include <cstdlib>

namespace constants
{
    constexpr double C = 2.0;
    constexpr double G = 6.6743e-11;
    constexpr int WinWid = 1080;
    constexpr int WinHei = 1920;
    constexpr int x = 30;
}

class BlackHole // custom data type Blackhole class
{
public:
    float r_s = 25;             // Schwarzschild radius of BH
    Vector3 place = {0, 0, 0};
};

class Photon // custom data type Photon class
{
public:
    double r = 930;    //  dist from centre of BH
    double phi = M_PI; // Angle from center of BH
    double dr_dt;
    double dphi_dt;
    double theta;
    double dtheta_dt;
    int maxlen = 127;           // Max length of the trail
    bool alive = true;          // Alive state of photon
    std::vector<Vector3> trail; // Cords History of the photon
};


class Game
{

public:
    int i = 0;

    void BeginSim(float dTime) // calls each function once
    {
        DrawBH();      // Draw BH at center
        DrawPH(dTime); // Draw Photon
        PosUpd(dTime); // Update Photon position

    }



    BlackHole B1; // B1 object creation of BH class

    void DrawBH() // Draw BH at center
    {
        DrawSphere(B1.place, B1.r_s, MAROON);
    }

    Photon P1; // P1 object class of Photo class

    std::vector<Photon> PhoList; // Vector using cutom data type of Photon class holding data of each Photon iteration

    void SpawnPH() // Assign data to each Photon
    {
        for (i = 0; i < 1000; i++)
        {
            P1.r = 150;
            P1.phi += 150;
            P1.theta = GetRandomValue(0, 314) / 100.0;

            double angV = GetRandomValue(1.7, 1.9);
            double radV = GetRandomValue(20, 40);
            double thetaV = GetRandomValue(0, 314) / 100.0;

            P1.dr_dt = -radV * cos(angV);
            P1.dphi_dt = radV * sin(angV) / P1.r;
            P1.dtheta_dt = thetaV;

            PhoList.push_back(P1);
        }
    }

    void DrawPH(float dTime)
    {

        for (size_t i = 0; i < PhoList.size(); i++) // loop for one photon (iterates between each photon)
        {
            if (PhoList[i].trail.size() < 2)
                continue;
            for (size_t j = 0; j < PhoList[i].trail.size() - 1; j++) // loop for assigning color,opacity and then drawing the trails
            {
                unsigned char opty = (j * 255) / PhoList[i].trail.size(); // opacity = index of cells divided by size of one cell

                Color trailColor = {255, 165, 0, opty};

                Vector3 trailPt1 = PhoList[i].trail[j]; // assigns the prev cords of the photons to a local Vector2 variable
                Vector3 trailPt2 = PhoList[i].trail[j + 1];

                DrawLine3D(trailPt1, trailPt2, trailColor);
                opty += 2;
            }
        }

    }
    void PosUpd(float dTime) // loop to move each photon and record it's curr pos into a vector
    {
        for (size_t i = 0; i < PhoList.size(); i++)
        {

            double xCord = 960 + PhoList[i].r * cos(PhoList[i].phi);
            double yCord = 540 + PhoList[i].r * sin(PhoList[i].phi);

            if (PhoList[i].alive)
            {
                PhoList[i].r += dTime * PhoList[i].dr_dt;
                PhoList[i].phi += dTime * PhoList[i].dphi_dt;
                PhoList[i].theta += dTime * PhoList[i].dtheta_dt;
                double xCord = PhoList[i].r * sin(PhoList[i].theta) * cos(PhoList[i].phi);
                double yCord = PhoList[i].r * cos(PhoList[i].theta);
                double zCord = PhoList[i].r * sin(PhoList[i].theta) * sin(PhoList[i].phi);
                PhoList[i].trail.push_back({(float)xCord, (float)yCord, (float)zCord});
            }
            if (PhoList[i].r < B1.r_s)
            {
                PhoList[i].alive = false;
            }

            if (!PhoList[i].trail.empty() && (PhoList[i].trail.size() > PhoList[i].maxlen || !PhoList[i].alive))
            {
                PhoList[i].trail.erase(PhoList[i].trail.begin());
            }
        }
    }
};

int main()
{
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);

    InitWindow(0, 0, "BlackHoleSim");

    SetTargetFPS(60);
    Camera3D camera = {0};
    camera.position = (Vector3){75.0f, 150.0f, 75.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 5.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

 
    Game G1;
    G1.SpawnPH();
    while (!WindowShouldClose())
    {
        float dTime = GetFrameTime();

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);

        HideCursor();
        static float camPhi = M_PI / 4;   // Horizontal rotation
        static float camTheta = M_PI / 4; // Vertical elevation

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            Vector2 delta = GetMouseDelta();
            camPhi -= delta.x * 0.01;   // Drag left/right rotates
            camTheta -= delta.y * 0.01; // Drag up/down changes height
        }

        // Convert to 3D position (orbit around center)
        float distance = 200;
        camera.position.x = distance * sin(camTheta) * cos(camPhi);
        camera.position.y = distance * cos(camTheta);
        camera.position.z = distance * sin(camTheta) * sin(camPhi);

        camera.target = (Vector3){0, 0, 0}; // Always look at center
        G1.BeginSim(dTime);
        DrawGrid(10, 15.0f);


        EndMode3D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}