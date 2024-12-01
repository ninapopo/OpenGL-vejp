#version 330 core


in vec2 vTexCoord;     // Ulaz: Teksturne koordinate
out vec4 FragColor;    // Izlaz: Boja fragmenta

uniform sampler2D uTex1; // Uniform za prvu teksturu
uniform sampler2D uTex2; // Uniform za drugu teksturu
uniform int activeTex;   // Indikator koja tekstura se koristi
uniform float darkeningFactor;


void main()
{
    if (activeTex == 0) {
        
        vec4 potamnjivanje= vec4(darkeningFactor, darkeningFactor, darkeningFactor,1);
        vec4 textureColor = texture(uTex1, vTexCoord);  // Selektuj odgovarajuću teksturu
        if (textureColor.a < 0.1){
            discard;}
        FragColor = textureColor+ potamnjivanje;                // Pluca



    } else if(activeTex == 1){
        vec4 textureColor = texture(uTex1, vTexCoord);
         if (textureColor.a < 0.1){
            discard;}
        FragColor = texture(uTex2, vTexCoord);                   // Vejp



    } else if (activeTex == 2) {
        FragColor = vec4(1.0, 1.0, 0.0, 1.0);                   // Ulje - zuto 
    }else if (activeTex == 3) {
        FragColor = vec4(0.7, 0.7, 0.7, 0.5);                   // pauza - providno sivo
   }
}
