attribute highp vec4 posAtr;
uniform highp mat4 matrix;
uniform lowp mat4 ModelViewMatrix;
varying lowp vec4 Position;
void main()
{
    Position = ModelViewMatrix*posAtr;
    gl_Position = matrix*posAtr;
}
