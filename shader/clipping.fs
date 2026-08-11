#version 330

in vec2 fragTexCoord;
in vec4 fragColor;
out vec4 finalColor;

uniform sampler2D texture0;

uniform vec2 u_clip_points[64];
uniform int u_point_count; 

float edge(vec2 a, vec2 b, vec2 p) {
    return (p.x - a.x) * (b.y - a.y) - (p.y - a.y) * (b.x - a.x);
} 

void main() {
    vec2 p = gl_FragCoord.xy;

    int number_point_in_clipping_area = 4;
    for (int i = 0; i < u_point_count / number_point_in_clipping_area; i++) {
        int start = i * number_point_in_clipping_area;
        int end = start + number_point_in_clipping_area;
        for (int j = start; j < end; j++) {
            int current = j;
            int next = start + ((j + 1) % number_point_in_clipping_area);
            if (edge(u_clip_points[current], u_clip_points[next], p) > 0.0) {
                discard;
            }
        }
    }

    vec4 matching = texture(texture0, fragTexCoord);
    if (matching.w <= 0) {
        discard;
    }
    finalColor = matching * fragColor;
}
