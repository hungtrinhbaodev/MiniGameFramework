#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0;

#define MAX_CLIPS 8

uniform vec2 u_quads[MAX_CLIPS * 4];
uniform int u_clipCount;

float edge(vec2 a, vec2 b, vec2 p) {
    return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
}

void main() {
    vec2 p = gl_FragCoord.xy;

    for (int i = 0; i < u_clipCount; i++) {
        int baseIdx = i * 4;
        vec2 p1 = u_quads[baseIdx + 0];
        vec2 p2 = u_quads[baseIdx + 1];
        vec2 p3 = u_quads[baseIdx + 2];
        vec2 p4 = u_quads[baseIdx + 3];

        if (edge(p1, p2, p) < 0.0 ||
            edge(p2, p3, p) < 0.0 ||
            edge(p3, p4, p) < 0.0 ||
            edge(p4, p1, p) < 0.0) 
        {
            discard;
        }
    }

    finalColor = texture(texture0, fragTexCoord) * fragColor;
}
