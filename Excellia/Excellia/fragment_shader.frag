// Fragment shader for darkening based on distance from player position
uniform sampler2D texture;
uniform vec2 playerPosition; // Player's position in normalized coordinates

void main() {
    // Get normalized texture coordinates
    vec2 texCoord = gl_TexCoord[0].xy;

    // Calculate distance from the player's position
    float distance = length(texCoord - playerPosition);

    // Darken based on distance (adjust factor to control darkness)
    float darkenFactor = distance * 3.6; // Adjust the multiplication factor as needed

    // Get the texture color
    vec4 color = texture2D(texture, texCoord);

    // Apply darkening effect with gradual decrease in intensity
    vec3 finalColor = color.rgb - vec3(darkenFactor);

    gl_FragColor = vec4(finalColor, color.a);
}