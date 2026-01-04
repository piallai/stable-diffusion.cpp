
#pragma once

#ifdef SD_EXAMPLES_GLOVE_GUI

#define GLOVE_ENABLE_JSON
#include "glove.h"

glvm_SlvEnum(ProcessingMode, img_gen, vid_gen, upscale, convert);
glvm_SlvEnum_named(WeightType, weight_file_type, "", f32, "f32", f16, "f16", q4_0, "q4_0", q4_1, "q4_1", q5_0, "q5_0", q5_1, "q5_1", q8_0, "q8_0", q2_k, "q2_k", q3_k, "q3_k", q4_k, "q4_k");
glvm_SlvEnum_named(SamplingMethod, euler, "euler", euler_a, "euler_a", heun, "heun", dpm2, "dpm2", dpmpp2s_a, "dpm++2s_a", dpmpp2m, "dpm++2m", dpmpp2mv2, "dpm++2mv2", ipndm, "ipndm", ipndm_v, "ipndm_v", lcm, "lcm", ddim_trailing, "ddim_trailing", tcd, "tcd");
glvm_SlvEnum(Rng, std_default, cuda, cpu);
glvm_SlvEnum(Prediction, eps, v, edm_v, sd3_flow, flux_flow);
glvm_SlvEnum_named(LoraApplyMode, Auto, "auto", immediately, "immediately", at_runtime, "at_runtime");
glvm_SlvEnum(Scheduler, discrete, karras, exponential, ays, gits, smoothstep, sgm_uniform, simple, kl_optimal, lcm);
glvm_SlvEnum(Preview, none, proj, tae, vae);

glvm_parametrization(GlvSdParamsPhotomaker, "Photomaker params",
    pm_id_embed_path, SlvFile, "--pm-id-embed-path", "path to PHOTOMAKER v2 id embed", SlvFile(SlvFile::IO::Read),
    pm_id_images_dir, SlvDirectory, "--pm-id-images-dir", "path to PHOTOMAKER input id images dir", SlvDirectory(),
    pm_style_strength, float, "--pm-style-strength", "", 20.f
)

glvm_parametrization(GlvSdParamsImageVideoInput, "Image/video input params",
    init_img, SlvFile, "--init-img@-i", "path to the init image", SlvFile(SlvFile::IO::Read),
    end_img, SlvFile, "--end-img", "path to the end image, required by flf2v", SlvFile(SlvFile::IO::Read),
    mask_img, SlvFile, "--mask", "mask for inpainting", SlvFile(SlvFile::IO::Read),
    control_image, SlvFile, "--control-image", "path to control image, control net", SlvFile(SlvFile::IO::Read),
    control_video, SlvDirectory, "--control-video", "path to control video frames, It must be a directory path. The video frames inside should be stored as images in\nlexicographical (character) order. For example, if the control video path is\n`frames`, the directory contain images such as 00.png, 01.png, ... etc.", SlvDirectory(),
    ref_image, SlvFile, "--ref-image", "reference image for Flux Kontext models (can be used multiple times)", SlvFile(SlvFile::IO::Read),
    disable_auto_resize_ref_image, bool, "--disable-auto-resize-ref-image", "disable auto resize of ref images", false
)

glvm_parametrization(GlvSdParamsVideo, "Video params",
    video_frames, int, "--video-frames", "video frames (default: 1)", 1,
    fps, unsigned int, "--fps", "fps (default: 24)", 24,
    timestep_shift, unsigned int, "--timestep-shift", "shift timestep for NitroFusion models (default: 0). recommended N for NitroSD-Realism around 250 and 500 for\nNitroSD-Vibrant", 0
)

