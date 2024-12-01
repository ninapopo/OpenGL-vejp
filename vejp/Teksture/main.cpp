
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>



#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool isEscPressed = false;
bool escHandled = false;
double xpos, ypos;


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{   //ako je na quit
    if(xpos>520 && xpos<1063 && ypos>510 && ypos<674){
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
     }  
}

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath); //Ucitavanje teksture, izdvojeno u funkciju

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE) { // Provera za ESC taster
        if (action == GLFW_PRESS && !escHandled) {
            isEscPressed = true;
            escHandled = true; // Obrađeno
        }
        if (action == GLFW_RELEASE) {
            isEscPressed = false; // Reset kada se taster pusti
            escHandled = false;  // Spremno za sledeće pritiskanje
        }
    }
}

void enableAlphaBlending() {
    glEnable(GL_BLEND); // Omogućava blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}



int main(void)
{

    if (!glfwInit())
    {
        std::cout << "GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 1600;
    unsigned int wHeight = 1200;
    const char wTitle[] = "[Generic Title]";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);

  

    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    unsigned int unifiedShader = createShader("basic.vert", "basic.frag");

    // Prvi kvadrat - cela slika
    float vertices1[] =
    {   //X    Y      S    T 
        -1.0f,  1.0f,  0.0f, 1.0f, // gornji levi ugao
        1.0f,  1.0f,  1.0f, 1.0f, // gornji desni ugao

       -1.0f, -1.0f,  0.0f, 0.0f, // donji levi ugao
        1.0f, -1.0f,  1.0f, 0.0f  // donji desni ugao
    };

    unsigned int indices1[] = {
        0, 1, 2, // Prvi trougao (gornji levi, gornji desni, donji levi)
        1, 3, 2  // Drugi trougao (gornji desni, donji desni, donji levi)
    };

    
    unsigned int stride = (2 + 2) * sizeof(float);

    unsigned int VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    unsigned int VBO1;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

    unsigned int EBO1;
    glGenBuffers(1, &EBO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices1), indices1, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Drugi kvadrat - vejp
    float vertices2[] = {
        //    x      y      s     t
            -0.85f,  0.4f,  0.0f, 1.0f, // gornji levi ugao
            -0.25f,  0.4f,  1.0f, 1.0f, // gornji desni ugao

            -0.85f, -0.8f,  0.0f, 0.0f, // donji levi ugao
            -0.25f, -0.8f,  1.0f, 0.0f  // donji desni ugao
    };

    unsigned int indices2[] = {
        0, 1, 2,
        1, 3, 2
    };

    unsigned int VAO2, VBO2, EBO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Ulje
    float vertices3[] = {
        //    x      y     
            -0.4f,  0.15f,  // gornji levi ugao
            -0.3f,  0.15f,  // gornji desni ugao

            -0.4f, -0.7f,  // donji levi ugao
            -0.3f, -0.7f// donji desni ugao
    };

    unsigned int indices3[] = {
        0, 1, 2,
        1, 3, 2
    };

    unsigned int VAO3, VBO3, EBO3;
    glGenVertexArrays(1, &VAO3);
    glBindVertexArray(VAO3);

    glGenBuffers(1, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO3);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);
    //xy
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // WARNING kvadrat 
    float vertices4[] = {
        //    x      y      s     t
            -0.6f,  0.45f,  0.0f, 1.0f, // gornji levi ugao
            0.6f,  0.45f,  1.0f, 1.0f,// gornji desni ugao

           -0.6f,  -0.4f,   0.0f, 0.0f, // donji levi ugao
            0.6f,  -0.4f,   1.0f, 0.0f// donji desni ugao
    };

    unsigned int indices4[] = {
        0, 1, 2,
        1, 3, 2
    };

    unsigned int VAO4, VBO4, EBO4;
    glGenVertexArrays(1, &VAO4);
    glBindVertexArray(VAO4);

    glGenBuffers(1, &VBO4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO4);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices4), vertices4, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO4);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO4);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices4), indices4, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // quit kvadrat 
    float vertices5[] = {
        //    x      y      s     t
            -0.35f,  0.15f,  0.0f, 1.0f, // gornji levi ugao
            0.35f,  0.15f,  1.0f, 1.0f,// gornji desni ugao

           -0.35f,  -0.15f,   0.0f, 0.0f, // donji levi ugao
            0.35f,  -0.15f,   1.0f, 0.0f// donji desni ugao
    };

    unsigned int indices5[] = {
        0, 1, 2,
        1, 3, 2
    };

    unsigned int VAO5, VBO5, EBO5;
    glGenVertexArrays(1, &VAO5);
    glBindVertexArray(VAO5);

    glGenBuffers(1, &VBO5);
    glBindBuffer(GL_ARRAY_BUFFER, VBO5);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices5), vertices5, GL_STATIC_DRAW);

    glGenBuffers(1, &EBO5);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO5);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices5), indices5, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //Teksture

    unsigned plucaTexture = loadImageToTexture("res/pluca1.png"); //Ucitavamo teksturu
    unsigned int textureJagoda = loadImageToTexture("res/jagoda.png");
    unsigned int textureBorovnica = loadImageToTexture("res/borovnica.png");
    unsigned int textureMalina = loadImageToTexture("res/malina.png");
    unsigned int textureLimun = loadImageToTexture("res/limun.png");
    unsigned pluca2Texture = loadImageToTexture("res/pluca2.png");
    unsigned pluca3Texture = loadImageToTexture("res/pluca3.png");
    unsigned pluca4Texture = loadImageToTexture("res/pluca4.png");
    unsigned pluca5Texture = loadImageToTexture("res/pluca5.png");
    unsigned int textureWarning = loadImageToTexture("res/WARNING.png");
    unsigned int textureQuit = loadImageToTexture("res/Quit.png");


    //pluca
    glBindTexture(GL_TEXTURE_2D, plucaTexture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR
    //2
    glBindTexture(GL_TEXTURE_2D, pluca2Texture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR
    //3
    glBindTexture(GL_TEXTURE_2D, pluca3Texture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR
    //4
    glBindTexture(GL_TEXTURE_2D, pluca4Texture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR
    //5
    glBindTexture(GL_TEXTURE_2D, pluca5Texture); //Podesavamo teksturu
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR

    //jagoda
    glBindTexture(GL_TEXTURE_2D, textureJagoda);
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR
    //borovnica
    glBindTexture(GL_TEXTURE_2D, textureBorovnica);
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR
    //malina
    glBindTexture(GL_TEXTURE_2D, textureMalina);
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR
    //limun
    glBindTexture(GL_TEXTURE_2D, textureLimun);
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR

    //warning
    glBindTexture(GL_TEXTURE_2D, textureWarning);
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR

    //quit
    glBindTexture(GL_TEXTURE_2D, textureQuit);
    glGenerateMipmap(GL_TEXTURE_2D); //Generisemo mipmape 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//S = U = X    GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// T = V = Y
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);//GL_LINEAR_MIPMAP_LINEAR --GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   //-- GL_NEAREST, GL_LINEAR




    glUseProgram(unifiedShader);
    unsigned uTexLoc = glGetUniformLocation(unifiedShader, "uTex");
    glUniform1i(uTexLoc, 0); // Indeks teksturne jedinice (sa koje teksture ce se citati boje)
    glUseProgram(0);
    //Odnosi se na glActiveTexture(GL_TEXTURE0) u render petlji
    //Moguce je sabirati indekse, tj GL_TEXTURE5 se moze dobiti sa GL_TEXTURE0 + 5 , sto je korisno za iteriranje kroz petlje

    unsigned int activeTexture = textureJagoda; // Podrazumevano: tekstura jagoda
    float potamnjivanje = 0.0f;
    float potrosnjaUlja = 0.0f; // Inicijalna Y koordinata ulja

    //animacija
    int plucaIndex = 0;
    //pauza
    int isPaused = 1;
    glfwSetKeyCallback(window, key_callback);
    //alfa
    enableAlphaBlending();

    //kursor
    unsigned char pixels[16 * 16 * 4]; // 16x16 piksela, 4 kanala (RGBA)
    memset(pixels, 0xff, sizeof(pixels)); // Popunjava piksele belom bojom (0xff u svakom kanalu)

    GLFWimage image;
    image.width = 16;
    image.height = 16;
    image.pixels = pixels; // Pikseli kursora

    GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
    if (!cursor) {
        std::cerr << "Failed to create cursor\n";
        glfwTerminate();
        return -1;
    }
    // Postavljanje kursora na prozor
    glfwSetCursor(window, cursor);

    while (!glfwWindowShouldClose(window))
    {
        //ciscenje ekrana
        glClearColor(0, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);


       
        //Tasteri
       
        if (isEscPressed) {
     
            isEscPressed = false; // Reset nakon obrade pritiska
            isPaused = isPaused*(-1);                               // pauziraj
        }

       
        if(isPaused==1){
        // Menjanje ukusa
        if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            activeTexture = textureMalina;                      // Pritisnuto 'M' -> Malina
        }
        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
            activeTexture = textureLimun;                       // Pritisnuto 'L' -> Limun
        }
        if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
            activeTexture = textureJagoda;                      // Pritisnuto 'J' -> Jagoda
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
            activeTexture = textureBorovnica;                   // Pritisnuto 'B' -> Borovnica
        }
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {


            if (potrosnjaUlja > -0.8) {
                potrosnjaUlja -= 0.001f; // Smanjuj Y koordinatu 
                potamnjivanje -= 0.0007f;
            }                                                           //potamnjivanje
            else {
                potrosnjaUlja = -0.8;
                potamnjivanje = -0.6;
            }

            if (plucaIndex < 800) {                                     //animaija
                plucaIndex=plucaIndex+5;
               
            }
            else plucaIndex = 0;
            

        }
        }// Paused zagrada

       
        

        // Crtanje pluca
        glUseProgram(unifiedShader);
        glBindVertexArray(VAO1);
        glUniform1i(glGetUniformLocation(unifiedShader, "activePos"), 0);//uniform za vert
        glUniform1i(glGetUniformLocation(unifiedShader, "activeTex"), 0);// uniforma za frag
        glActiveTexture(GL_TEXTURE0);//tekstura koja se bind-uje nakon ovoga ce se koristiti sa SAMPLER2D uniformom u sejderu koja odgovara njenom indeksu
       
        
        if (plucaIndex < 121) {
            glBindTexture(GL_TEXTURE_2D, plucaTexture);
        }
        if (plucaIndex>120 && plucaIndex < 201) {
            glBindTexture(GL_TEXTURE_2D, pluca2Texture);
        }
        if (plucaIndex > 200 && plucaIndex < 301) {
            glBindTexture(GL_TEXTURE_2D, pluca3Texture);
        }
        if (plucaIndex > 300 && plucaIndex < 382) {
            glBindTexture(GL_TEXTURE_2D, pluca4Texture);
        }
        if (plucaIndex > 380 && plucaIndex < 521) {
            glBindTexture(GL_TEXTURE_2D, pluca5Texture);                                       //animacija
        }
        if (plucaIndex > 520 && plucaIndex < 601) {
            glBindTexture(GL_TEXTURE_2D, pluca4Texture);
        }
        if (plucaIndex > 600 && plucaIndex < 701) {
            glBindTexture(GL_TEXTURE_2D, pluca3Texture);
        }
        if (plucaIndex > 700 && plucaIndex < 801) {
            glBindTexture(GL_TEXTURE_2D, pluca2Texture);
        }
       

        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
       
        glBindVertexArray(0);


        // Crtanje vejpa
        glBindVertexArray(VAO2);
        glUniform1i(glGetUniformLocation(unifiedShader, "activePos"), 1);//uniform za vert
        glUniform1i(glGetUniformLocation(unifiedShader, "activeTex"), 1);// uniforma za frag
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, activeTexture);//dugmetom odabrana slika(ukus)

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);



        //crtanje ulja
        glBindVertexArray(VAO3);
        glUniform1i(glGetUniformLocation(unifiedShader, "activePos"), 2);//uniform za vert
        glUniform1f(glGetUniformLocation(unifiedShader, "uYOffset"), potrosnjaUlja);
        glUniform1f(glGetUniformLocation(unifiedShader, "darkeningFactor"), potamnjivanje);
        glUniform1i(glGetUniformLocation(unifiedShader, "activeTex"), 2);// uniforma za frag

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        

        //WARNING
        if (potrosnjaUlja < -0.8) {

            glBindVertexArray(VAO4);
            glUniform1i(glGetUniformLocation(unifiedShader, "activePos"), 1);//uniform za vert
            glUniform1i(glGetUniformLocation(unifiedShader, "activeTex"), 1);// uniforma za frag
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureWarning);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

        }


        //Pauza
        if(isPaused==-1){

            //sivi
            glBindVertexArray(VAO1);
            glUniform1i(glGetUniformLocation(unifiedShader, "activePos"), 0);//uniform za vert
            glUniform1i(glGetUniformLocation(unifiedShader, "activeTex"), 3);// uniforma za frag                         
            glActiveTexture(GL_TEXTURE0);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            //quit
            glBindVertexArray(VAO5);
            glUniform1i(glGetUniformLocation(unifiedShader, "activePos"), 1);//uniform za vert
            glUniform1i(glGetUniformLocation(unifiedShader, "activeTex"), 1);// uniforma za frag                       
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textureQuit);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            //kursor pozicije za zatvaranje
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwSetMouseButtonCallback(window, mouse_button_callback);
        }


        //gasi bindove
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glUseProgram(0);


        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
 
   
    //obrisi
    glDeleteTextures(1, &plucaTexture); 
    glDeleteTextures(1, &pluca2Texture);
    glDeleteTextures(1, &pluca3Texture);
    glDeleteTextures(1, &pluca4Texture);
    glDeleteTextures(1, &pluca5Texture);
    glDeleteTextures(1, &textureJagoda);
    glDeleteTextures(1, &textureBorovnica);
    glDeleteTextures(1, &textureLimun);
    glDeleteTextures(1, &textureMalina);
    glDeleteTextures(1, &textureWarning);
    glDeleteTextures(1, &textureQuit);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &EBO1);
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO2);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO3);
    glDeleteBuffers(1, &EBO3);
    glDeleteVertexArrays(1, &VAO3);
    glDeleteBuffers(1, &VBO4);
    glDeleteBuffers(1, &EBO4);
    glDeleteVertexArrays(1, &VAO4);
    glDeleteBuffers(1, &VBO5); 
    glDeleteBuffers(1, &EBO5); 
    glDeleteVertexArrays(1, &VAO5); 
    glDeleteProgram(unifiedShader);
  
    glfwDestroyCursor(cursor); 
    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
    std::string temp = ss.str();
    const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);

    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{

    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);


    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}
