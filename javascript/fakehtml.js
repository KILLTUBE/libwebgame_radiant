


Console = function() {}
Console.prototype.log = function() {
    print(arguments);
}
Console.prototype.warn = function() {
    print(arguments);
}
Console.prototype.error = function() {
    print(arguments);
}
console = new Console

Element = function(name) {
    console.log("Create Element: ", name);
    
}

Document = function() {}

Document.prototype.addEventListener = function(name) {
    
    
}

Window = function() {}

Window.prototype.addEventListener = function(name) {
    
    
}

Canvas = function() {
    this.id = 0;
}


Canvas.prototype.addEventListener = function(name) {
    
    
}

GL = function() {
    
    this.MAX_TEXTURE_SIZE                      =  3379;
    this.MAX_CUBE_MAP_TEXTURE_SIZE             = 34076;
    this.MAX_RENDERBUFFER_SIZE                 = 34024;
    this.MAX_TEXTURE_IMAGE_UNITS               = 34930;
    this.MAX_COMBINED_TEXTURE_IMAGE_UNITS      = 35661;
    this.MAX_VERTEX_TEXTURE_IMAGE_UNITS        = 35660;
    this.MAX_VERTEX_UNIFORM_VECTORS            = 36347;
    this.MAX_FRAGMENT_UNIFORM_VECTORS          = 36349;
    this.MAX_DRAW_BUFFERS                      = 34852;
    this.MAX_COLOR_ATTACHMENTS                 = 36063;
    this.MAX_3D_TEXTURE_SIZE                   = 32883;
    this.MAX_DRAW_BUFFERS_EXT                  = undefined;
    this.MAX_COLOR_ATTACHMENTS_EXT             = undefined;
    
    this.FRAMEBUFFER_COMPLETE = 12345; // just some value
    
    //ext = this.extDebugRendererInfo;
    //this.unmaskedRenderer = ext ? gl.getParameter(ext.UNMASKED_RENDERER_WEBGL) : "";
    //this.unmaskedVendor = ext ? gl.getParameter(ext.UNMASKED_VENDOR_WEBGL) : "";
    //ext = this.extTextureFilterAnisotropic;
    //this.maxAnisotropy = ext ? gl.getParameter(ext.MAX_TEXTURE_MAX_ANISOTROPY_EXT) : 1;    
    
}
GL.prototype.getSupportedExtensions = function() {
    return [
        "EXT_color_buffer_float",
        "EXT_texture_filter_anisotropic",
        "OES_texture_float_linear",
        "WEBGL_compressed_texture_s3tc",
        "WEBGL_compressed_texture_s3tc_srgb",
        "WEBGL_debug_renderer_info",
        "WEBGL_debug_shaders",
        "WEBGL_lose_context"
    ];
}

GL.prototype.getContextAttributes = function() {
    return {
        alpha: true,
        antialias: true,
        depth: true,
        failIfMajorPerformanceCaveat: false,
        premultipliedAlpha: true,
        preserveDrawingBuffer: false,
        stencil: true
    };
}

GL.prototype.getParameter = function(parameter) {
    switch (parameter) {
        case this.MAX_TEXTURE_SIZE                     :  return 16384  ;
        case this.MAX_CUBE_MAP_TEXTURE_SIZE            :  return 16384  ;
        case this.MAX_RENDERBUFFER_SIZE                :  return 16384  ;
        case this.MAX_TEXTURE_IMAGE_UNITS              :  return 16     ;
        case this.MAX_COMBINED_TEXTURE_IMAGE_UNITS     :  return 32     ;
        case this.MAX_VERTEX_TEXTURE_IMAGE_UNITS       :  return 16     ;
        case this.MAX_VERTEX_UNIFORM_VECTORS           :  return 4096   ;
        case this.MAX_FRAGMENT_UNIFORM_VECTORS         :  return 1024   ;
        case this.MAX_DRAW_BUFFERS                     :  return 8      ;
        case this.MAX_COLOR_ATTACHMENTS                :  return 8      ;
        case this.MAX_3D_TEXTURE_SIZE                  :  return 2048   ;
        case this.MAX_DRAW_BUFFERS_EXT                 :  return null   ;
        case this.MAX_COLOR_ATTACHMENTS_EXT            :  return null   ;
        
    }
    
}