glvm_parametrization(GlvSdParamsVaeTiling, "Vae tiling params",
    vae_tiling, bool, "--vae-tiling", "process vae in tiles to reduce memory usage", false,
    vae_tile_overlap, float, "--vae-tile-overlap", "tile overlap for vae tiling, in fraction of tile size (default: 0.5)", 0.5f,
    vae_tile_size, SlvSize2d<unsigned int>, "--vae-tile-size", "tile size for vae tiling, format [X]x[Y] (default: 32x32)", SlvSize2d<unsigned int>(32 COMMA 32),
    vae_relative_tile_size, SlvSize2d<float>, "--vae-relative-tile-size", "relative tile size for vae tiling, format [X]x[Y], in fraction of image size if < 1, in number of tiles per dim if >=1\n(overrides --vae-tile-size)", SlvSize2d<float>(0.f COMMA 0.f)
)

glvm_parametrization(GlvSdParamsOnCPU, "On CPU",
    offload_to_cpu, bool, "--offload-to-cpu", "place the weights in RAM to save VRAM, and automatically load them into VRAM when needed", false,
    control_net_cpu, bool, "--control-net-cpu", "keep controlnet in cpu (for low vram)", false,
    clip_on_cpu, bool, "--clip-on-cpu", "keep clip in cpu (for low vram)", false,
    vae_on_cpu, bool, "--vae-on-cpu", "keep vae in cpu (for low vram)", false
)

glvm_parametrization(GlvSdParamsPreview, "Preview params",
    preview, Preview, "--preview", "preview method. must be one of the following [none, proj, tae, vae].] (default is none)", Preview::none,
    preview_path, SlvFile, "--preview-path", "path to write preview image to (default: ./preview.png)", SlvFile("./preview.png", SlvFileExtensions({".png", ".jpg", ".jpeg", ".jpe"}), SlvFile::IO::Write),
    preview_interval, unsigned int, "--preview-interval", "interval in denoising steps between consecutive updates of the image preview file (default is 1, meaning updating at every step)", 1,
    preview_noisy, bool, "--preview-noisy", "enables previewing noisy inputs of the models rather than the denoised outputs", true,
    taesd_preview_only, bool, "--taesd-preview-only", "prevents usage of taesd for decoding the final image. (for use with --preview tae)", true
)

glvm_parametrization(GlvSdParamsUpscale, "Upscale params",
    upscale_repeats, unsigned int, "--upscale-repeats", "Run the ESRGAN upscaler this many times (default 1)", 1,
    upscale_tile_size, unsigned int, "--upscale-tile-size", "tile size for ESRGAN upscaling (default: 128)", 128
)

glvm_parametrization(GlvSdModels, "Models",
    model, SlvFile, "--model@-m", "path to full model", SlvFile("", SlvFileExtensions({".safetensors", ".ckpt"}), SlvFile::IO::Read),
    clip_l, SlvFile, "--clip_l", "path to the clip-l text encoder", SlvFile(SlvFile::IO::Read),
    clip_g, SlvFile, "--clip_g", "path to the clip-g text encoder", SlvFile(SlvFile::IO::Read),
    clip_vision, SlvFile, "--clip_vision", "path to the clip-vision encoder", SlvFile(SlvFile::IO::Read),
    t5xxl, SlvFile, "--t5xxl", "path to the t5xxl text encoder", SlvFile(SlvFile::IO::Read),
    llm, SlvFile, "--llm@--qwen2vl", "path to the llm text encoder. For example: (qwenvl2.5 for qwen-image, mistral-small3.2 for flux2, ...)", SlvFile(SlvFile::IO::Read),
    llm_vision, SlvFile, "--llm_vision@--qwen2vl_vision", "path to the llm vit", SlvFile(SlvFile::IO::Read),
    diffusion_model, SlvFile, "--diffusion-model", "path to the standalone diffusion model", SlvFile(SlvFileExtensions({".gguf", ".safetensors", ".sft"}), SlvFile::IO::Read),
    high_noise_diffusion_model, SlvFile, "--high-noise-diffusion-model", "path to the standalone high noise diffusion model", SlvFile(SlvFileExtensions({".gguf", ".safetensors", ".sft"}), SlvFile::IO::Read),
    vae, SlvFile, "--vae", "path to vae", SlvFile("", SlvFileExtensions({".safetensors", ".sft"}), SlvFile::IO::Read),
    taesd, SlvFile, "--taesd@--tae", "path to taesd. Using Tiny AutoEncoder for fast decoding (low quality)", SlvFile(SlvFile::IO::Read),
    control_net, SlvFile, "--control-net", "path to control net model", SlvFile(SlvFile::IO::Read),
    embd_dir, SlvDirectory, "--embd-dir", "path to embeddings", SlvDirectory(),
    lora_model_dir, SlvDirectory, "--lora-model-dir", "lora model directory", SlvDirectory(),
    photo_maker_path, SlvFile, "--photo-maker", "path to PHOTOMAKER model", SlvFile(SlvFile::IO::Read),
    upscale_model, SlvFile, "--upscale-model", "path to esrgan model. Upscale images after generate, just RealESRGAN_x4plus_anime_6B supported by now", SlvFile(SlvFile::IO::Read)
)

