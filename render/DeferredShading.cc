#include "../include/engine.hh"

/*
ordor of shader

 g-uffer color
 Z-buffer
 sobel filter from z-buffer
 G-buffer normal
 Diffuse color
 Specular color

				//BeginShaderMode(engine.depth_shader);
				//	DrawTextureRec(engine.fbo.texture, {0, 0, static_cast<float>(display.width), -static_cast<float>(display.height)}, {0, 0}, WHITE);
				//EndShaderMode();
				BeginShaderMode(engine.fbo_shader);
					DrawTextureRec(engine.fbo.texture, {0, 0, static_cast<float>(display.width), -static_cast<float>(display.height)}, {0, 0}, WHITE);
				EndShaderMode();
				BeginBlendMode(BLEND_ADD_COLORS);
				EndBlendMode();

*/

// Load custom render texture, create a writable depth texture buffer
RenderTexture2D LoadRenderTextureDepthTex(int width, int height)
{
    RenderTexture2D target = { 0 };

    target.id = rlLoadFramebuffer(width, height);   // Load an empty framebuffer

    if (target.id > 0)
    {
        rlEnableFramebuffer(target.id);

        // Create color texture (default to RGBA)
        target.texture.id = rlLoadTexture(0, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
        target.texture.width = width;
        target.texture.height = height;
        target.texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
        target.texture.mipmaps = 1;

        // Create depth texture buffer (instead of raylib default renderbuffer)
        target.depth.id = rlLoadTextureDepth(width, height, false);
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19;//DEPTH_COMPONENT_24BIT?
        target.depth.mipmaps = 1;

        // Attach color texture and depth texture to FBO
        rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
        rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete(target.id)) {
			TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);
		}

        rlDisableFramebuffer();
    }
    else {
		TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");
	}

    return target;
}

// Unload render texture from GPU memory (VRAM)
void UnloadRenderTextureDepthTex(RenderTexture2D target)
{
    if (target.id > 0)
    {
        // Color texture attached to FBO is deleted
        rlUnloadTexture(target.texture.id);
        rlUnloadTexture(target.depth.id);

        // NOTE: Depth texture is automatically
        // queried and deleted before deleting framebuffer
        rlUnloadFramebuffer(target.id);
    }
}

void initRender(engine_t &engine, display_t &display) {
	engine.gbuffer = {};
	engine.shader = LoadShader("assets/shader/shader.vs", "assets/shader/shader.fs");
	engine.shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(engine.shader, "viewPos");
	float shaderposlocaltmp[4] = { 0.1f, 0.1f, 0.1f, 1.0f };
    SetShaderValue(engine.shader, GetShaderLocation(engine.shader, "ambient"), shaderposlocaltmp, SHADER_UNIFORM_VEC4);

	engine.fbo_shader = LoadShader(0, "assets/shader/depth_filter.fs");

	engine.fbo = LoadRenderTextureDepthTex(display.width, display.height);
    SetTextureFilter(engine.fbo.texture, TEXTURE_FILTER_BILINEAR);
}

void renderUpdate(engine_t &engine, display_t &display, Camera *camera, double delta_time) {
	SetShaderValue(engine.shader, engine.shader.locs[SHADER_LOC_VECTOR_VIEW], &camera->position, SHADER_UNIFORM_VEC3);
	SetShaderValue(engine.fbo_shader, engine.fbo_shader.locs[SHADER_LOC_VECTOR_VIEW], &camera->position, SHADER_UNIFORM_VEC3);
}

void renderAth(void) {
	DrawRectangle(0, 0, 100, 100, BLACK);
	DrawLine(50, 0, 50, 100, BLUE);
	DrawLine(0, 50, 100, 50, RED);
}

void renderRender(engine_t &engine, display_t &display, Camera *camera, double delta_time, Voxel *voxel_dirt) {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		rlEnableDepthTest();
		BeginMode3D(*camera);
			for (float i = 0; i < 10; i += 1) {
				for (float j = 0; j < 10; j += 1) {
					DrawVoxel(voxel_dirt->model, {i, -0.5f, j}, 1, RED);
				}
			}
			DrawLine3D({-100, 0,}, {100, 0, 0}, RED);
			DrawLine3D({0,-100,0}, {0, 100, 0}, GREEN);
			DrawLine3D({0,0,-100}, {0, 0, 100}, BLUE);
		EndMode3D();
	EndTextureMode();

	BeginShaderMode(engine.fbo_shader);
		DrawTextureRec(engine.fbo.depth, {0, 0, static_cast<float>(display.width), -static_cast<float>(display.height)}, {0, 0}, WHITE);
	EndShaderMode();
	//BeginBlendMode(BLEND_ADDITIVE);
	//	DrawTextureRec(engine.fbo.texture, {0, 0, static_cast<float>(display.width), -static_cast<float>(display.height)}, {0, 0}, WHITE);
	//EndBlendMode();
	renderAth();
}