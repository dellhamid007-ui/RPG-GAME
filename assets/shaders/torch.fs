#version 330

in vec2 fragTexCoord0;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec2 lightPos;
uniform vec2 resolution;
uniform float radius;
uniform float softness;

out vec4 finalColor;

void main()
{
    // flip Y to match the RenderTexture draw
    vec2 uv = vec2(fragTexCoord0.x, 1.0 - fragTexCoord0.y);

    // sample the world texture
    vec4 color = texture(texture0, uv);

    // aspect ratio fix
    vec2 pos = lightPos;
    uv.x *= resolution.x / resolution.y;
    pos.x *= resolution.x / resolution.y;

    float dist = distance(uv, pos);
    float light = smoothstep(radius, radius - softness, dist);

    vec3 darkness = vec3(0.05, 0.05, 0.1);
    color.rgb = mix(darkness, color.rgb, light);

    finalColor = color;
}
