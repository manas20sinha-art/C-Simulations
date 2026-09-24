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
    double x = 960, y = 540; // Center of BH
    double dx = x - 960;     //
    double dy = y - 540;
    double r = hypot(dx, dy);   // distance from centre of BH
    double phi = atan2(dy, dx); // Angle from center of BH
    Vector2 loc = {960, 540};   // location vector of center of BH
    float r_s = 125;            // Schwarzschild radius of BH
};

class Photon // custom data type Photon class
{
public:
    double r = 930;             //  dist from centre of BH
    double phi = M_PI;          // Angle from center of BH
    double wid = 10;            // width
    double len = 10;            // length
    double spd = 50;            // Speed
    int maxlen = 127;           // Max length of the trail
    bool alive = true;          // Alive state of photon
    std::vector<Vector2> trail; // Cords History of the photon
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
        DrawCircle(B1.loc.x, B1.loc.y, B1.r_s, MAROON);
    }

    Photon P1; // P1 object class of Photo class

    std::vector<Photon> PhoList; // Vector using cutom data type of Photon class holding data of each Photon iteration

    void SpawnPH() // Assign data to each Photon
    {
        for (i = 0; i < 15; i++)
        {
            P1.r = 530;
            P1.phi += 15;
            PhoList.push_back(P1);
        }
    }

    void DrawPH(float dTime)
    {

        for (size_t i = 0; i < PhoList.size(); i++) // loop for one photon (iterates between each photon)
        {
            for (size_t j = 0; j < PhoList[i].trail.size(); j++) // loop for assigning color,opacity and then drawing the trails
            {
                int opty = (j * 255) / PhoList[i].trail.size(); // opacity = index of cells divided by size of one cell

                Color trailColor = {255, 165, 0, opty};

                Vector2 trailPoint = PhoList[i].trail[j]; // assigns the prev cords of the photons to a local Vector2 variable

                DrawRectangle(trailPoint.x, trailPoint.y, PhoList[i].wid, PhoList[i].len, trailColor);
                opty += 2;
            }
        }

        for (size_t i = 0; i < PhoList.size(); i++) // loop to draw each photon
        {
            if (!PhoList[i].alive)
            {
                continue;
            }
            double xCord = 960 + PhoList[i].r * cos(PhoList[i].phi); // convert polar cordinates to cartesian
            double yCord = 540 + PhoList[i].r * sin(PhoList[i].phi); // convert polar cordinates to cartesian

            DrawRectangle(xCord, yCord, PhoList[i].wid, PhoList[i].len, RED);
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
                PhoList[i].r -= dTime * PhoList[i].spd;
                double xCord = 960 + PhoList[i].r * cos(PhoList[i].phi);
                double yCord = 540 + PhoList[i].r * sin(PhoList[i].phi);
                PhoList[i].trail.push_back({(float)xCord, (float)yCord});
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

    Game G1;
    G1.SpawnPH();
    while (!WindowShouldClose())
    {
        float dTime = GetFrameTime();

        BeginDrawing();

        ClearBackground(BLACK);

        G1.BeginSim(dTime);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}