glvm_parametrization(GlvSdModelSLG, "SLG params",
    slg_scale, float, "--slg-scale", "skip layer guidance (SLG) scale, only for DiT models: (default: 0)\n0 means disabled, a value of 2.5 is nice for sd3.5 medium.", 0.f,
    skip_layer_start, float, "--skip-layer-start", "SLG enabling point: (default: 0.01)", 0.01f,
    skip_layer_end, float, "--skip-layer-end", "SLG disabling point (default: 0.2)", 0.2f,
    skip_layers, std::vector<unsigned int>, "--skip-layers", "Layers to skip for SLG steps: (default: [7,8,9])", std::vector<unsigned int>({7, 8, 9})
)

glvm_parametrization(GlvSdModelSLGHighNoise, "SLG params (high noise)",
    slg_scale, float, "--high-noise-slg-scale", "(high noise) skip layer guidance (SLG) scale, only for DiT models: (default: 0)", 0.f,
    skip_layer_start, float, "--high-noise-skip-layer-start", "(high noise) SLG enabling point (default: 0.01)", 0.01f,
    skip_layer_end, float, "--high-noise-skip-layer-end", "(high noise) SLG disabling point (default: 0.2)", 0.2f,
    skip_layers, std::vector<unsigned int>, "--high-noise-skip-layers", "(high noise) layers to skip for SLG steps (default: [7,8,9])", std::vector<unsigned int>({7, 8, 9})
)

glvm_parametrization(GlvSdParamsScale, "Scale params",
    cfg_scale, float, "--cfg-scale", "unconditional guidance scale: (default: 7.0)", 7.0f,
    img_cfg_scale, float, "--img-cfg-scale", "image guidance scale for inpaint or instruct-pix2pix models: (default: same as --cfg-scale)", INFINITY,
    guidance, float, "--guidance", "distilled guidance scale for models with guidance input (default: 3.5)", 3.5f,
    SLG, GlvSdModelSLG, "Skip layer guidance", "", GlvSdModelSLG(),
    eta, float, "--eta", "eta in DDIM, only for DDIM and TCD: (default: 0)", 0.f
)

glvm_parametrization(GlvSdParamsScaleHighNoise, "Scale params (high noise)",
    high_noise_cfg_scale, float, "--high-noise-cfg-scale", "(high noise) unconditional guidance scale: (default: 7.0)", 7.0f,
    high_noise_img_cfg_scale, float, "--high-noise-img-cfg-scale", "(high noise) image guidance scale for inpaint or instruct-pix2pix models (default: same as --cfg-scale)", INFINITY,
    high_noise_guidance, float, "--high-noise-guidance", "(high noise) distilled guidance scale for models with guidance input (default: 3.5)", INFINITY,
    SLG_high_noise, GlvSdModelSLGHighNoise, "Skip layer guidance", "", GlvSdModelSLGHighNoise(),
    high_noise_eta, float, "--high-noise-eta", "(high noise) eta in DDIM, only for DDIM and TCD (default: 0)", 0.f
)

