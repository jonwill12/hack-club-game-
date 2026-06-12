#include "raylib.h"
#include <vector>
#include <cmath>
#include <algorithm>
struct Enemy
{
    Vector2 position;
    float speed;
    int health;
};
struct Bullet
{
    Vector2 position;
    Vector2 velocity;
    float radius;
};

enum GameState
{
    MENU,
    PLAYING,
    WIN,
    SHOP,
    GAME_OVER,
};

int main()
{
    int screenWidth = GetMonitorWidth(0);
    int screenHeight = GetMonitorHeight(0);

    InitWindow(screenWidth, screenHeight, "Shooter With Shop");
    SetTargetFPS(60);

    GameState gameState = MENU;

    // ================= VARIABLES =================
    auto centerX = [&](float width)
    {
        return (screenWidth - width) / 2.0f;
    };

    int currentLevel = 1;
    int maxUnlockedLevel = 1;
    int enemiesToKill = 10;
    int enemiesKilled = 0;
    int enemiesSpawned = 0;
    int money = 0;
    const int buttonWidth = 200;
    const int buttonHeight = 40;
    int damage = 1;
    float fireCooldown = 0.3f;
    float fireTimer = 0.0f;
    float spawnTimer = 0.0f;
    int healthUpgradeCost = 50;
    int damageUpgradeCost = 50;
    int fireRateUpgradeCost = 75;

    Vector2 playerPosition = { screenWidth / 5.0f, screenHeight / 5.0f };
    float bodySize = 40.0f;
    float gunWidth = 40.0f;
    float gunHeight = 10.0f;
    bool isDead = false;

    int maxHealth = 100;
    int health = 100;

    float rotation = 0.0f;

    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    // ================= MENU ================= 
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);

       
if (gameState == MENU)
{
    const float buttonWidth = 200;
    const float buttonHeight = 40;
    const float spacing = 20;

    // Center the title
    const char* title = "LEVEL SELECT";
    int titleSize = 40;

    DrawText(
        title,
        (screenWidth - MeasureText(title, titleSize)) / 2,
        screenHeight / 6,
        titleSize,
        WHITE
    );

    // Calculate where the buttons should start
    float totalHeight = 1 * buttonHeight + -11 * spacing;
    float startY = (screenHeight - totalHeight) / 2.0f; 

    for (int i = 1; i <= 5; i++)
    {
        Rectangle levelButton =
        {
            centerX(buttonWidth) + 60,
            startY + (i - 1) * (buttonHeight + spacing),
            buttonWidth,
            buttonHeight
        };

        if (i <= maxUnlockedLevel)
        {
            DrawRectangleRec(levelButton, GREEN);

            const char* text = TextFormat("LEVEL %d", i);
            int textWidth = MeasureText(text, 20);

            DrawText(
                text,
                levelButton.x + (buttonWidth - textWidth) / 2,
                levelButton.y + 10,
                20,
                BLACK
            );

            if (CheckCollisionPointRec(GetMousePosition(), levelButton) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                currentLevel = i;
                enemiesKilled = 0;
                enemiesSpawned = 0;

                int level = currentLevel;
                if (level > 4) level = 4;

                enemiesToKill = 5 + (level - 1) * 3;

                enemies.clear();
                bullets.clear();
                health = maxHealth;

                gameState = PLAYING;
            }
        }
        else
        {
            DrawRectangleRec(levelButton, DARKGRAY);

            const char* text = "LOCKED";
            int textWidth = MeasureText(text, 20);

            DrawText(
                text,
                levelButton.x + (buttonWidth - textWidth) / 2,
                levelButton.y + 10,
                20,
                BLACK
            );
        }
    }

    Rectangle shopButton =
    {
        centerX(buttonWidth) + 60,
        startY + 5 * (buttonHeight + spacing) + 20,
        buttonWidth,
        buttonHeight
    };

    DrawRectangleRec(shopButton, BLUE);

    const char* shopText = "SHOP";
    int shopWidth = MeasureText(shopText, 20);

    DrawText(
        shopText,
        shopButton.x + (buttonWidth - shopWidth) / 2,
        shopButton.y + 10,
        20,
        WHITE
    );

    if (CheckCollisionPointRec(GetMousePosition(), shopButton) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        gameState = SHOP;
    }

    DrawText(TextFormat("Money: %d", money), 20, 40, 20, YELLOW);

    EndDrawing();
    continue;
}

            // ================= SHOP =================
            if (gameState == SHOP)
            {
                DrawText("SHOP", 330, 60, 40, WHITE);
                DrawText(TextFormat("Money: %d", money), 20, 20, 20, YELLOW);
                
{
    DrawText("SHOP", 330, 60, 40, WHITE);
    DrawText(TextFormat("HP: %d / %d", health, maxHealth), 20, 80, 20, WHITE);
    DrawText(TextFormat("DMG: %d", damage), 20, 100, 20, WHITE);
    DrawText(TextFormat("FIRE: %.2f", fireCooldown), 20, 120, 20, WHITE);
    float bw = 300;
    float bh = 40;

    Rectangle healthButton = { centerX(bw) + 50, 150, bw, bh };
    Rectangle damageButton = { centerX(bw) + 50, 220, bw, bh };
    Rectangle fireButton   = { centerX(bw) + 50, 290, bw, bh };
    Rectangle backButton   = { centerX(bw) + 50, 360, bw, bh };

    DrawRectangleRec(healthButton, GREEN);
    DrawRectangleRec(damageButton, ORANGE);
    DrawRectangleRec(fireButton, PURPLE);
    DrawRectangleRec(backButton, DARKGRAY);

    DrawText(TextFormat("Health +20 - %d", healthUpgradeCost),
             healthButton.x + 110, healthButton.y + 10, 20, BLACK);

    DrawText(TextFormat("+1 Damage - %d", damageUpgradeCost),
             damageButton.x + 110, damageButton.y + 10, 20, BLACK);

    DrawText(TextFormat("+Fire Rate - %d", fireRateUpgradeCost),
             fireButton.x + 110, fireButton.y + 10, 20, WHITE);

    DrawText("BACK", backButton.x + 120, backButton.y + 10, 20, WHITE);

    // click logic (same scope!)
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 m = GetMousePosition();

        if (CheckCollisionPointRec(m, healthButton) && money >= healthUpgradeCost)
        {
        maxHealth += 20;
        health += 20;
        if (health > maxHealth)
        health = maxHealth; //heal when helth upgraded
        }

        if (CheckCollisionPointRec(m, damageButton) && money >= damageUpgradeCost)
        {
            damage++;
            money -= damageUpgradeCost;
            damageUpgradeCost += 25;
        }

        if (CheckCollisionPointRec(m, fireButton) && money >= fireRateUpgradeCost)
        {
           fireCooldown *= 0.9f;
           if (fireCooldown < 0.08f)
           fireCooldown = 0.08f;
        }

        if (CheckCollisionPointRec(m, backButton))
            gameState = MENU;
    }

    EndDrawing();
    continue;
}

        float bw = 300;
        float bh = 40;

        Rectangle healthButton = {
        centerX(bw),
        150,
        bw,
        bh
        };

        Rectangle damageButton = {
            (float)(screenWidth / 2 - 150),
            220.0f,
            300.0f,
            40.0f
        };

        Rectangle fireButton = {
            (float)(screenWidth / 2 - 150),
            290.0f,
            300.0f,
            40.0f
        };

        Rectangle backButton = {
            (float)(screenWidth / 2 - 150),
            360.0f,
            300.0f,
            40.0f
        };
           
            if (CheckCollisionPointRec(GetMousePosition(), healthButton) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && money >= healthUpgradeCost)
            {
                maxHealth += 20;
                money -= healthUpgradeCost;
                healthUpgradeCost += 25;
            }

            if (CheckCollisionPointRec(GetMousePosition(), damageButton) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && money >= damageUpgradeCost)
            {
                damage += 1;
                money -= damageUpgradeCost;
                damageUpgradeCost += 25 ;
            }

            if (CheckCollisionPointRec(GetMousePosition(), fireButton) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && money >= fireRateUpgradeCost)
            {
                fireCooldown -= 0.05f;
                if (fireCooldown < 0.05f) fireCooldown = 0.05f;
                money -= fireRateUpgradeCost;
                fireRateUpgradeCost += 50;
            }

            if (CheckCollisionPointRec(GetMousePosition(), backButton) &&
                IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                gameState = MENU;

            EndDrawing();
            continue;
        }

        // ================= WIN =================
        if (gameState == WIN)
        {
            DrawText("LEVEL COMPLETE!", 220, 150, 40, GREEN);
            DrawText("Press ENTER to return",
                     250, 220, 20, WHITE);

            if (IsKeyPressed(KEY_ENTER))
                gameState = MENU;

            EndDrawing();
            continue;
        }

        // ================= PLAYER =================
        float speed = 250.0f * deltaTime;
        if (IsKeyDown(KEY_D)) playerPosition.x += speed;
        if (IsKeyDown(KEY_A)) playerPosition.x -= speed;
        if (IsKeyDown(KEY_W)) playerPosition.y -= speed;
        if (IsKeyDown(KEY_S)) playerPosition.y += speed;

        Vector2 mousePos = GetMousePosition();
        float dx = mousePos.x - playerPosition.x;
        float dy = mousePos.y - playerPosition.y;
        rotation = atan2f(dy, dx);

        fireTimer -= deltaTime;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && fireTimer <= 0.0f)
        {
            fireTimer = fireCooldown + 0.5;

            Bullet b;
            b.position.x = playerPosition.x + cosf(rotation) * gunWidth;
            b.position.y = playerPosition.y + sinf(rotation) * gunWidth;

            float bulletSpeed = 400.0f;
            b.velocity.x = cosf(rotation) * bulletSpeed;
            b.velocity.y = sinf(rotation) * bulletSpeed;
            b.radius = 5.0f;

            bullets.push_back(b);
        }

        for (auto& bullet : bullets)
        {
            bullet.position.x += bullet.velocity.x * deltaTime;
            bullet.position.y += bullet.velocity.y * deltaTime;
        }
        // ================= ENEMY SPAWN =================
        
        spawnTimer += deltaTime;

