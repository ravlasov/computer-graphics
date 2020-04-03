#version 130

varying vec2 fTexcoords;
uniform sampler2D textureObj;

void main(void) {
    gl_FragColor = texture2D(textureObj, fTexcoords);
}