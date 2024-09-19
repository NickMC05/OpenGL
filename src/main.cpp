#include"Model.h"
#include"Mesh.h"

using namespace glm;
using namespace std;

/*
cd bin
mingw32-make
.\main.exe
*/

// Free Code Camp Tutorial

void CreateWindow()
{
    glfwInit();

    // setup
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 800, "Title", NULL /*full screen*/, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // wait until close window
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void CreateWindowWithBackgroundColor()
{
    // init
    glfwInit();

    // setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create OpenGL window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Title", NULL /*full screen*/, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // load GLAD to configure OpenGL
    gladLoadGL();
    // specify viewport for OpenGL (x, y, x, y)
    glViewport(0, 0, 800, 800);
    // give background color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // RGBA
    // clean and swap buffer screen
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    // wait until close window
    while (!glfwWindowShouldClose(window))
    {
        // get GLFW events
        glfwPollEvents();
    }

    // finish
    glfwDestroyWindow(window);
    glfwTerminate();
}

void CreateTriangle()
{
    // Vertex Shader source code
    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    //Fragment Shader source code
    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n" // fragment color here
    "}\n\0";

    // init
    glfwInit();

    // setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    GLfloat triangle_vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f
    };

    // create OpenGL window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Title", NULL /*full screen*/, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // load GLAD to configure OpenGL
    gladLoadGL();
    // specify viewport for OpenGL (x, y, x, y)
    glViewport(0, 0, 800, 800);

    // OpenGL integer to create vertex shader (with source on top)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // OpenGL integer to create fragment shader (with source on top)
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // to use both shader, need shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader); // put vertex
    glAttachShader(shaderProgram, fragmentShader); // put fragment
    glLinkProgram(shaderProgram);

    // no longer need shader integer data
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // now need to intepret shader program to OpenGL
    GLuint VAO; // to make OpenGL able to find the vertices
    GLuint VBO; // array of references (for now need 1)
    
    glGenVertexArrays(1, &VAO); // do VAO before VOB
    glGenBuffers(1, &VBO); // buffer for 1 object
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer that object
    // store vertices to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);
    // STREAM: modified once, used a few times
    // STATIC: vertices will be modified once, used many times
    // DYNAMIC: vertices will be modified many times, used many times
    // choose DRAW (vertices wll be modified), READ, COPY

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // give index to vertex attribute we want to use
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // to prevent VAO VBO changes by combining them in bind
    glBindVertexArray(0); // need to have same ordering

    // give background color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // RGBA
    // clean and swap buffer screen
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    // wait until close window
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // RGBA
        // clean and swap buffer screen
        glClear(GL_COLOR_BUFFER_BIT);
        // activate shader program
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);

        // get GLFW events
        glfwPollEvents();
    }

    // delete shaders
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // finish
    glfwDestroyWindow(window);
    glfwTerminate();
}

void CreateTriangleWithHole()
{
    // Vertex Shader source code
    const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
    //Fragment Shader source code
    const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.8f, 0.2f, 0.1f, 1.0f);\n" // fragment color here
    "}\n\0";

    // init
    glfwInit();

    // setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    GLfloat three_triangle_vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
        -0.5f/ 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
        0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f
    };
    /*
    triangle =     2   (3 4 5 empty hole)
                 3   4
               0   5   1
    */
   GLuint indices[] =
   {
        0, 3, 5, // lower left triangle
        3, 2, 4, // lower right triangle
        5, 4, 1  // upper triangle
   };

    // create OpenGL window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Title", NULL /*full screen*/, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // load GLAD to configure OpenGL
    gladLoadGL();
    // specify viewport for OpenGL (x, y, x, y)
    glViewport(0, 0, 800, 800);

    // OpenGL integer to create vertex shader (with source on top)
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // OpenGL integer to create fragment shader (with source on top)
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // to use both shader, need shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader); // put vertex
    glAttachShader(shaderProgram, fragmentShader); // put fragment
    glLinkProgram(shaderProgram);

    // no longer need shader integer data
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // now need to intepret shader program to OpenGL
    GLuint VAO; // to make OpenGL able to find the vertices
    GLuint VBO; // array of references (for now need 1)
    GLuint EBO; // generate index buffer
    
    glGenVertexArrays(1, &VAO); // do VAO before VOB
    glGenBuffers(1, &VBO); // buffer for 1 object
    glGenBuffers(1, &EBO); // for index buffer
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind buffer that object
    // store vertices to VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(three_triangle_vertices), three_triangle_vertices, GL_STATIC_DRAW);
    // STREAM: modified once, used a few times
    // STATIC: vertices will be modified once, used many times
    // DYNAMIC: vertices will be modified many times, used many times
    // choose DRAW (vertices wll be modified), READ, COPY
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // now link to indices array

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // give index to vertex attribute we want to use
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // to prevent VAO VBO changes by combining them in bind
    glBindVertexArray(0); // need to have same ordering
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // prevent VAO to use EBO

    // give background color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // RGBA
    // clean and swap buffer screen
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    // wait until close window
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // RGBA
        // clean and swap buffer screen
        glClear(GL_COLOR_BUFFER_BIT);
        // activate shader program
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3); to draw triangle
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // get GLFW events
        glfwPollEvents();
    }

    // delete shaders
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // finish
    glfwDestroyWindow(window);
    glfwTerminate();
}

