#pragma once
#include <iostream>
#include <fstream>
#include <string>

namespace Shader
{
    /*
     * Loads a file's ASCII content into a string and returns it.
     * Returns an empty string if it fails.
     */
    std::string LoadFile(const char* filePath)
    {
        std::string shader = "";

        // Load the file
        std::ifstream fileStream(filePath, std::ios::in);
        if (!fileStream.is_open())
        {
            std::cerr << "Could not read file " << filePath << ". File does not exist.\n";
            return "";
        }

        // Fill the string line by line
        std::string currentLine = "";
        while (!fileStream.eof())
        {
            std::getline(fileStream, currentLine);
            shader.append(currentLine + "\n");
        }

        fileStream.close();
        return shader;
    }

    /*
     * Loads the vertex and fragment shaders from their two respective paths.
     * Links the two shaders and returns the compiled program.
     */  
    GLuint LoadShader(const char* vertexPath, const char* fragmentPath)
    {
        // Create and load the two shaders
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        std::string vertexShaderStr = LoadFile(vertexPath);
        std::string fragmentShaderStr = LoadFile(fragmentPath);

        const char* vertexShaderSource = vertexShaderStr.c_str();
        const char* fragmentShaderSource = fragmentShaderStr.c_str();

        GLint compileStatus = GL_FALSE;
        GLint linkStatus = GL_FALSE;
        int logLength = 0;

        // Compile the vertex shader
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        // Verify that it was compiled successfully
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
        std::string vertexShaderError;
        vertexShaderError.resize((logLength > 1) ? logLength : 1);
        glGetShaderInfoLog(vertexShader, logLength, NULL, &vertexShaderError[0]);
        if (!vertexShaderError.empty())
            std::cout << vertexShaderError << std::endl;


        // Compile the fragment shader
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        // Verify that it was compiled successfully
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);
        std::string fragmentShaderError;
        fragmentShaderError.resize((logLength > 1) ? logLength : 1);
        glGetShaderInfoLog(fragmentShader, logLength, NULL, &fragmentShaderError[0]);
        if (!fragmentShaderError.empty())
            std::cout << fragmentShaderError << std::endl;


        // Create and link the shader program
        GLuint program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        // Verify that it was linked successfully
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::string programError;
        programError.resize((logLength > 1) ? logLength : 1);
        glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
        if (!programError.empty())
            std::cout << programError << std::endl;

        
        // Finishing up
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return program;
    }
}

namespace Shader_Utils {
    struct Attribute {
        const char* name;
        GLsizeiptr array_sz;

        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        const void* offset;
    };

    void UploadAttribute(GLuint program, const void *data, Attribute attrib) {
        // Get attribute location
        const GLubyte location = glGetAttribLocation(program, attrib.name);

        // Generate buffer
        GLuint buffer_object;
        glGenBuffers(1, &buffer_object);

        // Upload data to buffer
        glBindBuffer(GL_ARRAY_BUFFER, buffer_object);
        glBufferData(GL_ARRAY_BUFFER, attrib.array_sz, data, GL_DYNAMIC_DRAW);
        
        // Describe and enable data
        glVertexAttribPointer(location, attrib.size, attrib.type, attrib.normalized, attrib.stride, attrib.offset);
        glEnableVertexAttribArray(location);
    }

    int TestQuad() {
        GLFWwindow *window = InitWindow(800, 600);
        if (window == NULL) return -1; 

        // Load and compile a shader
        GLuint program = Shader::LoadShader("../shaders/generic.vert", "../shaders/generic.frag");
        glUseProgram(program);

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Position data
        const GLfloat data[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f,
        };

        Attribute position = {
            "position",
            sizeof(data),
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            nullptr
        };

        UploadAttribute(program, data, position);
        glClearColor(10.0f/256, 9.0f/256, 19.0f/256, 1.0f);
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            glClear(GL_COLOR_BUFFER_BIT);
            glDrawArrays(GL_LINE_LOOP, 0, 4);
            glfwSwapBuffers(window);
        }

        glfwTerminate();
        return 0;
    }
}