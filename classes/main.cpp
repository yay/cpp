#include <iostream>
#include <string>

// The linkage of the 'theAnswer' symbol is going to be internal:
// only visible to the translation unit where it is defined.
static int theAnswer = 42;

struct Vec2
{
    float x, y;
    void add(const Vec2 &other)
    {
        x += other.x;
        y += other.y;
    }
};

class Player
{
public: // Unlike struct fields, class fields are private by default.
        // Classes with a public section can be used in place of structs.
        // Structs exist in C++ mainly for backward compatibility with C.
    int x, y;
    int speed;

    void move(int dx, int dy)
    {
        x += dx * speed;
        y += dy * speed;
    }
};

enum CustomValues : unsigned char // underlying integral type, 4 bytes by default
{
    A = 5, B, C // B = 6, C = 7
};

class Log
{
public:
    enum Level
    {
        ERROR, WARN, INFO
    };
private:
    Level m_logLevel = INFO;
public:
    void setLevel(Level level)
    {
        m_logLevel = level;
    }

    void error(const std::string &msg)
    {
        if (m_logLevel >= ERROR)
            std::cout << "[ERROR]: " << msg << std::endl;
    }

    void warn(const std::string &msg)
    {
        if (m_logLevel >= WARN)
            std::cout << "[WARN]: " << msg << std::endl;
    }

    void info(const std::string &msg)
    {
        if (m_logLevel >= INFO)
            std::cout << "[INFO]: " << msg << std::endl;
    }
};

void usePlayer()
{
    Player player;
    player.x = 5;
    player.y = 5;
    player.speed = 2;
    player.move(2, 3);
}

void useLog()
{
    Log log;
    log.setLevel(Log::WARN);
    log.error("The sky is falling!");
    log.warn("Attention!");
    log.info("I just pooped.");
}

int main(int, char**) {
    useLog();
}