// OrganizedCodes() needs old version of classes
/*
void OrganizedCodes()
{
    // init
    glfwInit();

    // setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    GLfloat three_triangle_vertices[] =
    {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f,
        -0.5f/ 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
        0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,
        0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f
    };
    
    //triangle =     2   (3 4 5 empty hole)
    //             3   4
    //           0   5   1
    
   GLuint indices[] =
   {
        0, 3, 5, // lower left triangle
        3, 2, 4, // lower right triangle
        5, 4, 1  // upper triangle
   };

    // create OpenGL window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Title", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // load GLAD to configure OpenGL
    gladLoadGL();
    // specify viewport for OpenGL (x, y, x, y)
    glViewport(0, 0, 800, 800);

    Shader shaderProgram("../default/default.vert", "../default/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(three_triangle_vertices, sizeof(three_triangle_vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0);
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // wait until close window
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // RGBA
        // clean and swap buffer screen
        glClear(GL_COLOR_BUFFER_BIT);
        // activate shader program
        shaderProgram.Activate();
        VAO1.Bind();
        // glUseProgram(shaderProgram);
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3); to draw triangle
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // get GLFW events
        glfwPollEvents();
    }

    // delete shaders
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    // finish
    glfwDestroyWindow(window);
    glfwTerminate();
}
*/

// CreateTriangleWithShaders()
/*
void CreateTriangleWithShaders()
{
    // init
    glfwInit();

    // setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    GLfloat three_triangle_vertices[] =
    {   // Coordinates                                  // Colors
        -0.5f,  -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f,  0.02f,
         0.5f,  -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f,  0.02f,
         0.0f,   0.5f * float(sqrt(3)) * 2 / 3, 0.0f,   1.0f, 0.6f,  0.32f,
        -0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f,   0.9f, 0.45f, 0.17f,
         0.25f,  0.5f * float(sqrt(3)) / 6,     0.0f,   0.9f, 0.45f, 0.17f,
         0.0f,  -0.5f * float(sqrt(3)) / 3,     0.0f,   0.8f, 0.3f,  0.02f
    };
    
   GLuint indices[] =
   {
        0, 3, 5, // lower left triangle
        3, 2, 4, // lower right triangle
        5, 4, 1  // upper triangle
   };

    // create OpenGL window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Title", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // load GLAD to configure OpenGL
    gladLoadGL();
    // specify viewport for OpenGL (x, y, x, y)
    glViewport(0, 0, 800, 800);

    Shader shaderProgram("../shaders/default.vert", "../shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(three_triangle_vertices, sizeof(three_triangle_vertices));
    EBO EBO1(indices, sizeof(indices));

    // links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    // unbind all to prevent accident modification
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // wait until close window
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // RGBA
        // clean and swap buffer screen
        glClear(GL_COLOR_BUFFER_BIT);
        // activate shader program
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        VAO1.Bind();
        // glUseProgram(shaderProgram);
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3); to draw triangle
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // get GLFW events
        glfwPollEvents();
    }

    // delete shaders
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();

    // finish
    glfwDestroyWindow(window);
    glfwTerminate();
}
*/

