#include "UI.h"

Button::Button(const sf::Vector2f& size, const sf::String& text, const sf::Font& font, unsigned int characterSize) {
    m_shape.setSize(size);
    m_shape.setFillColor(sf::Color::White); // Default color, change as needed

    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(characterSize);
    m_text.setFillColor(sf::Color::Black); // Default text color, change as needed

    // Center text on the button
    sf::FloatRect textRect = m_text.getLocalBounds();
    m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    m_text.setPosition(sf::Vector2f(size.x / 2.0f, size.y / 2.0f));
}

void Button::set_on_click(std::function<void()> callback) {
    on_click = callback;
}

bool Button::update(sf::Event& event, const sf::RenderWindow& window) {

    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Button::Left) {
            // Use the default view (UI space) for coordinate mapping
            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Mouse::getPosition(window),
                window.getDefaultView()
            );

            // Transform the shape's local bounds to global bounds (in UI space)
            sf::FloatRect globalBounds = getTransform().transformRect(m_shape.getLocalBounds());

            if (globalBounds.contains(mousePos)) {
                if (on_click != nullptr) {
                    on_click();
                }
                return true;
            }
        }
    }
    
    return false;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform(); // Apply position to the drawable components
    target.draw(m_shape, states);
    target.draw(m_text, states);
}
