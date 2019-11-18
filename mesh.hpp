#pragma once
struct VertexDescriptor
{
    GLuint attributeLocation;
    std::string buffer;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    void* offset;
};

class Mesh
{
private:
    std::vector<VertexDescriptor> descriptors;
    std::map<std::string, GLuint> VBOs;

    GLuint EBO;
    GLuint VAO;

    GLsizei elementCount;
    GLenum elementType;

public:
    /*Mesh()
    {;}*/

    Mesh(VertexDescriptor* _descriptors, int n) : elementCount(0), elementType(GL_UNSIGNED_INT)
    {
        for (GLuint i = 0; i < n; i++)
            descriptors.push_back(_descriptors[i]);

        for (auto& desc : descriptors)
        {
            glGenBuffers(1, &VBOs[desc.buffer]);
        }

        glGenBuffers(1, &EBO);
        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        for (auto& descriptor : descriptors)
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[descriptor.buffer]);

            glVertexAttribPointer(descriptor.attributeLocation,
                descriptor.size,
                descriptor.type,
                descriptor.normalized,
                descriptor.stride,
                descriptor.offset);

            glEnableVertexAttribArray(descriptor.attributeLocation);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBindVertexArray(0);
    }

    ~Mesh()
    {
        for (auto& desc : descriptors)
        {
            glDeleteBuffers(1, &VBOs[desc.buffer]);
        }
        VBOs.clear();

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &EBO);
    }

    void dispose()
    {
        this->~Mesh();
    }

    void setBufferData(std::string bufferName, GLsizeiptr size, const void* bufferData, GLenum usage)
    {
        if (VBOs[bufferName] != 0)
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBOs[bufferName]);
            glBufferData(GL_ARRAY_BUFFER, size, bufferData, usage);
        }
        else
        {
            std::cerr << bufferName << " is not found in the buffers list\n";
        }
    }

    void setElementsData(GLsizeiptr size, const void* bufferData, GLenum usage, GLsizei count, GLenum type)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, bufferData, usage);

        elementCount = count;
        elementType = type;
    }

    void draw(GLenum mode = GL_TRIANGLES)
    {
        glBindVertexArray(VAO);
        glDrawElements(mode, elementCount, elementType, 0);
        glBindVertexArray(0);
    }
};