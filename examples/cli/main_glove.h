
#pragma once

#define GLOVE_ENABLE_JSON
#include "glove.h"

glvm_SlvEnum(ProcessingMode, img_gen, vid_gen, convert, upscale);
glvm_SlvEnum_named(WeightType, weight_file_type, "", f32, "f32", f16, "f16", q4_0, "q4_0", q4_1, "q4_1", q5_0, "q5_0", q5_1, "q5_1", q8_0, "q8_0", q2_k, "q2_k", q3_k, "q3_k", q4_k, "q4_k");
glvm_SlvEnum_named(SamplingMethod, euler, "euler", euler_a, "euler_a", heun, "heun", dpm2, "dpm2", dpmpp2s_a, "dpm++2s_a", dpmpp2m, "dpm++2m", dpmpp2mv2, "dpm++2mv2", ipndm, "ipndm", ipndm_v, "ipndm_v", lcm, "lcm", ddim_trailing, "ddim_trailing", tcd, "tcd");
glvm_SlvEnum(Rng, std_default, cuda, cpu);
glvm_SlvEnum(Prediction, eps, v, edm_v, sd3_flow, flux_flow);
glvm_SlvEnum_named(LoraApplyMode, Auto, "auto", immediately, "immediately", at_runtime, "at_runtime");
glvm_SlvEnum(Scheduler, discrete, karras, exponential, ays, gits, smoothstep, sgm_uniform, simple);
glvm_SlvEnum(Preview, none, proj, tae, vae);

glvm_parametrization(GlvSDParamsPhotomaker, "Photomaker params",
                        photo_maker_path, SlvFile, "--photo-maker", "path to PHOTOMAKER model", SlvFile(SlvFile::IO::Read),
                        pm_id_embed_path, SlvFile, "--pm-id-embed-path", "path to PHOTOMAKER v2 id embed", SlvFile(SlvFile::IO::Read),
                        pm_id_images_dir, SlvDirectory, "--pm-id-images-dir", "path to PHOTOMAKER input id images dir", SlvDirectory(),
                        pm_style_strength, float, "--pm-style-strength", "", 20.f);

glvm_parametrization(GlvSDParamsImageVideoInput, "Image/video input params",
                        init_img, SlvFile, "--init-img@-i", "path to the init image", SlvFile(SlvFile::IO::Read),
                        end_img, SlvFile, "--end-img", "path to the end image, required by flf2v", SlvFile(SlvFile::IO::Read),
                        mask_img, SlvFile, "--mask", "mask for inpainting", SlvFile(SlvFile::IO::Read),
                        control_image, SlvFile, "--control-image", "path to control image, control net", SlvFile(SlvFile::IO::Read),
                        control_video, SlvDirectory, "--control-video", "path to control video frames, It must be a directory path. The video frames inside should be stored as images in\nlexicographical (character) order. For example, if the control video path is\n`frames`, the directory contain images such as 00.png, 01.png, ... etc.", SlvDirectory(),
                        ref_image, SlvFile, "--ref-image", "reference image for Flux Kontext models (can be used multiple times)", SlvFile(SlvFile::IO::Read),
                        disable_auto_resize_ref_image, bool, "--disable-auto-resize-ref-image", "disable auto resize of ref images", false)

glvm_parametrization(GlvSDParamsVideo, "Video params",
                        video_frames, int, "--video-frames", "video frames (default: 1)", 1,
                        fps, unsigned int, "--fps", "fps (default: 24)", 24,
                        timestep_shift, unsigned int, "--timestep-shift", "shift timestep for NitroFusion models (default: 0). recommended N for NitroSD-Realism around 250 and 500 for\nNitroSD-Vibrant", 0);

glvm_parametrization(GlvSDParamsVaeTiling, "Vae tiling params",
                        vae_tiling, bool, "--vae-tiling", "process vae in tiles to reduce memory usage", false,
                        vae_tile_overlap, float, "--vae-tile-overlap", "tile overlap for vae tiling, in fraction of tile size (default: 0.5)", 0.5f,
                        vae_tile_size, SlvSize2d<unsigned int>, "--vae-tile-size", "tile size for vae tiling, format [X]x[Y] (default: 32x32)", SlvSize2d<unsigned int>(32 COMMA 32),
                        vae_relative_tile_size, SlvSize2d<float>, "--vae-relative-tile-size", "relative tile size for vae tiling, format [X]x[Y], in fraction of image size if < 1, in number of tiles per dim if >=1\n(overrides --vae-tile-size)", SlvSize2d<float>(0.f COMMA 0.f))