// void CreateTexture()
/*
void CreateTexture()
{
    // init
    glfwInit();

    // setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    GLfloat square_vertices[] =
    {   // Coordinates          // Colors           // Coordinates
        -0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,   1.0f, 0.0f
    };
   GLuint indices[] =
   {
        0, 2, 1, // upper triangle
        0, 3, 2  // lower triangle
   };

    // create OpenGL window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Title", NULL full screen, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // load GLAD to configure OpenGL
    gladLoadGL();
    // specify viewport for OpenGL (x, y, x, y)
    glViewport(0, 0, 800, 800);

    Shader shaderProgram("../shaders/default.vert", "../shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(square_vertices, sizeof(square_vertices));
    EBO EBO1(indices, sizeof(indices));

    // links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // unbind all to prevent accident modification
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("../textures/image0.png", &widthImg, &heightImg, &numColCh, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // GL_NEAREST = pixel art
    // GL_LINEAR = blur

    // Fill/else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // S axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // T axis

    // Border
    float flatColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes); // delete data
    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture prevent accident

    GLuint tex0Uni = glGetUniformLocation(shaderProgram.ID, "tex0");
    shaderProgram.Activate();
    glUniform1i(tex0Uni, 0);

    // wait until close window
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // RGBA
        // clean and swap buffer screen
        glClear(GL_COLOR_BUFFER_BIT);
        // activate shader program
        shaderProgram.Activate();
        glUniform1f(uniID, 0.5f);
        glBindTexture(GL_TEXTURE_2D, texture);
        VAO1.Bind();
        // glUseProgram(shaderProgram);
        // glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 3); to draw triangle
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);

        // get GLFW events
        glfwPollEvents();
    }

    // delete shaders
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    glDeleteTextures(1, &texture);
    shaderProgram.Delete();

    // finish
    glfwDestroyWindow(window);
    glfwTerminate();
}

// void Create3D()
/*
void Create3D()
{
    // constants
    const unsigned int width = 800;
    const unsigned int height = 800;

    // init
    glfwInit();

    // setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    GLfloat pyramid_vertices[] =
    {   // Coordinates          // Colors           // Coordinates
        -0.5f, -0.4f,  0.5f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
        -0.5f, -0.4f, -0.5f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
         0.5f, -0.4f, -0.5f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
         0.5f, -0.4f,  0.5f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
         0.0f,  0.4f,  0.0f,     0.92f, 0.86f, 0.76f,   2.5f, 5.0f
    };
   GLuint indices[] =
   {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
   };

    // create OpenGL window
    GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // load GLAD to configure OpenGL
    gladLoadGL();
    // specify viewport for OpenGL (x, y, x, y)
    glViewport(0, 0, width, height);

    Shader shaderProgram("../shaders/default.vert", "../shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(pyramid_vertices, sizeof(pyramid_vertices));
    EBO EBO1(indices, sizeof(indices));

    // links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // unbind all to prevent accident modification
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("../textures/image0.png", &widthImg, &heightImg, &numColCh, 0);
    Texture memeImage("../textures/image0.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	memeImage.texUnit(shaderProgram, "tex0", 0);

    float rotation = 0.0f;
    double previousTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    // wait until close window
    while (!glfwWindowShouldClose(window))
    {
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

        double currentTime = glfwGetTime();
        if (currentTime - previousTime >= 1/60) {
            rotation += 0.01f;
            previousTime = currentTime;
        }

        mat4 model = mat4(1.0f);
        mat4 view = mat4(1.0f);
        mat4 proj = mat4(1.0f);

        // rotate x
        model = rotate(model, radians(rotation/2), vec3(1.0f, 0.0f, 0.0f));
        // rotate y
        model = rotate(model, radians(rotation), vec3(0.0f, 1.0f, 0.0f));
        // rotate z
        model = rotate(model, radians(rotation/3), vec3(0.0f, 0.0f, 1.0f));

        view = translate(view, vec3(0.0f, 0.0f, -2.0f));
        proj = perspective(radians(45.0f), (float)(width/height), 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(proj));

		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(uniID, 0.5f);
		// Binds texture so that is appears in rendering
		memeImage.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
    }

    // delete shaders
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    memeImage.Delete();
    shaderProgram.Delete();

    // finish
    glfwDestroyWindow(window);
    glfwTerminate();
}
*/

