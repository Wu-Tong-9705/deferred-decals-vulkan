#pragma once
#include "stdafx.h"
#include "../scene/camera.h"
#include "../support/keys.h"
#include "../scene/model.h"
#define N_SWAPCHAIN_IMAGES (3)

struct MVPUniform
{
    alignas(16) mat4 model;
    alignas(16) mat4 view;
    alignas(16) mat4 proj;
};

struct DeferredConstants
{
    vec2 RTSize;
};

struct SunLightUniform
{
    vec3 SunDirectionWS;
    vec3 SunIrradiance;
};

struct CameraUniform
{
    vec3 CameraPosWS;
    mat4 InvViewProj;
};

class Engine
{
public:
    //单例模式，懒汉，线程不安全
    static unique_ptr<Engine>& Instance();

    //单例模式：删除外部的构造函数
    Engine(Engine const& r) = delete;
    Engine(Engine const&& r) = delete;
    Engine& operator=(Engine const& r) = delete;

    void run ();

    BaseDevice* getDevice();
    PipelineLayout* getPineLine(bool is_gfx = true);
    vector<DescriptorSet::CombinedImageSamplerBindingElement>* getCombinedImageSamplers();
    float getAspect();

    ~Engine();
private:
    Engine ();

    void init();
    void init_vulkan();

    void on_validation_callback(Anvil::DebugMessageSeverityFlags in_severity,
        const char* in_message_ptr);
    void mouse_callback(CallbackArgument* argumentPtr);
    void scroll_callback(CallbackArgument* argumentPtr);
    void key_press_callback(CallbackArgument* argumentPtr);
    void key_release_callback(CallbackArgument* argumentPtr);
    void recreate_swapchain();

    void init_window         ();
    void init_swapchain      ();

    void init_buffers        ();
    void init_dsgs           ();

    void init_render_pass    ();
    void init_shaders        ();
    void init_gfx_pipelines  ();
    void init_compute_pipelines();
    
    Format SelectSupportedFormat(
        const vector<Format>& candidates,
        ImageTiling tiling,
        FormatFeatureFlags features);
    void init_image          ();
    void init_image_view_and_sampler     ();
    void init_framebuffers   ();
    void init_command_buffers();

    void init_semaphores     ();
    void update_data(uint32_t in_n_swapchain_image);
    void draw_frame          ();

    void cleanup_swapwhain   ();
    void deinit              ();

    void init_events         ();

    shared_ptr<Model>         m_model;
    shared_ptr<Camera>        m_camera;
    shared_ptr<Key>           m_key;


    BaseDeviceUniquePtr       m_device_ptr;
    InstanceUniquePtr         m_instance_ptr;
    const PhysicalDevice*     m_physical_device_ptr;
    Queue*                    m_present_queue_ptr;
    RenderingSurfaceUniquePtr m_rendering_surface_ptr;
    SwapchainUniquePtr        m_swapchain_ptr;
    WindowUniquePtr           m_window_ptr;

    vector<DescriptorSet::CombinedImageSamplerBindingElement> m_combined_image_samplers;

    DescriptorSetGroupUniquePtr                  m_dsg_ptr;


    RenderPassUniquePtr                          m_renderpass_ptr;
    SubPassID                                    m_render_pass_subpass_GBuffer_id;
    unique_ptr<ShaderModuleStageEntryPoint>      m_vs_ptr;
    unique_ptr<ShaderModuleStageEntryPoint>      m_fs_ptr;
    unique_ptr<ShaderModuleStageEntryPoint>      m_cs_ptr;

    PipelineID                                   m_gfx_pipeline_id;
    PipelineID                                   m_compute_pipeline_id;


    vector<DescriptorSet::StorageImageBindingElement>   m_color_image_ptr;
    ImageUniquePtr                               m_depth_image_ptr;
    ImageViewUniquePtr                           m_depth_image_view_ptr;
    ImageUniquePtr                               m_tangent_frame_image_ptr;
    ImageViewUniquePtr                           m_tangent_frame_image_view_ptr;
    ImageUniquePtr                               m_uv_and_depth_gradient_image_ptr;
    ImageViewUniquePtr                           m_uv_and_depth_gradient_image_view_ptr;
    ImageUniquePtr                               m_uv_gradient_image_ptr;
    ImageViewUniquePtr                           m_uv_gradient_image_view_ptr;
    ImageUniquePtr                               m_material_id_image_ptr;
    ImageViewUniquePtr                           m_material_id_image_view_ptr;
    SamplerUniquePtr                             m_compute_shader_sampler;

    FramebufferUniquePtr                         m_fbo;
    PrimaryCommandBufferUniquePtr                m_command_buffers[N_SWAPCHAIN_IMAGES];


    uint32_t       m_n_last_semaphore_used;
    Format         m_depth_format;

    vector<SemaphoreUniquePtr> m_frame_signal_semaphores;
    vector<SemaphoreUniquePtr> m_frame_wait_semaphores;

    int m_width;
    int m_height;
    bool m_is_full_screen;
    RECT m_rect_before_full_screen;

    DeferredConstants         m_deferred_constants;
    BufferUniquePtr           m_mvp_uniform_buffer_ptr;
    VkDeviceSize              m_mvp_buffer_size_per_swapchain_image;
    BufferUniquePtr           m_camera_uniform_buffer_ptr;
    VkDeviceSize              m_camera_buffer_size_per_swapchain_image;
    BufferUniquePtr           m_sunLight_uniform_buffer_ptr;
    VkDeviceSize              m_sunLight_buffer_size_per_swapchain_image;

};
