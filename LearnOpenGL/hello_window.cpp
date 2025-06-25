//
// Created by amazinghorse on 6/15/25.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

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

        // Pushes the background rendered buffer to front
        glfwSwapBuffers(window);
        // Check for events
        glfwPollEvents();
    }

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