if (spawnTimer >= 1.0f)
{
    spawnTimer = 0.0f;

    if (enemies.size() < 5 && enemiesSpawned < enemiesToKill)
    {
        Enemy e;

        int corner = GetRandomValue(0, 3);

        if (corner == 0)      e.position = {0.0f, 0.0f};
        else if (corner == 1) e.position = {(float)screenWidth, 0.0f};
        else if (corner == 2) e.position = {0.0f, (float)screenHeight};
        else                  e.position = {(float)screenWidth, (float)screenHeight};

        e.speed = 70.0f + 20.0f * currentLevel;
        e.health = 2 + currentLevel;
        enemies.push_back(e);
        enemiesSpawned++;
    }
}
        // ================= ENEMY UPDATE =================
        for (auto& enemy : enemies)
        {
            Vector2 dir;
            dir.x = playerPosition.x - enemy.position.x;
            dir.y = playerPosition.y - enemy.position.y;

            float dist = sqrtf(dir.x * dir.x + dir.y * dir.y);

            if (dist > 0)
            {
                dir.x /= dist;
                dir.y /= dist;

                // enemy movement + anti-stuck
                enemy.position.x += dir.x * enemy.speed * deltaTime;
                enemy.position.y += dir.y * enemy.speed * deltaTime;

                // anti-stuck pushback
                if (dist < 25)
                {
                    float pushStrength = 80.0f;

                    enemy.position.x -= dir.x * pushStrength * deltaTime;
                    enemy.position.y -= dir.y * pushStrength * deltaTime;

                    health -= 1;
                }
            }
        }

        // GAME OVER CHECK (IMPORTANT: OUTSIDE enemy loop logic)
        if (health <= 0)
        {
            gameState = GAME_OVER;
        }

        if (gameState == GAME_OVER)
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawText("GAME OVER", 260, 150, 50, RED);
    DrawText("Press ENTER to restart", 240, 220, 20, WHITE);

    if (IsKeyPressed(KEY_ENTER))
    {
        gameState = MENU;

        health = maxHealth;
        enemiesKilled = 0;
        enemiesSpawned = 0;
        enemies.clear();
        bullets.clear();
    }

    EndDrawing();
    continue;
}
        // ================= COLLISION =================
        for (auto& bullet : bullets)
        {
            for (auto& enemy : enemies)
            {
                float dx = bullet.position.x - enemy.position.x;
                float dy = bullet.position.y - enemy.position.y;

                if (sqrtf(dx*dx + dy*dy) < 15)
                    enemy.health -= damage;
            }
        }

        for (auto it = enemies.begin(); it != enemies.end(); )
        {
            if (it->health <= 0)
            {
                enemiesKilled++;
                money += 10;
                it = enemies.erase(it);
            }
            else
                ++it;
        }

        if (enemiesKilled >= enemiesToKill)
        {
            if (currentLevel == maxUnlockedLevel)
                maxUnlockedLevel++;
            gameState = WIN;
        }

        // ================= DRAW boss =================
        auto DrawBoss = [](Vector2 pos)
        {
            DrawRectangle(pos.x + 30, pos.y + 90, 20, 30, PURPLE);
            DrawRectangle(pos.x + 78, pos.y + 90, 20, 30, PURPLE);

            DrawRectangle(pos.x + 20, pos.y + 30, 80, 70, PURPLE);

            DrawPoly({pos.x + 60, pos.y + 65}, 4, 15, 45, PINK);

            DrawTriangle({pos.x + 40, pos.y + 30},
                         {pos.x + 60, pos.y},
                         {pos.x + 80, pos.y + 30},
                         SKYBLUE);

           DrawRectangle(pos.x + 45, pos.y + 15, 10,50, PURPLE); //y x z x= how long not pos //y x z x= how long not pos
           DrawRectangle(pos.x + 65, pos.y + 15, 10, 50, PURPLE);

            
           DrawCircle(pos.x + 70, pos.y + 20, 3, RED);
           DrawCircle(pos.x + 50, pos.y + 20, 3, RED); 
            
            
            
        };

        DrawRectangle(playerPosition.x - bodySize/2,
                  playerPosition.y - bodySize/2,
                  bodySize, bodySize, GREEN);

        Rectangle gun = {playerPosition.x,
                         playerPosition.y - gunHeight/2,
                         gunWidth, gunHeight};

        Vector2 origin = {0, gunHeight/2};
        DrawRectanglePro(gun, origin, rotation * RAD2DEG, DARKGREEN);

        for (auto& bullet : bullets)
            DrawCircleV(bullet.position, bullet.radius, YELLOW);

        for (auto& enemy : enemies)
            DrawCircleV(enemy.position, 15, PURPLE);

        float barWidth = screenWidth * 0.25f;
        float barHeight = 20.0f;
        float x = 20.0f;
        float y = 120.0f;
        DrawRectangle(x, y, barWidth, barHeight, DARKGRAY);
        DrawRectangle(x, y,
              (barWidth * health) / maxHealth,
              barHeight,
              RED);

        DrawText(TextFormat("Level %d", currentLevel), 20, 50, 20, WHITE);
        
        DrawText(TextFormat("Kills: %d / %d", enemiesKilled, enemiesToKill),
         20, 80, 20, WHITE);
        DrawText(TextFormat("DMG: %d", damage), 20, 110, 20, WHITE);
        //max damege 
        
        DrawText(TextFormat("HP: %d / %d", health, maxHealth), 20, 140, 20, WHITE);
        //max hp
DrawText(TextFormat("Fire Rate: %.2f", fireCooldown), 20, 170, 20, WHITE);

EndDrawing();
}

CloseWindow();
return 0;
}
