#include "Chip8.hpp"
#include "OpcodesMatcher.hpp"

#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

Chip8::Chip8() : opcode(0), I(0), pc(0x200), sp(0), frequency(6400)
{
    Chip8::Opcodes::mask[0]= 0x0000; Chip8::Opcodes::id[0]=0x0FFF;          /* 0NNN */
    Chip8::Opcodes::mask[1]= 0xFFFF; Chip8::Opcodes::id[1]=0x00E0;          /* 00E0 */
    Chip8::Opcodes::mask[2]= 0xFFFF; Chip8::Opcodes::id[2]=0x00EE;          /* 00EE */
    Chip8::Opcodes::mask[3]= 0xF000; Chip8::Opcodes::id[3]=0x1000;          /* 1NNN */
    Chip8::Opcodes::mask[4]= 0xF000; Chip8::Opcodes::id[4]=0x2000;          /* 2NNN */
    Chip8::Opcodes::mask[5]= 0xF000; Chip8::Opcodes::id[5]=0x3000;          /* 3XNN */
    Chip8::Opcodes::mask[6]= 0xF000; Chip8::Opcodes::id[6]=0x4000;          /* 4XNN */
    Chip8::Opcodes::mask[7]= 0xF00F; Chip8::Opcodes::id[7]=0x5000;          /* 5XY0 */
    Chip8::Opcodes::mask[8]= 0xF000; Chip8::Opcodes::id[8]=0x6000;          /* 6XNN */
    Chip8::Opcodes::mask[9]= 0xF000; Chip8::Opcodes::id[9]=0x7000;          /* 7XNN */
    Chip8::Opcodes::mask[10]= 0xF00F; Chip8::Opcodes::id[10]=0x8000;        /* 8XY0 */
    Chip8::Opcodes::mask[11]= 0xF00F; Chip8::Opcodes::id[11]=0x8001;        /* 8XY1 */
    Chip8::Opcodes::mask[12]= 0xF00F; Chip8::Opcodes::id[12]=0x8002;        /* 8XY2 */
    Chip8::Opcodes::mask[13]= 0xF00F; Chip8::Opcodes::id[13]=0x8003;        /* BXY3 */
    Chip8::Opcodes::mask[14]= 0xF00F; Chip8::Opcodes::id[14]=0x8004;        /* 8XY4 */
    Chip8::Opcodes::mask[15]= 0xF00F; Chip8::Opcodes::id[15]=0x8005;        /* 8XY5 */
    Chip8::Opcodes::mask[16]= 0xF00F; Chip8::Opcodes::id[16]=0x8006;        /* 8XY6 */
    Chip8::Opcodes::mask[17]= 0xF00F; Chip8::Opcodes::id[17]=0x8007;        /* 8XY7 */
    Chip8::Opcodes::mask[18]= 0xF00F; Chip8::Opcodes::id[18]=0x800E;        /* 8XYE */
    Chip8::Opcodes::mask[19]= 0xF00F; Chip8::Opcodes::id[19]=0x9000;        /* 9XY0 */
    Chip8::Opcodes::mask[20]= 0xF000; Chip8::Opcodes::id[20]=0xA000;        /* ANNN */
    Chip8::Opcodes::mask[21]= 0xF000; Chip8::Opcodes::id[21]=0xB000;        /* BNNN */
    Chip8::Opcodes::mask[22]= 0xF000; Chip8::Opcodes::id[22]=0xC000;        /* CXNN */
    Chip8::Opcodes::mask[23]= 0xF000; Chip8::Opcodes::id[23]=0xD000;        /* DXYN */
    Chip8::Opcodes::mask[24]= 0xF0FF; Chip8::Opcodes::id[24]=0xE09E;        /* EX9E */
    Chip8::Opcodes::mask[25]= 0xF0FF; Chip8::Opcodes::id[25]=0xE0A1;        /* EXA1 */
    Chip8::Opcodes::mask[26]= 0xF0FF; Chip8::Opcodes::id[26]=0xF007;        /* FX07 */
    Chip8::Opcodes::mask[27]= 0xF0FF; Chip8::Opcodes::id[27]=0xF00A;        /* FX0A */
    Chip8::Opcodes::mask[28]= 0xF0FF; Chip8::Opcodes::id[28]=0xF015;        /* FX15 */
    Chip8::Opcodes::mask[29]= 0xF0FF; Chip8::Opcodes::id[29]=0xF018;        /* FX18 */
    Chip8::Opcodes::mask[30]= 0xF0FF; Chip8::Opcodes::id[30]=0xF01E;        /* FX1E */
    Chip8::Opcodes::mask[31]= 0xF0FF; Chip8::Opcodes::id[31]=0xF029;        /* FX29 */
    Chip8::Opcodes::mask[32]= 0xF0FF; Chip8::Opcodes::id[32]=0xF033;        /* FX33 */
    Chip8::Opcodes::mask[33]= 0xF0FF; Chip8::Opcodes::id[33]=0xF055;        /* FX55 */
    Chip8::Opcodes::mask[34]= 0xF0FF; Chip8::Opcodes::id[34]=0xF065;        /* FX65 */

    memory.fill(0);

    memory[0] = 0xF0; memory[1] = 0x90; memory[2] = 0x90; memory[3] = 0x90; memory[4] = 0xF0; // 0
    memory[5] = 0x20; memory[6] = 0x60; memory[7] = 0x20; memory[8] = 0x20; memory[9] = 0x70; // 1
    memory[10] = 0xF0; memory[11] = 0x10; memory[12] = 0xF0; memory[13] = 0x80; memory[14] = 0xF0; // 2
    memory[15] = 0xF0; memory[16] = 0x10; memory[17] = 0xF0; memory[18] = 0x10; memory[19] = 0xF0; // 3
    memory[20] = 0x90; memory[21] = 0x90; memory[22] = 0xF0; memory[23] = 0x10; memory[24] = 0x10; // 4
    memory[25] = 0xF0; memory[26] = 0x80; memory[27] = 0xF0; memory[28] = 0x10; memory[29] = 0xF0; // 5
    memory[30] = 0xF0; memory[31] = 0x80; memory[32] = 0xF0; memory[33] = 0x90; memory[34] = 0xF0; // 6
    memory[35] = 0xF0; memory[36] = 0x10; memory[37] = 0x20; memory[38] = 0x40; memory[39] = 0x40; // 7
    memory[40] = 0xF0; memory[41] = 0x90; memory[42] = 0xF0; memory[43] = 0x90; memory[44] = 0xF0; // 8
    memory[45] = 0xF0; memory[46] = 0x90; memory[47] = 0xF0; memory[48] = 0x10; memory[49] = 0xF0; // 9
    memory[50] = 0xF0; memory[51] = 0x90; memory[52] = 0xF0; memory[53] = 0x90; memory[54] = 0x90; // A
    memory[55] = 0xE0; memory[56] = 0x90; memory[57] = 0xE0; memory[58] = 0x90; memory[59] = 0xE0; // B
    memory[60] = 0xF0; memory[61] = 0x80; memory[62] = 0x80; memory[63] = 0x80; memory[64] = 0xF0; // C
    memory[65] = 0xE0; memory[66] = 0x90; memory[67] = 0x90; memory[68] = 0x90; memory[69] = 0xE0; // D
    memory[70] = 0xF0; memory[71] = 0x80; memory[72] = 0xF0; memory[73] = 0x80; memory[74] = 0xF0; // E
    memory[75] = 0xF0; memory[76] = 0x80; memory[77] = 0xF0; memory[78] = 0x80; memory[79] = 0x80; // F

    std::srand(std::time(nullptr));

    for (auto& i : gfx)
                i.fill(false);
}