glvm_parametrization(GlvSDParamsOnCPU, "On CPU",
                        offload_to_cpu, bool, "--offload-to-cpu", "place the weights in RAM to save VRAM, and automatically load them into VRAM when needed", false,
                        control_net_cpu, bool, "--control-net-cpu", "keep controlnet in cpu (for low vram)", false,
                        clip_on_cpu, bool, "--clip-on-cpu", "keep clip in cpu (for low vram)", false,
                        vae_on_cpu, bool, "--vae-on-cpu", "keep vae in cpu (for low vram)", false)

glvm_parametrization(GlvSDParamsPreview, "Preview params",
    preview, Preview, "--preview", "preview method. must be one of the following [none, proj, tae, vae].] (default is none)", Preview::none,
    preview_path, SlvFile, "--preview-path", "path to write preview image to (default: ./preview.png)", SlvFile("./preview.png", SlvFileExtensions({".png", ".jpg", ".jpeg", ".jpe"}), SlvFile::IO::Write),
    preview_interval, unsigned int, "--preview-interval", "interval in denoising steps between consecutive updates of the image preview file (default is 1, meaning updating at every step)", 1,
    preview_noisy, bool, "--preview-noisy", "enables previewing noisy inputs of the models rather than the denoised outputs", true,
    taesd_preview_only, bool, "--taesd-preview-only", "prevents usage of taesd for decoding the final image. (for use with --preview tae)", true
)

glvm_parametrization(GlvSDParamsUpscale, "Upscale params",
    upscale_model, SlvFile, "--upscale-model", "path to esrgan model. Upscale images after generate, just RealESRGAN_x4plus_anime_6B supported by now", SlvFile(SlvFile::IO::Read),
    upscale_repeats, unsigned int, "--upscale-repeats", "Run the ESRGAN upscaler this many times (default 1)", 1,
    upscale_tile_size, unsigned int, "--upscale-tile-size", "tile size for ESRGAN upscaling (default: 128)", 128
)

glvm_parametrization(GlvSDParamsAdvanced, "Advanced params",
                        preview_params, GlvSDParamsPreview, "Preview", "", GlvSDParamsPreview(),
                        rng, Rng, "--rng", "RNG, one of [std_default, cuda, cpu], default: cuda(sd-webui), cpu(comfyui)", Rng::std_default,
                        sampler_rng, Rng, "--sampler-rng", "sampler RNG, one of [std_default, cuda, cpu]. If not specified, use --rng", Rng::std_default,
                        threads, int, "--threads@-t", "number of threads to use during computation (default: -1) \nIf threads <= 0, then threads will be set to the number of CPU physical cores", -1,
                        type, WeightType, "--type", "weight type (examples: f32, f16, q4_0, q4_1, q5_0, q5_1, q8_0, q2_k, q3_k, q4_k) \nIf not specified, the default is the type of the weight file", WeightType::weight_file_type,
                        tensor_type_rules, std::string, "--tensor-type-rules", "weight type per tensor pattern (example: \"^vae\\.=f16,model\\.=q8_0\")", "",
                        prediction, Prediction, "--prediction", "prediction type override, one of [eps, v, edm_v, sd3_flow, flux_flow, flux2_flow]", Prediction::eps,
                        lora_apply_mode, LoraApplyMode, "--lora-apply-mode", "the way to apply LoRA, one of [auto, immediately, at_runtime], default is auto. In auto mode, if the model weights\ncontain any quantized parameters, the at_runtime mode will be used; otherwise,\nimmediately will be used.The immediately mode may have precision and\ncompatibility issues with quantized parameters, but it usually offers faster inference\nspeed and, in some cases, lower memory usage. The at_runtime mode, on the\nother hand, is exactly the opposite.", LoraApplyMode::Auto,
                        clip_skip, int, "--clip-skip", "ignore last layers of CLIP network; 1 ignores none, 2 ignores one layer (default: -1) \n<= 0 represents unspecified, will be 1 for SD1.x, 2 for SD2.x", -1,
                        moe_boundary, float, "--moe-boundary", "timestep boundary for Wan2.2 MoE model. (default: 0.875). Only enabled if `--high-noise-steps` is set to -1", 0.875f,
                        flow_shift, float, "--flow-shift", "shift value for Flow models like SD3.x or WAN (default: auto)", INFINITY,
                        vace_strength, float, "--vace-strength", "wan vace strength", 1.f,
                        Vae_tiling_params, GlvSDParamsVaeTiling, "Vae tiling", "", GlvSDParamsVaeTiling(),
                        force_sdxl_vae_conv_scale, bool, "--force-sdxl-vae-conv-scale", "force use of conv scale on sdxl vae", false,
                        On_CPU_params, GlvSDParamsOnCPU, "On CPU", "", GlvSDParamsOnCPU(),
                        diffusion_fa, bool, "--diffusion-fa", "use flash attention in the diffusion model (for low vram)\nMight lower quality, since it implies converting k and v to f16.\nThis might crash if it is not supported by the backend.", false,
                        diffusion_conv_direct, bool, "--diffusion-conv-direct", "use ggml_conv2d_direct in the diffusion model", false,
                        vae_conv_direct, bool, "--vae-conv-direct", "use ggml_conv2d_direct in the vae model", false,
                        sigmas, std::vector<float>, "--sigmas", "custom sigma values for the sampler, comma-separated (e.g., \"14.61,7.8,3.5,0.0\").\nCan not set values for now. Parsing of values would be more convenient in a vector format such as: [14.61,7.8,3.5,0.0]. Alike skip layers.", {},             
                        easycache, bool, "--easycache", "enable EasyCache for DiT models with optional \"threshold,start_percent,end_percent\" (default: 0.2,0.15,0.95)\nCan not set values for now. Parsing of values would be more convenient in a vector format such as: [0.2,0.15,0.95]. Alike skip layers.", false,             
                        canny, bool, "--canny", "apply canny preprocessor (edge detection)", false,
                        color, bool, "--color", "colors the logging tags according to level", false,
                        increase_ref_index, bool, "--increase-ref-index", "automatically increase the indices of references images based on the order they are listed (starting with 1).", false,
                        verbose, bool, "--verbose@-v", "print extra info", false);

