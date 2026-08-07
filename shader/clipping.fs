#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0;

#define MAX_CLIPS 8

// Uniforms: Custom data passed from C++ to GLSL
uniform sampler2D texture0; // Raylib passes the current texture here automatically
uniform vec2 u_p1;
uniform vec2 u_p2;
uniform vec2 u_p3;
uniform vec2 u_p4;

float edge(vec2 a, vec2 b, vec2 p) {
    return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

void main() {
    // Get fragment's screen pixel coordinates
    vec2 p = gl_FragCoord.xy;

    // Test fragment position against the 4 points of the bounding quad
    if (edge(u_p1, u_p2, p) < 0.0 ||
        edge(u_p2, u_p3, p) < 0.0 ||
        edge(u_p3, u_p4, p) < 0.0 ||
        edge(u_p4, u_p1, p) < 0.0) 
    {
        discard; // Drop/clip this pixel
    }

    // Standard Raylib texture sampling and tinting
    finalColor = texture(texture0, fragTexCoord) * fragColor;
}