glvm_parametrization(GlvSdScaleStrength, "Scale/Strength params",
    Scale, GlvSdParamsScale, "Scale", "", GlvSdParamsScale(),
    Scale_high_noise, GlvSdParamsScaleHighNoise, "Scale (high noise)", "", GlvSdParamsScaleHighNoise(),
    strength, float, "--strength", "strength for noising/unnoising (default: 0.75)", 0.75f,
    control_strength, SlvProportion, "--control-strength", "strength to apply Control Net (default: 0.9) \n1.0 corresponds to full destruction of information in init", 0.9
)

glvm_parametrization(GlvSdParamsChroma, "Chroma params",
    chroma_use_dit_mask, bool, "--chroma-disable-dit-mask", "disable dit mask for chroma", true,
    chroma_use_t5_mask, bool, "--chroma-enable-t5-mask", "enable t5 mask for chroma", false,
    chroma_t5_mask_pad, int, "--chroma-t5-mask-pad", "t5 mask pad size of chroma", 1
)

glvm_parametrization(GlvSdParamsHighNoise, "SD params (high noise)",
    high_noise_sampling_method, SamplingMethod, "--high-noise-sampling-method", "(high noise) sampling method, one of [euler, euler_a, heun, dpm2, dpm++2s_a, dpm++2m, dpm++2mv2, ipndm, ipndm_v, lcm,\nddim_trailing, tcd] default: euler for Flux/SD3/Wan, euler_a otherwise", SamplingMethod::euler_a,
    high_noise_steps, int, "--high-noise-steps", "(high noise) number of sample steps (default: -1 = auto)", -1)

#ifdef SD_EXAMPLES_IMG2IMG_REPEAT
    glvm_parametrization(GlvSdImagesSequence, "Images sequence params",
        crop, SlvSides2d<unsigned int>, "Crop", "Image crop at each frame", SlvSides2d<unsigned int>({2, 2, 2, 2}),
        Nframes, unsigned int, "Nframes", "Number of frames", 100
    )
#else
    typedef nullptr_t GlvSdImagesSequence;
#endif

glvm_parametrization(GlvSdCLIOptionsAdvanced, "CLI options advanced",
    output_begin_idx, unsigned int, "--output-begin-idx", "starting index for output image sequence, must be non-negative (default 0 if specified %d in output path, 1 otherwise)", 1,
    canny, bool, "--canny", "apply canny preprocessor (edge detection)", false,
    convert_name, bool, "--convert-name", "convert tensor name (for convert mode)", false,
    color, bool, "--color", "colors the logging tags according to level", false,
    verbose, bool, "--verbose@-v", "print extra info", false
)

glvm_parametrization(GlvSdCLIOptions, "CLI options",
    mode, ProcessingMode, "--mode@-M", "run mode, one of [img_gen, vid_gen, upscale, convert], default: img_gen", ProcessingMode::img_gen,
    output, SlvFile, "--output@-o", "path to write result image to. you can use printf-style %d format specifiers for image sequences (default: ./output.png) (eg. output_%03d.png)", SlvFile("./output.png", SlvFileExtensions({".png", ".jpg", ".jpeg", ".jpe"}), SlvFile::IO::Write),
    preview_params, GlvSdParamsPreview, "Preview", "", GlvSdParamsPreview(),
    advanced_params, GlvSdCLIOptionsAdvanced, "Advanced", "", GlvSdCLIOptionsAdvanced()
)