// CameraMovement()
/*
void CameraMovement()
{
    // constants
    const unsigned int width = 800;
    const unsigned int height = 800;

    // init
    glfwInit();

    // setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    GLfloat pyramid_vertices[] =
    {   // Coordinates          // Colors           // Coordinates
        -0.5f, -0.4f,  0.5f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
        -0.5f, -0.4f, -0.5f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
         0.5f, -0.4f, -0.5f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
         0.5f, -0.4f,  0.5f,     0.83f, 0.70f, 0.44f,   5.0f, 0.0f,
         0.0f,  0.4f,  0.0f,     0.92f, 0.86f, 0.76f,   2.5f, 5.0f
    };
   GLuint indices[] =
   {
        0, 1, 2,
        0, 2, 3,
        0, 1, 4,
        1, 2, 4,
        2, 3, 4,
        3, 0, 4
   };

    // create OpenGL window
    GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL full screen, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // load GLAD to configure OpenGL
    gladLoadGL();
    // specify viewport for OpenGL (x, y, x, y)
    glViewport(0, 0, width, height);

    Shader shaderProgram("../shaders/default.vert", "../shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(pyramid_vertices, sizeof(pyramid_vertices));
    EBO EBO1(indices, sizeof(indices));

    // links VBO to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // unbind all to prevent accident modification
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    //GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // Texture
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("../textures/image0.png", &widthImg, &heightImg, &numColCh, 0);
    Texture memeImage("../textures/image0.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	memeImage.texUnit(shaderProgram, "tex0", 0);

    //float rotation = 0.0f;
    //double previousTime = glfwGetTime();

    glEnable(GL_DEPTH_TEST);

    // Camera object
    Camera camera(width, height, vec3(0.0f, 0.0f, 2.0f));

    // wait until close window
    while (!glfwWindowShouldClose(window))
    {
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        double currentTime = glfwGetTime();
        if (currentTime - previousTime >= 1/60) {
            rotation += 0.01f;
            previousTime = currentTime;
        }

        mat4 model = mat4(1.0f);
        mat4 view = mat4(1.0f);
        mat4 proj = mat4(1.0f);

        // rotate x
        model = rotate(model, radians(rotation/2), vec3(1.0f, 0.0f, 0.0f));
        // rotate y
        model = rotate(model, radians(rotation), vec3(0.0f, 1.0f, 0.0f));
        // rotate z
        model = rotate(model, radians(rotation/3), vec3(0.0f, 0.0f, 1.0f));

        view = translate(view, vec3(0.0f, 0.0f, -2.0f));
        proj = perspective(radians(45.0f), (float)(width/height), 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(proj));

		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		//glUniform1f(uniID, 0.5f);
		// Binds texture so that is appears in rendering
		memeImage.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
    }

    // delete shaders
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    memeImage.Delete();
    shaderProgram.Delete();

    // finish
    glfwDestroyWindow(window);
    glfwTerminate();
}
*/

