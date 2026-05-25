uniform float phase;

void main()
{
    float u = gl_Vertex.x;
    float v = gl_Vertex.y;

    float Rs = 2.0;         
    float Rt = 2.0;         
    float rt = 0.8;         

    float v_sphere = v / 2.0;
    vec4 posSphere = vec4(
        Rs * sin(v_sphere) * cos(u),
        Rs * cos(v_sphere), 
        Rs * sin(v_sphere) * sin(u),
        1.0
    );

    vec4 posTorus = vec4(
        (Rt + rt * cos(v)) * cos(u),
        rt * sin(v),
        (Rt + rt * cos(v)) * sin(u),
        1.0
    );

    vec4 currentPos = mix(posSphere, posTorus, phase);

    gl_Position = gl_ModelViewProjectionMatrix * currentPos;
    gl_FrontColor = gl_Color;
}