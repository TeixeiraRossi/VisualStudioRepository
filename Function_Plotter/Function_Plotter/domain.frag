#version 330 core
out vec4 FragColor;

in vec2 uv;

uniform float zoom;
uniform vec2 offset;

// multiplicação complexa
vec2 cmul(vec2 a, vec2 b)
{
    return vec2(
        a.x*b.x - a.y*b.y,
        a.x*b.y + a.y*b.x
    );
}

// função: f(z) = z²
vec2 func(vec2 z)
{
    return cmul(z, z);
}

vec3 hsv2rgb(vec3 c)
{
    vec3 rgb = clamp(
        abs(mod(c.x*6.0 + vec3(0,4,2), 6.0) - 3.0) - 1.0,
        0.0,
        1.0
    );

    rgb = rgb * rgb * (3.0 - 2.0 * rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

float grid(vec2 coord)
{
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / fwidth(coord);

    float line = min(grid.x, grid.y);
    return 1.0 - clamp(line, 0.0, 1.0);
}

void main()
{
    vec2 z = uv * zoom + offset;
    vec2 w = func(z);

    float magnitude = length(w);
    float angle = atan(w.y, w.x);

    float hue = (angle + 3.1415926) / (2.0 * 3.1415926);

    float logMag = log(magnitude + 1e-6);
    float stripes = 0.5 + 0.5 * cos(6.0 * logMag);

    float value = 0.6 + 0.4 * stripes;

    float saturation = 0.9;

    vec2 gridCoord = w * 1.0; // escala do grid
    float g = grid(gridCoord);

    vec3 color = hsv2rgb(vec3(hue, saturation, value));
    color = mix(color, vec3(0.0), g * 0.5);

    FragColor = vec4(color, 1.0);
}
