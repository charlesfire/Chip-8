#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <SFML\Graphics.hpp>

class Screen
{
    public:
        Screen(unsigned int width, unsigned int height);
        virtual ~Screen();
    protected:
    private:
        Screen() = default;
};

#endif // SCREEN_HPP