Chip8::~Chip8()
{
    //dtor
}


bool Chip8::emulateCycle()
{
    opcode = memory[pc] << 8 | memory[pc + 1];
    std::cout << "Opcode #" << OpcodesMatcher<Chip8::Opcodes>::getActionId(opcode) << std::endl;

    unsigned char b1, b2, b3;
    b3 = (opcode&(0x0F00)) >> 8;
    b2 = (opcode&(0x00F0)) >> 4;
    b1 = (opcode&(0x000F));

    bool ret = false;

    switch(OpcodesMatcher<Chip8::Opcodes>::getActionId(opcode))
    {
        case 0: //Unimplemented opcode
            break;
        case 1: //Clear screen
            for (auto& i : gfx)
                i.fill(false);
            break;
        case 2: //Return from the last jump
            pc = stack[--sp];
            break;
        case 3: //Jump to NNN
            pc = ((b3 << 8) + (b2 << 4) + b1) - 2;
            break;
        case 4: //Run the subroutine at NNN
            if (sp < 15)
            {
                stack[sp++] = pc;
                pc = ((b3 << 8) + (b2 << 4) + b1) - 2;
            }
            else
                std::cerr << "Stack overflow!" << std::endl;
            break;
        case 5: //Skip next instruction if Vx == NN
            if (V[b3] == (b2 << 4) + b1)
                pc += 2;
            break;
        case 6: //Skip next instruction if Vx != NN
            if (V[b3] != (b2 << 4) + b1)
                pc += 2;
            break;
        case 7: //Skip next instruction if Vx == Vy
            if (V[b3] == V[b2])
                pc += 2;
            break;
        case 8: //Set Vx to NN
            V[b3] = (b2 << 4) + b1;
            break;
        case 9: //Add NN to Vx
            V[b3] += (b2 << 4) + b1;
            break;
        case 10: //Set Vx to Vy
            V[b3] = V[b2];
            break;
        case 11: //Set Vx to Vx OR Vy
            V[b3] = V[b3] | V[b2];
            break;
        case 12: //Set Vx to Vx AND Vy
            V[b3] = V[b3] & V[b2];
            break;
        case 13: //Set Vx to Vx XOR Vy
            V[b3] = V[b3] ^ V[b2];
            break;
        case 14: //Add Vy to Vx and set VF to 1 if Vy + Vx is out of memory
            if (V[b3] + V[b2] > 0xFF)
                V[15] = 1;
            else
                V[15] = 0;
            V[b3] += V[b2];
            break;
        case 15: //Substract Vy to Vx and set VF to 0 if Vx - Vy < 0
            if (V[b3] - V[b2] < 0)
                V[15] = 0;
            else
                V[15] = 1;
            V[b3] = V[b2] - V[b3];
            break;
        case 16: //Set VF to the lowest bit of Vx and shift right Vx
            V[15] = V[b3] << 15;
            V[15] = V[15] >> 15;
            V[b3] = V[b3] >> 1;
            break;
        case 17: //Set Vx to Vy - Vx and set VF to 0 if Vy - Vx < 0
            if (V[b3] > V[b2])
                V[15] = 0;
            else
                V[15] = 1;
            V[b3] = V[b2] - V[b3];
            break;
        case 18: //Set VF to the highest bit of Vx and shift left Vx
            V[15] = V[b3] >> 7;
            V[b3] = V[b3] << 1;
            break;
        case 19: //Skip next instruction if Vx != Vy
            if (V[b3] != V[b2])
                pc += 2;
            break;
        case 20: //Set I to NNN
            I = ((b3 << 8) + (b2 << 4) + b1);
            break;
        case 21: //Jump to the instruction at NNN + V0
            pc = ((b3 << 8) + (b2 << 4) + b1) + V[0] - 2;
            break;
        case 22: //Set Vx to a random number, masked by NN
            V[b3] = (std::rand() % 256) & ((b2 << 4) + b1);
            std::cout << "Random number : " << (int)V[b3] << std::endl;
            break;
        case 23: //Draw a sprite at Vx, Vy with a height of N pixel and reading pixel starting at I
            for (int i(0); i < b1; i++)
            {
                int y = (V[b2] + i) % 32;
                for (int j(0); j < 8; j++)
                {
                    int x = (V[b3] + j) % 64;
                    if (memory[I + i] & (0x1 << (7 - j)))
                    {
                        if (gfx[x][y])
                        {
                            gfx[x][y] = false;
                            V[15] = 1;
                        }
                        else
                            gfx[x][y] = true;
                    }
                }
            }
            break;
        case 24: //Skip next instruction if the key at Vx is pressed
            if (keys[V[b3]])
                pc += 2;
            break;
        case 25: //Skip next instruction if the key at Vx isn't pressed
            if (!keys[V[b3]])
                pc += 2;
            break;
        case 26: //Set Vx to the value of the delay timer
            V[b3] = delay_timer;
            break;
        case 27: //Wait for a key press and stock it in Vx
            ret = true;
            for (int i(0); i < keys.size(); i++)
            {
                if (keys[i])
                {
                    V[b3] = i;
                    ret = false;
                    break;
                }
            }
            break;
        case 28: //Set the delay timer to Vx
            delay_timer = V[b3];
            break;
        case 29: //Set the sound timer to Vx
            sound_timer = V[b3];
            break;
        case 30: //Add Vx to I and set VF to 1 if there is an overflow
            if (V[b3] + I > 0xFFF)
                V[15] = 1;
            else
                V[15] = 0;
            I += V[b3];
            break;
        case 31: //Set I to the place of the character in Vx
            I = V[b3] * 5;
            break;
        case 32: //Save the decimal representation of Vx at I, I + 1, I + 2
            memory[I] = V[b3] % 1000 - V[b3] % 100 - V[b3] % 10;
            memory[I + 1] = V[b3] % 100 - V[b3] % 10;
            memory[I + 2] = V[b3] % 10;
            break;
        case 33: //Store V0 to Vx in memory starting at address I
            for (int i(0); i <= b3; i++)
                memory[I + i] = V[i];
            break;
        case 34: //Fill V0 to Vx with values from memory starting at address I
            for (int i(0); i <= b3; i++)
                V[i] = memory[I + i];
            break;
        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            break;
    }
    if (!ret)
        pc += 2;
    return ret;
}