// void Light()
/*
void Light()
{
    const unsigned int width = 800;
    const unsigned int height = 800;

    glfwInit();

    // Setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    // Vertices coordinates
    GLfloat pyramid_vertices[] =
    { //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 5.0f,      0.0f, -1.0f, 0.0f, // Bottom side
         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, -1.0f, 0.0f, // Bottom side

        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,     -0.8f, 0.5f,  0.0f, // Left Side
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,     -0.8f, 0.5f,  0.0f, // Left Side

        -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.0f, 0.5f, -0.8f, // Non-facing side
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f, -0.8f, // Non-facing side

         0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.8f, 0.5f,  0.0f, // Right side
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.8f, 0.5f,  0.0f, // Right side

         0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 5.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
        -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.0f,      0.0f, 0.5f,  0.8f, // Facing side
         0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 2.5f, 5.0f,      0.0f, 0.5f,  0.8f  // Facing side
    };
    // Indices for vertices order
    GLuint pyramid_indices[] =
    {
        0, 1, 2, // Bottom side
        0, 2, 3, // Bottom side
        4, 6, 5, // Left side
        7, 9, 8, // Non-facing side
        10, 12, 11, // Right side
        13, 15, 14 // Facing side
    };

    // Light as cube
    GLfloat light_vertices[] =
    {
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
         0.1f,  0.1f,  0.1f
    };
    GLuint light_indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };

    GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL(); // Load GLAD to configure OpenGL
    glViewport(0, 0, width, height); // Specify viewport for OpenGL (x, y, x, y)

    // Generates shader
    Shader shaderProgram("../shaders/default.vert", "../shaders/default.frag");

    // Generates Vertex Array Object and binds it
	VAO VAO1;
    VAO1.Bind();

    VBO VBO1(pyramid_vertices, sizeof(pyramid_vertices)); // Generates Vertex Buffer Object and links it to vertices
    EBO EBO1(pyramid_indices, sizeof(pyramid_indices)); // Generates Element Buffer Object and links it to indices

    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Shader for light cube
	Shader lightShader("../shaders/light.vert", "../shaders/light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(light_vertices, sizeof(light_vertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(light_indices, sizeof(light_indices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

    vec4 lightColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	vec3 lightPos = vec3(0.5f, 0.5f, 0.5f);
	mat4 lightModel = mat4(1.0f);
	lightModel = translate(lightModel, lightPos);

	vec3 pyramidPos = vec3(0.0f, 0.0f, 0.0f);
	mat4 pyramidModel = mat4(1.0f);
	pyramidModel = translate(pyramidModel, pyramidPos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Texture
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load("../textures/image0.png", &widthImg, &heightImg, &numColCh, 0);
    Texture memeImage("../textures/image0.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	memeImage.texUnit(shaderProgram, "tex0", 0);

    glEnable(GL_DEPTH_TEST); // Enables depth buffer

    Camera camera(width, height, vec3(0.0f, 0.0f, 2.0f)); // Camera object

    while (!glfwWindowShouldClose(window))
    {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Background color
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f); // Clean back buffer and depth buffer
        
        shaderProgram.Activate(); // Tell OpenGL which Shader Program we want to use
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        camera.Matrix(shaderProgram, "camMatrix");

		// Binds texture so that is appears in rendering
		memeImage.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(pyramid_indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		
        lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(light_indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        
        // Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
    }

    // Delete all created objects
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    memeImage.Delete();
    shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

    // Terminate
    glfwDestroyWindow(window);
    glfwTerminate();
}
*/