glvm_parametrization(GlvSDModelAddons, "SD model addons",
    clip_l, SlvFile, "--clip_l", "path to the clip-l text encoder", SlvFile(SlvFile::IO::Read),
    clip_g, SlvFile, "--clip_g", "path to the clip-g text encoder", SlvFile(SlvFile::IO::Read),
    clip_vision, SlvFile, "--clip_vision", "path to the clip-vision encoder", SlvFile(SlvFile::IO::Read),
    t5xxl, SlvFile, "--t5xxl", "path to the t5xxl text encoder", SlvFile(SlvFile::IO::Read),
    llm, SlvFile, "--llm", "path to the llm text encoder. For example: (qwenvl2.5 for qwen-image, mistral-small3.2 for flux2, ...)", SlvFile(SlvFile::IO::Read),
    llm_vision, SlvFile, "--llm_vision", "path to the llm vit", SlvFile(SlvFile::IO::Read),
    qwen2vl, SlvFile, "--qwen2vl", "alias of --llm. Deprecated.", SlvFile(SlvFile::IO::Read),
    qwen2vl_vision, SlvFile, "--qwen2vl_vision", "alias of --llm_vision. Deprecated.", SlvFile(SlvFile::IO::Read),
    diffusion_model, SlvFile, "--diffusion-model", "path to the standalone diffusion model", SlvFile(SlvFileExtensions({".gguf", ".safetensors", ".sft"}), SlvFile::IO::Read),
    high_noise_diffusion_model, SlvFile, "--high-noise-diffusion-model", "path to the standalone high noise diffusion model", SlvFile(SlvFileExtensions({".gguf", ".safetensors", ".sft"}), SlvFile::IO::Read),
    vae, SlvFile, "--vae", "path to vae", SlvFile("", SlvFileExtensions({".safetensors", ".sft"}), SlvFile::IO::Read),
    taesd, SlvFile, "--taesd@--tae", "path to taesd. Using Tiny AutoEncoder for fast decoding (low quality)", SlvFile(SlvFile::IO::Read),
    control_net, SlvFile, "--control-net", "path to control net model", SlvFile(SlvFile::IO::Read),
    embd_dir, SlvDirectory, "--embd-dir", "path to embeddings", SlvDirectory(),
    lora_model_dir, SlvDirectory, "--lora-model-dir", "lora model directory", SlvDirectory())

