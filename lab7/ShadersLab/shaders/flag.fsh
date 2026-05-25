varying vec2 v_pos;

void main()
{
    vec2 pos = gl_TexCoord[0].xy;
    vec3 color;

    if (pos.y > 0.6666)
    {
        color = vec3(1.0, 1.0, 1.0);
    }
    else if (pos.y > 0.3333)
    {
        color = vec3(0.0, 0.0, 1.0);
    }
    else
    {
        color = vec3(1.0, 0.0, 0.0);
    }

    gl_FragColor = vec4(color, 1.0);
}