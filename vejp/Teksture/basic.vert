#version 330 core

layout(location = 0) in vec2 aPos;       // Pozicija tjemena (x, y)
layout(location = 1) in vec2 aTexCoord;  // Teksturne koordinate (s, t)

out vec2 vTexCoord; // Prosledjene teksturne koordinate za fragment šejder

uniform float uYOffset; // Uniform za Y pomeraj
uniform int activePos; //indikator na kom objektu radim

void main()
{

    float uljeY=aPos.y;

    if(activePos == 0 || activePos == 1){  //VEJP i PLUCA
        gl_Position = vec4(aPos, aPos.y , 1.0); // Transformacija pozicije u NDC


    }else if(activePos == 2){    //ULJE
        
        if(aPos.y > -0.7){ //da li je gornji verteks

            uljeY = aPos.y + uYOffset;// Dodaj pomeraj na Y osu

           
        }

        gl_Position = vec4(aPos.x, uljeY, 0.0, 1.0); 
       
    }


    vTexCoord = aTexCoord; // Prosledjivanje teksturnih koordinata
    
}