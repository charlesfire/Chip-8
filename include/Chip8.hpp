#ifndef CHIP8_HPP
#define CHIP8_HPP

#include <string>
#include <array>
#include <SFML/Graphics/Drawable.hpp>

class Chip8 : public sf::Drawable
{
    public:
        Chip8();
        virtual ~Chip8();
        void loadProgram(const std::string& path);
        void update(const unsigned int deltaTime);
        void setFrequency(unsigned int frequency);
        unsigned int getFrequency() const;
        void pressKey(unsigned int key);
        void releaseKey(unsigned int key);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    private:
        struct Opcodes
        {
            static const int NBOPCODE = 35;
            static std::array<uint16_t, NBOPCODE> mask;
            static std::array<int, NBOPCODE> id;
        };

        uint16_t opcode;
        std::array<uint8_t, 4096> memory;
        std::array<uint8_t, 16> V;

        uint16_t I;
        uint16_t pc;

        std::array<std::array<bool, 32>, 64> gfx;

        uint8_t delay_timer;
        uint8_t sound_timer;

        std::array<uint16_t, 16> stack;
        uint8_t sp;

        std::array<bool, 16> keys;

        unsigned int frequency;
        unsigned int elapsedTime;

        bool emulateCycle();
};

#endif // CHIP8_HPP