// void SpecularLightAndTypesOfLights()
/*
void SpecularLightAndTypesOfLights()
{
    const unsigned int width = 800;
    const unsigned int height = 800;

    glfwInit();

    // Setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    // Vertices coordinates
    GLfloat plane_vertices[] =
    { //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
        -1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	    -1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	     1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	     1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
    };
    // Indices for vertices order
    GLuint plane_indices[] =
    {
        0, 1, 2,
	    0, 2, 3
    };

    // Light as cube
    GLfloat light_vertices[] =
    {
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
         0.1f,  0.1f,  0.1f
    };
    GLuint light_indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };

    GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL(); // Load GLAD to configure OpenGL
    glViewport(0, 0, width, height); // Specify viewport for OpenGL (x, y, x, y)

    Shader shaderProgram("../shaders/default.vert", "../shaders/default.frag"); // Generates shader

    // Generates Vertex Array Object and binds it
	VAO VAO1;
    VAO1.Bind();

    VBO VBO1(plane_vertices, sizeof(plane_vertices)); // Generates Vertex Buffer Object and links it to vertices
    EBO EBO1(plane_indices, sizeof(plane_indices)); // Generates Element Buffer Object and links it to indices

    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

	Shader lightShader("../shaders/light.vert", "../shaders/light.frag"); // Shader for light cube
	
    // Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	
	VBO lightVBO(light_vertices, sizeof(light_vertices)); // Generates Vertex Buffer Object and links it to vertices
	EBO lightEBO(light_indices, sizeof(light_indices)); // Generates Element Buffer Object and links it to indices

    // Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

    // Light object
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 lightPos = vec3(0.5f, 0.5f, 0.5f);
	mat4 lightModel = mat4(1.0f);
	lightModel = translate(lightModel, lightPos);

    // Plane object
	vec3 planePos = vec3(0.0f, 0.0f, 0.0f);
	mat4 planeModel = mat4(1.0f);
	planeModel = translate(planeModel, planePos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, value_ptr(planeModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Texture
    Texture planksTex("../textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	planksTex.texUnit(shaderProgram, "tex0", 0);
    Texture planksSpec("../textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	planksSpec.texUnit(shaderProgram, "tex1", 1);

    glEnable(GL_DEPTH_TEST); // Enables depth buffer

    Camera camera(width, height, vec3(0.0f, 0.0f, 2.0f)); // Camera object

    while (!glfwWindowShouldClose(window))
    {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean the back buffer and assign the new color to it

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f); // Clean back buffer and depth buffer
        
        shaderProgram.Activate(); // Tell OpenGL which Shader Program we want to use
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z); // Exports the camera Position to the Fragment Shader for specular lighting
        camera.Matrix(shaderProgram, "camMatrix"); // Export the camMatrix to the Vertex Shader of the pyramid

		// Binds texture so that is appears in rendering
		planksTex.Bind();
		planksSpec.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(plane_indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		
        lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(light_indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        
		glfwSwapBuffers(window); // Swap the back buffer with the front buffer

		glfwPollEvents(); // Take care of all GLFW events
    }

    // Delete all created objects
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    planksTex.Delete();
    planksSpec.Delete();
    shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

    // Terminate
    glfwDestroyWindow(window);
    glfwTerminate();
}
*/

