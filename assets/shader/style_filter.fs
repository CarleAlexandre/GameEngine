#version 330 core

in vec2 TexCoord;  // Texture coordinates
uniform sampler2D textureSampler;  // Input texture

out vec4 FragColor;

void main() {
    float texelSizeX = 1.0 / float(textureSize(textureSampler, 0).x);
    float texelSizeY = 1.0 / float(textureSize(textureSampler, 0).y);

    // Sample the current pixel
    vec4 currentColor = texture(textureSampler, TexCoord);

    // Sample pixels for Sobel edge detection
    vec4 topColor = texture(textureSampler, TexCoord + vec2(0, texelSizeY));
    vec4 bottomColor = texture(textureSampler, TexCoord - vec2(0, texelSizeY));
    vec4 leftColor = texture(textureSampler, TexCoord - vec2(texelSizeX, 0));
    vec4 rightColor = texture(textureSampler, TexCoord + vec2(texelSizeX, 0));

    // Sobel operator for edge detection
    float sobelX = (leftColor.r + 2.0 * currentColor.r + rightColor.r) - (bottomColor.r + 2.0 * currentColor.r + topColor.r);
    float sobelY = (bottomColor.r + 2.0 * currentColor.r + topColor.r) - (leftColor.r + 2.0 * currentColor.r + rightColor.r);

    // Combine Sobel X and Y results
    float edge = length(vec2(sobelX, sobelY));

    // Threshold for edge detection
    float edgeThreshold = 0.4;

    // Create black outline if the edge is detected
    if (edge > edgeThreshold) {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);  // Black outline
    } else {
        FragColor = currentColor;  // No outline
    }
}