glvm_parametrization(GlvSdContextOptionsAdvanced, "Context options advanced",
    threads, int, "--threads@-t", "number of threads to use during computation (default: -1) \nIf threads <= 0, then threads will be set to the number of CPU physical cores", -1,
    chroma_params, GlvSdParamsChroma, "Chroma", "", GlvSdParamsChroma(),
    Vae_tiling_params, GlvSdParamsVaeTiling, "Vae tiling", "", GlvSdParamsVaeTiling(),
    flow_shift, float, "--flow-shift", "shift value for Flow models like SD3.x or WAN (default: auto)", INFINITY,
    force_sdxl_vae_conv_scale, bool, "--force-sdxl-vae-conv-scale", "force use of conv scale on sdxl vae", false,
    On_CPU_params, GlvSdParamsOnCPU, "On CPU", "", GlvSdParamsOnCPU(),
    diffusion_fa, bool, "--diffusion-fa", "use flash attention in the diffusion model (for low vram)\nMight lower quality, since it implies converting k and v to f16.\nThis might crash if it is not supported by the backend.", false,
    diffusion_conv_direct, bool, "--diffusion-conv-direct", "use ggml_conv2d_direct in the diffusion model", false,
    vae_conv_direct, bool, "--vae-conv-direct", "use ggml_conv2d_direct in the vae model", false,
    circular, bool, "--circular", "enable circular padding for convolutions", false,
    circularx, bool, "--circularx", "enable circular RoPE wrapping on x-axis (width) only", false,
    circulary, bool, "--circulary", "enable circular RoPE wrapping on y-axis (height) only", false,
    type, WeightType, "--type", "weight type (examples: f32, f16, q4_0, q4_1, q5_0, q5_1, q8_0, q2_k, q3_k, q4_k) \nIf not specified, the default is the type of the weight file", WeightType::weight_file_type,
    rng, Rng, "--rng", "RNG, one of [std_default, cuda, cpu], default: cuda(sd-webui), cpu(comfyui)", Rng::std_default,
    sampler_rng, Rng, "--sampler-rng", "sampler RNG, one of [std_default, cuda, cpu]. If not specified, use --rng", Rng::std_default,
    tensor_type_rules, std::string, "--tensor-type-rules", "weight type per tensor pattern (example: \"^vae\\.=f16,model\\.=q8_0\")", "",
    prediction, Prediction, "--prediction", "prediction type override, one of [eps, v, edm_v, sd3_flow, flux_flow, flux2_flow]", Prediction::eps,
    lora_apply_mode, LoraApplyMode, "--lora-apply-mode", "the way to apply LoRA, one of [auto, immediately, at_runtime], default is auto. In auto mode, if the model weights\ncontain any quantized parameters, the at_runtime mode will be used; otherwise,\nimmediately will be used.The immediately mode may have precision and\ncompatibility issues with quantized parameters, but it usually offers faster inference\nspeed and, in some cases, lower memory usage. The at_runtime mode, on the\nother hand, is exactly the opposite.", LoraApplyMode::Auto
)

glvm_parametrization(GlvSdContextOptions, "Context options",
    model_addons, GlvSdModels, "Models", "", GlvSdModels(),
    context_options_advanced, GlvSdContextOptionsAdvanced, "Advanced", "", GlvSdContextOptionsAdvanced()
)