glvm_parametrization(GlvSDModelSLG, "SLG params",
    slg_scale, float, "--slg-scale", "skip layer guidance (SLG) scale, only for DiT models: (default: 0)\n0 means disabled, a value of 2.5 is nice for sd3.5 medium.", 0.f,
    skip_layer_start, float, "--skip-layer-start", "SLG enabling point: (default: 0.01)", 0.01f,
    skip_layer_end, float, "--skip-layer-end", "SLG disabling point (default: 0.2)", 0.2f,
    skip_layers, std::vector<unsigned int>, "--skip-layers", "Layers to skip for SLG steps: (default: [7,8,9])", std::vector<unsigned int>({7, 8, 9}))

glvm_parametrization(GlvSDModelSLGHighNoise, "SLG params (high noise)",
    slg_scale, float, "--high-noise-slg-scale", "(high noise) skip layer guidance (SLG) scale, only for DiT models: (default: 0)", 0.f,
    skip_layer_start, float, "--high-noise-skip-layer-start", "(high noise) SLG enabling point (default: 0.01)", 0.01f,
    skip_layer_end, float, "--high-noise-skip-layer-end", "(high noise) SLG disabling point (default: 0.2)", 0.2f,
    skip_layers, std::vector<unsigned int>, "--high-noise-skip-layers", "(high noise) layers to skip for SLG steps (default: [7,8,9])", std::vector<unsigned int>({7, 8, 9}))

glvm_parametrization(GlvSDParamsScale, "Scale params",
    cfg_scale, float, "--cfg-scale", "unconditional guidance scale: (default: 7.0)", 7.0f,
    img_cfg_scale, float, "--img-cfg-scale", "image guidance scale for inpaint or instruct-pix2pix models: (default: same as --cfg-scale)", INFINITY,
    guidance, float, "--guidance", "distilled guidance scale for models with guidance input (default: 3.5)", 3.5f,
    SLG, GlvSDModelSLG, "Skip layer guidance", "", GlvSDModelSLG(),
    eta, float, "--eta", "eta in DDIM, only for DDIM and TCD: (default: 0)", 0.f)

glvm_parametrization(GlvSDParamsScaleHighNoise, "Scale params (high noise)",
    high_noise_cfg_scale, float, "--high-noise-cfg-scale", "(high noise) unconditional guidance scale: (default: 7.0)", 7.0f,
    high_noise_img_cfg_scale, float, "--high-noise-img-cfg-scale", "(high noise) image guidance scale for inpaint or instruct-pix2pix models (default: same as --cfg-scale)", INFINITY,
    high_noise_guidance, float, "--high-noise-guidance", "(high noise) distilled guidance scale for models with guidance input (default: 3.5)", INFINITY,
    SLG_high_noise, GlvSDModelSLGHighNoise, "Skip layer guidance", "", GlvSDModelSLGHighNoise(),
    high_noise_eta, float, "--high-noise-eta", "(high noise) eta in DDIM, only for DDIM and TCD (default: 0)", 0.f)

glvm_parametrization(GlvSDScaleStrength, "Scale/Strength params",
    Scale, GlvSDParamsScale, "Scale", "", GlvSDParamsScale(),
    Scale_high_noise, GlvSDParamsScaleHighNoise, "Scale (high noise)", "", GlvSDParamsScaleHighNoise(),
    strength, float, "--strength", "strength for noising/unnoising (default: 0.75)", 0.75f,
    control_strength, SlvProportion, "--control-strength", "strength to apply Control Net (default: 0.9) \n1.0 corresponds to full destruction of information in init", 0.9f)

glvm_parametrization(GlvSDParamsChroma, "Chroma params",
    chroma_use_dit_mask, bool, "--chroma-disable-dit-mask", "disable dit mask for chroma", true,
    chroma_use_t5_mask, bool, "--chroma-enable-t5-mask", "enable t5 mask for chroma", false,
    chroma_t5_mask_pad, int, "--chroma-t5-mask-pad", "t5 mask pad size of chroma", 1)

#ifdef SD_EXAMPLES_IMG2IMG_REPEAT
glvm_parametrization(GlvSDImagesSequence, "Images sequence params",
    crop, SlvSides2d<unsigned int>, "Crop", "Image crop at each frame", SlvSides2d<unsigned int>({2, 2, 2, 2}),
    Nframes, unsigned int, "Nframes", "Number of frames", 100)
