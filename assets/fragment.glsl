#version 330 core

precision mediump float;

out vec4 color;

uniform float timeUniform;

void main()
{
    color = vec4(
        1.0,
        0.0,
        0.5,
        1.0
    );
}
