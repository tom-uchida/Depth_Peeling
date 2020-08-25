#pragma once
#include <kvs/Module>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObject>
#include <kvs/IndexBufferObject>
#include <kvs/FrameBufferObject>
#include <kvs/Texture2D>
#include <kvs/PolygonObject>
#include <kvs/PolygonRenderer>


namespace kvs
{
class ObjectBase;
class Camera;
class Light;
}

namespace local
{

class DepthPeelingRenderer : public kvs::PolygonRenderer
// class PolygonRenderer : public kvs::RendererBase
{
    // 
    kvsModule( local::DepthPeelingRenderer, Renderer );
    kvsModuleBaseClass( kvs::PolygonRenderer );

private:
    size_t m_width; ///< window width
    size_t m_height; ///< window height
    const kvs::ObjectBase* m_object; ///< pointer to the rendering object
    bool m_has_normal; ///< check flag for the normal array
    bool m_has_connection; ///< check flag for the connection array
    kvs::Shader::ShadingModel* m_shader; ///< shading method

    kvs::VertexBufferObject m_vbo; ///< vertex buffer object
    kvs::IndexBufferObject m_ibo; ///< index buffer object

    int m_cycle;
    size_t m_npeels;
    kvs::RGBColor m_background_color;

    kvs::ProgramObject m_peeling_shader; ///< shader program for depth peeling
    kvs::ProgramObject m_blending_shader; ///< shader program for blending
    kvs::ProgramObject m_finalizing_shader; ///< shader program for finalizing

    kvs::FrameBufferObject m_framebuffer[3];
    kvs::Texture2D m_color_buffer[3];
    kvs::Texture2D m_depth_buffer[3];

public:
    DepthPeelingRenderer();
    ~DepthPeelingRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    template <typename ShadingType>
    void setShader( const ShadingType shader );

    void setNumberOfPeels( const size_t npeels ) { m_npeels = npeels; }
    void setBackgroundColor( const kvs::RGBColor& color ) { m_background_color = color; }

private:
    void create_shader_program();
    void create_buffer_object( const kvs::PolygonObject* polygon );
    void create_framebuffer( const size_t width, const size_t height );
    void update_framebuffer( const size_t width, const size_t height );

    void initialize_pass();
    void finalize_pass();
    void peel_pass( const kvs::PolygonObject* polygon );
    void draw( const kvs::PolygonObject* polygon );
    void blend();
};

template <typename ShadingType>
inline void DepthPeelingRenderer::setShader( const ShadingType shader )
{
    if ( m_shader )
    {
        delete m_shader;
        m_shader = NULL;
    }

    m_shader = new ShadingType( shader );
    if ( !m_shader )
    {
        kvsMessageError("Cannot create a specified shader.");
    }
};

} // end of namespace local
