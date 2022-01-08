#version 330 core

precision mediump float;

out vec4 color;

uniform float timeUniform;

void main()
{
    color = vec4(
        abs(sin(timeUniform)),
        abs(cos(timeUniform)),
        abs(tan(timeUniform)),
        1.0
    );
}