void Chip8::loadProgram(const std::string& path)
{
    std::ifstream file;
    std::streampos fileSize;
    char* buffer = nullptr;
    file.open(path, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        fileSize = file.tellg();
        buffer = new char [fileSize];
        file.seekg (0, std::ios::beg);
        file.read (buffer, fileSize);
        file.close();

        for (int i = 0; i < fileSize; ++i)
        {
            memory[i + 512] = buffer[i];
        }

        delete[] buffer;
    }
}

void Chip8::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    sf::RectangleShape rect({16, 16});
    rect.setFillColor(sf::Color::Green);
    rect.setOutlineThickness(1);
    rect.setOutlineColor(sf::Color::Red);
    for (int i(0); i < 64; i++)
    {
        for (int j(0); j < 32; j++)
        {
            if (gfx[i][j])
            {
                rect.setPosition(i * 16, j * 16);
                target.draw(rect, states);
            }
        }
    }
}

void Chip8::update(const unsigned int deltaTime)
{
    elapsedTime += deltaTime;
    if (elapsedTime > 16)
    {
        if(delay_timer > 0)
            --delay_timer;

        if(sound_timer > 0)
        {
            std::cout << '\a';
            --sound_timer;
        }
    }

    for (int i(0); i < (frequency * deltaTime)/1000; i++)
        if (emulateCycle())
            return;
}

void Chip8::setFrequency(unsigned int frequency)
{
    this->frequency = frequency;
}

unsigned int Chip8::getFrequency() const
{
    return frequency;
}

void Chip8::pressKey(unsigned int key)
{
    keys[key] = true;
}

void Chip8::releaseKey(unsigned int key)
{
    keys[key] = false;
}

std::array<uint16_t, Chip8::Opcodes::NBOPCODE> Chip8::Opcodes::mask;
std::array<int, Chip8::Opcodes::NBOPCODE> Chip8::Opcodes::id;