glvm_parametrization(GlvSdGenerationOptionsAdvanced, "Generation options advanced",
    clip_skip, int, "--clip-skip", "ignore last layers of CLIP network; 1 ignores none, 2 ignores one layer (default: -1) \n<= 0 represents unspecified, will be 1 for SD1.x, 2 for SD2.x", -1,
    moe_boundary, float, "--moe-boundary", "timestep boundary for Wan2.2 MoE model. (default: 0.875). Only enabled if `--high-noise-steps` is set to -1", 0.875f,
    vace_strength, float, "--vace-strength", "wan vace strength", 1.f,
    sigmas, std::vector<float>, "--sigmas", "custom sigma values for the sampler, comma-separated (e.g., \"14.61,7.8,3.5,0.0\").\nCan not set values for now. Parsing of values would be more convenient in a vector format such as: [14.61,7.8,3.5,0.0]. Alike skip layers.", {},
    easycache, bool, "--easycache", "enable EasyCache for DiT models with optional \"threshold,start_percent,end_percent\" (default: 0.2,0.15,0.95)\nCan not set values for now. Parsing of values would be more convenient in a vector format such as: [0.2,0.15,0.95]. Alike skip layers.", false,
    increase_ref_index, bool, "--increase-ref-index", "automatically increase the indices of references images based on the order they are listed (starting with 1).", false,
    sampling_method, SamplingMethod, "--sampling-method", "{euler, euler_a, heun, dpm2, dpm++2s_a, dpm++2m, dpm++2mv2, ipndm, ipndm_v, lcm, ddim_trailing, tcd} \nsampling method (default: 'euler_a')", SamplingMethod::euler_a,
    scheduler, Scheduler, "--scheduler", "denoiser sigma scheduler, one of [discrete, karras, exponential, ays, gits, smoothstep, sgm_uniform, simple, kl_optimal, lcm],\ndefault: discrete", Scheduler::discrete,
    high_noise_params, GlvSdParamsHighNoise, "High noise", "", GlvSdParamsHighNoise()
)

glvm_parametrization(GlvSdGenerationOptions, "Generation options",
    prompt, std::string, "--prompt@-p", "the prompt to render", "",
    negative_prompt, std::string, "--negative-prompt@-n", "the negative prompt (default: '')", "",
    height, unsigned int, "--height@-H", "image height, in pixel space (default: 512)", 512,
    width, unsigned int, "--width@-W", "image width, in pixel space (default: 512)", 512,
    steps, unsigned int, "--steps", "number of sample steps (default: 20)", 20,
    batch_count, unsigned int, "--batch-count@-b", "number of images to generate", 1,
    seed, int, "--seed@-s", "RNG seed (default: 42, use random seed for < 0)", 42,
    scale_strength_params, GlvSdScaleStrength, "Scale/strength", "", GlvSdScaleStrength(),
    image_video_input_params, GlvSdParamsImageVideoInput, "Image/video input", "", GlvSdParamsImageVideoInput(),
    photomaker_params, GlvSdParamsPhotomaker, "Photomaker", "", GlvSdParamsPhotomaker(),
    video_params, GlvSdParamsVideo, "Video", "", GlvSdParamsVideo(),
    upscale_params, GlvSdParamsUpscale, "Upscale", "", GlvSdParamsUpscale(),
    advanced_params, GlvSdGenerationOptionsAdvanced, "Advanced", "", GlvSdGenerationOptionsAdvanced()
)

glvm_parametrization(GlvSdParams, "SD params",
    CLI_options, GlvSdCLIOptions, "CLI", "", GlvSdCLIOptions(),
    context_options, GlvSdContextOptions, "Context", "Options related to model configuration. Modification of the parameters makes the context to be reinitalized (model loading).", GlvSdContextOptions(),
    generation_options, GlvSdGenerationOptions, "Generation", "Options related to inference. Can generate new results on the fly.", GlvSdGenerationOptions(),
    images_sequence_params, GlvSdImagesSequence, "Images\nsequence", "Used only with img_gen mode and if --init-img points to the same image as --output", GlvSdImagesSequence()
)

GLOVE_APP_CLI_PARAMETRIZATION_OUTPUT_DIRECTORY(GlvSdParams, "--output")

glvm_parametrization_open(GlvSdCLIOptions)
glvm_parametrization_open(GlvSdContextOptions)
glvm_parametrization_open(GlvSdGenerationOptions)