GL.prototype.disable         = function() {} // (gl.BLEND);
GL.prototype.blendFunc       = function() {} // (gl.ONE, gl.ZERO);
GL.prototype.blendEquation   = function() {} // (gl.FUNC_ADD);
GL.prototype.colorMask       = function() {} // (true, true, true, true);
GL.prototype.enable          = function() {} // (gl.CULL_FACE);
GL.prototype.cullFace        = function() {} // (gl.BACK);
GL.prototype.depthFunc       = function() {} // (gl.LEQUAL);
GL.prototype.depthMask       = function() {} // (true);
GL.prototype.stencilFunc     = function() {} // (gl.ALWAYS, 0, 255);
GL.prototype.stencilOp       = function() {} // (gl.KEEP, gl.KEEP, gl.KEEP);
GL.prototype.stencilMask     = function() {} // (255);
GL.prototype.clearDepth      = function() {} // (1);
GL.prototype.clearColor      = function() {} // (0, 0, 0, 0);
GL.prototype.clearStencil    = function() {} // (0);
GL.prototype.hint            = function() {} // (gl.FRAGMENT_SHADER_DERIVATIVE_HINT, gl.NICEST);
GL.prototype.pixelStorei     = function() {} // (gl.UNPACK_COLORSPACE_CONVERSION_WEBGL, gl.NONE);

GL.prototype.createShader   = function() {} // (type);
GL.prototype.shaderSource   = function() {} // (shader, src);
GL.prototype.compileShader  = function() {} // (shader);
GL.prototype.createProgram  = function() {}
GL.prototype.attachShader   = function() {}
GL.prototype.deleteShader   = function() {}
GL.prototype.getActiveAttrib   = function() { return 1; }
GL.prototype.linkProgram   = function() {}
GL.prototype.getShaderParameter   = function() { return true; }
GL.prototype.getProgramParameter   = function() { return 1; }
GL.prototype.getAttribLocation   = function() { return 1; }


GL.prototype.createBuffer = function() {} // ();
GL.prototype.deleteBuffer = function() {} // ();
GL.prototype.bindBuffer   = function() {} // (gl.ARRAY_BUFFER, this.bufferId);
GL.prototype.bufferData   = function() {} // (gl.ARRAY_BUFFER, this.storage, glUsage);



GL.prototype.createFramebuffer   = function() {}
GL.prototype.bindFramebuffer   = function() {}
GL.prototype.framebufferTexture2D   = function() {}
GL.prototype.checkFramebufferStatus   = function() {return this.FRAMEBUFFER_COMPLETE}
GL.prototype.bindTexture   = function() {}
GL.prototype.deleteTexture   = function() {}
GL.prototype.deleteFramebuffer   = function() {}
GL.prototype.createTexture   = function() {}
GL.prototype.texParameteri   = function() {}
GL.prototype.texParameterf   = function() {}
GL.prototype.texImage2D   = function() {}
GL.prototype.generateMipmap   = function() {}
GL.prototype.viewport   = function() {}
GL.prototype.scissor   = function() {}
GL.prototype.useProgram   = function() {}
GL.prototype.drawArrays   = function() {}
GL.prototype.readPixels   = function() {}





Extension = function(name) {
    this.name = name;
}

GL.prototype.getExtension = function(name) {
    return new Extension(name);
}

Canvas.prototype.getContext = function(name) {
    return new GL;
    
}

DOMRect = function() {
    this.bottom = 920;
    this.height = 920;
    this.left = 0;
    this.right = 1086;
    this.top = 0;
    this.width = 1086;
    this.x = 0;
    this.y = 0;
}

Canvas.prototype.getBoundingClientRect = function() {
    return new DOMRect;
}

Navigator = function() {
    this.userAgent = "duktape";
}


HTMLCanvasElement = function() {}
HTMLImageElement  = function() {}
HTMLVideoElement  = function() {}

document = new Document;
window = new Window;
canvas = new Canvas;
navigator = new Navigator;

