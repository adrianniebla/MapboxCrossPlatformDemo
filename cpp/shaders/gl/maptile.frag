#version 300 es

uniform mediump vec4 color;
uniform bool hasTexture;
uniform mediump sampler2D tex;

in mediump vec2 texpos;
out mediump vec4 fragColor;

void main(void) {
    if (hasTexture) {
        fragColor = texture(tex, texpos);
    } else {
        fragColor = color;
    }
}
