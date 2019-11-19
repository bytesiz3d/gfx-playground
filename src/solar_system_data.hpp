#pragma once
struct SolarSystemDescription
{
    int scale;
    int distanceFromParent;

    float rotationSpeedAroundParent;
    float rotationSpeedAroundSelf;

    float tint[16];
    std::vector<SolarSystemDescription> children;

    SolarSystemDescription(int _s = 0, int _d = 0, float _rp = 0, float _rs = 0, const float* _t = 0, int _ch = 0)
    {
        scale = _s;
        distanceFromParent = _d;
        rotationSpeedAroundParent = _rp;
        rotationSpeedAroundSelf = _rs;
        if (_t)
        {
            for (char i = 0; i < 16; i++)
                tint[i] = _t[i];
        }
        children = std::vector<SolarSystemDescription>(_ch);
    }
} System0, System1, System2;

void InitSystems()
{
    float tint0[] = { 0.5, 0.3, 0, 0, 0.3, 0.2, 0, 0, 0.3, 0.2, 0.1, 0, 0, 0, 0, 1 };
    System0 = {
        50,
        0,
        0.000f,
        0.005f,
        tint0,
        1
    };
    {
        float tint00[] = { 0.8,0,0,0, 0.2,0,0,0, 0.2,0,0,0, 0,0,0,1 };
        System0.children[0] = {
            10,
            90,
            0.001,
            0.000,
            tint00
        };
    }

    float tint1[] = { 0.5,0.3,0,0, 0.3,0.2,0,0, 0.3,0.2,0.1,0, 0,0,0,1 };
    System1 = {
        30,
        0,
        0.000,
        0.001,
        tint1,
        3
    };
    {
        float tint10[] = { 0.8,0,0,0, 0.2,0,0,0, 0.2,0,0,0, 0,0,0,1 };
        System1.children[0] = {
            5,
            50,
            0.005,
            0.005,
            tint10,
        };

        float tint11[] = { 0.1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
        System1.children[1] = {
            10,
            100,
            0.002,
            0.005,
            tint11,
            1
        };
        {
            float tint110[] = { 0.2,0.2,0.2,0, 0.2,0.2,0.2,0, 0.2,0.2,0.2,0, 0,0,0,1 };
            System1.children[1].children[0] = {
                5,
                20,
                0.005,
                0.010,
                tint110
            };
        }

        float tint12[] = { 0.5,0,0.3,0, 0,0.1,0,0, 0.4,0,1,0, 0,0,0,1 };
        System1.children[2] = {
            7,
            200,
            0.001,
            0.003,
            tint12,
            2
        };
        {
            float tint120[] = { 0.2,0.2,0.2,0, 0.2,0.2,0.2,0, 0.2,0.2,0.2,0, 0,0,0,1 };
            System1.children[2].children[0] = {
                2,
                20,
                0.005,
                0.010,
                tint120
            };

            float tint121[] = { 0.2, 0.2, 0.2, 0, 0.2, 0.2, 0.2, 0, 0.2, 0.2, 0.2, 0, 0, 0, 0, 1 };
            System1.children[2].children[1] = {
                2,
                30,
                0.003,
                0.010,
                tint121
            };
        }
    };

    float tint2[] = { 0.5, 0.3, 0, 0, 0.3, 0.2, 0, 0, 0.3, 0.2, 0.1, 0, 0, 0, 0, 1 };
    System2 = {
        40,
        0,
        0.000,
        0.001,
        tint2,
        1
    };
    {
        float tint20[] = { 0.1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
        System2.children[0] = {
            20,
            120,
            0.002,
            0.005,
            tint20,
            1
        };
        {
            float tint200[] = { 0.2, 0.2, 0.2, 0, 0.2, 0.2, 0.2, 0, 0.2, 0.2, 0.2, 0, 0, 0, 0, 1 };
            System2.children[0].children[0] = {
                10,
                50,
                0.005,
                0.010,
                tint200,
                1
            };
            {
                float tint2000[] = { 0.2, 0.2, 0.2, 0, 0.2, 0.2, 0.2, 0, 0.2, 0.2, 0.2, 0, 0, 0, 0, 1 };
                System2.children[0].children[0].children[0] = {
                    5,
                    20,
                    -0.020,
                    0.020,
                    tint2000,
                    1
                };
            }
        }
    }
}