// Manage 'server'-like application. Ie: keep the model loaded at each run.
struct RecurrentStruct {
    sd_ctx_t* sd_ctx = NULL;
    SlvStatus clear() {
        if (sd_ctx) {
            free_sd_ctx(sd_ctx);
            sd_ctx = NULL;
        }
        return SlvStatus();
    }
    GlvSdParams params;
    bool context_changed(const GlvSdParams& _params) {// ie: model changed
        return _params.get_context_options() != params.get_context_options();
    }
    unsigned int count = 0;
#ifdef SD_EXAMPLES_IMG2IMG_REPEAT
    bool l_img2img_sequence = false;
    unsigned int Nframes    = 100000;
    // Auto repeat if returns true
    operator bool() const {
        return l_img2img_sequence && count > 0 && count <= Nframes;
    }
#else
    operator bool() const {
        return false;
    }
#endif
};

#ifdef SD_EXAMPLES_GLOVE_GUI_DESKTOP
#define GLOVE_APP_AUTO true
#pragma GLOVE_APP_MSVC_NO_CONSOLE
#endif
#define GLOVE_APP_RECURRENT_MODE true
#define GLOVE_APP_RECURRENT_TYPE RecurrentStruct

#include "common/common.hpp" // for version_string()

void glove_app_init() {

    GlvApp::get_progression("Model");
    GlvApp::get_progression("clip_l");
    GlvApp::get_progression("clip_g");
    GlvApp::get_progression("t5xxl");
    GlvApp::get_progression("diffusion-model");
    GlvApp::get_progression("VAE");
    GlvApp::get_progression("LoRA");
    GlvApp::get_progression("Batch");
    GlvApp::get_progression("Generating image")->set_recurrent(true);
    GlvApp::get_progression("Decoding");
    GlvApp::get_progression("Result");
    GlvApp::get_progression("Decoding latent video");

    GLOVE_APP_TITLE("stable-diffusion.cpp CLI-GUI");
    GLOVE_APP_MENU_HELP(true);
    GLOVE_APP_MENU_ABOUT(version_string());
    GLOVE_APP_MENU_LICENSE_ADD("stable-diffusion.cpp", "MIT", "Diffusion model(SD,Flux,Wan,Qwen Image,Z-Image,...) inference in pure C/C++ ", "https://github.com/leejet/stable-diffusion.cpp");
    GLOVE_APP_MENU_LICENSE_ADD("ggml", "MIT", "Tensor library for machine learning", "https://github.com/ggml-org/ggml");
    GLOVE_APP_MENU_LICENSE_ADD("Qt", "LGPLv3", "Cross-platform application development framework for creating graphical user interfaces", "https://www.qt.io/");
    GLOVE_APP_MENU_LICENSE_ADD("Glove", "GPLv3", "C++ Qt library for easy graphical user interfaces ", "https://github.com/piallai/glove");

}


// Usefull only for img2img sequence. Optional.
#ifdef SD_EXAMPLES_IMG2IMG_REPEAT
sd_image_t* crop(const sd_image_t& _image, int _left, int _right, int _up, int _bottom) {
    sd_image_t* image = new sd_image_t;

    image->width   = _image.width - _left - _right;
    image->height  = _image.height - _up - _bottom;
    image->channel = _image.channel;
    image->data    = (uint8_t*)malloc(image->width * image->height * image->channel);

    uint8_t* data  = _image.data;
    uint8_t* data2 = image->data;
    for (int m = 0; m < _image.width * _image.height * _image.channel; m++) {
        int j = (m - m % (_image.width * _image.channel)) / (_image.width * _image.channel);
        int n = m - j * _image.width * _image.channel;
        int i = (n - n % _image.channel) / (_image.channel);
        int k = n - i * _image.channel;

        bool l_ok = false;

        if (i >= _left) {
            if (i < _image.width - _right) {
                if (j >= _up) {
                    if (j < _image.height - _bottom) {
                        *data2 = *data;
                        data2++;
                        l_ok = true;
                    }
                }
            }
        }
        data++;
    }

    return image;
}
#endif

#endif
