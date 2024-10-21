#version 300 es

uniform highp mat4 transform;

in highp vec4 coord;
out highp vec2 texpos;

void main(void) {
    gl_Position = transform * vec4(coord.xy, .0f, 1.f);
    texpos = coord.zw;
}
