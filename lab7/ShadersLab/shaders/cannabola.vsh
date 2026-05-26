void main()
{
    float x = gl_Vertex.x;
    float r = (1.0 + sin(x)) * (1.0 + 0.9 * cos(8.0 * x)) * (1.0 + 0.1 * cos(24.0 * x)) * (0.5 + 0.05 * cos(140.0 * x));
	
    vec4 pos = vec4(
        r * cos(x),
        r * sin(x),
        0.0,
        1.0
    );

    gl_Position = gl_ModelViewProjectionMatrix * pos;
    gl_FrontColor = gl_Color;
}