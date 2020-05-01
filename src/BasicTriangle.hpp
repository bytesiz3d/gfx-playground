namespace BasicTriangle
{
    // Attribute positions
    const GLubyte PGND_POSITION = 0;
    const GLubyte PGND_COLOR    = 1;
    GLuint VAO, PBO, CBO;

    void Upload() 
    {
        // Vertex Array Object
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Position data
        const GLfloat positionBufferData[] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.0f,  0.5f,
        };
        // Position Buffer Object
        glGenBuffers(1, &PBO);
        glBindBuffer(GL_ARRAY_BUFFER, PBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positionBufferData), positionBufferData, GL_STATIC_DRAW);
        glVertexAttribPointer(PGND_POSITION, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(PGND_POSITION);

        // Color data
        const GLubyte colorBufferData[] = {
            0xFF, 0x00, 0x00,
            0x00, 0xFF, 0x00,
            0x00, 0x00, 0xFF,
        };
        // Color Buffer Object
        glGenBuffers(1, &CBO);
        glBindBuffer(GL_ARRAY_BUFFER, CBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorBufferData), colorBufferData, GL_STATIC_DRAW);
        glVertexAttribPointer(PGND_COLOR, 3, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);
        glEnableVertexAttribArray(PGND_COLOR);

        glBindVertexArray(0);
    }

    void Draw()
    {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
    }

    void Destroy()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &PBO);
        glDeleteBuffers(1, &CBO);
    }
}
