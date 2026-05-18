void main()
{
    vec2 stepXY = step(vec2(0.5, 0.5), fract(gl_TexCoord[0].xy));
    gl_FragColor = vec4((stepXY.x != stepXY.y) ? 1.0 : 0.0);
}