#else
    typedef nullptr_t GlvSDAnim;
#endif

glvm_parametrization(GlvSDParamsHighNoise, "SD params (high noise)",
    high_noise_sampling_method, SamplingMethod, "--high-noise-sampling-method", "(high noise) sampling method, one of [euler, euler_a, heun, dpm2, dpm++2s_a, dpm++2m, dpm++2mv2, ipndm, ipndm_v, lcm,\nddim_trailing, tcd] default: euler for Flux/SD3/Wan, euler_a otherwise", SamplingMethod::euler_a,
    high_noise_steps, int, "--high-noise-steps", "(high noise) number of sample steps (default: -1 = auto)", -1)

glvm_parametrization(GlvSDParams, "SD params",
    mode, ProcessingMode, "--mode", "run mode, one of: [img_gen, convert], default: img_gen", ProcessingMode::img_gen,
    model, SlvFile, "--model@-m", "path to full model", SlvFile("./", SlvFileExtensions({".safetensors", ".ckpt"}), SlvFile::IO::Read),
    model_addons, GlvSDModelAddons, "Model addons", "", GlvSDModelAddons(), photomaker_params, GlvSDParamsPhotomaker, "Photomaker", "", GlvSDParamsPhotomaker(),
    upscale_params, GlvSDParamsUpscale, "Upscale", "", GlvSDParamsUpscale(),
    chroma_params, GlvSDParamsChroma, "Chroma", "", GlvSDParamsChroma(),
    image_video_input_params, GlvSDParamsImageVideoInput, "Image/video input", "", GlvSDParamsImageVideoInput(),
    output, SlvFile, "--output@-o", "path to write result image to (default: ./output.png)", SlvFile("./output.png", SlvFileExtensions({".png", ".jpg", ".jpeg", ".jpe"}), SlvFile::IO::Write),
    prompt, std::string, "--prompt@-p", "the prompt to render", "",
    negative_prompt, std::string, "--negative-prompt@-n", "the negative prompt (default: '')", "",
    scale_strength_params, GlvSDScaleStrength, "Scale/strength", "", GlvSDScaleStrength(),
    height, unsigned int, "--height@-H", "image height, in pixel space (default: 512)", 512,
    width, unsigned int, "--width@-W", "image width, in pixel space (default: 512)", 512,
    sampling_method, SamplingMethod, "--sampling-method", "{euler, euler_a, heun, dpm2, dpm++2s_a, dpm++2m, dpm++2mv2, ipndm, ipndm_v, lcm, ddim_trailing, tcd} \nsampling method (default: 'euler_a')", SamplingMethod::euler_a,
    scheduler, Scheduler, "--scheduler", "denoiser sigma scheduler, one of [discrete, karras, exponential, ays, gits, smoothstep, sgm_uniform, simple], default: discrete", Scheduler::discrete,
    steps, unsigned int, "--steps", "number of sample steps (default: 20)", 20,
    high_noise_params, GlvSDParamsHighNoise, "High noise", "", GlvSDParamsHighNoise(),
    seed, int, "--seed", "RNG seed (default: 42, use random seed for < 0)", 42,
    batch_count, unsigned int, "--batch-count@-b", "number of images to generate", 1,
    video_params, GlvSDParamsVideo, "Video", "", GlvSDParamsVideo(),
    advanced_params, GlvSDParamsAdvanced, "Advanced", "", GlvSDParamsAdvanced(),
    images_sequence_params, GlvSDImagesSequence, "Images sequence", "Used only with img_gen mode and if --init-img points to the same image as --output", GlvSDImagesSequence())

GLOVE_APP_CLI_PARAMETRIZATION_OUTPUT_DIRECTORY(GlvSDParams, "--output")


#ifdef SD_EXAMPLES_IMG2IMG_REPEAT
sd_image_t* crop(const sd_image_t& _image, int _left, int _right, int _up, int _bottom) {

    sd_image_t* image = new sd_image_t;

    image->width = _image.width - _left - _right;
    image->height  = _image.height - _up - _bottom;
    image->channel = _image.channel;
    image->data    = (uint8_t*)malloc(image->width * image->height * image->channel);

    uint8_t* data = _image.data;
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
