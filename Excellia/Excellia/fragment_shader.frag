// Fragment shader for dithering
uniform sampler2D texture;
void main() {
    vec2 texCoord = gl_TexCoord[0].xy;
    vec4 color = texture2D(texture, texCoord);

    // Define a 4x4 Bayer matrix
    const mat4 bayerMatrix = mat4(
        0.00, 0.50, 0.13, 0.63,
        0.75, 0.25, 0.88, 0.38,
        0.19, 0.69, 0.06, 0.56,
        0.94, 0.44, 0.81, 0.31
    );

    // Normalize texture coordinates
    vec2 texSize = textureSize(texture, 0);
    texCoord *= texSize;

    // Calculate the pattern index based on texture coordinates
    int x = int(mod(texCoord.x, 4.0));
    int y = int(mod(texCoord.y, 4.0));
    float threshold = bayerMatrix[x][y];

    // Apply dithering by comparing the color to the threshold
    vec3 ditheredColor = color.rgb;
    if (ditheredColor.r > threshold) {
        ditheredColor = vec3(1.0);
    } else {
        ditheredColor = vec3(0.0);
    }

    gl_FragColor = vec4(ditheredColor, color.a);
}