// PlaneRotation()
/*
void PlaneRotation()
{
    const unsigned int width = 800;
    const unsigned int height = 800;

    glfwInit();

    // Setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    // Vertices coordinates
    GLfloat plane_vertices[] =
    { //     COORDINATES     /        COLORS          /    TexCoord   /        NORMALS       //
        -1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
	    -1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	     1.0f, 0.0f, -1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 1.0f,		0.0f, 1.0f, 0.0f,
	     1.0f, 0.0f,  1.0f,		0.0f, 0.0f, 0.0f,		1.0f, 0.0f,		0.0f, 1.0f, 0.0f
    };
    // Indices for vertices order
    GLuint plane_indices[] =
    {
        0, 1, 2,
	    0, 2, 3
    };

    // Light as cube
    GLfloat light_vertices[] =
    {
        -0.1f, -0.1f,  0.1f,
        -0.1f, -0.1f, -0.1f,
         0.1f, -0.1f, -0.1f,
         0.1f, -0.1f,  0.1f,
        -0.1f,  0.1f,  0.1f,
        -0.1f,  0.1f, -0.1f,
         0.1f,  0.1f, -0.1f,
         0.1f,  0.1f,  0.1f
    };
    GLuint light_indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };

    GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL(); // Load GLAD to configure OpenGL
    glViewport(0, 0, width, height); // Specify viewport for OpenGL (x, y, x, y)

    Shader shaderProgram("../shaders/default.vert", "../shaders/default.frag"); // Generates shader

    // Generates Vertex Array Object and binds it
	VAO VAO1;
    VAO1.Bind();

    VBO VBO1(plane_vertices, sizeof(plane_vertices)); // Generates Vertex Buffer Object and links it to vertices
    EBO EBO1(plane_indices, sizeof(plane_indices)); // Generates Element Buffer Object and links it to indices

    // Links VBO attributes such as coordinates and colors to VAO
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

	Shader lightShader("../shaders/light.vert", "../shaders/light.frag"); // Shader for light cube
	
    // Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	
	VBO lightVBO(light_vertices, sizeof(light_vertices)); // Generates Vertex Buffer Object and links it to vertices
	EBO lightEBO(light_indices, sizeof(light_indices)); // Generates Element Buffer Object and links it to indices

    // Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();

    // Light object
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 lightPos = vec3(0.5f, 0.5f, 0.5f);
	mat4 lightModel = mat4(1.0f);
	lightModel = translate(lightModel, lightPos);

    // Plane object
	vec3 planePos = vec3(0.0f, 0.0f, 0.0f);
	mat4 planeModel = mat4(2.0f);
	planeModel = translate(planeModel, planePos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, value_ptr(planeModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Texture
    Texture planksTex("../textures/planks.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
	planksTex.texUnit(shaderProgram, "tex0", 0);
    Texture planksSpec("../textures/planksSpec.png", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
	planksSpec.texUnit(shaderProgram, "tex1", 1);

    glEnable(GL_DEPTH_TEST); // Enables depth buffer

    Camera camera(width, height, vec3(0.0f, 0.25f, 2.0f)); // Camera object    

    while (!glfwWindowShouldClose(window))
    {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean the back buffer and assign the new color to it

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f); // Clean back buffer and depth buffer
        
        shaderProgram.Activate(); // Tell OpenGL which Shader Program we want to use
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z); // Exports the camera Position to the Fragment Shader for specular lighting
        camera.Matrix(shaderProgram, "camMatrix"); // Export the camMatrix to the Vertex Shader of the pyramid

        // Plane rotate
        planeModel = rotate(planeModel, radians(0.01f), vec3(0.0f, 1.0f, 0.0f));
	    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, value_ptr(planeModel));
	    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

		// Binds texture so that is appears in rendering
		planksTex.Bind();
		planksSpec.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(plane_indices)/sizeof(int), GL_UNSIGNED_INT, 0);
		
        lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(light_indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        
		glfwSwapBuffers(window); // Swap the back buffer with the front buffer

		glfwPollEvents(); // Take care of all GLFW events
    }

    // Delete all created objects
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    planksTex.Delete();
    planksSpec.Delete();
    shaderProgram.Delete();
	lightVAO.Delete();
	lightVBO.Delete();
	lightEBO.Delete();
	lightShader.Delete();

    // Terminate
    glfwDestroyWindow(window);
    glfwTerminate();
}
*/

