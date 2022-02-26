#include "VertexBuffer.h"
#include "VertexArray.h"

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
}

// We bind our VAO, bind our Buffer and set up our Layout
void VertexArray::AddBuffer(const VertexBuffer& vb, VertexBufferLayout& layout)
{
    Bind();
	vb.Bind();
    const auto& elements = layout.GetElements();
    for (unsigned int i = 0; i < elements.size(); ++i)
    {
        const auto& element = elements[i];
        unsigned int offset = 0;

        // We need to enable our vertexAttribArray
        glEnableVertexAttribArray(i);
        // The Vertex Attrib Pointer is what links this buffer to the current VAO!!!
        // 
        // Witch attribute we want?
        // How many types build this attribute? Ex: 2 floats build a 2D position
        // Witch type are this attribute? Ex: float, int, ...
        // Are normalize? Ex: RGB(255, 0, 0) needs to pass to [0, 1]
        // Is the size of the vertex, as the vertex has many attributes, we need
        //      to call vertex[2], so we need to know the size of witch vertex. 
        //      We can say that is the amount of bytes between each vertex
        // Inside this Vertex, what is the offset for this attribute? 
        //      Ex: if a vertex has (position, color, textCoord)
        //      then the offset for 0, 12, 20;
        // 
        // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
        glVertexAttribPointer(
            i,
            element.count,
            element.type,
            element.normalized,
            layout.GetStride(),
            (const void*)offset);

        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }

}

void VertexArray::Bind() const
{
    glBindVertexArray(m_RendererID);
}

void VertexArray::Unbind() const
{
    glBindVertexArray(0);
}
