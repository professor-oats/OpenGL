//
// Created by amazinghorse on 6/15/25.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// Settings

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    if (window == NULL) {

        std::cout << "There was an error creating the GLFW window" << std::endl;
        glfwTerminate();
        return -1;

    }

    // glad: load all OpenGL function pointers
    glfwMakeContextCurrent(window);

    // Tells our app to use our frambuffer_size_callback upon resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Shader building
    // Vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Compile the shader source code dynamically
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, 0.5f, 0.0f, // left 1
        -0.5f, -0.5f, 0.0f, // down 1
         0.0f,  0.5f, 0.0f,  // right 1
         // triangle 2
         0.0f, 0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
         0.5f,  0.0f, 0.0f
    };

    unsigned int VBO, VAO, EBO;
    // Generate a VAO as vertex array to put the data of our vertices in it
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).

    /* A vertex array object (also known as VAO) can be bound just like a vertex buffer object and any
subsequent vertex attribute calls from that point on will be stored inside the VAO. This has the
advantage that when configuring vertex attribute pointers you only have to make those calls once
and whenever we want to draw the object, we can just bind the corresponding VAO. This makes
switching between different vertex data and attribute configurations as easy as binding a different
VAO. All the state we just set is stored inside the VAO.
    */
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // param 1: what vertex attribute to configure - We set the location = 0 for the vertex shader we coded
    // param 2: specify the size of the vertex attributes, since we use coordinates x,y,z the size is 3
    // param 3: The type of the stored data which is float//GL_FLOAT
    // param 4: Let normalization take place or not, for converting signed integers into floats
    // param 5: Specifying the stride - the chunks of memory that we process to store the coordinates (in this case)
    // for each vertex. Since it's floats for each x,y,z it's simply times 3 to declare the chunk to process
    // param 6: Offset for where the position data begins in the buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Enable the vertex attribute by the location that we set in shader code
    glEnableVertexAttribArray(0);

    // !!! Now the VAO that was active is finished configured so we can unbind the buffers and unbind
    // the vertex array to configure another VAO

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window))
    {

        processInput(window);
        /* Double buffer When an application draws in a single buffer the resulting image may
        display flickering issues. This is because the resulting output image is not drawn in an
        instant, but drawn pixel by pixel and usually from left to right and top to bottom. Because
        this image is not displayed at an instant to the user while still being rendered to, the result
        may contain artifacts. To circumvent these issues, windowing applications apply a double
        buffer for rendering. The front buffer contains the final output image that is shown at
        the screen, while all the rendering commands draw to the back buffer. As soon as all
        the rendering commands are finished we swap the back buffer to the front buffer so the
        image can be displayed without still being rendered to, removing all the aforementioned
        artifacts. */

        // Render

        /* As you may recall from the OpenGL chapter, the glClearColor function is a state-
        setting function and glClear is a state-using function in that it uses the current state to
        retrieve the clearing color from. */

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // glBindVertexArray(0); // no need to unbind it every time

        // Pushes the background rendered buffer to front
        glfwSwapBuffers(window);
        // Check for events
        glfwPollEvents();
    }

    // De-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