// void Meshes()
/*
void Meshes()
{
    const unsigned int width = 800;
    const unsigned int height = 800;

    glfwInit();

    // Setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // float array data for OpenGL, (0,0) is on mid, screen coordinate range [-1,1]
    // Vertices coordinates
    Vertex plane_vertices[] =
    { //               COORDINATES           /            COLORS          /           NORMALS         /       TEXTURE COORDINATES    //
        Vertex{vec3(-1.0f, 0.0f,  1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 0.0f)},
        Vertex{vec3(-1.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec2(0.0f, 1.0f)},
        Vertex{vec3( 1.0f, 0.0f, -1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 1.0f)},
        Vertex{vec3( 1.0f, 0.0f,  1.0f), vec3(0.0f, 1.0f, 0.0f), vec3(1.0f, 1.0f, 1.0f), vec2(1.0f, 0.0f)}
    };

    // Indices for vertices order
    GLuint plane_indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };

    Vertex light_vertices[] =
    { //     COORDINATES     //
        Vertex{vec3(-0.1f, -0.1f,  0.1f)},
        Vertex{vec3(-0.1f, -0.1f, -0.1f)},
        Vertex{vec3(0.1f, -0.1f, -0.1f)},
        Vertex{vec3(0.1f, -0.1f,  0.1f)},
        Vertex{vec3(-0.1f,  0.1f,  0.1f)},
        Vertex{vec3(-0.1f,  0.1f, -0.1f)},
        Vertex{vec3(0.1f,  0.1f, -0.1f)},
        Vertex{vec3(0.1f,  0.1f,  0.1f)}
    };

    GLuint light_indices[] =
    {
        0, 1, 2,
        0, 2, 3,
        0, 4, 7,
        0, 7, 3,
        3, 7, 6,
        3, 6, 2,
        2, 6, 5,
        2, 5, 1,
        1, 5, 4,
        1, 4, 0,
        4, 5, 6,
        4, 6, 7
    };

    GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL(); // Load GLAD to configure OpenGL
    glViewport(0, 0, width, height); // Specify viewport for OpenGL (x, y, x, y)

    // Texture data
	Texture textures[]
	{
		Texture("../textures/planks.png", "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("../textures/planksSpec.png", "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

    Shader shaderProgram("../shaders/default.vert", "../shaders/default.frag"); // Generates shader
    // Store mesh data in vectors for the mesh
	vector <Vertex> verts(plane_vertices, plane_vertices + sizeof(plane_vertices) / sizeof(Vertex));
	vector <GLuint> ind(plane_indices, plane_indices + sizeof(plane_indices) / sizeof(GLuint));
	vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
	Mesh floor(verts, ind, tex); // Create floor mesh

	Shader lightShader("../shaders/light.vert", "../shaders/light.frag"); // Shader for light cube
	// Store mesh data in vectors for the mesh
	vector <Vertex> lightVerts(light_vertices, light_vertices + sizeof(light_vertices) / sizeof(Vertex));
	vector <GLuint> lightInd(light_indices, light_indices + sizeof(light_indices) / sizeof(GLuint));
	Mesh light(lightVerts, lightInd, tex); // Create light mesh

    // Light object
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 lightPos = vec3(0.5f, 0.5f, 0.5f);
	mat4 lightModel = mat4(1.0f);
	lightModel = translate(lightModel, lightPos);

    // Plane object
	vec3 planePos = vec3(0.0f, 0.0f, 0.0f);
	mat4 planeModel = mat4(1.0f);
	planeModel = translate(planeModel, planePos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, value_ptr(planeModel));
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    glEnable(GL_DEPTH_TEST); // Enables depth buffer

    Camera camera(width, height, vec3(0.0f, 0.25f, 2.0f)); // Camera object    

    while (!glfwWindowShouldClose(window))
    {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean the back buffer and assign the new color to it

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f); // Clean back buffer and depth buffer

        // Draws different meshes
		floor.Draw(shaderProgram, camera);
		light.Draw(lightShader, camera);
        
        // Plane rotate
        // planeModel = rotate(planeModel, radians(30.0f), vec3(0.0f, 1.0f, 0.0f));
	    // glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, value_ptr(planeModel));
	    // glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

		glfwSwapBuffers(window); // Swap the back buffer with the front buffer

		glfwPollEvents(); // Take care of all GLFW events
    }

    // Delete all created objects
    shaderProgram.Delete();
	lightShader.Delete();

    // Terminate
    glfwDestroyWindow(window);
    glfwTerminate();
}
*/

void FreeCodeCamp()
{
    const unsigned int width = 800;
    const unsigned int height = 800;

    glfwInit();

    // Setup versions
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, "Title", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    gladLoadGL(); // Load GLAD to configure OpenGL
    glViewport(0, 0, width, height); // Specify viewport for OpenGL (x, y, x, y)

    Shader shaderProgram("../shaders/default.vert", "../shaders/default.frag"); // Generates shader

    // Light object
    vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec3 lightPos = vec3(0.5f, 0.5f, 0.5f);
	mat4 lightModel = mat4(1.0f);
	lightModel = translate(lightModel, lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    glEnable(GL_DEPTH_TEST); // Enables depth buffer

    Camera camera(width, height, vec3(0.0f, 0.25f, 2.0f)); // Camera object    

    Model model("../models/sword/scene.gltf"); // Load in a model

    while (!glfwWindowShouldClose(window))
    {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f); // Background color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clean the back buffer and assign the new color to it

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f); // Clean back buffer and depth buffer

        model.Draw(shaderProgram, camera); // Draw a model

		glfwSwapBuffers(window); // Swap the back buffer with the front buffer

		glfwPollEvents(); // Take care of all GLFW events
    }

    // Delete all created objects
    shaderProgram.Delete();

    // Terminate
    glfwDestroyWindow(window);
    glfwTerminate();
}

// Main
int main() {
    FreeCodeCamp();
    return 0;
}