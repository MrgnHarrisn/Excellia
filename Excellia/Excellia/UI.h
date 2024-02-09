#pragma once

#include <SFML/Graphics.hpp>
#include <functional>

class Button : public sf::Drawable, public sf::Transformable {
public:
    Button(const sf::Vector2f& size, const sf::String& text, const sf::Font& font, unsigned int characterSize = 30);

    void set_on_click(std::function<void()> callback);
    void update(sf::Event event, const sf::RenderWindow& window);

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    std::function<void()> on_click;
};
